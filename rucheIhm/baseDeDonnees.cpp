/**
  *
  * @file basededonnees.cpp
  *
  * @brief Définition de la classe BaseDeDonnees
  *
  * @author Thierry Vaira
  *
  * @version 0.2
  *
  */

#include "baseDeDonnees.h"
#include "parametres.h"
#include <QDebug>
#include <QMessageBox>

BaseDeDonnees* BaseDeDonnees::baseDeDonnees = NULL;
QString BaseDeDonnees::typeBase = "QMYSQL";
int BaseDeDonnees::nbAcces = 0;

BaseDeDonnees::BaseDeDonnees(QString type)
{
    #ifdef DEBUG_BASEDEDONNEES
    qDebug() << Q_FUNC_INFO << type;
    #endif
    db = QSqlDatabase::addDatabase(type);
    typeBase = type;
}

BaseDeDonnees::~BaseDeDonnees()
{
    #ifdef DEBUG_BASEDEDONNEES
    qDebug() << Q_FUNC_INFO;
    #endif
}

BaseDeDonnees* BaseDeDonnees::getInstance(QString type)
{
    if(baseDeDonnees == NULL)
        baseDeDonnees = new BaseDeDonnees(type);

    nbAcces++;
    #ifdef DEBUG_BASEDEDONNEES
    qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
    #endif

    return baseDeDonnees;
}

void BaseDeDonnees::detruireInstance()
{
    // instance ?
    if(baseDeDonnees != NULL)
    {
        if(nbAcces > 0)
            nbAcces--;
        #ifdef DEBUG_BASEDEDONNEES
        qDebug() << Q_FUNC_INFO << "nbAcces restants" << nbAcces;
        #endif
        // dernier ?
        if(nbAcces == 0)
        {
            delete baseDeDonnees;
            baseDeDonnees = NULL;
        }
    }
}

bool BaseDeDonnees::estConnecte()
{
    QMutexLocker verrou(&mutex);
    return db.isOpen();
}

bool BaseDeDonnees::connecter(QString nomBase, QString username/*=BDD_USERNAME*/, QString password/*=BDD_PASSWORD*/, QString serveur/*=BDD_HOSTNAME*/)
{
    if(typeBase != "QMYSQL")
        return false;
    QMutexLocker verrou(&mutex);
    if(!db.isOpen())
    {
       db.setHostName(serveur);
       db.setUserName(username);
       db.setPassword(password);
       db.setDatabaseName(nomBase);

       #ifdef DEBUG_BASEDEDONNEES
       qDebug() << Q_FUNC_INFO;
       qDebug() << "HostName : " << db.hostName();
       qDebug() << "UserName : " << db.userName();
       qDebug() << "DatabaseName : " << db.databaseName();
       #endif
       if(db.open())
       {
           #ifdef DEBUG_BASEDEDONNEES
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Connexion réussie à %1").arg(db.hostName());
           #endif
           return true;
       }
       else
       {
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : impossible de se connecter à la base de données !");
           QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Impossible de se connecter à la base de données !"));
           return false;
       }
    }
    else
        return true;
}

bool BaseDeDonnees::estOuvert()
{
    QMutexLocker verrou(&mutex);
    return db.isOpen();
}

bool BaseDeDonnees::ouvrir(QString fichierBase)
{
    if(typeBase != "QSQLITE")
        return false;
    QMutexLocker verrou(&mutex);
    if(!db.isOpen())
    {
       db.setDatabaseName(fichierBase);

       #ifdef DEBUG_BASEDEDONNEES
       qDebug() << Q_FUNC_INFO << db.databaseName();       
       #endif
       if(db.open())
       {
           #ifdef DEBUG_BASEDEDONNEES
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Ouvertir réussie à %1").arg(db.databaseName());
           #endif

           return true;
       }
       else
       {
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : impossible d'ouvrir la base de données !");
           QMessageBox::critical(0, QString::fromUtf8("BaseDeDonnees"), QString::fromUtf8("Impossible d'ouvrir la base de données !"));
           return false;
       }
    }
    else
        return true;
}

/* pour : UPDATE, INSERT et DELETE */
bool BaseDeDonnees::executer(QString requete)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("UPDATE") || requete.contains("INSERT") || requete.contains("DELETE"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;

}

/* uniquement pour récupérer (SELECT) UN champ d'UN seul enregistrement
   Remarque : le champ est stocké dans un QString
 */
bool BaseDeDonnees::recuperer(QString requete, QString &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // on se positionne sur l'enregistrement
                r.first();

                // on vérifie l'état de l'enregistrement retourné
                if(!r.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif
                    return false;
                }

                // on récupère sous forme de QString la valeur du champ
                if(r.isNull(0))
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Aucun résultat !");
                    #endif
                    return false;
                }
                donnees = r.value(0).toString();
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/* uniquement pour récupérer (SELECT) plusieurs champs d'UN seul enregistrement
   Remarque : les différents champs sont stockés dans un QStringList
 */
bool BaseDeDonnees::recuperer(QString requete, QStringList &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery r;
    bool retour;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << QString::fromUtf8("<BaseDeDonnees::recuperer(QString, QStringList)> retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // on se positionne sur l'enregistrement
                r.first();

                // on vérifie l'état de l'enregistrement retourné
                if(!r.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif
                    return false;
                }

                // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                // et on les stocke dans une liste de QString
                for(int i=0;i<r.record().count();i++)
                    if(!r.isNull(i))
                        donnees << r.value(i).toString();
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/* uniquement pour récupérer (SELECT) un seul champ de plusieurs enregistrements
   Remarque : le champ des différents enregistrements est stocké dans un QVector de QString
 */
bool BaseDeDonnees::recuperer(QString requete, QVector<QString> &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery r;
    bool retour;
    QString data;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // pour chaque enregistrement
                while ( r.next() )
                {
                    // on récupère sous forme de QString la valeur du champs sélectionné
                    data = r.value(0).toString();

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(data);
                }
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}

/* uniquement pour récupérer (SELECT) plusieurs champs de plusieurs enregistrements
   Remarque : les différents champs des différents enregistrements sont stockés dans un QVector de QStringList
 */
bool BaseDeDonnees::recuperer(QString requete, QVector<QStringList> &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery r;
    bool retour;
    QStringList data;

    if(db.isOpen())
    {
        if(requete.contains("SELECT"))
        {
            retour = r.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(retour)).arg(requete);
            #endif
            if(retour)
            {
                // pour chaque enregistrement
                while ( r.next() )
                {
                    // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                    // et on les stocke dans une liste de QString
                    for(int i=0;i<r.record().count();i++)
                        data << r.value(i).toString();

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    /*for(int i=0;i<r.record().count();i++)
                        qDebug() << r.value(i).toString();*/
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(data);

                    // on efface la liste de QString pour le prochain enregistrement
                    data.clear();
                }
                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(r.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
        return false;
}
