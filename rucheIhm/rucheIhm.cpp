 #include "rucheIhm.h"
#include "ui_rucheihm.h"
#include "ruche.h"
#include "baseDeDonnees.h"
#include "nouvelleRucheIhm.h"
#include "alertes.h"
#include "reglagesAlertesIhm.h"

#include <qwt_color_map.h>
#include <qwt_thermo.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_samples.h>
#include <qwt_plot_item.h>
#include <qwt_plot_marker.h>
#include <QDebug>

/**
  *
  * \file rucheIhm/nouvelleRucheIhm.cpp
  *
  * \brief Définition de la classe RucheIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la fenêtre principale
 *
 * \fn RucheIhm::RucheIhm
 *
 * \param parent QObject Adresse de l'objet Qt parent (0 = fenêtre principale)
 */
RucheIhm::RucheIhm(QWidget *parent) : QWidget(parent), ui(new Ui::RucheIhm), maRuche(0), bdd(0), nouvelleRuche(0), reglagesAlertesIhm(0)
{
    qDebug()<< Q_FUNC_INFO;
    // Initialisation de l'IHM
    ui->setupUi(this);

    // Initialisation des widgets
    inititialiserCourbes();
    initialiserWidgets();

    // Affichage plein écran
    const int width = qApp->desktop()->availableGeometry(this).width(); // ou : qApp->desktop()->width()
    const int height = qApp->desktop()->availableGeometry(this).height(); // ou : qApp->desktop()->height()
    resize(width, height);

    // Ajout de l'action Quitter
    QAction *actionQuitter = new QAction("&Quitter", this);
    actionQuitter->setShortcut(QKeySequence(QKeySequence::Quit)); // Ctrl+Q
    addAction(actionQuitter);

    // Connexion signaux/slots
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quitter()));
    connect(ui->comboBoxChoixRuche, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionnerRuche(int)));
    connect(ui->pushButtonNouvelleRuche, SIGNAL(clicked()), this, SLOT(creerNouvelleRuche()));
    connect(ui->pushButtonReglages, SIGNAL(clicked()), this, SLOT(ouvrirReglagesAlertes()));
    connect(ui->pushButtonSuppressionRuche, SIGNAL(clicked()), this, SLOT(supprimerRuche()));

    bdd = BaseDeDonnees::getInstance();
    if(!bdd->estConnecte())
        bdd->connecter(BDD_NOMBASE, BDD_USERNAME, BDD_PASSWORD, BDD_SERVEUR);

    nouvelleRuche = new NouvelleRucheIhm(this);
    reglagesAlertesIhm = new ReglagesAlertesIhm(this);

    mettreAjourListeRuches();
    //ui->comboBoxChoixRuche->setCurrentIndex(1);
}

/**
 * \brief Destructeur de la fenêtre principale
 *
 * \fn RucheIhm::~RucheIhm
 *
 */
RucheIhm::~RucheIhm()
{
    delete ui;
    BaseDeDonnees::detruireInstance();
    qDebug()<< Q_FUNC_INFO;
}

/**
 * \brief
 *
 * \fn RucheIhm::quitter
 *
 */
void RucheIhm::quitter()
{
    // Fermeture de la fenêtre
    close();
}

/**
 * \brief initialise les thermometres
 *
 * \fn RucheIhm::initialiserThermometre
 *
 */

void RucheIhm::initialiserThermometre() const
{
    ui->thermoTemperatureInterieur->setUpperBound(50);
    ui->thermoTemperatureInterieur->setLowerBound(-10);
    ui->thermoTemperatureInterieur->setScaleStepSize(5);
    ui->thermoTemperatureInterieur->setPipeWidth(30);
    ui->thermoTemperatureInterieur->alarmEnabled();

    ui->thermoTemperatureExterieur->setUpperBound(50);
    ui->thermoTemperatureExterieur->setLowerBound(-10);
    ui->thermoTemperatureExterieur->setScaleStepSize(5);
    ui->thermoTemperatureExterieur->setPipeWidth(30);
    ui->thermoTemperatureExterieur->alarmEnabled();

    // Initialisations Couleurs
    QPalette paletteCouleur = ui->thermoTemperatureInterieur->palette();
    paletteCouleur.setColor(QPalette::ButtonText, QColor(0,128,0));
    ui->thermoTemperatureInterieur->setPalette(paletteCouleur);
    ui->thermoTemperatureExterieur->setPalette(paletteCouleur);
}

/**
 * \brief Déconnecte les signaux
 *
 * \fn RucheIhm::deconnecterSignaux
 *
 */
