#ifndef __OPERATIONS_HPP__
#define __OPERATIONS_HPP__

#include "testeigenhelper.hpp"

#include <QObject>
#include <QString>

class Operations : public QObject {
    Q_OBJECT
public:
    explicit Operations(QObject *parent = nullptr);

    QString runBasics();
    QString runLinearSystems();
    QString runEigenSolver();
    QString runSvd();
    QString runFactorizations();
    QString runInteroperability();

Q_SIGNALS:
    void diagnosticReady(const QString &message);
};

#endif
