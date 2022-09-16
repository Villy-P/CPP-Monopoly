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

        Plot();

        void displayTitleCard();
        std::string getRentWithHouseString();
};

#endif