void RucheIhm::deconnecterSignaux()
{
    /**********************************************SLOT AFFICHAGE MESURE**********************************************************************/
    disconnect(maRuche,SIGNAL(nouvelleMesureTemperatureInterieurTraite(double,QString)),this,SLOT(afficherTemperatureInterieur(double,QString)));
    disconnect(maRuche,SIGNAL(nouvelleMesureHumiditeInterieurTraite(double,QString)),this,SLOT(afficherHumiditeInterieur(double,QString)));
    disconnect(maRuche,SIGNAL(nouvelleMesureTemperatureExterieurTraite(double,QString)),this,SLOT(afficherTemperatureExterieur(double,QString)));
    disconnect(maRuche,SIGNAL(nouvelleMesureHumiditeExterieurTraite(double,QString)),this,SLOT(afficherHumiditeExterieur(double,QString)));
    disconnect(maRuche,SIGNAL(nouvellePressionAtmospheriqueTraite(double,QString)),this,SLOT(afficherPressionAtmospherique(double,QString)));
    disconnect(maRuche,SIGNAL(nouvelleMesureEnsoleillementTraite(double,QString)),this,SLOT(afficherEnsoleillement(double,QString)));
    disconnect(maRuche, SIGNAL(nouvelleMesureCharge(double, QString)),this, SLOT(afficherBatterie(double, QString)));
    disconnect(maRuche, SIGNAL(nouvelleMesurePoids(double, QString)), this, SLOT(afficherPoids(double, QString)));
    /**********************************************SLOT AFFICHAGE Alertes*********************************************************************/
    disconnect(maRuche,SIGNAL(envoiAlertesTemperatureInterieur(SeuilsAlertes)),this,SLOT(afficherAlertesTemperatureInterieur(SeuilsAlertes)));
    disconnect(maRuche,SIGNAL(envoiAlertesTemperatureExterieur(SeuilsAlertes)),this,SLOT(afficherAlertesTemperatureExterieur(SeuilsAlertes)));
    disconnect(maRuche,SIGNAL(envoiAlertesHumiditeInterieur(SeuilsAlertes)),this,SLOT(afficherAlertesHumiditeInterieur(SeuilsAlertes)));
    disconnect(maRuche,SIGNAL(envoiAlertesHumiditeExterieur(SeuilsAlertes)),this,SLOT(afficherAlertesHumiditeExterieur(SeuilsAlertes)));
    disconnect(maRuche,SIGNAL(envoiAlertesPressionAtmospherique(SeuilsAlertes)),this,SLOT(afficherAlertesPressionAtmospherique(SeuilsAlertes)));
    disconnect(maRuche, SIGNAL(envoiAlertesPoids(SeuilsAlertes)),this, SLOT(afficherAlertesPoids(SeuilsAlertes)));
    disconnect(maRuche, SIGNAL(envoiAlertesEnsoleillement(SeuilsAlertes,double)),this,SLOT(afficherAlertesEnsoleillement(SeuilsAlertes, double)));
    disconnect(maRuche, SIGNAL(envoiAlertesBatterie(SeuilsAlertes, double)),this, SLOT(afficherAlertesBatterie(SeuilsAlertes,double)));
    /**********************************SLOT Courbes*********************************/
    disconnect(maRuche, SIGNAL(nouvelleMesureTemperatureInterieurTraiteParHeure()),this,SLOT(afficherMesuresJournalieresRuche()));
    disconnect(maRuche, SIGNAL(nouvelleMesureTemperatureExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
    disconnect(maRuche, SIGNAL(nouvelleMesureHumiditeInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresRuche()));
    disconnect(maRuche, SIGNAL(nouvelleMesureHumiditeExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
    disconnect(maRuche, SIGNAL(nouvelleMesurePressionAtmospheriqueTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
    disconnect(maRuche, SIGNAL(nouvelleMesureEnsoleillementTraiteParHeure()),this,SLOT(afficherMesuresJournalieresEnsoleillement()));
    /********************************SLOT TABLEAU***********************************/
    disconnect(ui->listeMesures, SIGNAL(currentIndexChanged(int)), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesureTemperatureInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesureHumiditeInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesureTemperatureExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesureHumiditeExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesurePressionAtmospheriqueTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
    disconnect(maRuche, SIGNAL(nouvelleMesureEnsoleillementTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
}

void RucheIhm::selectionnerRuche(int numeroRuche)
{
    if(mesRuches.size() == 0)
        return;
    numeroRuche -= 1;
    qDebug() << Q_FUNC_INFO << "numeroRuche" << numeroRuche << "nb ruches" << mesRuches.size();
    if(maRuche != 0)
    {
        deconnecterSignaux();
        delete maRuche;
        maRuche = 0;      
    }
    if(maRuche == 0 && numeroRuche >= 0)
    {
        maRuche = new Ruche(mesRuches.at(numeroRuche), this);
        reglagesAlertesIhm->setAlertes(maRuche->getAlertes());
        connect(maRuche,SIGNAL(etatClientConnexion(bool)),this,SLOT(recevoirEtatClientConnexion(bool)));
        /**********************************************SLOT AFFICHAGE MESURE**********************************************************************/
        connect(maRuche,SIGNAL(nouvelleMesureTemperatureInterieurTraite(double,QString)),this,SLOT(afficherTemperatureInterieur(double,QString)));
        connect(maRuche,SIGNAL(nouvelleMesureHumiditeInterieurTraite(double,QString)),this,SLOT(afficherHumiditeInterieur(double,QString)));
        connect(maRuche,SIGNAL(nouvelleMesureTemperatureExterieurTraite(double,QString)),this,SLOT(afficherTemperatureExterieur(double,QString)));
        connect(maRuche,SIGNAL(nouvelleMesureHumiditeExterieurTraite(double,QString)),this,SLOT(afficherHumiditeExterieur(double,QString)));
        connect(maRuche,SIGNAL(nouvellePressionAtmospheriqueTraite(double,QString)),this,SLOT(afficherPressionAtmospherique(double,QString)));
        connect(maRuche,SIGNAL(nouvelleMesureEnsoleillementTraite(double,QString)),this,SLOT(afficherEnsoleillement(double,QString)));
        connect(maRuche, SIGNAL(nouvelleMesureCharge(double, QString)),this, SLOT(afficherBatterie(double, QString)));
        connect(maRuche, SIGNAL(nouvelleMesurePoids(double, QString)), this, SLOT(afficherPoids(double, QString)));
        /**********************************************SLOT AFFICHAGE Alertes*********************************************************************/
        connect(maRuche,SIGNAL(envoiAlertesTemperatureInterieur(SeuilsAlertes)),this,SLOT(afficherAlertesTemperatureInterieur(SeuilsAlertes)));
        connect(maRuche,SIGNAL(envoiAlertesTemperatureExterieur(SeuilsAlertes)),this,SLOT(afficherAlertesTemperatureExterieur(SeuilsAlertes)));
        connect(maRuche,SIGNAL(envoiAlertesHumiditeInterieur(SeuilsAlertes)),this,SLOT(afficherAlertesHumiditeInterieur(SeuilsAlertes)));
        connect(maRuche,SIGNAL(envoiAlertesHumiditeExterieur(SeuilsAlertes)),this,SLOT(afficherAlertesHumiditeExterieur(SeuilsAlertes)));
        connect(maRuche,SIGNAL(envoiAlertesPressionAtmospherique(SeuilsAlertes)),this,SLOT(afficherAlertesPressionAtmospherique(SeuilsAlertes)));
        connect(maRuche, SIGNAL(envoiAlertesPoids(SeuilsAlertes)),this, SLOT(afficherAlertesPoids(SeuilsAlertes)));
        connect(maRuche, SIGNAL(envoiAlertesEnsoleillement(SeuilsAlertes,double)),this,SLOT(afficherAlertesEnsoleillement(SeuilsAlertes, double)));
        connect(maRuche, SIGNAL(envoiAlertesBatterie(SeuilsAlertes, double)),this, SLOT(afficherAlertesBatterie(SeuilsAlertes,double)));
        /**********************************SLOT Courbes*********************************/
        connect(maRuche, SIGNAL(nouvelleMesureTemperatureInterieurTraiteParHeure()),this,SLOT(afficherMesuresJournalieresRuche()));
        connect(maRuche, SIGNAL(nouvelleMesureTemperatureExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
        connect(maRuche, SIGNAL(nouvelleMesureHumiditeInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresRuche()));
        connect(maRuche, SIGNAL(nouvelleMesureHumiditeExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
        connect(maRuche, SIGNAL(nouvelleMesurePressionAtmospheriqueTraiteParHeure()), this, SLOT(afficherMesuresJournalieresEnvironement()));
        connect(maRuche, SIGNAL(nouvelleMesureEnsoleillementTraiteParHeure()),this,SLOT(afficherMesuresJournalieresEnsoleillement()));
        /********************************SLOT TABLEAU***********************************/
        connect(ui->listeMesures, SIGNAL(currentIndexChanged(int)), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesureTemperatureInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesureHumiditeInterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesureTemperatureExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesureHumiditeExterieurTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesurePressionAtmospheriqueTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));
        connect(maRuche, SIGNAL(nouvelleMesureEnsoleillementTraiteParHeure()), this, SLOT(afficherMesuresJournalieresSelectionnee()));

        QDate date = QDate::fromString(mesRuches.at(numeroRuche).at(10),Qt::ISODate);
        ui->labelRuche->setText(mesRuches.at(numeroRuche).at(1) + " : "  + mesRuches.at(numeroRuche).at(9) + " mise en service le " + date.toString("dd/MM/yyyy") + " [" + mesRuches.at(numeroRuche).at(12) + " " + mesRuches.at(numeroRuche).at(11) + "]");

        connect(maRuche, SIGNAL(envoiLocalisationPasserelle(QString,QString)),this, SLOT(afficherLocalisationPasserelle(QString,QString)));

        //initialiserSeuilsCourbe();
        afficherMesuresJournalieresRuche();
        afficherMesuresJournalieresEnvironement();
        afficherMesuresJournalieresEnsoleillement();
    }
}

/**
 * \brief creer une nouvelle ruche
 *
 * \fn RucheIhm::creerNouvelleRuche
 *
 */

void RucheIhm::creerNouvelleRuche()
{
    int retour = nouvelleRuche->exec();
    if(retour == QDialog::Accepted)
    {
        mettreAjourListeRuches();
    }
    //QDialog::Rejected
}

void RucheIhm::ouvrirReglagesAlertes()
{
    if(ui->comboBoxChoixRuche->currentIndex() == 0)
    {
        QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Aucune ruche selectionnée !"));
        return;
    }
    reglagesAlertesIhm->exec();
}

void RucheIhm::recevoirEtatClientConnexion(bool etat)
{
    if(etat)
    {
        ui->labelEtatConnexionRuche->setText(QString::fromUtf8("Connectée"));
        //ui->webViewPasserelle->setVisible(true);
        //ui->webViewRuche->setVisible(true);

        ui->labelLocalisationRuche->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:18pt; font-weight:600;\">Localisation Ruche</span></p></body></html>");
        ui->webViewRuche->load(QUrl("https://www.openstreetmap.org/export/embed.html?bbox=" + QString::number((mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(11).toDouble()-0.001)) + "%2C" + QString::number((mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(12).toDouble()-0.001)) + "," + QString::number((mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(11).toDouble()+0.001)) + "%2C" + QString::number((mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(12).toDouble()+0.001)) + "&marker=" + mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(12) + "," + mesRuches.at(ui->comboBoxChoixRuche->currentIndex()-1).at(11) + ""));

    }
    else
    {
        ui->labelEtatConnexionRuche->setText(QString::fromUtf8("Déconnectée"));
        initialiserWidgets();        
    }
}

/**
 * \brief affiche temperature interieur à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherTemperatureInterieur
 *
 * \param temperatureInterieur double la température dans la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherTemperatureInterieur(double temperatureInterieur, QString horodatage)
{
    qDebug() << Q_FUNC_INFO << "temperatureInterieur=" << temperatureInterieur;
    ui->lcdNumberTemperatureInterieur->display(temperatureInterieur);
    ui->thermoTemperatureInterieur->setValue(temperatureInterieur);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

void RucheIhm::afficherLocalisationPasserelle(QString longitude, QString latitude)
{
    //https://www.openstreetmap.org/export/embed.html?bbox= 4.81696%2C43.9483,4.81696%2C43.9483&marker= 43.9483,4.81696
    ui->labelLocalisationPasserelle->setText("<html><head/><body><p align=\"center\"><span style=\"font-size:18pt; font-weight:600;\">Localisation Passerelle</span></p></body></html>");
    ui->webViewPasserelle->load(QUrl("https://www.openstreetmap.org/export/embed.html?bbox=" + QString::number(longitude.toDouble()-0.001) + "%2C" + QString::number(latitude.toDouble()-0.001) + "," + QString::number(longitude.toDouble()-0.001) + "%2C" + QString::number(latitude.toDouble()-0.001) + "&marker=" + QString::number(latitude.toDouble()-0.001) + "," + QString::number(longitude.toDouble()-0.001) + ""));
}

/**
 * \brief affiche humidite interieur à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherHumiditeInterieur
 *
 * \param humiditeInterieur humidite interieur de la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherHumiditeInterieur(double humiditeInterieur,QString horodatage)
{
    ui->lcdNumberHumiditeInterieur->display(humiditeInterieur);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

void RucheIhm::afficherBatterie(double charge, QString horodatage)
{
    qDebug() << Q_FUNC_INFO << "niveau de batterie = " << charge;
    ui->lcdNumberBatterie->display(charge);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

void RucheIhm::afficherPoids(double poids, QString horodatage)
{
    qDebug() << Q_FUNC_INFO << "poids = " << poids;
    ui->lcdNumberPoids->display(poids);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

/**
 * \brief affiche temperature exterieur à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherTemperatureExterieur
 *
 * \param temperatureExterieur temperature exterieur de la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherTemperatureExterieur(double temperatureExterieur, QString horodatage)
{
    ui->thermoTemperatureExterieur->setValue(temperatureExterieur);
    ui->lcdNumberTemperatureExterieur->display(temperatureExterieur);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

/**
 * \brief affiche humidite exterieur à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherHumiditeExterieur
 *
 * \param humiditeExterieur humidite exterieur de la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherHumiditeExterieur(double humiditeExterieur, QString horodatage)
{
    ui->lcdNumberHumiditeExterieur->display(humiditeExterieur);
    ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

/**
 * \brief affiche pression atmospherique  à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherPressionAtmospherique
 *
 * \param pressionAtmospherique pression atmospherique exterieur de la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherPressionAtmospherique(double pressionAtmospherique, QString horodatage)
{
     qDebug() << Q_FUNC_INFO << "pressionAtmospherique:" << pressionAtmospherique;
     ui->lcdNumberPressionAtmospherique->display(pressionAtmospherique);
     ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

/**
 * \brief affiche l'ensoleillement à l'aide de widget graphique
 *
 * \fn RucheIhm::afficherEnsoleillement
 *
 * \param ensoleillement  l'ensoleillement exterieur de la ruche
 * \param horodatage QString
 */

void RucheIhm::afficherEnsoleillement(double ensoleillement, QString horodatage)
{
   qDebug() << Q_FUNC_INFO << "ensoleillement:" << ensoleillement;
   ui->lcdNumberEnsoleillement->display(ensoleillement);
   ui->groupBoxTemperatures->setTitle("Mesures : " + horodatage);
}

void RucheIhm::afficherAlertesTemperatureInterieur(SeuilsAlertes typeAlerte)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;
    QPalette paletteCouleur = ui->thermoTemperatureInterieur->palette();
    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(255,0,0));
        ui->thermoTemperatureInterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureInterieur->setText("<strong>Température Int. Elevée</strong>");
        QPixmap imageTemperatureInterieur(":/images/images/temperatureHaute.png");
        ui->labelAlerteImageTemperature->setPixmap(imageTemperatureInterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteTemperatureInterieur) + "', 'Température Intérieur Elevée','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == tropBas)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(135,206,250));
        ui->thermoTemperatureInterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureInterieur->setText("<strong>Température Int. Basse</strong>");
        QPixmap imageTemperatureInterieur(":/images/images/temperatureBasseAlerte.png");
        ui->labelAlerteImageTemperature->setPixmap(imageTemperatureInterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteTemperatureInterieur) + "', 'Température Intérieur Basse','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == bon)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(0,128,0));
        ui->thermoTemperatureInterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureInterieur->setText("<strong>Température Int. Normale</strong>");
        QPixmap imageTemperatureInterieur(":/images/images/temperatureNormal.png");
        ui->labelAlerteImageTemperature->setPixmap(imageTemperatureInterieur);
     }
}

void RucheIhm::afficherAlertesTemperatureExterieur(SeuilsAlertes typeAlerte)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;

    QPalette paletteCouleur = ui->thermoTemperatureExterieur->palette();
    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(255,0,0));
        ui->thermoTemperatureExterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureExterieur->setText("<strong>Température Ext. Elevée</strong>");
        QPixmap imageTemperatureExterieur(":/images/images/temperatureHaute.png");
        ui->labelImageTemperatureExterieur->setPixmap(imageTemperatureExterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteTemperatureExterieur) + "', 'Température Extérieur Elevée','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == tropBas)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(135,206,250));
        ui->thermoTemperatureExterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureExterieur->setText("<strong>Température Ext. Basse</strong>");
        QPixmap imageTemperatureExterieur(":/images/images/temperatureBasseAlerte.png");
        ui->labelImageTemperatureExterieur->setPixmap(imageTemperatureExterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteTemperatureExterieur) + "', 'Température Extérieur Basse','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == bon)
     {
        paletteCouleur.setColor(QPalette::ButtonText, QColor(0,128,0));
        ui->thermoTemperatureExterieur->setPalette(paletteCouleur);
        ui->labelAlerteTemperatureExterieur->setText("<strong>Température Ext. Normale</strong>");
        QPixmap imageTemperatureExterieur(":/images/images/temperatureNormal.png");
        ui->labelImageTemperatureExterieur->setPixmap(imageTemperatureExterieur);
     }
}

void RucheIhm::afficherAlertesHumiditeInterieur(SeuilsAlertes typeAlerte)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;
    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
     {
        ui->labelAlerteHumiditeInterieur->setText("<strong>Humidité Int. Elevée</strong>");
        QPixmap imageHumiditeInterieur(":/images/images/humidityHaute.png");
        ui->labelAlerteImageHumiditeInterieur->setPixmap(imageHumiditeInterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteHumiditeInterieur) + "', 'Humidité Intérieur Elevée','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == tropBas)
     {
        ui->labelAlerteHumiditeInterieur->setText("<strong>Humidité Int. Basse</strong>");
        QPixmap imageHumiditeInterieur(":/images/images/humidityHaute.png");
        ui->labelAlerteImageHumiditeInterieur->setPixmap(imageHumiditeInterieur);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteHumiditeInterieur) + "', 'Humidité Intérieur Basse','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
     }
     else if(typeAlerte == bon)
     {
        ui->labelAlerteHumiditeInterieur->setText("<strong>Humidité Int. Normale</strong>");
        QPixmap imageHumiditeInterieur(":/images/images/humiditeNormal.png");
        ui->labelAlerteImageHumiditeInterieur->setPixmap(imageHumiditeInterieur);
     }
}

void RucheIhm::afficherAlertesHumiditeExterieur(SeuilsAlertes typeAlerte)
{
    {
        qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;
        QString requete;
        QDateTime maintenant = QDateTime::currentDateTime();

        if(typeAlerte == tropHaut)
         {
            ui->labelAlerteHumiditeExterieur->setText("<strong>Humidité Ext. Elevée</strong>");
            QPixmap imageHumiditeExterieur(":/images/images/humidityHaute.png");
            ui->labelAlerteImageHumiditeExterieur->setPixmap(imageHumiditeExterieur);
            requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteHumiditeExterieur) + "', 'Humidité Extérieur Elevée','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
            bdd->executer(requete);
         }
         else if(typeAlerte == tropBas)
         {
            ui->labelAlerteHumiditeExterieur->setText("<strong>Humidité Ext. Basse</strong>");
            QPixmap imageHumiditeExterieur(":/images/images/humidityHaute.png");
            ui->labelAlerteImageHumiditeExterieur->setPixmap(imageHumiditeExterieur);
            requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteHumiditeExterieur) + "', 'Humidité Extérieur Basse','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
            bdd->executer(requete);
         }
         else if(typeAlerte == bon)
         {
            ui->labelAlerteHumiditeExterieur->setText("<strong>Humidité Ext. Normale</strong>");
            QPixmap imageHumiditeExterieur(":/images/images/humiditeNormal.png");
            ui->labelAlerteImageHumiditeExterieur->setPixmap(imageHumiditeExterieur);
         }
    }
}

void RucheIhm::afficherAlertesPressionAtmospherique(SeuilsAlertes typeAlerte)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;
    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
    {
        ui->labelAlertePressionAtmospherique->setText("<strong>Pression Atmospherique Elevée</strong>");
        QPixmap imagePressionAtmospherique(":/images/images/pressionAlerte.png");
        ui->labelAlerteImagePressionAtmospherique->setPixmap(imagePressionAtmospherique);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alertePressionAtmospherique) + "', 'Pression Atmosphérique Elevé','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == tropBas)
    {
        ui->labelAlertePressionAtmospherique->setText("<strong>Pression Atmospherique Basse</strong>");
        QPixmap imagePressionAtmospherique(":/images/images/pressionAlerte.png");
        ui->labelAlerteImagePressionAtmospherique->setPixmap(imagePressionAtmospherique);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alertePressionAtmospherique) + "', 'Pression Atmosphérique Basse','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == bon)
    {
        ui->labelAlertePressionAtmospherique->setText("<strong>Pression Atmospherique Normale</strong>");
        QPixmap imagePressionAtmospherique(":/images/images/pressionNormal.png");
        ui->labelAlerteImagePressionAtmospherique->setPixmap(imagePressionAtmospherique);
    }
}

void RucheIhm::afficherAlertesPoids(SeuilsAlertes typeAlerte)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte;
    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
    {
        ui->labelAlertePoids->setText("<strong>Poids Elevé</strong>");
        QPixmap imagePoids(":/images/images/poidsAlerte.png");
        ui->labelAlerteImagePoids->setPixmap(imagePoids);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alertePoids) + "', 'Poids Elevé','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == tropBas)
    {
        ui->labelAlertePoids->setText("<strong>Poids Bas</strong>");
        QPixmap imagePoids(":/images/images/poidsAlerte.png");
        ui->labelAlerteImagePoids->setPixmap(imagePoids);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alertePoids) + "', 'Poids Bas','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == bon)
    {
        ui->labelAlertePoids->setText("<strong>Poids Normal</strong>");
        QPixmap imagePoids(":/images/images/poidsnormal.png");
        ui->labelAlerteImagePoids->setPixmap(imagePoids);
    }
}
void RucheIhm::afficherAlertesEnsoleillement(SeuilsAlertes typeAlerte, double mesure)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte << "mesure " << mesure;

    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropHaut)
    {
        ui->labelAlerteEnsoleillement->setText("<strong>Ensoleillement Elevé</strong>");
        QPixmap imageEnsoleillement(":/images/images/alerteEnsoleillement.png");
        ui->labelAlerteImageEnsoleillement->setPixmap(imageEnsoleillement);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteEnsoleillement) + "', 'Ensoleillement Elevé','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == bon)
    {
        if(mesure >=800 && mesure <= 1800)
        {
            ui->labelAlerteEnsoleillement->setText("<strong>Ensoleillé</strong>");
            QPixmap imageEnsoleillement(":/images/images/soleil.png");
            ui->labelAlerteImageEnsoleillement->setPixmap(imageEnsoleillement);
        }
        if(mesure >= 400 && mesure < 800)
        {
            ui->labelAlerteEnsoleillement->setText("<strong>Ensoleillé dans l'ensemble</strong>");
            QPixmap imageEnsoleillement(":/images/images/soleilCouvert.png");
            ui->labelAlerteImageEnsoleillement->setPixmap(imageEnsoleillement);
        }
        if(mesure > 40 && mesure < 400)
        {
            ui->labelAlerteEnsoleillement->setText("<strong>Couvert</strong>");
            QPixmap imageEnsoleillement(":/images/images/nuageux.png");
            ui->labelAlerteImageEnsoleillement->setPixmap(imageEnsoleillement);
            requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteEnsoleillement) + "', 'Couvert','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
            bdd->executer(requete);
        }
        if(mesure <= 40)
        {
            ui->labelAlerteEnsoleillement->setText("<strong>Nuit</strong>");
            QPixmap imageEnsoleillement(":/images/images/moon.png");
            ui->labelAlerteImageEnsoleillement->setPixmap(imageEnsoleillement);
        }
    }
}

