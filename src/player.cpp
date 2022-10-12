#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "player.hpp"
#include "functions.hpp"
#include "board.cpp"
#include "plot.hpp"

#include <vector>
#include <iostream>
#include <limits>

namespace functions {};

player::Player::Player(bool isMainPlayer) {
    this->isMainPlayer = isMainPlayer;
}

void player::Player::movePlayer(board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, card_managment::CardManagment& cardManager) {
    functions::clear();
    if (!this->inGame)
        return;
    if (this->inJail) {
        this->whileInJail(board, mainPlayer, computers, cardManager);
        if (this->inJail)
            return;
    }
    std::vector<unsigned char> dieRoll = board.rollDice();
    int squaresToMove = dieRoll[0] + dieRoll[1];
    std::string currentSquareColor = board.getPlot(this->plotPosition).stringProperties.at("COLORCODE");
    std::string currentSquareName = board.getPlot(this->plotPosition).stringProperties.at("NAME");
    std::string currentSquareText = board.getPlot(this->plotPosition).stringProperties.at("TEXT");
    std::string nextSquareColor = board.getPlot(this->plotPosition + squaresToMove).stringProperties.at("COLORCODE");
    std::string nextSquareName = board.getPlot(this->plotPosition + squaresToMove).stringProperties.at("NAME");
    std::string nextSquareText = board.getPlot(this->plotPosition + squaresToMove).stringProperties.at("TEXT");
    plot::Plot nextPlot = board.getPlot(this->plotPosition + squaresToMove);
    functions::printlnBlue(this->name + " rolled:");
    std::cout << "+---+ +---+" << std::endl;
    std::cout << "| " << std::to_string(dieRoll[0]) << " | | " << std::to_string(dieRoll[1]) << " |" << std::endl;
    std::cout << "+---+ +---+" << std::endl;
    std::cout << this->name << " moved " << std::to_string(squaresToMove) << " spaces from" << std::endl;
    std::cout << " " << functions::ANSI_GREEN << std::string(board.getPlot(this->plotPosition).intProperties.at("HOUSES"), 'O') << functions::ANSI_RESET;
    std::cout << " " << functions::ANSI_RED << std::string(board.getPlot(this->plotPosition).intProperties.at("HOTELS"), 'O') << functions::ANSI_RESET << std::endl;
    std::cout << currentSquareColor << "+" << std::string(currentSquareName.length() + currentSquareText.length() + 6, '-') << "+" << std::endl;
    std::cout << "| " << currentSquareName << "    " << currentSquareText << " |" << std::endl;
    std::cout << "+" << std::string(currentSquareName.length() + currentSquareText.length() + 6, '-') << "+" << functions::ANSI_RESET << std::endl;
    std::cout << "To" << std::endl;
    std::cout << " " << functions::ANSI_GREEN << std::string(nextPlot.intProperties.at("HOUSES"), 'O') << functions::ANSI_RESET;
    std::cout << " " << functions::ANSI_RED << std::string(nextPlot.intProperties.at("HOTELS"), 'O') << functions::ANSI_RESET << std::endl;
    std::cout << nextSquareColor << "+" << std::string(nextSquareName.length() + nextSquareText.length() + 6, '-') << "+" << std::endl;
    std::cout << "| " << nextSquareName << "    " << nextSquareText << " |" << std::endl;
    std::cout << "+" << std::string(nextSquareName.length() + nextSquareText.length() + 6, '-') << "+" << functions::ANSI_RESET << std::endl;
    for (unsigned char i = this->plotPosition; i < this->plotPosition + squaresToMove; i++) {
        plot::Plot plot = board.getPlot(i);
        if (this->plotPosition != 0 && functions::setContains(plot.flags, "GOSQUARE")) {
            std::cout << this->name << " passes by GO. +$200" << std::endl;
            this->cash += 200;
        }
    }
    this->landOnSquare(nextPlot, board, mainPlayer, computers, cardManager, dieRoll);
    if (this->plotPosition + squaresToMove < board.plots.size())
        this->plotPosition += squaresToMove;
    else
        this->plotPosition = this->plotPosition + squaresToMove - board.plots.size();
    functions::readStringInput("");
    if (dieRoll[0] == dieRoll[1]) {
        functions::printlnBlue(this->name + " rolled a double.");
        this->doubles++;
        if (this->doubles == 3) {
            this->doubles = 0;
            functions::printlnRed(this->name + " has been speeding and must go to jail!");
            functions::readStringInput("");
            this->plotPosition = 10;
            this->inJail = true;
            return;
        }
        functions::readStringInput("");
        this->movePlayer(board, mainPlayer, computers, cardManager);
    }
}

