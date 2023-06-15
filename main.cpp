#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include "ppmodule.h"
#include "ppsolver.h"
using namespace std;


int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        cout << "Not enough argument\n";
        return -1;
    }
    int dieWidth, dieHeight;
    int softModuleNum, fixedModuleNum;
    int connectionNum;
    PPSolver solver;

    //************************************************
    //               read input file
    //************************************************
    ifstream istream(argv[1]);
    if ( istream.fail() ) {
        cout << argv[1] << " doesn't exist.\n";
        return -1;
    }
    string s, m0, m1;
    int area, x, y, w, h, value;

    istream >> s >> dieWidth >> dieHeight;
    solver.setOutline(dieWidth, dieHeight);
    istream >> s >> softModuleNum;
    solver.setSoftModuleNum(softModuleNum);

    for ( int i = 0; i < softModuleNum; i++ ) {
        istream >> s >> area;
        PPModule* mod = new PPModule(s, dieWidth / 2., dieHeight / 2., (float) area, false);
        solver.addModule(mod);
        cout << "Reading Soft Module " << s << "..." << endl;
    }

    istream >> s >> fixedModuleNum;
    solver.setFixedModuleNum(fixedModuleNum);

    for ( int i = 0; i < fixedModuleNum; i++ ) {
        istream >> s >> x >> y >> w >> h;
        PPModule* mod = new PPModule(s, x + w / 2., y + h / 2., (float) w * h, true);
        solver.addModule(mod);
        cout << "Reading Fixed Module " << s << "..." << endl;
    }

    istream >> s >> connectionNum;

    for ( int i = 0; i < connectionNum; i++ ) {
        istream >> m0 >> m1 >> value;
        solver.addConnection(m0, m1, (float) value);
        cout << "Reading Connection " << m0 << "<->" << m1 << endl;
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

    cout << "Dead Space: " << solver.calcDeadspace() << endl;

    return 0;
}