void RucheIhm::afficherAlertesBatterie(SeuilsAlertes typeAlerte, double mesure)
{
    qDebug() << Q_FUNC_INFO << "SeuilsAlertes " << typeAlerte << "mesure " << mesure;

    QString requete;
    QDateTime maintenant = QDateTime::currentDateTime();

    if(typeAlerte == tropBas && mesure > 0 && mesure < 25)
    {
        ui->labelAlerteBatterie->setText("<strong>Batterie faible</strong>");
        QPixmap imageBatterie(":/images/images/batteryChargeMin.png");
        ui->labelAlerteImageBatterie->setPixmap(imageBatterie);
        requete = "INSERT INTO Alertes (Alertes.idRuche, Alertes.idType, Alertes.Description, Alertes.Horodatage) VALUES ('" + maRuche->getIdRuche() + "', '" + QString::number(alerteBatterie) + "', 'Batterie Faible','" + maintenant.toString("yyyy-MM-dd  HH:mm:ss") + "')";
        bdd->executer(requete);
    }
    else if(typeAlerte == bon)
    {
        if(mesure >= 75 && mesure <= 100)
        {
            ui->labelAlerteBatterie->setText("<strong>Batterie</strong>");
            QPixmap imageBatterie(":/images/images/batteryChargeMax.png");
            ui->labelAlerteImageBatterie->setPixmap(imageBatterie);
        }
        if(mesure >= 50 && mesure < 75)
        {
            ui->labelAlerteBatterie->setText("<strong>Batterie</strong>");
            QPixmap imageBatterie(":/images/images/batteryChargeBonne.png");
            ui->labelAlerteImageBatterie->setPixmap(imageBatterie);
        }
        if(mesure >= 25 && mesure < 50)
        {
            ui->labelAlerteBatterie->setText("<strong>Batterie</strong>");
            QPixmap imageBatterie(":/images/images/batteryChargeMoitie.png");
            ui->labelAlerteImageBatterie->setPixmap(imageBatterie);
        }
    }
}

