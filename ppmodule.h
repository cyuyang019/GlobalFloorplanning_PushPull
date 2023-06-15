#ifndef __PPMODULE_H__
#define __PPMODULE_H__

#include <string>
#include <vector>

struct PPModule;

struct Connection {
    PPModule* module;
    float value;
};

struct PPModule {
    std::string name;
    float x, y;
    float area, radius;
    bool fixed;
    std::vector<Connection*> connections;
    PPModule(std::string in_name, float in_x, float in_y, float in_area, bool in_fixed);
    ~PPModule();
    void addConnection(PPModule* in_module, float in_value);
};

#endif