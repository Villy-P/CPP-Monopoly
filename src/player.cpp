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
    std::vector<unsigned char> dieRoll = board.rollDice();
    int squaresToMove = dieRoll[0] + dieRoll[1];
    std::string currentSquareColor =  board.getStringProperty(this->plotPosition, "COLORCODE");
    std::string currentSquareName = board.getStringProperty(this->plotPosition, "NAME");
    std::string currentSquareText = board.getStringProperty(this->plotPosition, "TEXT");
    std::string nextSquareColor = board.getStringProperty((this->plotPosition + squaresToMove), "COLORCODE");
    std::string nextSquareName = board.getStringProperty(this->plotPosition + squaresToMove, "NAME");
    std::string nextSquareText = board.getStringProperty(this->plotPosition + squaresToMove, "TEXT");
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
    functions::readStringInput("");
    this->plotPosition += squaresToMove;
    this->plotPosition = this->plotPosition >= (board.plots.size()) ? this->plotPosition - board.plots.size() : this->plotPosition;
    this->movePlayer(board, mainPlayer, computers, cardManager);
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
    for (plot::Plot p : this->ownedPlots) {
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
            this->payRentOnUtility(nextPlot, board, mainPlayer, computers, whoOwns, dieRoll);
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

void player::Player::payRentOnUtility(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers, player::Player& whoOwns, std::vector<unsigned char> dieRoll) {
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
            std::cout << "You bought " << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << std::endl;
            this->buyProperty(nextPlot, board, mainPlayer, computers);
        } else {
            nextPlot.auction(board, mainPlayer, computers);
        }
    }
}

void player::Player::buyPropertyAsComputer(plot::Plot& nextPlot, board::Board& board, player::Player& mainPlayer, std::vector<player::Player>& computers) {
    if (this->cash >= nextPlot.intProperties.at("PRICE")) {
        std::cout << this->name << " bought " << nextPlot.stringProperties.at("COLORCODE") << nextPlot.stringProperties.at("NAME") << functions::ANSI_RESET << std::endl;
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
        cardManager.drawChestCard();
    }
}

#endif