void RucheIhm::mettreAjourListeRuches()
{
    mesRuches.clear();
    ui->comboBoxChoixRuche->clear();
    QString requete = "SELECT Ruche.idRuche, Ruche.Nom, Ruche.DeviceID, TTN.idTTN, TTN.Hostname, TTN.Port, TTN.Username, TTN.Password, TTN.ApplicationID, Ruche.Adresse, Ruche.DateMiseEnService, Ruche.Longitude, Ruche.Latitude FROM Ruche INNER JOIN TTN ON Ruche.idTTN = TTN.idTTN ";
    qDebug()<< Q_FUNC_INFO << requete;
    bdd->recuperer(requete, mesRuches);
    qDebug()<< Q_FUNC_INFO << mesRuches;
    if(mesRuches.size() == 0)
        ui->comboBoxChoixRuche->addItem("Aucune ruche");
    else
        ui->comboBoxChoixRuche->addItem("Choix d'une ruche");
    for(int i=0;i<mesRuches.size();i++)
    {
        QStringList maRuche = mesRuches.at(i);
        ui->comboBoxChoixRuche->addItem(maRuche.at(1));
    }
}

/**
 * \brief initialise les widgets incluant la methode initialiserThermometre
 *
 * \fn RucheIhm::initialiserWidget()
 *
 */
