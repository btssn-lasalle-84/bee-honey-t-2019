    #include "reglagesAlertesIhm.h"
#include "ui_reglagesAlertesIhm.h"
#include "baseDeDonnees.h"
#include <QDebug>

/**
  *
  * \file rucheIhm/reglagesAlertesIhm.cpp
  *
  * \brief Définition de la classe ReglagesAlertesIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la fenêtre principale
 *
 * \fn ReglagesAlertesIhm::ReglagesAlertesIhm
 *
 * \param parent QObject Adresse de l'objet Qt parent (0 = fenêtre principale)
 */
ReglagesAlertesIhm::ReglagesAlertesIhm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReglagesAlertesIhm), alertes(0)
{
    ui->setupUi(this);

    bdd = BaseDeDonnees::getInstance();
    if(!bdd->estConnecte())
        bdd->connecter(BDD_NOMBASE, BDD_USERNAME, BDD_PASSWORD, BDD_SERVEUR);

    connect(ui->pushButtonOk,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(ui->pushButtonOk,SIGNAL(clicked(bool)),this ,SLOT(recevoirReglagesAlertes()));
}

/**
 * \brief Destructeur de la fenêtre principale
 *
 * \fn ReglagesAlertesIhm::~ReglagesAlertesIhm
 *
 */
ReglagesAlertesIhm::~ReglagesAlertesIhm()
{
    delete ui;
    BaseDeDonnees::detruireInstance();
}

/**
 * @brief setter de l'objet alertes
 * \fn    ReglagesAlertesIhm::setAlertes
 * @param *alertes correspond à l'objet de la classe RucheIhm
 */
void ReglagesAlertesIhm::setAlertes(Alertes *alertes)
{
    if(this->alertes == 0)
    {
        qDebug() << Q_FUNC_INFO << this->alertes;
        this->alertes = alertes;
        connect(this, SIGNAL(envoiSeuilTemperatureInterieurMin(QString)), this->alertes, SLOT(recevoirSeuilTemperatureInterieurMin(QString)));
        connect(this, SIGNAL(envoiSeuilTemperatureInterieurMax(QString)), this->alertes, SLOT(recevoirSeuilTemperatureInterieurMax(QString)));
        connect(this, SIGNAL(envoiSeuilHumiditeInterieurMin(QString)), this->alertes, SLOT(recevoirSeuilHumiditeInterieurMin(QString)));
        connect(this, SIGNAL(envoiSeuilHumiditeInterieurMax(QString)), this->alertes, SLOT(recevoirSeuilHumiditeInterieurMax(QString)));
        connect(this, SIGNAL(envoiSeuilTemperatureExterieurMin(QString)), this->alertes, SLOT(recevoirSeuilTemperatureExterieurMin(QString)));
        connect(this, SIGNAL(envoiSeuilTemperatureExterieurMax(QString)), this->alertes, SLOT(recevoirSeuilTemperatureExterieurMax(QString)));
        connect(this, SIGNAL(envoiSeuilHumiditeExterieurMin(QString)), this->alertes, SLOT(recevoirSeuilHumiditeExterieurMin(QString)));
        connect(this, SIGNAL(envoiSeuilHumiditeExterieurMax(QString)), this->alertes, SLOT(recevoirSeuilHumiditeExterieurMax(QString)));
        connect(this, SIGNAL(envoiSeuilPoidsMin(QString)), this->alertes, SLOT(recevoirSeuilPoidsMin(QString)));
        connect(this, SIGNAL(envoiSeuilPoidsMax(QString)), this->alertes, SLOT(recevoirSeuilPoidsMax(QString)));
        connect(this, SIGNAL(envoiSeuilPressionAtmospheriqueMin(QString)), this->alertes, SLOT(recevoirSeuilPressionAtmospherique(QString)));
        connect(this, SIGNAL(envoiSeuilEnsoleillementMin(QString)), this->alertes, SLOT(recevoirSeuilEnsoleillement(QString)));
    }
}

/**
 * @brief affiche les seuils à chaque fois que la fenêtre sera affichée
 * \fn    ReglagesAlertesIhm::showEvent
 * @param ev QShowEvent* l'évènement
 */
void ReglagesAlertesIhm::showEvent(QShowEvent *ev)
{
    Q_UNUSED(ev)
    if(this->alertes != 0)
    {
        ui->lineEditSeuilTemperatureInterieurMin->setText(QString::number(this->alertes->getSeuilTemperatureInterieurMin()));
        ui->lineEditSeuilTemperatureInterieurMax->setText(QString::number(this->alertes->getSeuilTemperatureInterieurMax()));
        ui->lineEditSeuilHumiditeInterieurMin->setText(QString::number(this->alertes->getSeuilHumiditeInterieurMin()));
        ui->lineEditSeuilHumiditeInterieurMax->setText(QString::number(this->alertes->getSeuilHumiditeInterieurMax()));
        ui->lineEditSeuilTemperatureExterieurMin->setText(QString::number(this->alertes->getSeuilTemperatureExterieurMin()));
        ui->lineEditSeuilTemperatureExterieurMax->setText(QString::number(this->alertes->getSeuilTemperatureExterieurMax()));
        ui->lineEditSeuilHumiditeExterieurMin->setText(QString::number(this->alertes->getSeuilHumiditeExterieurMin()));
        ui->lineEditSeuilHumiditeExterieurMax->setText(QString::number(this->alertes->getSeuilHumiditeExterieurMax()));
        ui->lineEditSeuilPoidsMin->setText(QString::number(this->alertes->getSeuilPoidsMin()));
        ui->lineEditSeuilPoidsMax->setText(QString::number(this->alertes->getSeuilPoidsMax()));
        ui->lineEditSeuilPressionAtmospherique->setText(QString::number(this->alertes->getSeuilPressionAtmospherique()));
        ui->lineEditSeuilEnsoleillement->setText(QString::number(this->alertes->getSeuilEnsoleillement()));
    }
}

/**
 * @brief recoit les paramétres de l'ihm
 * \fn    ReglagesAlertesIhm::recevoirReglagesAlertes()
 */
void ReglagesAlertesIhm::recevoirReglagesAlertes()
{
    QString temperatureInterieurMin;
    QString temperatureInterieurMax;
    QString humiditeInterieurMin;
    QString humiditeInterieurMax;
    QString temperatureExterieurMin;
    QString temperatureExterieurMax;
    QString humiditeExterieurMin;
    QString humiditeExterieurMax;
    QString poidsMin;
    QString poidsMax;
    QString pressionAtmospheriqueMin;
    QString ensoleillementMin;

    temperatureInterieurMin = ui->lineEditSeuilTemperatureInterieurMin->text();
    temperatureInterieurMax= ui->lineEditSeuilTemperatureInterieurMax->text();
    humiditeInterieurMin = ui->lineEditSeuilHumiditeInterieurMin->text();
    humiditeInterieurMax = ui->lineEditSeuilHumiditeInterieurMax->text();
    temperatureExterieurMin = ui->lineEditSeuilTemperatureExterieurMin->text();
    temperatureExterieurMax = ui->lineEditSeuilTemperatureExterieurMax->text();
    humiditeExterieurMin = ui->lineEditSeuilHumiditeExterieurMin->text();
    humiditeExterieurMax = ui->lineEditSeuilHumiditeExterieurMax->text();
    poidsMin = ui->lineEditSeuilPoidsMin->text();
    poidsMax = ui->lineEditSeuilPoidsMax->text();
    pressionAtmospheriqueMin = ui->lineEditSeuilPressionAtmospherique->text();
    ensoleillementMin = ui->lineEditSeuilEnsoleillement->text();

    QString requete = "UPDATE Seuils SET TemperatureIntMin='" + temperatureInterieurMin + "', TemperatureIntMax='" + temperatureInterieurMax + "', HumiditeIntMin='" + humiditeInterieurMin + "', HumiditeIntMax='" + humiditeInterieurMax + "', TemperatureExtMin='" + temperatureExterieurMin + "', TemperatureExtMax='" + temperatureExterieurMax + "', HumiditeExtMin='" + humiditeExterieurMin + "', HumiditeExtMax='" + humiditeExterieurMax +  "', PressionMin='" + pressionAtmospheriqueMin + "', PressionMax='" + QString::number(PRESSION_ATMOSPHERIQUE_SEUIL_MAX) + "', PoidsMin='" + poidsMin + "', PoidsMax='" + poidsMax + "' WHERE idRuche='" + this->alertes->getIdRuche() + "'";
    qDebug()<< Q_FUNC_INFO << requete;
    bool retour = bdd->executer(requete);
    if(retour)
    {
        qDebug() << Q_FUNC_INFO << temperatureInterieurMin;
        emit envoiSeuilTemperatureInterieurMin(temperatureInterieurMin);
        emit envoiSeuilTemperatureInterieurMax(temperatureInterieurMax);
        emit envoiSeuilHumiditeInterieurMin(humiditeInterieurMin);
        emit envoiSeuilHumiditeInterieurMax(humiditeInterieurMax);
        emit envoiSeuilTemperatureExterieurMin(temperatureExterieurMin);
        emit envoiSeuilTemperatureExterieurMax(temperatureExterieurMax);
        emit envoiSeuilHumiditeExterieurMin(humiditeExterieurMin);
        emit envoiSeuilHumiditeExterieurMax(humiditeExterieurMax);
        emit envoiSeuilPoidsMin(poidsMin);
        emit envoiSeuilPoidsMax(poidsMax);
        emit envoiSeuilPressionAtmospheriqueMin(pressionAtmospheriqueMin);
        emit envoiSeuilEnsoleillementMin(ensoleillementMin);
    }
}
