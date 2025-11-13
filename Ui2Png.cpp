/***********************************************************************************************************************
** Qt UI to PNG File Converter
** Ui2Png.cpp
** Copyright © KirHut Software Company
**
** This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
** License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
** version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
** details.
**
** You should have received a copy of the GNU General Public License along with this program.  If not, see
** <http://www.gnu.org/licenses/>.
***********************************************************************************************************************/
#include "Ui2Png.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QImage>
#include <QLocale>
#include <QTranslator>
#include <QUiLoader>
#include <QWidget>

struct Ui2Png::Impl
{
    int argCount;
    QApplication coreApp;
    QTranslator translator;
    QCommandLineParser parser;
    QTextStream out;

    Impl(int argc, char **argv)
        : argCount(argc)
        , coreApp(argCount, argv)
        , out(stdout)
    {
        coreApp.setApplicationName("Ui2Png");
        coreApp.setApplicationVersion(UI2PNG_VERSION_STRING);

        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "ui2png_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                coreApp.installTranslator(&translator);
                break;
            }
        }

        parser.setApplicationDescription(tr("Generate PNG images from a Qt .ui file."));
        parser.addPositionalArgument(tr("source"), tr("The .ui file to generate a PNG from."));
        parser.addPositionalArgument(
            tr("destination"),
            tr("The filename or directory of the output PNG file (optional)."),
            tr("[destination]"));
        parser.addHelpOption();
        parser.addVersionOption();

        parser.addOptions(
            QList<QCommandLineOption>{{QStringList{"f", "force"}, tr("Overwrite existing file.")},
                                      {QStringList{"s", "size"},
                                       tr("Set height (HHH) and width (WWW) of output"),
                                       tr("WWWxHHH"),
                                       "640x480"},
                                      {QStringList{"q", "quality"},
                                       tr("Set PNG compression quality between 0 and 100."),
                                       tr("quality")}});
    }

    QSize getUserSize()
    {
        if (parser.isSet("s")) {
            QString userInput = parser.value("s");
            QStringList dimensions = userInput.split('x');
            if (dimensions.size() == 2) {
                int width = dimensions.at(0).toInt();
                int height = dimensions.at(1).toInt();
                if (width > 15 and height > 15) {
                    return {width, height};
                }
            }

            out << tr("The dimensions provided (%1) are invalid, using default.").arg(userInput);
        }

        return {640, 480};
    }

    int run()
    {
        parser.process(coreApp);
        const QStringList args = parser.positionalArguments();
        if (args.empty())
            parser.showHelp(0);

        QFile source(args.front());
        QFile destination(args.size() > 1 ? args[1] : args[0] + ".png");

        if (not source.exists()) {
            out << tr("Given source file does not exist, please select a valid Qt .ui file.");
            out.flush();
            parser.showHelp(1);
        }
        if (destination.exists() and not parser.isSet("f")) {
            out << tr("Destination file already exists (use '-f' or '--force' to overwrite).")
                << "\n";
            out.flush();
            parser.showHelp(0);
        }

        QUiLoader uiLoader;

        QSize windowSize = getUserSize();

        auto input = std::unique_ptr<QWidget>(uiLoader.load(&source));
        if (not input) {
            out << tr("Reading input UI file failed: %1").arg(source.fileName()) << "\n";
            out << uiLoader.errorString() << "\n";
            out.flush();
            return 1;
        }

        QImage buffer(windowSize, QImage::Format_RGB32);
        buffer.fill(0);
        input->resize(windowSize);
        input->render(&buffer);

        bool didSave = false;
        if (QString qualityString = parser.value("q"); not qualityString.isEmpty()) {
            if (int quality = qualityString.toInt(&didSave);
                didSave and quality >= 0 and quality <= 100) {
                didSave = buffer.save(&destination, "PNG", quality);
            } else {
                out << tr("The given quality (%1) was invalid, using default.").arg(qualityString)
                    << "\n";
                didSave = buffer.save(&destination, "PNG");
            }
        } else
            didSave = buffer.save(&destination, "PNG");

        if (not didSave) {
            out << tr("Saving the PNG file to its destination failed.") << "\n";
            return 1;
        }

        return 0;
    }

    Q_DECLARE_TR_FUNCTIONS(Ui2Png::Impl)
};

Ui2Png::Ui2Png(int argc, char **argv)
    : im(std::make_unique<Ui2Png::Impl>(argc, argv))
{
    // No further implementation.
}

Ui2Png::~Ui2Png() noexcept
{
    // No further implementation.
}

int Ui2Png::run()
{
    return im->run();
}
