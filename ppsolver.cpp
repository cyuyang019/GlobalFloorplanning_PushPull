#include "ppsolver.h"

#include <fstream>
#include <iostream>
#include <cmath>

PPSolver::PPSolver() {
    softModuleNum = 0;
    fixedModuleNum = 0;
}

PPSolver::~PPSolver() {
    for ( int i = 0; i < modules.size(); i++ ) {
        delete modules[i];
    }
}

void PPSolver::setOutline(int width, int height) {
    DieWidth = (float) width;
    DieHeight = (float) height;
}

void PPSolver::setSoftModuleNum(int num) {
    softModuleNum = num;
    moduleNum = softModuleNum + fixedModuleNum;
    xForce.resize(moduleNum);
    yForce.resize(moduleNum);
}

void PPSolver::setFixedModuleNum(int num) {
    fixedModuleNum = num;
    moduleNum = softModuleNum + fixedModuleNum;
    xForce.resize(moduleNum);
    yForce.resize(moduleNum);
}

void PPSolver::addModule(PPModule* in_module) {
    modules.push_back(in_module);
}

void PPSolver::addConnection(std::string ma, std::string mb, float value) {
    PPModule* m0;
    PPModule* m1;
    for ( int i = 0; i < modules.size(); i++ ) {
        if ( modules[i]->name == ma )
            m0 = modules[i];
        else if ( modules[i]->name == mb )
            m1 = modules[i];
    }
    m0->addConnection(m1, value);
    m1->addConnection(m0, value);
}

void PPSolver::currentPosition2txt(std::string file_name) {
    std::ofstream ostream(file_name);
    ostream << moduleNum << std::endl;
    ostream << DieWidth << " " << DieHeight << std::endl;
    for ( int i = 0; i < moduleNum; i++ ) {
        ostream << modules[i]->name << " ";
        ostream << !( modules[i]->fixed ) << " ";
        ostream << modules[i]->x << " " << modules[i]->y << " ";
        ostream << modules[i]->radius << std::endl;
    }
    ostream.close();
}

float PPSolver::calcDeadspace() {
    float dieArea = DieWidth * DieHeight;
    float moduleArea = 0;
    for ( int i = 0; i < moduleNum; i++ ) {
        moduleArea += modules[i]->area;
    }
    return 1. - moduleArea / dieArea;
}

float calcDistance(PPModule* ma, PPModule* mb) {
    float x1 = ma->x, x2 = mb->x;
    float y1 = ma->y, y2 = mb->y;
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void PPSolver::calcModuleForce() {
    PPModule* curModule;
    for ( int i = 0; i < moduleNum; i++ ) {
        if ( modules[i]->fixed == true )
            continue;

        curModule = modules[i];
        float x_force = 0;
        float y_force = 0;

        for ( int j = 0; j < curModule->connections.size(); j++ ) {
            PPModule* pullModule = curModule->connections[j]->module;
            float pullValue = curModule->connections[j]->value;
            float x_distance = pullModule->x - curModule->x;
            float y_distance = pullModule->y - curModule->y;
            float angle = std::atan2(y_distance, x_distance);
            if ( x_distance == 0 || y_distance == 0 )
                continue;

            float distance = calcDistance(curModule, pullModule);
            if ( distance < curModule->radius + pullModule->radius )
                continue;

            distance -= curModule->radius + pullModule->radius;
            x_distance = distance * std::cos(angle);
            y_distance = distance * std::sin(angle);
            float force = distance * pullValue;
            x_force += force * ( x_distance / distance );
            y_force += force * ( y_distance / distance );
        }

        for ( int j = 0; j < moduleNum; j++ ) {
            if ( j == i )
                continue;
            PPModule* pushModule = modules[j];
            float x_distance = pushModule->x - curModule->x;
            float y_distance = pushModule->y - curModule->y;
            float angle = std::atan2(y_distance, x_distance);
            if ( x_distance == 0 || y_distance == 0 )
                continue;

            float distance = calcDistance(curModule, pushModule);
            if ( distance > curModule->radius + pushModule->radius )
                continue;

            distance -= curModule->radius + pushModule->radius;
            x_distance = distance * std::cos(angle);
            y_distance = distance * std::sin(angle);
            float force = 1e5 * distance;
            x_force -= force * ( x_distance / distance );
            y_force -= force * ( y_distance / distance );
        }

        xForce[i] = x_force;
        yForce[i] = y_force;
    }

    //for ( int i = 0; i < moduleNum; i++ ) {
    //    std::cout << modules[i]->name << ": " << xForce[i] << " " << yForce[i] << std::endl;
    //}
}

void PPSolver::setTimeSpan(float in_time) {
    timeSpan = in_time;
}

void PPSolver::setModuleMass(float in_mass) {
    moduleMass = in_mass;
}

void PPSolver::moveModule() {
    PPModule* curModule;
    for ( int i = 0; i < moduleNum; i++ ) {
        if ( modules[i]->fixed == true )
            continue;

        curModule = modules[i];

        curModule->x += xForce[i] * timeSpan;
        curModule->y += yForce[i] * timeSpan;

        if ( curModule->x < curModule->radius )
            curModule->x = curModule->radius;
        if ( curModule->y < curModule->radius )
            curModule->y = curModule->radius;
        if ( curModule->x > DieWidth - curModule->radius )
            curModule->x = DieWidth - curModule->radius;
        if ( curModule->y > DieHeight - curModule->radius )
            curModule->y = DieHeight - curModule->radius;
    }
}

void PPSolver::updateTimeSpan() {
    timeSpan *= 1.0003;
    std::cout << "current time span: " << timeSpan << std::endl;
}