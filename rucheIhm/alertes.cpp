#include "alertes.h"
#include "infosHumidite.h"
#include "infosPressionAtmospherique.h"
#include "infosTemperature.h"
#include "infosEnsoleillement.h"
#include "infosPoids.h"
#include "infosBatterie.h"
#include "parametres.h"
#include "reglagesAlertesIhm.h"
#include "baseDeDonnees.h"
#include "simple-mail/SmtpMime"
#include "ruche.h"
#include <QDebug>

using namespace SimpleMail;

/**
  *
  * \file rucheIhm/alertes.cpp
  *
  * \brief Définition de la classe Alertes
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la classe Alertes
 *
 * \fn Alertes::Alertes
 *
 * \details Définition des attributs de la classe Alertes
 *
 */

Alertes::Alertes(QString idRuche, QString nomRuche, QObject *parent) : QObject(parent), idRuche(idRuche), nomRuche(nomRuche), infosTemperature(0), infosHumidite(0), infosPressionAtmospherique(0),
    seuilTemperatureInterieurMin(TEMPERATURE_INTERIEUR_SEUIL_MIN), seuilTemperatureInterieurMax(TEMPERATURE_INTERIEUR_SEUIL_MAX), seuilHumiditeInterieurMin(HUMIDITE_INTERIEUR_SEUIL_MIN), seuilHumiditeInterieurMax(HUMIDITE_INTERIEUR_SEUIL_MAX), seuilTemperatureExterieurMin(TEMPERATURE_EXTERIEUR_SEUIL_MIN), seuilTemperatureExterieurMax(TEMPERATURE_EXTERIEUR_SEUIL_MAX), seuilHumiditeExterieurMin(HUMIDITE_EXTERIEUR_SEUIL_MIN), seuilHumiditeExterieurMax(HUMIDITE_EXTERIEUR_SEUIL_MAX), seuilPoidsMin(POIDS_SEUIL_MIN), seuilPoidsMax(POIDS_SEUIL_MAX), seuilEnsoleillement(ENSOLEILLEMENT_SEUIL_MIN), seuilPressionAtmospherique(PRESSION_ATMOSPHERIQUE_SEUIL_MIN)

{
    bdd = BaseDeDonnees::getInstance();
    if(!bdd->estConnecte())
        bdd->connecter(BDD_NOMBASE, BDD_USERNAME, BDD_PASSWORD, BDD_SERVEUR);

    QStringList seuils;
    QString requete = "SELECT * FROM Seuils WHERE idRuche='" + idRuche + "'";
    qDebug()<< Q_FUNC_INFO << requete;
    bool retour = bdd->recuperer(requete, seuils);
    if(retour)
    {
        qDebug() << Q_FUNC_INFO << seuils;
        /*seuilTemperatureInterieurMin = seuils.at(1).toDouble();
        seuilTemperatureInterieurMax = ;
        seuilHumiditeInterieurMin = ;
        seuilHumiditeInterieurMax = ;
        seuilTemperatureExterieurMin = ;
        seuilTemperatureExterieurMax = ;
        seuilHumiditeExterieurMin = ;
        seuilHumiditeExterieurMax = ;
        seuilPoidsMin = ;
        seuilPoidsMax = ;
        seuilEnsoleillement = ;
        seuilPressionAtmospherique = ;*/
    }
}

Alertes::~Alertes()
{
    BaseDeDonnees::detruireInstance();
}

QString Alertes::getIdRuche()
{
    return idRuche;
}

double Alertes::getSeuilTemperatureInterieurMin()
{
    return seuilTemperatureInterieurMin;
}

double Alertes::getSeuilTemperatureInterieurMax()
{
    return seuilTemperatureInterieurMax;
}

double Alertes::getSeuilHumiditeInterieurMin()
{
    return seuilHumiditeInterieurMin;
}

double Alertes::getSeuilHumiditeInterieurMax()
{
    return seuilHumiditeInterieurMax;
}

double Alertes::getSeuilTemperatureExterieurMin()
{
    return seuilTemperatureExterieurMin;
}

