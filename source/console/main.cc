//
// Aspia Project
// Copyright (C) 2016-2023 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include "build/version.h"
#include "console/application.h"
#include "console/main_window.h"
#include "qt_base/scoped_qt_logging.h"

#if defined(OS_WIN)
#include "base/win/mini_dump_writer.h"
#endif

#include <QCommandLineParser>

//--------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#if !defined(I18L_DISABLED)
    Q_INIT_RESOURCE(client);
    Q_INIT_RESOURCE(client_translations);
    Q_INIT_RESOURCE(common);
    Q_INIT_RESOURCE(common_translations);
#endif

#if defined(OS_WIN)
    base::installFailureHandler(L"aspia_console");
#endif

    base::LoggingSettings logging_settings;
    logging_settings.min_log_level = base::LOG_LS_INFO;

    qt_base::ScopedQtLogging scoped_logging(logging_settings);

    console::Application::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    console::Application::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    console::Application application(argc, argv);

    LOG(LS_INFO) << "Version: " << ASPIA_VERSION_STRING;
#if defined(GIT_CURRENT_BRANCH) && defined(GIT_COMMIT_HASH)
    LOG(LS_INFO) << "Git branch: " << GIT_CURRENT_BRANCH;
    LOG(LS_INFO) << "Git commit: " << GIT_COMMIT_HASH;
#endif
    LOG(LS_INFO) << "Qt version: " << QT_VERSION_STR;
    LOG(LS_INFO) << "Command line: " << application.arguments();

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::translate("Console", "Aspia Console"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QApplication::translate("Console", "file"),
        QApplication::translate("Console", "The file to open."));
    parser.process(application);

    std::unique_ptr<console::MainWindow> console_window;
    QStringList arguments = parser.positionalArguments();

    QString file_path;
    if (!arguments.isEmpty())
        file_path = arguments.front();

    if (application.isRunning())
    {
        if (file_path.isEmpty())
            application.activateWindow();
        else
            application.openFile(file_path);

        return 0;
    }
    else
    {
        console_window.reset(new console::MainWindow(file_path));

        QObject::connect(&application, &console::Application::sig_windowActivated,
            console_window.get(), &console::MainWindow::showConsole);

        QObject::connect(&application, &console::Application::sig_fileOpened,
            console_window.get(), &console::MainWindow::openAddressBook);

        console_window->show();
        console_window->activateWindow();
    }

    return application.exec();
}