void RucheIhm::initialiserWidgets()
{
    ui->groupBoxTemperatures->setTitle("Mesures");
    ui->lcdNumberHumiditeInterieur->display("--");
    ui->lcdNumberHumiditeExterieur->display("--");
    ui->lcdNumberTemperatureInterieur->display("--");
    ui->lcdNumberTemperatureExterieur->display("--");
    ui->lcdNumberPressionAtmospherique->display("--");
    ui->lcdNumberEnsoleillement->display("--");
    ui->lcdNumberBatterie->display("--");
    ui->lcdNumberPoids->display("--");
    ui->labelRuche->setText("");
    initialiserThermometre();
    ui->QTabOnglet->setCurrentIndex(0); //Onglet Mesure == 0

    ui->labelAlerteHumiditeExterieur->setText("<strong>Humidité Ext. Normale</strong>");
    QPixmap imageHumiditeExterieur(":/images/images/humiditeNormal.png");
    ui->labelAlerteImageHumiditeInterieur->setPixmap(imageHumiditeExterieur);

    ui->labelAlerteHumiditeInterieur->setText("<strong>Humidité Int. Normale</strong>");
    QPixmap imageHumiditeInterieur(":/images/images/humiditeNormal.png");
    ui->labelAlerteImageHumiditeInterieur->setPixmap(imageHumiditeInterieur);

    ui->labelAlerteTemperatureExterieur->setText("<strong>Température Ext. Normale</strong>");
    QPixmap imageTemperatureExterieur(":/images/images/temperatureNormal.png");
    ui->labelAlerteImageTemperature->setPixmap(imageTemperatureExterieur);

    ui->labelAlerteTemperatureInterieur->setText("<strong>Température Int. Normale</strong>");
    QPixmap imageTemperatureInterieur(":/images/images/temperatureNormal.png");
    ui->labelAlerteImageTemperature->setPixmap(imageTemperatureInterieur);

    ui->labelAlerteBatterie->setText("<strong>Niveau de batterie correct</strong>");
    QPixmap imageBatterie(":/images/images/batteryChargeMax.png");
    ui->labelAlerteImageBatterie->setPixmap(imageBatterie);

    ui->labelAlertePoids->setText("<strong>Poids Normal</strong>");
    QPixmap imagePoids(":/images/images/poidsnormal.png");
    ui->labelAlerteImagePoids->setPixmap(imagePoids);

    ui->labelAlerteEnsoleillement->setText("<strong>Ensoleillement Normal</strong>");

    ui->labelAlertePressionAtmospherique->setText("<strong>Pression Atmospherique Normale</strong>");

    ui->labelLocalisationPasserelle->setText("");
    ui->webViewPasserelle->setStyleSheet("background-color: #FFFFB9;");
    ui->webViewPasserelle->load(QUrl(""));
    //ui->webViewPasserelle->setVisible(false);
    ui->labelLocalisationRuche->setText("");
    ui->webViewRuche->setStyleSheet("background-color: #FFFFB9;");
    ui->webViewRuche->load(QUrl(""));
    //ui->webViewRuche->setVisible(false);

    reinititialiserCourbes();
    ui->listeMesures->setCurrentIndex(0);
    effacerTableau();
}

void RucheIhm::supprimerRuche()
{


   if(ui->comboBoxChoixRuche->currentIndex() == 0)
   {
       QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Supression impossible ! Aucune ruche selectionnée !"));
       return;
   }
   QMessageBox::StandardButton reply = QMessageBox::warning(0,"Voulez vous supprimer cette ruche ? ", "Toutes les mesures et données lui correspondant seront supprimé !", QMessageBox::Ok |QMessageBox::Cancel);

    if (reply == QMessageBox::Ok)
    {
        int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
        QString requete = "DELETE FROM Ruche WHERE Ruche.DeviceID ='" + mesRuches[positionDeLaRuche].at(2) + "'";
        bool etatRequete = bdd->executer(requete);
        if(etatRequete == true)
        {
            ui->comboBoxChoixRuche->removeItem(ui->comboBoxChoixRuche->currentIndex());
            ui->comboBoxChoixRuche->setCurrentIndex(0);
        }
        else
        {
            QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Impossible de suprimmer la ruche"));
        }
    }
}