void player::Player::reduceMoney(int amount, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, bool doesOwe, player::Player& oweTo) {
    if (!this->inGame)
        return;
    if (this->cash - amount < 0) {
        if (this->isMainPlayer) {
            functions::printlnRed("It seems that you don't have enough money to buy this.");
            canMakeMoney:
            if (this->moneyCanMake() < amount) {
                functions::printlnRed("You can't fathom gathering this money.");
                checkCanSellCards:
                if (this->getOutOfJailFreeCards > 0) {
                    functions::printlnGreen("However there is still hope. You have " + std::to_string(this->getOutOfJailFreeCards) + " get out of jail free cards.");
                    functions::printlnBlue("You can attempt to trade them to one of the computers for some cash.");
                    functions::printlnBlue("You need to raise $" + std::to_string(this->cash - this->moneyCanMake()));
                    int amountToSell = functions::readIntInput("Enter an amount to sell your card(s): >", this->cash - this->moneyCanMake(), std::numeric_limits<int>::max());
                    for (player::Player p : computers) {
                        int pity = rand() % (10) + 1;
                        if ((amountToSell > 50 * this->getOutOfJailFreeCards || pity <= 7) && p.cash >= amountToSell) {
                            functions::printlnRed(p.name + " doesn't want to buy them.");
                        } else {
                            functions::printlnBlue(p.name + " wants to buy them.");
                            p.getOutOfJailFreeCards += this->getOutOfJailFreeCards;
                            this->getOutOfJailFreeCards = 0;
                            p.cash -= amountToSell;
                            this->cash += amountToSell;
                            goto canMakeMoney;
                        }
                    }
                    goto checkCanSellCards;
                } else {
                    functions::printlnBlue("You couldn't muster up the money!");
                    if (doesOwe) {
                        functions::printlnRed("You give all your properties to the person you owed");
                    } else {
                        functions::printlnRed("You give all your properties and money to the bank.");
                        functions::printlnRed("The properties are all auctioned off.");
                    }
                    functions::printlnRed("YOU LOSE");
                    functions::readStringInput("PRESS ENTER TO EXIT");
                    exit(0);
                }
            } else {
                functions::printlnBlue("You do the math and you can make the money you need. You just need to sell some stuff");
                functions::printlnCyan("You can either sell any houses you have at half price or mortgage properties.");
                functions::printlnMagenta("You need to sell houses evenly across the color set.");
                functions::printlnYellow("If you sell a hotel, you get half the value back and 4 houses get placed on that square");
                functions::printlnRed("To mortgage a property, you first need to sell all hotels and houses on it. To unmortage a property, you must pay the amount listed on the title card.");
                functions::printlnGreen("If someone lands on a unmortaged property, they don't have to pay rent.");
                displayProperties:
                if (this->cash >= amount) {
                    functions::printlnGreen("You can now pay the amount!");
                    this->cash -= amount;
                    return;
                }
                functions::printlnRed("Here are your properties:");
                for (int i = 0; i < this->ownedPlots.size(); i++) {
                    plot::Plot p = this->ownedPlots[i];
                    if (!functions::setContains(p.flags, "MORTGAGED")) {
                        std::cout << p.stringProperties.at("COLORCODE") << std::to_string(i) << p.stringProperties.at("NAME");
                        std::cout << " with a mortgage value of " << std::to_string(p.intProperties.at("MORTGAGEVALUE"));
                        std::cout << " and a total of " << std::to_string(p.intProperties.at("HOUSES")) << " houses and ";
                        std::cout << std::to_string(p.intProperties.at("HOTELS")) << " hotels." << functions::ANSI_RESET << std::endl;
                    }
                }
                functions::printlnRed("Enter the corresponding number to either morgage a property or sell hotels/houses");
                pay:
                int input = functions::readIntInput(">", 0, this->ownedPlots.size() - 1);
                plot::Plot pickedPlot = this->ownedPlots[input];
                if (functions::setContains(pickedPlot.flags, "MORTGAGED")) {
                    functions::printlnRed("That property is mortgaged. Try again");
                    goto pay;
                }
                if (pickedPlot.intProperties.at("HOTELS") > 0) {
                    functions::printlnRed("You got " + std::to_string(pickedPlot.intProperties.at("HOTELSCOST") / 2) + " for selling the hotel.");
                    this->cash += pickedPlot.intProperties.at("HOTELSCOST") / 2;
                    pickedPlot.intProperties.at("HOTELS") = 0;
                    pickedPlot.intProperties.at("HOUSES") = 4;
                    goto displayProperties;
                } else if (pickedPlot.intProperties.at("HOUSES") > 0) {
                    for (plot::Plot p : board.plots) {
                        if (functions::setContains(p.flags, "PROPERTYSQUARE") && p.stringProperties.at("COLORCODE") == pickedPlot.stringProperties.at("COLORCODE")) {
                            if (p.intProperties.at("HOUSES") < pickedPlot.intProperties.at("HOUSES") - 1) {
                                functions::printlnRed("You need to sell houses equally!");
                                goto displayProperties;
                            }
                        }
                    }
                    pickedPlot.intProperties.at("HOUSES") -= 1;
                    this->cash += pickedPlot.intProperties.at("HOUSESCOST") / 2;
                    goto displayProperties;
                } else {
                    pickedPlot.flags.insert("MORTGAGED");
                    this->cash += pickedPlot.intProperties.at("MORTGAGEVALUE");
                    goto displayProperties;
                }
            }
        } else {
            checkIfCanAfford:
            if (this->moneyCanMake() < amount) {
                if (this->getOutOfJailFreeCards > 0) {
                    functions::printlnBlue(this->name + " needs money, so he offers to sell you " + std::to_string(this->getOutOfJailFreeCards) + " get out of jail free cards.");
                    functions::printlnBlue("He will sell them to you for $" + std::to_string(this->cash - this->moneyCanMake()));
                    functions::printlnBlue("Enter 1 to accept and 2 to reject (Rejecting will cause the computer to go bankrupt)");
                    int input = functions::readIntInput(">", 1, 2);
                    if (input == 1) {
                        functions::printlnBlue("You got the cards");
                        this->cash += this->cash - this->moneyCanMake();
                        mainPlayer.cash -= this->cash - this->moneyCanMake();
                        mainPlayer.getOutOfJailFreeCards += this->getOutOfJailFreeCards;
                        this->getOutOfJailFreeCards = 0;
                        goto checkIfCanAfford;
                    } else {
                        this->computerBankruptcy(board, computers, mainPlayer, doesOwe, oweTo);
                        return;
                    }
                } else {
                    this->computerBankruptcy(board, computers, mainPlayer, doesOwe, oweTo);
                    return;
                }
            } else {
                forloop:
                for (int i = 0; i < this->ownedPlots.size(); i++) {
                    if (this->cash >= amount) {
                        this->cash -= amount;
                        return;
                    }
                    plot::Plot pickedPlot = this->ownedPlots[i];
                    if (functions::setContains(pickedPlot.flags, "MORTGAGED"))
                        continue;
                    if (pickedPlot.intProperties.at("HOTELS") > 0) {
                        this->cash += pickedPlot.intProperties.at("HOTELSCOST") / 2;
                        pickedPlot.intProperties.at("HOTELS") = 0;
                        pickedPlot.intProperties.at("HOUSES") = 4;
                    } else if (pickedPlot.intProperties.at("HOUSES") > 0) {
                        for (plot::Plot p : board.plots) {
                            if (functions::setContains(p.flags, "PROPERTYSQUARE") && p.stringProperties.at("COLORCODE") == pickedPlot.stringProperties.at("COLORCODE")) {
                                if (p.intProperties.at("HOUSES") < pickedPlot.intProperties.at("HOUSES") - 1) {
                                    continue;
                                }
                            }
                        }
                        pickedPlot.intProperties.at("HOUSES") -= 1;
                        this->cash += pickedPlot.intProperties.at("HOUSESCOST") / 2;
                    } else {
                        pickedPlot.flags.insert("MORTGAGED");
                        this->cash += pickedPlot.intProperties.at("MORTGAGEVALUE");
                    }
                }
                goto forloop;
            }
        }
    } else {
        this->cash -= amount;
    }
}

