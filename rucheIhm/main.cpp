#include "rucheIhm.h"
#include <QApplication>
#include <QString>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <baseDeDonnees.h>


/**
 * \file rucheIhm/main.cpp
 *
 * \brief Programme principal
 *
 * \details Crée et affiche la fenêtre principale de l'application
 *
 * \author Florentin Mellah, Enzo Rossi
 *
 * \version 1.1
 *
 * \fn main(int argc, char *argv[])
 * \param argc
 * \param argv[]
 * \return int
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    RucheIhm w;
    w.show();

    return a.exec();
}
