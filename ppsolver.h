#ifndef __PPSOLVER_H__
#define __PPSOLVER_H__

#include "ppmodule.h"
#include <vector>
#include <string>

class PPSolver {
private:
    float DieWidth, DieHeight;
    int softModuleNum, fixedModuleNum, moduleNum, connectionNum;
    std::vector<PPModule*> modules;
    std::vector<float> xForce, yForce;
    float xMaxMovement, yMaxMovement;
    float radiusRatio;
    float pushForce;
public:
    PPSolver();
    ~PPSolver();
    void setOutline(int width, int height);
    void setSoftModuleNum(int num);
    void setFixedModuleNum(int num);
    void setConnectionNum(int num);
    void addModule(PPModule* in_module);
    void addConnection(std::string ma, std::string mb, float value);
    void currentPosition2txt(std::string file_name);
    float calcDeadspace();
    void calcModuleForce();
    void moveModule();
    float calcEstimatedHPWL();
    void setRadiusRatio(float ratio);
    void setPushForce(float force);
    void setupPushForce(float amplification = 1);
};



#endif