int player::Player::moneyCanMake() {
    int cashAvailable = this->cash;
    for (plot::Plot& p : this->ownedPlots) {
        cashAvailable += (p.intProperties.at("HOTELSCOST") / 2) * p.intProperties.at("HOTELS");
        cashAvailable += (p.intProperties.at("HOUSESCOST") / 2) * p.intProperties.at("HOUSES");
        cashAvailable += p.intProperties.at("UNMORTGAGEVALUE");
    }
    return cashAvailable;
}

bool player::Player::ownsPlot(plot::Plot& plot) {
    return plot.stringProperties.at("OWNER") == this->name;
}

void player::Player::computerBankruptcy(board::Board& board, std::vector<player::Player>& computers, player::Player& mainPlayer, bool doesOwe, player::Player& oweTo) {
    functions::printlnCyan(this->name + " has gone bankrupt.");
    this->inGame = false;
    if (doesOwe)
        for (plot::Plot p : this->ownedPlots)
            oweTo.ownedPlots.push_back(p);
    else
        for (plot::Plot p : this->ownedPlots)
            p.auction(board, mainPlayer, computers);
}

bool player::Player::ownsColorSet(std::string color) {
    unsigned char matching = 0;
    for (plot::Plot p : this->ownedPlots)
        if (p.stringProperties.at("COLORCODE") == color && !functions::setContains(p.flags, "PROPERTYSQUARE"))
            matching++;
    return color == "BROWN" || color == "BLUE" ? matching == 2 : matching == 3;
}

