#include "plot.h"

#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace std;

Plot::Plot(unordered_set<string> flags, unordered_map<string, string> stringProperties, unordered_map<string, int> intProperties) {
    this->flags = flags;
    this->stringProperties = stringProperties;
    this->intProperties = intProperties;
}
