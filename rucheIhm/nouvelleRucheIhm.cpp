    #include "nouvelleRucheIhm.h"
#include "ui_nouvelleRucheIhm.h"
#include "baseDeDonnees.h"
#include <QMessageBox>

/**
  *
  * \file rucheIhm/rucheIhm.cpp
  *
  * \brief Définition de la classe NouvelleRucheIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la fenêtre principale
 *
 * \fn NouvelleRucheIhm::NouvelleRucheIhm
 *
 * \param parent QObject Adresse de l'objet Qt parent (0 = fenêtre principale)
 */
NouvelleRucheIhm::NouvelleRucheIhm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NouvelleRucheIhm)
{
    qDebug()<< Q_FUNC_INFO;
    ui->setupUi(this);
    setWindowTitle("Ruche 2019 - Création d'une ruche");

    bdd = BaseDeDonnees::getInstance();
    if(!bdd->estConnecte())
        bdd->connecter(BDD_NOMBASE, BDD_USERNAME, BDD_PASSWORD, BDD_SERVEUR);

    if(bdd->estConnecte())
    {
        // Une ruche correspond à un DeviceId attaché à un ApplicationID dans le réseau TTN
        // récupération des ApplicationID disponibles
        QString requeteRecuperationIdTTN = "SELECT idTTN, ApplicationID FROM TTN";
        bool retourRequeteRecuperationIdTTN = bdd->recuperer(requeteRecuperationIdTTN, listesTTN);
        if(retourRequeteRecuperationIdTTN)
        {
            for(int i=0;i<listesTTN.size();i++)
            {
                ui->comboBoxListeAppID->addItem(listesTTN.at(i).at(1));
            }
        }
        else
            ui->comboBoxListeAppID->addItem("");
    }

    QDate aujourdhui = QDate::currentDate();
    ui->dateEditMiseEnService->setDate(aujourdhui);

    connect(ui->boutonOk,SIGNAL(clicked(bool)),this,SLOT(recevoirDonneeAjoutRuche()));
    connect(ui->boutonAnnuler,SIGNAL(clicked(bool)),this,SLOT(annulerDonneesNouvelleRuche()));
}

/**
 * \brief Destructeur de la fenêtre principale
 *
 * \fn NouvelleRucheIhm::~NouvelleRucheIhm
 *
 */
NouvelleRucheIhm::~NouvelleRucheIhm()
{
    delete ui;
    BaseDeDonnees::detruireInstance();
}

/**
 * \fn       NouvelleRucheIhm::recevoirDonneeAjoutRuche
 * \brief    slot permetant l'emission des données rentrées par l'utilisateur
 */

void NouvelleRucheIhm::recevoirDonneeAjoutRuche()
{
    if(listesTTN.size() == 0)
    {
        QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Impossible d'ajouter la nouvelle ruche : TTN manquant !"));
        return;
    }

    QDate date = QDate::fromString(ui->dateEditMiseEnService->text(), "dd/MM/yyyy");
    QString requete = "INSERT INTO Ruche (idTTN, Nom, Description, DateMiseEnService, Adresse, Longitude, Latitude, DeviceID) VALUES ('" + listesTTN.at(ui->comboBoxListeAppID->currentIndex()).at(0) + "' , '" + ui->zoneDeTexteNom->text() + "', '" + ui->zoneDeTexteDescription->text() + "', '" + date.toString("yyyy-MM-dd") + "', '" + ui->zoneDeTexteAdresse->text() + "', '" + ui->zoneDeTexteLongitude->text() + "', '" + ui->zoneDeTexteLatitude->text() + "', '" + ui->zoneDeTexteDeviceID->text() + "')";
    qDebug()<< Q_FUNC_INFO << requete;
    bool retour = bdd->executer(requete);
    if(retour)
    {
        QString idRuche;
        requete = "SELECT idRuche FROM Ruche WHERE DeviceID='" + ui->zoneDeTexteDeviceID->text() + "'";
        retour = bdd->recuperer(requete, idRuche);
        if(retour)
        {
            requete = "INSERT INTO Seuils (idRuche, TemperatureIntMin, TemperatureIntMax, HumiditeIntMin, HumiditeIntMax, TemperatureExtMin, TemperatureExtMax, HumiditeExtMin, HumiditeExtMax, PressionMin, PressionMax, PoidsMin, PoidsMax, EnsoleillementMin, EnsoleillementMax, Charge) VALUES ('" + idRuche + "' , '" + "25." + "', '" + "35.0" + "', '" + "20" + "', '" + "30" + "', '" + "5" + "', '" + "35" + "', '" + "20" + "', '" + "35" + "', '" + "1000" + "', '" + "1200" + "', '" + "10" + "', '" + "100" + "', '" + "10" + "', '" + "1000" + "', '" + "25" + "')";
            retour = bdd->executer(requete);
        }
        nettoyerZoneDeTexte();
        accept();
    }
    else
        QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Impossible d'ajouter la nouvelle ruche !"));
}

/**
 * \fn       NouvelleRucheIhm::annulerDonneesNouvelleRuche
 * \brief    slot permetant grâce a la methode nettoyerZoneDeTexte d'anuler la saisi des lors que l'on appui sur le bouton annuler
 */

void NouvelleRucheIhm::annulerDonneesNouvelleRuche()
{
    nettoyerZoneDeTexte();
    reject();
}

/**
 * \fn       NouvelleRucheIhm::nettoyerZoneDeTexte
 * \brief    methode qui clear les zones de textes
 */

void NouvelleRucheIhm::nettoyerZoneDeTexte()
{
    ui->zoneDeTexteNom->clear();
    ui->zoneDeTexteDescription->clear();
    ui->zoneDeTexteAdresse->clear();
    ui->zoneDeTexteLongitude->clear();
    ui->zoneDeTexteLatitude->clear();
    ui->zoneDeTexteDeviceID->clear();
}