void RucheIhm::inititialiserCourbes()
{
    //initialisation Titre
    ui->qwtPlotHumidite->setTitle(QString::fromUtf8("Humidité"));
    ui->qwtPlotPressionAtmospherique->setTitle(QString::fromUtf8("Pression atmosphérique"));
    ui->qwtPlotTemperature->setTitle(QString::fromUtf8("Température"));
    ui->qwtPlotPoids->setTitle(QString::fromUtf8("Poids"));
    ui->qwtPlotEnsoleillement->setTitle(QString::fromUtf8("Ensoleillement"));
    ui->qwtPlotCharge->setTitle(QString::fromUtf8("Charge"));

    // une légende à droite
    ui->qwtPlotPoids->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
    ui->qwtPlotCharge->insertLegend(new QwtLegend(),QwtPlot::RightLegend);

    // cadrillage
    QwtPlotGrid *cadrillageHumidite = new QwtPlotGrid ;;
    QwtPlotGrid *cadrillagePressionAtmospherique = new QwtPlotGrid ;;
    QwtPlotGrid *cadrillageTemperature = new QwtPlotGrid ;;
    QwtPlotGrid *cadrillagePoids = new QwtPlotGrid ;;
    QwtPlotGrid *cadrillageEnsoleillement = new QwtPlotGrid ;;
    QwtPlotGrid *cadrillageCharge = new QwtPlotGrid ;

    cadrillageHumidite->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    cadrillagePressionAtmospherique->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    cadrillageTemperature->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    cadrillagePoids->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    cadrillageEnsoleillement->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
    cadrillageCharge->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));


    cadrillageHumidite->attach(ui->qwtPlotHumidite);
    cadrillagePressionAtmospherique->attach(ui->qwtPlotPressionAtmospherique);
    cadrillageTemperature->attach(ui->qwtPlotTemperature);
    cadrillagePoids->attach(ui->qwtPlotPoids);
    cadrillageEnsoleillement->attach(ui->qwtPlotEnsoleillement);
    cadrillageCharge->attach(ui->qwtPlotCharge);

    //configuration des axes
    ui->qwtPlotHumidite->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotHumidite->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("% -->")); // nom de l'Axe y
    ui->qwtPlotHumidite->setAxisScale(QwtPlot::xBottom, 0.0, 24.0,0); // scale de l'axe x
    ui->qwtPlotHumidite->setAxisScale(QwtPlot::yLeft, 0.0, 100.0); // scale de l'axe y

    ui->qwtPlotPressionAtmospherique->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotPressionAtmospherique->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("HPa -->")); // nom de l'Axe y
    ui->qwtPlotPressionAtmospherique->setAxisScale(QwtPlot::xBottom, 0.0, 24.0, 0); // scale de l'axe x
    ui->qwtPlotPressionAtmospherique->setAxisScale(QwtPlot::yLeft, 0.0, 1300.0); // scale de l'axe y

    ui->qwtPlotTemperature->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotTemperature->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("°C -->")); // nom de l'Axe y
    ui->qwtPlotTemperature->setAxisScale(QwtPlot::xBottom, 0.0, 24.0, 0); // scale de l'axe x
    ui->qwtPlotTemperature->setAxisScale(QwtPlot::yLeft, 0.0, 55.0); // scale de l'axe y

    ui->qwtPlotPoids->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotPoids->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("Kg -->")); // nom de l'Axe y
    ui->qwtPlotPoids->setAxisScale(QwtPlot::xBottom, 0.0, 24.0, 0); // scale de l'axe x
    ui->qwtPlotPoids->setAxisScale(QwtPlot::yLeft, 0.0, 100.0); // scale de l'axe y

    ui->qwtPlotEnsoleillement->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotEnsoleillement->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("W/M² -->")); // nom de l'Axe y
    ui->qwtPlotEnsoleillement->setAxisScale(QwtPlot::xBottom, 0.0, 24.0, 0); // scale de l'axe x
    ui->qwtPlotEnsoleillement->setAxisScale(QwtPlot::yLeft, 0.0, 1500); // scale de l'axe y

    ui->qwtPlotCharge->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("heures -->")); // nom de l'axe x
    ui->qwtPlotCharge->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("% -->")); // nom de l'Axe y
    ui->qwtPlotCharge->setAxisScale(QwtPlot::xBottom, 0.0, 24.0, 0); // scale de l'axe x
    ui->qwtPlotCharge->setAxisScale(QwtPlot::yLeft, 0.0, 100); // scale de l'axe y

    initialiserTemperatureInterieurCourbe();
    initialiserTemperatureExterieurCourbe();
    initialiserHumiditeInterieurCourbe();
    initialiserHumiditeExterieurCourbe();
    initialiserPressionAtmospheriqueCourbe();
    initialiserEnsoleillementCourbe();
}

void RucheIhm::reinititialiserCourbes()
{
    QVector<double> temperatures;
    QVector<double> humidites;
    QVector<double> pression;
    QVector<double> ensoleillements;
    QVector<double> heures;

    courbeTemperatureInterieur->setSamples(heures, temperatures);
    courbeTemperatureExterieur->setSamples(heures, temperatures);
    ui->qwtPlotTemperature->replot();
    courbeHumiditeInterieur->setSamples(heures, humidites);
    courbeHumiditeExterieur->setSamples(heures, humidites);
    ui->qwtPlotHumidite->replot();
    courbePressionAtmospherique->setSamples(heures, pression);
    ui->qwtPlotPressionAtmospherique->replot();
    courbeEnsoleillement->setSamples(heures, ensoleillements);
    ui->qwtPlotEnsoleillement->replot();

}

void RucheIhm::initialiserTemperatureInterieurCourbe()
{
    qDebug() << Q_FUNC_INFO << "COURBE Temperature Interieur";

    // Courbes
    courbeTemperatureInterieur = new QwtPlotCurve("Temperature interieur");

     // Ajout de la courbe a la legende
    courbeTemperatureInterieur->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    //couleur de la courbe
    courbeTemperatureInterieur->setPen(QPen(QColor( 52, 152, 219 )));
    courbeTemperatureInterieur->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbeTemperatureInterieur->setRenderHint(QwtPlotItem::RenderAntialiased);

    //type de tracé
    //courbeTemperatureInterieur->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, Qt::yellow, QPen(Qt::black), QSize(5, 5)));

    //atachement de la courbe a son repere
    courbeTemperatureInterieur->attach(ui->qwtPlotTemperature);
}

void RucheIhm::initialiserTemperatureExterieurCourbe()
{
    qDebug() << Q_FUNC_INFO << "COURBE Temperature Exterieur";
    ui->qwtPlotTemperature->insertLegend(new QwtLegend(),QwtPlot::BottomLegend);
    courbeTemperatureExterieur = new QwtPlotCurve("Temperature Exterieur");
    courbeTemperatureExterieur->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    courbeTemperatureExterieur->setPen(QPen(Qt::green));
    courbeTemperatureExterieur->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbeTemperatureExterieur->setRenderHint(QwtPlotItem::RenderAntialiased);
    //courbeTemperatureExterieur->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::red, QPen(Qt::black), QSize(5,5)));

    // on trace les points x,y
    courbeTemperatureExterieur->attach(ui->qwtPlotTemperature);
}

void RucheIhm::initialiserHumiditeInterieurCourbe()
{
    courbeHumiditeInterieur = new QwtPlotCurve("Humidite interieur");
    courbeHumiditeInterieur->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    courbeHumiditeInterieur->setPen(QPen(QColor( 52, 152, 219 )));
    courbeHumiditeInterieur->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbeHumiditeInterieur->setRenderHint(QwtPlotItem::RenderAntialiased);
    courbeHumiditeInterieur->attach(ui->qwtPlotHumidite);
}

void RucheIhm::initialiserHumiditeExterieurCourbe()
{
    ui->qwtPlotHumidite->insertLegend(new QwtLegend(),QwtPlot::BottomLegend);
    courbeHumiditeExterieur = new QwtPlotCurve("humidite exterieur");
    courbeHumiditeExterieur->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    courbeHumiditeExterieur->setPen(QPen(Qt::green));
    courbeHumiditeExterieur->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbeHumiditeExterieur->setRenderHint(QwtPlotItem::RenderAntialiased);
    courbeHumiditeExterieur->attach(ui->qwtPlotHumidite);
}

void RucheIhm::initialiserPressionAtmospheriqueCourbe()
{
    ui->qwtPlotPressionAtmospherique->insertLegend(new QwtLegend(),QwtPlot::BottomLegend);
    courbePressionAtmospherique = new QwtPlotCurve("Pression atmospherique");
    courbePressionAtmospherique->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    courbePressionAtmospherique->setPen(QPen(Qt::blue));
    courbePressionAtmospherique->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbePressionAtmospherique->setRenderHint(QwtPlotItem::RenderAntialiased);
    courbePressionAtmospherique->attach(ui->qwtPlotPressionAtmospherique);
}

void RucheIhm::initialiserEnsoleillementCourbe()
{
    ui->qwtPlotEnsoleillement->insertLegend(new QwtLegend(),QwtPlot::BottomLegend);
    courbeEnsoleillement = new QwtPlotCurve("Ensoleillement");
    courbeEnsoleillement->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    courbeEnsoleillement->setPen(QPen(QColor(245, 176, 65)));
    courbeEnsoleillement->setCurveAttribute(QwtPlotCurve::Fitted); // courbe
    courbeEnsoleillement->setRenderHint(QwtPlotItem::RenderAntialiased);
    courbeEnsoleillement->setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::blue, QPen(Qt::black), QSize(5, 5)));
    courbeEnsoleillement->attach(ui->qwtPlotEnsoleillement);
}


