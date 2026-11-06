#ifndef __EIGEN_HPP__
#define __EIGEN_HPP__

#include <QString>

#include <QStringList>
#include <Eigen/Dense>

class EigenManager {
public:
    template <typename Derived>
    static QString matrixToString(const Eigen::DenseBase<Derived> &m) {
        QString out;
        for (Eigen::Index r = 0; r < m.rows(); ++r) {
            for (Eigen::Index c = 0; c < m.cols(); ++c) {
                if (c > 0) {
                    out += u'\t';
                }
                out += QString::number(static_cast<double>(m(r, c)), 'f', 3);
            }
            out += u'\n';
        }
        return out;
    }

    static QString diagnose(const Eigen::Ref<const Eigen::MatrixXd> &m);
};

#endif