double Alertes::getSeuilTemperatureExterieurMax()
{
    return seuilTemperatureExterieurMax;
}

double Alertes::getSeuilHumiditeExterieurMin()
{
    return seuilHumiditeExterieurMin;
}

double Alertes::getSeuilHumiditeExterieurMax()
{
    return seuilHumiditeExterieurMax;
}

double Alertes::getSeuilPoidsMin()
{
    return seuilPoidsMin;
}

double Alertes::getSeuilPoidsMax()
{
    return seuilPoidsMax;
}

double Alertes::getSeuilPressionAtmospherique()
{
    return seuilPressionAtmospherique;
}

double Alertes::getSeuilEnsoleillement()
{
    return seuilEnsoleillement;
}

double Alertes::getSeuilBatterie()
{
    return seuilBatterie;
}

void Alertes::setInfosTemperature(InfosTemperature *infosTemperature)
{
    this->infosTemperature = infosTemperature;
}

void Alertes::setInfosHumidite(InfosHumidite *infosHumidite)
{
    this->infosHumidite = infosHumidite;
}

void Alertes::setInfosPressionAtmospherique(InfosPressionAtmospherique *infosPressionAtmospherique)
{
    this->infosPressionAtmospherique = infosPressionAtmospherique;
}

void Alertes::setInfosPoids(InfosPoids *infosPoids)
{
    this->infosPoids = infosPoids;
}

void Alertes::setInfosEnsoleillement(InfosEnsoleillement *infosEnsoleillement)
{
    this->infosEnsoleillement = infosEnsoleillement;
}

void Alertes::setInfosBatterie(InfosBatterie *infosBatterie)
{
    this->infosBatterie = infosBatterie;
}

/**
 * @brief defini les seuils d'alertes de la temperature interieur
 * \fn    Alertes::alertesTemperatureInterieur()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */

