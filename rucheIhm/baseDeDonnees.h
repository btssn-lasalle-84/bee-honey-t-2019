#ifndef BASEDEDONNEES_H
#define BASEDEDONNEES_H

/**
  *
  * @file rucheIhm/basededonnees.h
  *
  * @brief Déclaration de la classe BaseDeDonnees
  *
  * @author Thierry VAIRA
  *
  * @version 0.2
  *
  */

#include <QObject>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QMutex>
#include <QString>
#include "parametres.h"

//#define DEBUG_BASEDEDONNEES

class BaseDeDonnees : public QObject
{
    Q_OBJECT
    public:
        static BaseDeDonnees* getInstance(QString type="QMYSQL");
        static void detruireInstance();

        bool estConnecte();
        bool connecter(QString nomBase=BDD_NOMBASE, QString username=BDD_USERNAME, QString password=BDD_PASSWORD, QString serveur=BDD_SERVEUR);
        
        bool estOuvert();
        bool ouvrir(QString fichierBase);

        /* uniquement pour : UPDATE, INSERT et DELETE */
        bool executer(QString requete);

        /* uniquement pour : SELECT */
        bool recuperer(QString requete, QString &donnees); // 1 -> 1
        bool recuperer(QString requete, QStringList &donnees); // 1 -> 1..*
        bool recuperer(QString requete, QVector<QString> &donnees); // 1..* -> 1
        bool recuperer(QString requete, QVector<QStringList> &donnees); // 1..* -> 1..*

    private:
        BaseDeDonnees(QString type);
        ~BaseDeDonnees();

        static BaseDeDonnees*  baseDeDonnees;
        static QString         typeBase;
        static int             nbAcces;

        QSqlDatabase           db;
        QMutex                 mutex;
};

#endif // BASEDEDONNEES_H
