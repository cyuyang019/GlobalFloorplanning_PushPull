#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include "ppmodule.h"
#include "ppsolver.h"


int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        std::cout << "Not enough argument\n";
        return -1;
    }
    int dieWidth, dieHeight;
    int softModuleNum, fixedModuleNum;
    int connectionNum;
    PPSolver solver;

    //************************************************
    //               read input file
    //************************************************
    std::ifstream istream(argv[1]);
    if ( istream.fail() ) {
        std::cout << argv[1] << " doesn't exist.\n";
        return -1;
    }
    std::string s, m0, m1;
    int area, x, y, w, h, value;

    istream >> s >> dieWidth >> dieHeight;
    solver.setOutline(dieWidth, dieHeight);
    istream >> s >> softModuleNum;
    solver.setSoftModuleNum(softModuleNum);

    for ( int i = 0; i < softModuleNum; i++ ) {
        istream >> s >> area;
        PPModule* mod = new PPModule(s, dieWidth / 2., dieHeight / 2., (float) area, false);
        solver.addModule(mod);
        std::cout << "Reading Soft Module " << s << "..." << std::endl;
    }

    istream >> s >> fixedModuleNum;
    solver.setFixedModuleNum(fixedModuleNum);

    for ( int i = 0; i < fixedModuleNum; i++ ) {
        istream >> s >> x >> y >> w >> h;
        PPModule* mod = new PPModule(s, x + w / 2., y + h / 2., (float) w * h, true);
        solver.addModule(mod);
        std::cout << "Reading Fixed Module " << s << "..." << std::endl;
    }

    istream >> s >> connectionNum;

    for ( int i = 0; i < connectionNum; i++ ) {
        istream >> m0 >> m1 >> value;
        solver.addConnection(m0, m1, (float) value);
        std::cout << "Reading Connection " << m0 << "<->" << m1 << std::endl;
    }

    istream.close();


    //************************************************
    //               apply Hook's law
    //************************************************

    int iteration = ( argc >= 4 ) ? std::atoi(argv[3]) : 10000;

    for ( int i = 0; i < iteration; i++ ) {
        solver.calcModuleForce();
        solver.moveModule();
    }


    //************************************************
    //               output result
    //************************************************
    solver.currentPosition2txt(argv[2]);

    std::cout << "Dead Space: " << solver.calcDeadspace() << std::endl;
    std::cout << "Estimated HPWL: " << solver.calcEstimatedHPWL() << std::endl;

    return 0;
}