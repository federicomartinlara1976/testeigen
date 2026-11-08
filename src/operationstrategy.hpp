#ifndef __OPERATIONSTRATEGY_HPP__
#define __OPERATIONSTRATEGY_HPP__

#include <QObject>
#include <QString>

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
    virtual QString doOperation() = 0;

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
    QString doOperation() override;
};

class LinearSystemsOperationStrategy : public OperationStrategy
{
    QString doOperation() override;
};

class EigenSolverOperationStrategy : public OperationStrategy
{
    QString doOperation() override;
};

class SvdOperationStrategy : public OperationStrategy
{
    QString doOperation() override;
};

class FactorizationOperationStrategy : public OperationStrategy
{
    QString doOperation() override;
};

class InteroperabilityOperationStrategy : public OperationStrategy
{
    QString doOperation() override;
};

#endif
