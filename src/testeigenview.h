/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef TESTEIGENVIEW_H
#define TESTEIGENVIEW_H

#include "ui_testeigenview.h"

#include "operationcontext.hpp"


/**
 * This class serves as the main window for testeigen.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class TestEigenView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    explicit TestEigenView(QWidget *parent);

    QString accumulatedDiagnostics() const;

    void applySettings();

    /**
     * Default Destructor
     */
    ~TestEigenView() = default;

Q_SIGNALS:
    void diagnosticReady(const QString &message);

private:
    // this is the name of the root widget inside our Ui file
    // you can rename it in designer and then change it here
    Ui::TestEigenView m_ui;

    QStringList m_diagnostics;

    OperationContext m_context;

private Q_SLOTS:
    void emitDiagnostics(const QString& message);
    void loadResults();
    void initInteractiveTables();
    void calculateLinearSystems();
    void calculateSvd();

};

#endif // TESTEIGENVIEW_H
