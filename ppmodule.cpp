#include "ppmodule.h"
#include <cmath>

PPModule::PPModule(std::string in_name, float in_x, float in_y, float in_area, bool in_fixed) {
    name = in_name;
    x = in_x;
    y = in_y;
    area = in_area;
    fixed = in_fixed;
    radius = std::sqrt(area / 3.1415926);
}

PPModule::~PPModule() {
    for ( int i = 0; i < connections.size(); i++ ) {
        delete connections[i];
    }
}

void PPModule::addConnection(PPModule* in_module, float in_value) {
    Connection* nc = new Connection;
    nc->module = in_module;
    nc->value = in_value;
    connections.push_back(nc);
}