void RucheIhm::afficherMesuresJournalieresRuche()
{
    QDate dateCourante = QDate::currentDate();
    int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
    QVector<QStringList> mesuresJournalieresRuche;
    QString requete = "SELECT Temperature, Humidite, HeureMesure FROM MesuresJournalieresRuche WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
    qDebug()<< Q_FUNC_INFO << requete;
    bdd->recuperer(requete, mesuresJournalieresRuche);
    qDebug()<< Q_FUNC_INFO << mesuresJournalieresRuche;

    if(mesuresJournalieresRuche.size() < 1)
        return;

    QVector<double> temperatures;
    QVector<double> humidites;
    QVector<double> heures;

    for(int i=0; i < mesuresJournalieresRuche.size(); i++)
    {
        temperatures.push_back(mesuresJournalieresRuche.at(i).at(0).toDouble());
        humidites.push_back(mesuresJournalieresRuche.at(i).at(1).toDouble());
        QString heure = mesuresJournalieresRuche.at(i).at(2);
        QTime heureMesure = QTime::fromString(heure, "HH:mm:ss");
        heures.push_back(heureMesure.hour());
    }

    courbeTemperatureInterieur->setSamples(heures, temperatures);
    ui->qwtPlotTemperature->replot();
    courbeHumiditeInterieur->setSamples(heures, humidites);
    ui->qwtPlotHumidite->replot();
}

void RucheIhm::afficherMesuresJournalieresEnvironement()
{
    QDate dateCourante = QDate::currentDate();
    int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
    QVector<QStringList> mesuresJournalieresEnvironement;
    QString requete = "SELECT Temperature, Humidite, Pression, HeureMesure FROM MesuresJournalieresEnvironnement WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
    qDebug()<< Q_FUNC_INFO << requete;
    bdd->recuperer(requete, mesuresJournalieresEnvironement);
    qDebug()<< Q_FUNC_INFO << mesuresJournalieresEnvironement;

    if(mesuresJournalieresEnvironement.size() < 1)
        return;

    QVector<double> temperatures;
    QVector<double> humidites;
    QVector<double> pression;
    QVector<double> heures;

    for(int i=0; i < mesuresJournalieresEnvironement.size(); i++)
    {
        temperatures.push_back(mesuresJournalieresEnvironement.at(i).at(0).toDouble());
        humidites.push_back(mesuresJournalieresEnvironement.at(i).at(1).toDouble());
        pression.push_back(mesuresJournalieresEnvironement.at(i).at(2).toDouble());
        QString heure = mesuresJournalieresEnvironement.at(i).at(3);
        QTime heureMesure = QTime::fromString(heure, "HH:mm:ss");
        heures.push_back(heureMesure.hour());
    }
    courbeTemperatureExterieur->setSamples(heures, temperatures);
    ui->qwtPlotTemperature->replot();
    courbeHumiditeExterieur->setSamples(heures, humidites);
    ui->qwtPlotHumidite->replot();
    courbePressionAtmospherique->setSamples(heures, pression);
    ui->qwtPlotPressionAtmospherique->replot();
}

void RucheIhm::afficherMesuresJournalieresEnsoleillement()
{
    QDate dateCourante = QDate::currentDate();
    int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
    QVector<QStringList> mesuresJournalieresEnsoleillement;
    QString requete = "SELECT Ensoleillement, HeureMesure FROM MesuresJournalieresEnsoleillement WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
    qDebug()<< Q_FUNC_INFO << requete;
    bdd->recuperer(requete, mesuresJournalieresEnsoleillement);
    qDebug()<< Q_FUNC_INFO << mesuresJournalieresEnsoleillement;

    if(mesuresJournalieresEnsoleillement.size() < 1)
        return;

    QVector<double> ensoleillements;
    QVector<double> heures;

    for(int i=0; i < mesuresJournalieresEnsoleillement.size(); i++)
    {
        ensoleillements.push_back(mesuresJournalieresEnsoleillement.at(i).at(0).toDouble());
        QString heure = mesuresJournalieresEnsoleillement.at(i).at(1);
        QTime heureMesure = QTime::fromString(heure, "HH:mm:ss");
        heures.push_back(heureMesure.hour());
    }
    courbeEnsoleillement->setSamples(heures, ensoleillements);
    ui->qwtPlotEnsoleillement->replot();
}

void RucheIhm::insererMesuresRucheTableau(QVector<QStringList> mesuresJournalieresRuche)
{
    for(int i=0; i < mesuresJournalieresRuche.size(); i++)
   {
        double temperatureDouble = QString(mesuresJournalieresRuche.at(i).at(0)).toDouble();
        double temperatureMinDouble = QString(mesuresJournalieresRuche.at(i).at(1)).toDouble();
        double temperatureMaxDouble = QString(mesuresJournalieresRuche.at(i).at(2)).toDouble();
        double humiditeDouble = QString(mesuresJournalieresRuche.at(i).at(3)).toDouble();
        double humiditeMinDouble = QString(mesuresJournalieresRuche.at(i).at(4)).toDouble();
        double humiditeMaxDouble = QString(mesuresJournalieresRuche.at(i).at(5)).toDouble();

        QTableWidgetItem *itemTemperature = new QTableWidgetItem(QString::number(temperatureDouble,'g',4));
        QTableWidgetItem *itemTemperatureMin = new QTableWidgetItem(QString::number(temperatureMinDouble,'g',4));
        QTableWidgetItem *itemTemperatureMax = new QTableWidgetItem(QString::number(temperatureMaxDouble,'g',4));
        QTableWidgetItem *itemHumidite = new QTableWidgetItem(QString::number(humiditeDouble,'g',4));
        QTableWidgetItem *itemHumiditeMin = new QTableWidgetItem(QString::number(humiditeMinDouble,'g',4));
        QTableWidgetItem *itemHumiditeMax = new QTableWidgetItem(QString::number(humiditeMaxDouble,'g',4));
        QTableWidgetItem *itemHeureMesure = new QTableWidgetItem(mesuresJournalieresRuche.at(i).at(6));

        int nombreDeLigne = ui->tableauxMesures->rowCount();
        ui->tableauxMesures->insertRow(nombreDeLigne);
        ui->tableauxMesures->setItem(nombreDeLigne, 0, itemTemperature);
        ui->tableauxMesures->setItem(nombreDeLigne, 1, itemTemperatureMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 2, itemTemperatureMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 3, itemHumidite);
        ui->tableauxMesures->setItem(nombreDeLigne, 4, itemHumiditeMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 5, itemHumiditeMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 6, itemHeureMesure);

    }
}

void RucheIhm::insererMesuresEnvironementTableau(QVector<QStringList> mesuresJournalieresEnvironement)
{
    for(int i = 0; i < mesuresJournalieresEnvironement.size(); i++)
    {
        double temperatureDouble = QString(mesuresJournalieresEnvironement.at(i).at(0)).toDouble();
        double temperatureMinDouble = QString(mesuresJournalieresEnvironement.at(i).at(1)).toDouble();
        double temperatureMaxDouble = QString(mesuresJournalieresEnvironement.at(i).at(2)).toDouble();
        double humiditeDouble = QString(mesuresJournalieresEnvironement.at(i).at(3)).toDouble();
        double humiditeMinDouble = QString(mesuresJournalieresEnvironement.at(i).at(4)).toDouble();
        double humiditeMaxDouble = QString(mesuresJournalieresEnvironement.at(i).at(5)).toDouble();
        double pressionDouble = QString(mesuresJournalieresEnvironement.at(i).at(6)).toDouble();
        double pressionMinDouble = QString(mesuresJournalieresEnvironement.at(i).at(7)).toDouble();
        double pressionMaxDouble = QString(mesuresJournalieresEnvironement.at(i).at(8)).toDouble();


        QTableWidgetItem *itemTemperature = new QTableWidgetItem(QString::number(temperatureDouble,'g',4));
        QTableWidgetItem *itemTemperatureMin = new QTableWidgetItem(QString::number(temperatureMinDouble,'g',4));
        QTableWidgetItem *itemTemperatureMax = new QTableWidgetItem(QString::number(temperatureMaxDouble,'g',4));
        QTableWidgetItem *itemHumidite = new QTableWidgetItem(QString::number(humiditeDouble,'g',4));
        QTableWidgetItem *itemHumiditeMin = new QTableWidgetItem(QString::number(humiditeMinDouble,'g',4));
        QTableWidgetItem *itemHumiditeMax = new QTableWidgetItem(QString::number(humiditeMaxDouble,'g',4));
        QTableWidgetItem *itemPression = new QTableWidgetItem(QString::number(pressionDouble,'g',4));
        QTableWidgetItem *itemPressionMin = new QTableWidgetItem(QString::number(pressionMinDouble,'g',4));
        QTableWidgetItem *itemPressionMax = new QTableWidgetItem(QString::number(pressionMaxDouble,'g',4));
        QTableWidgetItem *itemHeureMesure = new QTableWidgetItem(mesuresJournalieresEnvironement.at(i).at(9));

        int nombreDeLigne = ui->tableauxMesures->rowCount();
        ui->tableauxMesures->insertRow(nombreDeLigne);
        ui->tableauxMesures->setItem(nombreDeLigne, 0, itemTemperature);
        ui->tableauxMesures->setItem(nombreDeLigne, 1, itemTemperatureMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 2, itemTemperatureMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 3, itemHumidite);
        ui->tableauxMesures->setItem(nombreDeLigne, 4, itemHumiditeMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 5, itemHumiditeMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 6, itemPression);
        ui->tableauxMesures->setItem(nombreDeLigne, 7, itemPressionMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 8, itemPressionMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 9, itemHeureMesure);
    }
}