void  Alertes::alertesTemperatureInterieur()
{
    qDebug() << Q_FUNC_INFO;

    double mesureTemperatureInterieur = infosTemperature->getTemperatureInterieur();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureTemperatureInterieur" << mesureTemperatureInterieur << "seuilTemperatureInterieurMin" << seuilTemperatureInterieurMin << "seuilTemperatureInterieurMax" << seuilTemperatureInterieurMax;

    if(mesureTemperatureInterieur > seuilTemperatureInterieurMax)
    {
        qDebug() << Q_FUNC_INFO << "TROP HAUT";
        emit envoiAlertesTemperatureInterieur(tropHaut);
        envoyerMailAlerte(mailApiculteur, "Alerte Température Intérieur", "Bonjour, une alerte a été détectée dans"  + nomRuche + ": température intérieur élevée.");
    }
    else if (mesureTemperatureInterieur < seuilTemperatureInterieurMin)
    {
        qDebug() << Q_FUNC_INFO << "TROP BAS";
        emit envoiAlertesTemperatureInterieur(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Température Intérieur", "Bonjour, une alerte a été détectée dans"  + nomRuche + ": température intérieur basse.");
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "CORRECT";
        emit envoiAlertesTemperatureInterieur(bon);
    }
}

/**
 * @brief defini les seuils d'alertes de la temperature exterieur
 * \fn    Alertes::alertesTemperatureExterieur()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */

void  Alertes::alertesTemperatureExterieur()
{
    double mesureTemperatureExterieur = infosTemperature->getTemperatureExterieur();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureTemperatureExterieur" << mesureTemperatureExterieur << "seuilTemperatureExterieurMin" << seuilTemperatureExterieurMin << "seuilTemperatureExterieurMax" << seuilTemperatureExterieurMax;

    if(mesureTemperatureExterieur > seuilTemperatureExterieurMax)
    {
        emit envoiAlertesTemperatureExterieur(tropHaut);
        envoyerMailAlerte(mailApiculteur, "Alerte Température Extérieur", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : température extérieur élevée.");
        qDebug() << Q_FUNC_INFO << "TROP HAUT";
    }
    else if (mesureTemperatureExterieur < seuilTemperatureExterieurMin)
    {
        qDebug() << Q_FUNC_INFO << "TROP BAS";
        emit envoiAlertesTemperatureExterieur(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Température Extérieur", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : température extérieur basse.");
    }
    else
    {
        emit envoiAlertesTemperatureExterieur(bon);
        qDebug() << Q_FUNC_INFO << "CORRECT";
    }
}

/**
 * @brief defini les seuils d'alertes de l'humidite interieur
 * \fn    Alertes::alertesHumiditeInterieur()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */

void Alertes::alertesHumiditeInterieur()
{
    double mesureHumiditeInterieur = infosHumidite->getHumiditeInterieur();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureHumiditeInterieur" << mesureHumiditeInterieur << "seuilHumiditeInterieurMax" << seuilHumiditeInterieurMax << "mesureHumiditeInterieur" << mesureHumiditeInterieur;

    if(mesureHumiditeInterieur > seuilHumiditeInterieurMax)
    {
        emit envoiAlertesHumiditeInterieur(tropHaut);
        envoyerMailAlerte(mailApiculteur, "Alerte Humidité Intérieur", "Bonjour, une alerte a été détectée dans la " + nomRuche + ": humidité intérieur élevée.");
        qDebug() << Q_FUNC_INFO << "TROP HAUT";
    }
    else if (mesureHumiditeInterieur < seuilHumiditeInterieurMin)
    {
        emit envoiAlertesHumiditeInterieur(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Humidité Intérieur", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  ": humidité intérieur basse.");
        qDebug() << Q_FUNC_INFO << "TROP BAS";
    }
    else
    {
        emit envoiAlertesHumiditeInterieur(bon);
        qDebug() << Q_FUNC_INFO << "CORRECT";
    }
}

/**
 * @brief defini les seuils d'alertes de l'humidite exterieur
 * \fn    Alertes::alertesHumiditeExterieur()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */

void Alertes::alertesHumiditeExterieur()
{
    double mesureHumiditeExterieur = infosHumidite->getHumiditeExterieur();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureHumiditeExterieur" << mesureHumiditeExterieur << "seuilHumiditeExterieurMax" << seuilHumiditeExterieurMax << "seuilHumiditeExterieurMin" << seuilHumiditeExterieurMin;

    if(mesureHumiditeExterieur > seuilHumiditeExterieurMax)
    {
        emit envoiAlertesHumiditeExterieur(tropHaut);
        envoyerMailAlerte(mailApiculteur, "Alerte Extérieur Intérieur", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  ": humidité Extérieur élevée.");
        qDebug() << Q_FUNC_INFO << "TROP HAUT";
    }
    else if (mesureHumiditeExterieur < seuilHumiditeExterieurMin)
    {
        qDebug() << Q_FUNC_INFO << "TROP BAS";
        emit envoiAlertesHumiditeExterieur(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Extérieur Intérieur", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  ": humidité Extérieur basse.");
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "CORRECT";
        emit envoiAlertesHumiditeExterieur(bon);
    }
}

/**
 * @brief defini les seuils d'alertes de pression atmospherique
 * \fn    Alertes::alertesPressionAtmospherique()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */
void Alertes::alertesPressionAtmospherique()
{
    double mesurePressionAtmospherique = infosPressionAtmospherique->getPressionAtmospherique();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesurePressionAtmospherique" << mesurePressionAtmospherique << "seuilPressionAtmospherique" << seuilPressionAtmospherique;

    if (mesurePressionAtmospherique < seuilPressionAtmospherique)
    {
        qDebug() << Q_FUNC_INFO << "TROP BAS";
        emit envoiAlertesPressionAtmospherique(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Pression Atmosphérique", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : pression atmosphérique basse.");
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "CORRECT";
        emit envoiAlertesPressionAtmospherique(bon);
    }
}

/**
 * @brief defini les seuils d'alertes de poids
 * \fn    Alertes::alertesPoids()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */
void Alertes::alertesPoids()
{
    double mesurePoids = infosPoids->getPoids();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesurePoids" << mesurePoids << "seuilPoidsMax" << seuilPoidsMax << "seuilPoidsMin" << seuilPoidsMin;

    if(mesurePoids > seuilPoidsMax)
    {
        emit envoiAlertesPoids(tropHaut);
        envoyerMailAlerte(mailApiculteur, "Alerte Poids", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : poids élevé.");
    }
    else if (mesurePoids < seuilPoidsMin)
    {
        qDebug() << Q_FUNC_INFO;
        emit envoiAlertesPoids(tropBas);
        envoyerMailAlerte(mailApiculteur, "Alerte Poids", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : poids faible.");
    }
    else
        emit envoiAlertesPoids(bon);
}

/**
 * @brief defini les seuils d'alertes d'ensoleillement
 * \fn    Alertes::alertesEnsoleillement()
 * @return \e tropHaut est definit dans le type enum SeuilsAlertes et retourne 0
 * @return \e tropBas est definit dans le type enum SeuilsAlertes et retourne 1
 * @return \e bon est definit dans le type enum SeuilsAlertes et retourne 2
 */
void Alertes::alertesEnsoleillement()
{
    double mesureEnsoleillement = infosEnsoleillement->getEnsoleillement();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureEnsoleillement" << mesureEnsoleillement << "seuilEnsoleillement" << seuilEnsoleillement;

    if (mesureEnsoleillement < seuilEnsoleillement)
    {
        qDebug() << Q_FUNC_INFO << "TROP BAS";
        emit envoiAlertesEnsoleillement(tropBas, mesureEnsoleillement);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "CORRECT";
        emit envoiAlertesEnsoleillement(bon, mesureEnsoleillement);
    }
}

void Alertes::alertesBatterie()
{
    double mesureBatterie = infosBatterie->getMesureBatterieCharge();
    QString requete = "SELECT Email FROM Apiculteur WHERE idApiculteur = '3'";
    QString mailApiculteur;
    bdd->recuperer(requete, mailApiculteur);

    qDebug() << Q_FUNC_INFO << "mesureBatterie" << mesureBatterie << "seuilBatterie" << seuilBatterie;

    if(mesureBatterie < seuilBatterie)
    {
        emit envoiAlertesBatterie(tropBas, mesureBatterie);
        envoyerMailAlerte(mailApiculteur, "Alerte Batterie", "Bonjour, une alerte a été détectée dans la "  + nomRuche +  " : niveau de batterie faible.");
    }
    else
    {
        emit envoiAlertesBatterie(bon, mesureBatterie);
    }
}

/**
 * @brief defini les différents appels des alertes
 * \fn    Alertes::appelerLesAlertes
 * \param typeAlertes TypeAlertes
 */
void Alertes::appelerLesAlertes(TypeAlertes typeAlertes)
{
    qDebug() << Q_FUNC_INFO << "typeAlertes" << typeAlertes;

    switch(typeAlertes)
    {
        case alerteTemperatureInterieur :
            alertesTemperatureInterieur();
        break;
        case alerteTemperatureExterieur :
            alertesTemperatureExterieur();
        break;
        case  alerteHumiditeInterieur :
            alertesHumiditeInterieur();
        break;
        case  alerteHumiditeExterieur :
            alertesHumiditeExterieur();
        break;
        case  alertePressionAtmospherique :
            alertesPressionAtmospherique();
        break;
        case alertePoids :
            alertesPoids();
        break;
        case alerteEnsoleillement :
            alertesEnsoleillement();
        break;
        case alerteBatterie :
            alertesBatterie();
        break;
        case toutesLesAlertes:
            alertesHumiditeExterieur();
            alertesHumiditeInterieur();
            alertesPressionAtmospherique();
            alertesTemperatureExterieur();
            alertesTemperatureInterieur();
            alertesEnsoleillement();
            alertesPoids();
        break;
    }
}

/**
 * @brief slot de reception des seuils de temperature interieur minimum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilTemperatureInterieurMin
 * @param seuil
 */
void Alertes::recevoirSeuilTemperatureInterieurMin(QString seuil)
{
    seuilTemperatureInterieurMin = seuil.toDouble();
    qDebug() << Q_FUNC_INFO << seuilTemperatureInterieurMin;
}

/**
 * @brief slot de reception des seuils de temperature interieur maxmum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilTemperatureInterieurMax
 * @param seuil
 */
void Alertes::recevoirSeuilTemperatureInterieurMax(QString seuil)
{
    seuilTemperatureInterieurMax = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de humidite interieur minimum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilHumiditeInterieurMin
 * @param seuil
 */
void Alertes::recevoirSeuilHumiditeInterieurMin(QString seuil)
{
    seuilHumiditeInterieurMin = seuil.toDouble();
    qDebug() << Q_FUNC_INFO << seuilHumiditeInterieurMin;
}

/**
 * @brief slot de reception des seuils de humidite interieur maximum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilHumiditeInterieurMax
 * @param seuil
 */
void Alertes::recevoirSeuilHumiditeInterieurMax(QString seuil)
{
    seuilHumiditeInterieurMax = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de temperature exterieur minimum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilTemperatureExterieurMin
 * @param seuil
 */
void Alertes::recevoirSeuilTemperatureExterieurMin(QString seuil)
{
    seuilTemperatureExterieurMin = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de temperature exterieur maximum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilTemperatureExterieurMax
 * @param seuil
 */
void Alertes::recevoirSeuilTemperatureExterieurMax(QString seuil)
{
    seuilTemperatureExterieurMax = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de humidite exterieur minimum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilHumiditeExterieurMin
 * @param seuil
 */
void Alertes::recevoirSeuilHumiditeExterieurMin(QString seuil)
{
    seuilHumiditeExterieurMin = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de humidite exterieur maximum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilHumiditeExterieurMax
 * @param seuil
 */
void Alertes::recevoirSeuilHumiditeExterieurMax(QString seuil)
{
    seuilHumiditeExterieurMax = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de poids minimum venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilPoidsMin
 * @param seuil
 */
void Alertes::recevoirSeuilPoidsMin(QString seuil)
{
    seuilPoidsMin = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de poids maximum  venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilPoidsMax
 * @param seuil
 */
void Alertes::recevoirSeuilPoidsMax(QString seuil)
{
    seuilPoidsMax = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils de pression atmospherique venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilPressionAtmospherique
 * @param seuil
 */
void Alertes::recevoirSeuilPressionAtmospherique(QString seuil)
{
    seuilPressionAtmospherique = seuil.toDouble();
}

/**
 * @brief slot de reception des seuils d'ensoleillement venant de la classe ReglagesAlertesIhm
 * \fn Alertes::recevoirSeuilEnsoleillement
 * @param seuil
 */
void Alertes::recevoirSeuilEnsoleillement(QString seuil)
{
    seuilEnsoleillement = seuil.toDouble();
}

void Alertes::recevoirSeuilBatterie(QString seuil)
{
    seuilBatterie = seuil.toDouble();
}

void Alertes::envoyerMailAlerte(QString email, QString objet, QString messageAlerte)
{
    Sender smtp(QLatin1String("smtp.gmail.com"), 465, Sender::SslConnection);
    smtp.setUser(USER_GMAIL);
    smtp.setPassword(PASSWORD_GMAIL);

    MimeMessage message;
    EmailAddress sender(QLatin1String(TTN_EMAIL), QLatin1String("APP_TITRE"));
    message.setSender(sender);
    EmailAddress to(email);
    message.addTo(to);
    message.setSubject(objet);
    MimeText text;
    text.setText(messageAlerte);
    message.addPart(&text);

    if (!smtp.sendMail(message))
    {
        qDebug() << Q_FUNC_INFO << "Erreur" << smtp.lastError();
        return;
    }

    smtp.quit();
}