unsigned char player::Player::ownedRailroads() {
    unsigned char result = 0;
    for (plot::Plot p : this->ownedPlots)
        if (functions::setContains(p.flags, "RAILROAD"))
            result++;
    return result;
}

unsigned char player::Player::ownedUtilities() {
    unsigned char result = 0;
    for (plot::Plot p : this->ownedPlots)
        if (functions::setContains(p.flags, "UTILITYSQUARE"))
            result++;
    return result;
}

void player::Player::payRent(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, std::vector<unsigned char>& dieRoll) {
    player::Player whoOwns(false);
    int previousMoney = this->cash;
    if (mainPlayer.ownsPlot(nextPlot))
        whoOwns = mainPlayer;
    for (player::Player p : computers)
        if (p.ownsPlot(nextPlot))
            whoOwns = p;
    functions::printlnRed(whoOwns.name + " owns " + nextPlot.stringProperties.at("COLORCODE") + nextPlot.stringProperties.at("NAME") + functions::ANSI_RESET);
    if (nextPlot.stringProperties.at("OWNER") != this->name) {
        if (functions::setContains(nextPlot.flags, "PROPERTYSQUARE"))
            this->payRentOnProperty(nextPlot, board, mainPlayer, computers, whoOwns);
        else if (functions::setContains(nextPlot.flags, "RAILROAD"))
            this->payRentOnRailroad(nextPlot, board, mainPlayer, computers, whoOwns, false);
        else
            this->payRentOnUtility(nextPlot, board, mainPlayer, computers, whoOwns, dieRoll, 0);
        functions::printlnRed(this->name + " payed " + whoOwns.name + " $" + std::to_string(this->cash - previousMoney));
    }
}