void RucheIhm::insererMesuresEnsoleillementTableau(QVector<QStringList> mesuresJournalieresEnsoleillement)
{
    for(int i = 0 ; i < mesuresJournalieresEnsoleillement.size() ; i++ )
    {
        double ensoleillementDouble = QString(mesuresJournalieresEnsoleillement.at(i).at(0)).toDouble();
        double ensoleillementMinDouble = QString(mesuresJournalieresEnsoleillement.at(i).at(1)).toDouble();
        double ensoleillementMaxDouble = QString(mesuresJournalieresEnsoleillement.at(i).at(2)).toDouble();

        QTableWidgetItem *itemEnsoleillement = new QTableWidgetItem(QString::number(ensoleillementDouble,'g',4));
        QTableWidgetItem *itemEnsoleillementMin = new QTableWidgetItem(QString::number(ensoleillementMinDouble,'g',4));
        QTableWidgetItem *itemEnsoleillementMax = new QTableWidgetItem(QString::number(ensoleillementMaxDouble,'g',4));
        QTableWidgetItem *itemHeureMesure = new QTableWidgetItem(mesuresJournalieresEnsoleillement.at(i).at(3));

        int nombreDeLigne = ui->tableauxMesures->rowCount();
        ui->tableauxMesures->insertRow(nombreDeLigne);
        ui->tableauxMesures->setItem(nombreDeLigne, 0, itemEnsoleillement);
        ui->tableauxMesures->setItem(nombreDeLigne, 1, itemEnsoleillementMin);
        ui->tableauxMesures->setItem(nombreDeLigne, 2, itemEnsoleillementMax);
        ui->tableauxMesures->setItem(nombreDeLigne, 3, itemHeureMesure);
    }
}

void RucheIhm::afficherMesuresJournalieresSelectionnee()
{
    if(ui->listeMesures->currentIndex() == 0)
        effacerTableau();

    else if(ui->listeMesures->currentIndex() == 1)
    {
        effacerTableau();
        ui->tableauxMesures->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QDate dateCourante = QDate::currentDate();
        int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
        QVector<QStringList> mesuresJournalieresRuche;
        QString requete = "SELECT Temperature, TemperatureMin, TemperatureMax, Humidite, HumiditeMin, HumiditeMax, HeureMesure FROM MesuresJournalieresRuche WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
        bdd->recuperer(requete, mesuresJournalieresRuche);

        if(mesuresJournalieresRuche.size() == 0)
        {
            QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Aucunes données"));
            return;
        }

        QStringList header; // nom des colonnes
        header << "Temperature" << "Temperature minimum" << "Temperature maximum" << "Humidite" << "Humidite minimum" << "Humidite maximum"  << "Heure";

        // On fixe le nombre de colonnes
        ui->tableauxMesures->setColumnCount(header.size());

        // On applique les noms des colonnes
        ui->tableauxMesures->setHorizontalHeaderLabels(header);

        insererMesuresRucheTableau(mesuresJournalieresRuche);

        // redimensionner les colonnes en fonction du contenu
        ui->tableauxMesures->resizeColumnsToContents();
        // redimensionner les lignes en fonction du contenu
        ui->tableauxMesures->resizeRowsToContents();
    }
    else if(ui->listeMesures->currentIndex() == 2)
    {
        effacerTableau();
        ui->tableauxMesures->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QDate dateCourante = QDate::currentDate();
        int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
        QVector<QStringList> mesuresJournalieresEnvironement;
        QString requete = "SELECT Temperature, TemperatureMin, TemperatureMax, Humidite, HumiditeMin, HumiditeMax, Pression, PressionMin, PressionMax, HeureMesure FROM MesuresJournalieresEnvironnement WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
        bdd->recuperer(requete, mesuresJournalieresEnvironement);

        if(mesuresJournalieresEnvironement.size() == 0)
        {
            QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Aucunes données"));
            return;
        }

        QStringList header; // nom des colonnes
        header << "Temperature" << "Temperature minimum" << "Temperature maximum" << "Humidite" << "Humidite minimum" << "Humidite maximum"  << "Pression" <<  "Pression minimum" << "Pression maximum"  << "Heure";

        // On fixe le nombre de colonnes
        ui->tableauxMesures->setColumnCount(header.size());
        ui->tableauxMesures->setHorizontalHeaderLabels(header);

        // On applique les noms des colonnes
        insererMesuresEnvironementTableau(mesuresJournalieresEnvironement);
        // redimensionner les colonnes en fonction du contenu
        ui->tableauxMesures->resizeColumnsToContents();
        // redimensionner les lignes en fonction du contenu
        ui->tableauxMesures->resizeRowsToContents();
    }
    else if(ui->listeMesures->currentIndex() == 3)
    {
        effacerTableau();
        ui->tableauxMesures->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QDate dateCourante = QDate::currentDate();
        int positionDeLaRuche = ui->comboBoxChoixRuche->currentIndex()-1;
        QVector<QStringList> mesuresJournalieresEnsoleillement;
        QString requete = "SELECT  Ensoleillement, EnsoleillementMin, EnsoleillementMax, HeureMesure FROM MesuresJournalieresEnsoleillement WHERE DateMesure = '" + dateCourante.toString("yyyy-MM-dd") + "' AND idRuche = '" + mesRuches[positionDeLaRuche].at(0) + "' ORDER BY HeureMesure ASC";
        bdd->recuperer(requete, mesuresJournalieresEnsoleillement);

        if(mesuresJournalieresEnsoleillement.size() == 0)
        {
            QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Aucunes données"));
            return;
        }

        QStringList header; // nom des colonnes
        header << "Ensoleillement" << "Ensoleillement minimum" << "Ensoleillement maximum" << "Heure";

        // On fixe le nombre de colonnes
        ui->tableauxMesures->setColumnCount(header.size());
        ui->tableauxMesures->setHorizontalHeaderLabels(header);

        insererMesuresEnsoleillementTableau(mesuresJournalieresEnsoleillement);
        // redimensionner les colonnes en fonction du contenu
        ui->tableauxMesures->resizeColumnsToContents();
        // redimensionner les lignes en fonction du contenu
        ui->tableauxMesures->resizeRowsToContents();
    }
}

void RucheIhm::effacerTableau()
{
    int nombreDeLignes = ui->tableauxMesures->rowCount();
    if(nombreDeLignes != 0)
    {
        for(int i = 0 ; i < nombreDeLignes ; i++)
        {
            ui->tableauxMesures->removeRow(0);
        }

    }
    QStringList header;
    ui->tableauxMesures->setColumnCount(header.size());
    ui->tableauxMesures->setHorizontalHeaderLabels(header);
}

