/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// application header
#include "testeigenwindow.h"
#include "testeigendebug.h"

// KF headers
#include <KCrash>
#include <KDBusService>
#include <KAboutData>
#include <KLocalizedString>

// Qt headers
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QLoggingCategory>


int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    KLocalizedString::setApplicationDomain("testeigen");
    KCrash::initialize();

    KAboutData aboutData(QStringLiteral("testeigen"),
                         i18n("TestEigen"),
                         i18n("%{VERSION}"),
                         i18n("Aplicación para probar funcionalidades de Eigen 3.4"),
                         KAboutLicense::GPL,
                         i18n("Copyright %{CURRENT_YEAR}, %{AUTHOR} <%{EMAIL}>"));

    aboutData.addAuthor(i18n("%{AUTHOR}"),i18n("Author"), QStringLiteral("%{EMAIL}"));
    aboutData.setOrganizationDomain("chronos.bounceme.net");
    aboutData.setDesktopFileName(QStringLiteral("net.bounceme.chronos.testeigen"));

    KAboutData::setApplicationData(aboutData);
    application.setWindowIcon(QIcon::fromTheme(QStringLiteral("testeigen")));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.process(application);
    aboutData.processCommandLine(&parser);

    KDBusService appDBusService(KDBusService::Multiple | KDBusService::NoExitOnFailure);

    TestEigenWindow *window = new TestEigenWindow;
    window->show();

    return application.exec();
}