void player::Player::payRentOnProperty(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, player::Player& whoOwns) {
    if (nextPlot.intProperties.at("HOTELS") == 1) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHHOTEL"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHHOTEL");
    } else if (nextPlot.intProperties.at("HOUSES") == 4) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHFOURHOUSES"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHFOURHOUSES");
    } else if (nextPlot.intProperties.at("HOUSES") == 3) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHTHREEHOUSES"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHTHREEHOUSES");
    } else if (nextPlot.intProperties.at("HOUSES") == 2) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHTWOHOUSES"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHTWOHOUSES");
    } else if (nextPlot.intProperties.at("HOUSES") == 1) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHONEHOUSE"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHONEHOUSE");
    } else if (this->ownsColorSet(nextPlot.stringProperties.at("COLORCODE"))) {
        this->reduceMoney(nextPlot.intProperties.at("RENTWITHCOLORSET"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENTWITHCOLORSET");
    } else {
        this->reduceMoney(nextPlot.intProperties.at("RENT"), board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += nextPlot.intProperties.at("RENT");
    }
}

void player::Player::payRentOnRailroad(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, player::Player& whoOwns, bool payTwice) {
    unsigned char ownedRailroads = this->ownedRailroads();
    switch (ownedRailroads) {
        case 1:
            this->reduceMoney(payTwice ? nextPlot.intProperties.at("RENT") * 2 : nextPlot.intProperties.at("RENT"), board, mainPlayer, computers, true, whoOwns);
            whoOwns.cash += payTwice ? nextPlot.intProperties.at("RENT") * 2 : nextPlot.intProperties.at("RENT");
            break;
        case 2:
            this->reduceMoney(payTwice ? nextPlot.intProperties.at("RENT2RROWNED") * 2 : nextPlot.intProperties.at("RENT2RROWNED"), board, mainPlayer, computers, true, whoOwns);
            whoOwns.cash += payTwice ? nextPlot.intProperties.at("RENT2RROWNED") * 2 : nextPlot.intProperties.at("RENT2RROWNED");
            break;
        case 3:
            this->reduceMoney(payTwice ? nextPlot.intProperties.at("RENT3RROWNED") * 2 : nextPlot.intProperties.at("RENT3RROWNED"), board, mainPlayer, computers, true, whoOwns);
            whoOwns.cash += payTwice ? nextPlot.intProperties.at("RENT3RROWNED") * 2 : nextPlot.intProperties.at("RENT3RROWNED");
            break;
        case 4:
            this->reduceMoney(payTwice ? nextPlot.intProperties.at("RENT4RROWNED") * 2 : nextPlot.intProperties.at("RENT4RROWNED"), board, mainPlayer, computers, true, whoOwns);
            whoOwns.cash += payTwice ? nextPlot.intProperties.at("RENT4RROWNED") * 2 : nextPlot.intProperties.at("RENT4RROWNED");
            break;
    }
}

void player::Player::payRentOnUtility(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, player::Player& whoOwns, std::vector<unsigned char> dieRoll, unsigned char timesAmount) {
    if (timesAmount != 0) {
        this->reduceMoney((dieRoll[0] + dieRoll[1]) * timesAmount, board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += (dieRoll[0] + dieRoll[1]) * timesAmount;
        return;
    }
    unsigned char ownedUtilities = this->ownedRailroads();
    if (ownedUtilities == 1) {
        this->reduceMoney((dieRoll[0] + dieRoll[1]) * 4, board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += (dieRoll[0] + dieRoll[1]) * 4;
    } else {
        this->reduceMoney((dieRoll[0] + dieRoll[1]) * 10, board, mainPlayer, computers, true, whoOwns);
        whoOwns.cash += (dieRoll[0] + dieRoll[1]) * 10;
    }
}

void player::Player::buyProperty(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    if (functions::setContains(nextPlot.flags, "PROPERTYSQUARE"))
        nextPlot.displayTitleCard();
    else if (functions::setContains(nextPlot.flags, "RAILROAD"))
        nextPlot.displayRailroadCard();
    else
        nextPlot.displayUtilityCard();
    this->ownedPlots.push_back(nextPlot);
    nextPlot.flags.insert("OWNEDPLOT");
    this->cash -= nextPlot.intProperties.at("PRICE");
    nextPlot.stringProperties["OWNER"] = this->name;
    functions::readStringInput("");
}

void player::Player::buyPropertyAsMainPlayer(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    std::cout << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << " is unowned." << std::endl;
    if (this->cash < nextPlot.intProperties.at("PRICE")) {
        functions::printlnRed("You don't have enough money to buy this!");
        functions::printlnRed("It will be auctioned");
        nextPlot.auction(board, mainPlayer, computers);
    } else {
        functions::printlnBlue("Do you want to buy it? It costs $" + std::to_string(nextPlot.intProperties.at("PRICE")) + " and you have $" + std::to_string(this->cash));
        functions::printlnGreen("Enter 1 to buy it or 2 to auction it.");
        int input = functions::readIntInput(">", 1, 2);
        if (input == 1) {
            std::cout << "You bought " << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << " and got a title card:" << std::endl;
            this->buyProperty(nextPlot, board, mainPlayer, computers);
        } else {
            nextPlot.auction(board, mainPlayer, computers);
        }
    }
}

void player::Player::buyPropertyAsComputer(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    if (this->cash >= nextPlot.intProperties.at("PRICE")) {
        std::cout << this->name << " bought " << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << " and got a title card:" << std::endl;
        this->buyProperty(nextPlot, board, mainPlayer, computers);
    } else {
        std::cout << this->name << " won't buy it, so it will be auctioned." << std::endl;
        nextPlot.auction(board, mainPlayer, computers);
    }
}

void player::Player::buyPropertySegment(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    if (this->isMainPlayer)
        this->buyPropertyAsMainPlayer(nextPlot, board, mainPlayer, computers);
    else
        this->buyPropertyAsComputer(nextPlot, board, mainPlayer, computers);
}

void player::Player::landOnSquare(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, card_managment::CardManagment& cardManager, std::vector<unsigned char> dieRoll) {
    std::cout << this->name << " landed on " << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << std::endl;
    for (std::string s : nextPlot.flags)
        std::cout << "WOW " << s << std::endl;
    if (functions::setContains(nextPlot.flags, "PROPERTYSQUARE") || functions::setContains(nextPlot.flags, "RAILROAD") || functions::setContains(nextPlot.flags, "UTILITYSQUARE")) {
        if (!functions::setContains(nextPlot.flags, "OWNEDPLOT"))
            this->buyPropertySegment(nextPlot, board, mainPlayer, computers);
        else if (functions::setContains(nextPlot.flags, "OWNEDPLOT") && !functions::setContains(nextPlot.flags, "MORTGAGED"))
            this->payRent(nextPlot, board, mainPlayer, computers, dieRoll);
        else
            functions::printlnBlue("Someone owns " + nextPlot.stringProperties.at("COLORCODE") + nextPlot.stringProperties.at("NAME") + functions::ANSI_RESET + " but it is morgaged so nothing happens");
    } else if (functions::setContains(nextPlot.flags, "TAXSQUARE")) {
        functions::printlnRed(this->name + " landed on " + nextPlot.stringProperties.at("NAME") + " and has to pay " + nextPlot.stringProperties.at("TEXT"));
        this->reduceMoney(nextPlot.intProperties.at("TAXAMOUNT"), board, mainPlayer, computers, false, computers[0]);
    } else if (functions::setContains(nextPlot.flags, "COMMUNITYCHEST")) {
        functions::printlnGreen(this->name + " landed on a Community Chest square.");
        cardManager.drawChestCard(board, mainPlayer, computers, *this);
    } else if (functions::setContains(nextPlot.flags, "CHANCE")) {
        functions::printlnGreen(this->name + " landed on a Chance square.");
        cardManager.drawChanceCard(board, mainPlayer, computers, *this);     
    } else if (functions::setContains(nextPlot.flags, "GOTOJAIL")) {
        functions::printlnRed(this->name + " is going to jail.");
        this->plotPosition = 10;
        this->inJail = true;
    }
}

void player::Player::whileInJail(board::Board& board, Player& mainPlayer, std::vector<Player>& computers, card_managment::CardManagment& cardManager) {
    functions::printlnRed(this->name + " has been in jail for " + std::to_string(this->turnsInJail) + " turns.");
    functions::printlnBlue("In order to get out, they must either pay $50 or roll a double.");
    functions::printlnGreen("If they cannot get out in three turns, they must pay $50 or go bankrupt.");
    functions::printlnCyan("They also can reedeem a get out of jail free card. They have " + std::to_string(this->getOutOfJailFreeCards));
    functions::printlnYellow(this->name + " has $" + std::to_string(this->cash));
    if (this->isMainPlayer) {
        functions::printlnMagenta("Enter (1) to pay $50 dollars, (2) to roll a dice or (3) to reedeem a get out of jail free card.");
        int input = functions::readIntInput(">", 1, 3);
        if (input == 1) {
            this->reduceMoney(50, board, mainPlayer, computers, false, mainPlayer);
            this->getOutOfJail(board, mainPlayer, computers, cardManager);
        } else if (input == 2) {
            std::vector<unsigned char> roll = board.rollDice();
            functions::printlnBlue(this->name + " rolled:");
            std::cout << "+---+ +---+" << std::endl;
            std::cout << "| " << std::to_string(roll[0]) << " | | " << std::to_string(roll[1]) << " |" << std::endl;
            std::cout << "+---+ +---+" << std::endl;
            if (roll[0] == roll[1]) {
                functions::printlnCyan("You got out of jail!");
                functions::readStringInput("");
                this->getOutOfJail(board, mainPlayer, computers, cardManager);
            } else {
                functions::printlnRed("You did not roll a double...");
                functions::readStringInput("");
                this->turnsInJail++;
                if (this->turnsInJail == 3) {
                    functions::printlnRed("It has been 3 turns, so you must pay $50");
                    functions::readStringInput("");
                    this->reduceMoney(50, board, mainPlayer, computers, false, mainPlayer);
                    this->getOutOfJail(board, mainPlayer, computers, cardManager);
                }
                return;
            }
        } else {
            if (this->getOutOfJailFreeCards = 0) {
                functions::printlnRed("You don't have any get out of jail free cards!");
                functions::readStringInput("");
                this->whileInJail(board, mainPlayer, computers, cardManager);
            }
            this->getOutOfJailFreeCards--;
            functions::printlnRed("You used one of your get out of jail free cards!");
            functions::readStringInput("");
            this->getOutOfJail(board, mainPlayer, computers, cardManager);
        }
    } else {
        if (this->cash >= 50 && this->getOutOfJailFreeCards == 0) {
            functions::printlnGreen(this->name + " payed $50 to get out of jail.");
            functions::readStringInput("");
            this->reduceMoney(50, board, mainPlayer, computers, false, mainPlayer);
            this->getOutOfJail(board, mainPlayer, computers, cardManager);
        } else if (this->getOutOfJailFreeCards > 0) {
            functions::printlnRed(this->name + " used one of their get out of jail free cards!");
            functions::readStringInput("");
            this->getOutOfJailFreeCards--;
            this->getOutOfJail(board, mainPlayer, computers, cardManager);
        } else {
            std::vector<unsigned char> roll = board.rollDice();
            functions::printlnBlue(this->name + " rolled:");
            std::cout << "+---+ +---+" << std::endl;
            std::cout << "| " << std::to_string(roll[0]) << " | | " << std::to_string(roll[1]) << " |" << std::endl;
            std::cout << "+---+ +---+" << std::endl;
            if (roll[0] == roll[1]) {
                functions::printlnCyan(this->name + " got out of jail!");
                functions::readStringInput("");
                this->getOutOfJail(board, mainPlayer, computers, cardManager);
            } else {
                functions::printlnRed(this->name + " did not roll a double...");
                functions::readStringInput("");
                this->turnsInJail++;
                if (this->turnsInJail == 3) {
                    functions::printlnRed("It has been 3 turns, so " + this->name + " must pay $50");
                    functions::readStringInput("");
                    this->reduceMoney(50, board, mainPlayer, computers, false, mainPlayer);
                    this->getOutOfJail(board, mainPlayer, computers, cardManager);
                }
                return;
            }
        }
    }
}

void player::Player::getOutOfJail(board::Board& board, Player& mainPlayer, std::vector<Player>& computers, card_managment::CardManagment& cardManager) {
    this->turnsInJail = 0;
    this->inJail = false;
    this->movePlayer(board, mainPlayer, computers, cardManager);
}

void player::Player::playerMenu(board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    while (true) {
        functions::printlnCyan("1: Buy Houses");
        functions::printlnCyan("2: Buy Hotels");
        functions::printlnCyan("3: View Opponents");
        functions::printlnCyan("4: View Title Cards");
        functions::printlnCyan("5: Trade with computers");
        functions::printlnCyan("6: Mortgage property");
        functions::printlnCyan("7: Sell Houses");
        functions::printlnCyan("8: Sell Hotels");
        functions::printlnCyan("9: Roll/End Turn");
        switch (functions::readIntInput(">", 1, 9)) {
            case 1:
                this->buyHouse(board, mainPlayer, computers);
                break;
            case 3:
                this->displayOpponents(board, computers);
                break;
        }
    }
}

void player::Player::displayOpponents(board::Board& board, std::vector<player::Player>& computers) {
    for (player::Player& p : computers) {
        if (!p.inGame)
            continue;
        functions::printlnBlue(p.name);
        functions::printlnMagenta("Located at " + board.getPlot(p.plotPosition).stringProperties.at("COLORCODE") + board.getPlot(p.plotPosition).stringProperties.at("NAMW") + functions::ANSI_RESET);
        functions::printlnYellow("Cash: " + p.cash);
        if (p.inJail)
            functions::printlnRed(p.name + " is in jail.");
        functions::printlnCyan(p.name + " owns:");
        for (plot::Plot& plt : this->ownedPlots)
            std::cout << plt.stringProperties.at("COLORCODE") << plt.stringProperties.at("NAME") << functions::ANSI_RESET << std::endl;
        functions::readStringInput("");
    }
}

void player::Player::buyHouse(board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    if (!this->canBuyBuilding()) {
        if (this->isMainPlayer) {
            functions::printlnRed("You do not own any color sets");
            functions::readStringInput("");
        }
        return;
    }
    if (this->isMainPlayer) {
        for (int i = 0; i < this->ownedPlots.size(); i++) {
            std::cout << this->ownedPlots[i].stringProperties.at("COLORCODE") << std::to_string(i + 1) << ": " << this->ownedPlots[i].stringProperties.at("NAME");
            std::cout << " with " << std::to_string(this->ownedPlots[i].intProperties.at("HOUSES")) << " houses.";
            std::cout << " A house there costs " << std::to_string(this->ownedPlots[i].intProperties.at("HOUSESCOST")) << functions::ANSI_RESET << std::endl;
        }
        functions::printlnRed("Enter 0 to exit");
        int input = functions::readIntInput(">", 0, this->ownedPlots.size());
        if (input == 0)
            return;
        if (!this->ownsColorSet(this->ownedPlots[input - 1].stringProperties.at("COLORCODE"))) {
            functions::printlnRed("You do not own that color set.");
            functions::readStringInput("");
            this->buyHouse(board, mainPlayer, computers);
        } else if (!this->canBuyHouseOnPlot(this->ownedPlots[input - 1])) {
            functions::printlnRed("You must build houses equally.");
            functions::readStringInput("");
            this->buyHouse(board, mainPlayer, computers);
        } else if (this->ownedPlots[input - 1].intProperties.at("HOUSES") == 4) {
            functions::printlnRed("You already have 4 houses there. Build a hotel instead.");
            functions::readStringInput("");
            this->buyHouse(board, mainPlayer, computers);
        } else if (this->ownedPlots[input - 1].intProperties.at("HOTELS") == 1) {
            functions::printlnRed("You already have a hotel there.");
            functions::readStringInput("");
            this->buyHouse(board, mainPlayer, computers);
        } else if (this->cash < this->ownedPlots[input - 1].intProperties.at("HOUSESCOST")) {
            functions::printlnRed("You can't afford a house there.");
            functions::readStringInput("");
            this->buyHouse(board, mainPlayer, computers);
        }
        this->ownedPlots[input - 1].intProperties.at("HOUSES") += 1;
        this->reduceMoney(this->ownedPlots[input - 1].intProperties.at("HOUSESCOST"), board, mainPlayer, computers, false, mainPlayer);
    } else {
        for (plot::Plot& p : this->ownedPlots) {
            if (
                this->ownsColorSet(p.stringProperties.at("COLORCODE")) &&
                this->canBuyHouseOnPlot(p) &&
                p.intProperties.at("HOUSES") != 4 &&
                p.intProperties.at("HOTELS") != 1 &&
                this->cash >= p.intProperties.at("HOUSESCOST")
            ) {
                functions::printlnBlue(this->name + " bought a house on " + p.stringProperties.at("NAME"));
                p.intProperties.at("HOUSES") += 1;
                this->reduceMoney(p.intProperties.at("HOUSESCOST"), board, mainPlayer, computers, false, mainPlayer);
                return;
            }
        }
    }
}

bool player::Player::canBuyBuilding() {
    for (plot::Plot& p : this->ownedPlots)
        if (this->ownsColorSet(p.stringProperties.at("COLORCODE")))
            return true;
    return false;
}

bool player::Player::canBuyHouseOnPlot(plot::Plot& plot) {
    for (plot::Plot& p : this->ownedPlots) {
        if (
            !(p.stringProperties.at("COLORCODE") == plot.stringProperties.at("COLORCODE") && 
            p.intProperties.at("HOUSES") == plot.intProperties.at("HOUSES") ||
            p.intProperties.at("HOUSES") - 1 == plot.intProperties.at("HOUSES"))
        )
            return false;
    }
    return true;
}

#endif
