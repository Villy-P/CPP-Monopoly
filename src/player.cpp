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

void player::Player::movePlayer(board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers) {
    functions::clear();
    std::vector<unsigned char> dieRoll = board.rollDice();
    int squaresToMove = dieRoll[0] + dieRoll[1];
    std::string currentSquareColor =  board.getStringProperty(this->plotPosition, "COLORCODE");
    std::string currentSquareName = board.getStringProperty(this->plotPosition, "NAME");
    std::string currentSquareText = board.getStringProperty(this->plotPosition, "TEXT");
    std::string nextSquareColor = board.getStringProperty((this->plotPosition + squaresToMove), "COLORCODE");
    std::string nextSquareName = board.getStringProperty(this->plotPosition + squaresToMove, "NAME");
    std::string nextSquareText = board.getStringProperty(this->plotPosition + squaresToMove, "TEXT");
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
    std::cout << " " << functions::ANSI_GREEN << std::string(board.getPlot(this->plotPosition + squaresToMove).intProperties.at("HOUSES"), 'O') << functions::ANSI_RESET;
    std::cout << " " << functions::ANSI_RED << std::string(board.getPlot(this->plotPosition + squaresToMove).intProperties.at("HOTELS"), 'O') << functions::ANSI_RESET << std::endl;
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
    std::cout << this->name << " landed on " << nextSquareColor << nextSquareName << functions::ANSI_RESET << std::endl;
    functions::readStringInput("");
    this->plotPosition += squaresToMove;
    this->plotPosition = this->plotPosition >= (board.plots.size()) ? this->plotPosition - board.plots.size() : this->plotPosition;
    this->movePlayer(board, mainPlayer, computers);
}

void player::Player::buyProperty(board::Board& board, unsigned char squaresToMove) {
    board.getPlot(this->plotPosition + squaresToMove).displayTitleCard();
    board.getPlot(this->plotPosition + squaresToMove).flags.insert("OWNEDPLOT");
    this->ownedPlots.push_back(board.getPlot(this->plotPosition + squaresToMove));
    functions::readStringInput("");
}

void player::Player::reduceMoney(int amount, board::Board& board, player::Player mainPlayer, std::vector<player::Player> computers, bool doesOwe, player::Player oweTo) {
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
                        bankrupt:
                        functions::printlnCyan(this->name + " has gone bankrupt.");
                        computers.erase(computers.begin() + (std::find(computers.begin(), computers.end(), this) - computers.begin()));
                        if (doesOwe)
                            for (plot::Plot p : this->ownedPlots)
                                oweTo.ownedPlots.push_back(p);
                        else
                            for (plot::Plot p : this->ownedPlots)
                                p.auction(board, mainPlayer, computers);
                        return;
                    }
                } else {
                    goto bankrupt;
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

#endif
