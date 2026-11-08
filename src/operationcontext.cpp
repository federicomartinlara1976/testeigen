#include "operationcontext.hpp"

OperationContext::OperationContext(std::unique_ptr<OperationStrategy> &&strategy) : m_strategy(std::move(strategy)) {
    connectStrategy();
}

void OperationContext::set_strategy(std::unique_ptr<OperationStrategy> &&strategy) {
    if (m_strategy) {
        disconnect(m_strategy.get(), &OperationStrategy::diagnosticReady,
                   this, &OperationContext::diagnosticReady);
    }
    m_strategy = std::move(strategy);
    connectStrategy();
}

QString OperationContext::doOperation() const {
    if (!m_strategy) {
        return QStringLiteral("sin estrategia asignada");
    }
    return m_strategy->doOperation();
}

void OperationContext::connectStrategy() {
    if (m_strategy) {
        connect(m_strategy.get(), &OperationStrategy::diagnosticReady,
                this, &OperationContext::diagnosticReady);
    }
}
