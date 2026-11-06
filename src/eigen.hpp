#ifndef __EIGEN_HPP__
#define __EIGEN_HPP__

#include <QString>

#include <Eigen/Dense>

class EigenManager {
public:
    template <typename Derived>
    QString matrixToString(const Eigen::DenseBase<Derived> &m);
};

#endif
