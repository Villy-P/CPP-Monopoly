#ifndef PLOT_H
#define PLOT_H

#include <unordered_set>
#include <unordered_map>
#include <string>

class Plot {
    public:
        std::unordered_set<std::string> flags;
        std::unordered_map<std::string, std::string> stringProperties;
        std::unordered_map<std::string, int> intProperties;

        Plot(
            std::unordered_set<std::string> flags,
            std::unordered_map<std::string, std::string> stringProperties,
            std::unordered_map<std::string, int> intProperties
        );

        void displayTitleCard();
        std::string getRentWithHouseString(unsigned char number, std::string name, unsigned char titleCardLength);
        void auction(Board& board, Player player, std::vector<Player> computers);
};

#endif
