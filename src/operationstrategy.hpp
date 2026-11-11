#ifndef __OPERATIONSTRATEGY_HPP__
#define __OPERATIONSTRATEGY_HPP__

#include <QObject>
#include <QString>
#include <Eigen/Dense>

// Entrada genérica para las estrategias: cada una usa lo que necesita
struct OperationInput {
    Eigen::MatrixXd A;   // matriz principal
    Eigen::VectorXd b;   // lado derecho (solo sistemas/SVD)
    Eigen::MatrixXd transform;  // opcional: transformación 4×4 (interop)
    Eigen::MatrixXd point;      // opcional: punto 3×1 (interop)
};

/**
 * The Strategy interface declares operations common to all supported versions
 * of some algorithm.
 *
 * The Context uses this interface to call the algorithm defined by Concrete
 * Strategies.
 */
class OperationStrategy : public QObject
{
    Q_OBJECT
public:
    virtual ~OperationStrategy() = default;
    virtual QString doOperation(const OperationInput &input) = 0;

Q_SIGNALS:
    void diagnosticReady(const QString &message);
};

/**
 * Concrete Strategies implement the algorithm while following the base Strategy
 * interface. The interface makes them interchangeable in the Context.
 */
class BasicOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

class LinearSystemsOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

class EigenSolverOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

class SvdOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

class FactorizationOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

class InteroperabilityOperationStrategy : public OperationStrategy
{
public:
    QString doOperation(const OperationInput &input = {}) override;
};

#endif
