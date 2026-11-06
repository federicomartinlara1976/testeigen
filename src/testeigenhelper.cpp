#include "testeigenhelper.hpp"

#include <QStringBuilder>

using namespace Qt::Literals::StringLiterals;

QString TestEigenHelper::section(const QString &title, const QString &body) {
    return u"== %1 ==\n%2\n"_s.arg(title, body);
}
