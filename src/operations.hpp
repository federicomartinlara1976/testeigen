#ifndef __OPERATIONS_HPP__
#define __OPERATIONS_HPP__

#include "testeigenhelper.hpp"

class QString;

class Operations {
private:
    TestEigenHelper helper;

public:
    QString runBasics();
    QString runLinearSystems();
    QString runEigenSolver();
    QString runSvd();
    QString runFactorizations();
};

#endif
