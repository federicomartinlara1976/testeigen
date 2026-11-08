#ifndef __OPERATIONCONTEXT_HPP__
#define __OPERATIONCONTEXT_HPP__

#include <memory>

#include "operationstrategy.hpp"

/**
 * The Context defines the interface of interest to clients.
 */

class OperationContext : public QObject
{
    Q_OBJECT
    /**
     * @var Strategy The Context maintains a reference to one of the Strategy
     * objects. The Context does not know the concrete class of a strategy. It
     * should work with all strategies via the Strategy interface.
     */
private:
    std::unique_ptr<OperationStrategy> m_strategy;

    void connectStrategy();

public:
    /**
     * Usually, the Context accepts a strategy through the constructor, but also
     * provides a setter to change it at runtime.
     */
    explicit OperationContext(std::unique_ptr<OperationStrategy> &&strategy = {});
    /**
     * Usually, the Context allows replacing a Strategy object at runtime.
     */
    void set_strategy(std::unique_ptr<OperationStrategy> &&strategy);
    /**
     * The Context delegates some work to the Strategy object instead of
     * implementing +multiple versions of the algorithm on its own.
     */
    QString doOperation() const;

Q_SIGNALS:
    void diagnosticReady(const QString &message);
};

#endif
