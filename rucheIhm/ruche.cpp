#include "ruche.h"
#include "parametres.h"
#include "infosEnsoleillement.h"
#include "infosHumidite.h"
#include "infosPressionAtmospherique.h"
#include "infosTemperature.h"
#include "infosPoids.h"
#include "infosBatterie.h"
#include "alertes.h"
#include "communication.h"
#include "baseDeDonnees.h"
#include "infosBatterie.h"

#include <QTextCodec>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>


/**
  *
  * \file rucheIhm/ruche.cpp
  *
  * \brief Définition de la classe Ruche
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la classe Ruche
 * 
 * \fn Ruche::Ruche
 *
 * \details Définition des pointeurs ensoleillementRuche, humiditeRuche, pressionAtmospheriqueRuche, temperatureRuche, communicationRuche à 0 (0 = NULL soit aucune addresse)
 *
 * \param donneesRucheTTN QStringList Informations sur la ruche à créer
 * \param parent QObject Adresse de l'objet Qt parent
 *
 */
Ruche::Ruche(QStringList donneesRucheTTN, QObject *parent) : QObject(parent), ensoleillementRuche(0), humiditeRuche(0), pressionAtmospheriqueRuche(0), temperatureRuche(0), infosBatterie(0), infosPoids(0), communicationRuche(0), alertes(0), donneesRucheTTN(donneesRucheTTN)
{
    qDebug()<< Q_FUNC_INFO << "donneesRucheTTN" << donneesRucheTTN;
    bdd = BaseDeDonnees::getInstance();
    if(!bdd->estConnecte())
        bdd->connecter(BDD_NOMBASE, BDD_USERNAME, BDD_PASSWORD, BDD_SERVEUR);

    infosBatterie = new InfosBatterie(this);
    infosPoids = new InfosPoids(this);
    ensoleillementRuche = new InfosEnsoleillement(this);
    humiditeRuche = new InfosHumidite(this);
    pressionAtmospheriqueRuche = new InfosPressionAtmospherique(this);
    temperatureRuche = new InfosTemperature(this);

    communicationRuche = new Communication(donneesRucheTTN);

    alertes = new Alertes(donneesRucheTTN.at(0), donneesRucheTTN.at(1), this);

    alertes->setInfosBatterie(infosBatterie);
    alertes->setInfosPoids(infosPoids);
    alertes->setInfosEnsoleillement(ensoleillementRuche);
    alertes->setInfosHumidite(humiditeRuche);
    alertes->setInfosPressionAtmospherique(pressionAtmospheriqueRuche);
    alertes->setInfosTemperature(temperatureRuche);

    connecterGestionMessageBrut();
    connecterGestionMessageTraite();
    gererAlertes();

    connect(communicationRuche, SIGNAL(messageRecu(const QByteArray &, const QMqttTopicName &)), this, SLOT(recevoirMessage(const QByteArray &, const QMqttTopicName &)));
    connect(communicationRuche, SIGNAL(etatClientConnexion(bool)), this, SLOT(recevoirEtatClientConnexion(bool)));
}

/**
 * \brief Destructeur de la classe Ruche
 *
 * \fn Ruche::~Ruche
 *
 */
Ruche::~Ruche()
{
    delete ensoleillementRuche;
    delete humiditeRuche;
    delete pressionAtmospheriqueRuche;
    delete temperatureRuche;
    delete communicationRuche;
    delete infosBatterie;
    delete infosPoids;
    BaseDeDonnees::detruireInstance();
    qDebug()<< Q_FUNC_INFO;
}

Alertes* Ruche::getAlertes()
{
    return alertes;
}

QString Ruche::getIdRuche()
{
    return donneesRucheTTN.at(0);
}

/**
 * \fn       Ruche::connecterGestionMessageBrut
 * \brief    methode regroupant les conection des messages brut
 * \details  methode permetant les conection des messages brut vers les classe infos pour traitement
 */

void Ruche::connecterGestionMessageBrut()
{
    qDebug() << Q_FUNC_INFO;
    connect(this ,SIGNAL(nouvelleTemperatureInterieur(QString, QString)),temperatureRuche,SLOT(traiterNouvelleTemperatureInterieur(QString, QString)));
    connect(this,SIGNAL(nouvelleTemperatureExterieur(QString,QString)),temperatureRuche,SLOT(traiterNouvelleTemperatureExterieur(QString,QString)));
    connect(this,SIGNAL(nouvelleHumiditeInterieur(QString, QString)),humiditeRuche,SLOT(traiterNouvelleHumiditeInterieur(QString, QString)));
    connect(this,SIGNAL(nouvelleHumiditeExterieur(QString,QString)),humiditeRuche,SLOT(traiterNouvelleHumiditeExterieur(QString,QString)));
    connect(this,SIGNAL(nouvelleMesureEnsoleillement(QString,QString)),ensoleillementRuche,SLOT(traiterNouvelleEnsoleillement(QString,QString)));
    connect(this,SIGNAL(nouvellePressionAtmospherique(QString,QString)),pressionAtmospheriqueRuche,SLOT(traiterNouvellePressionAtmospherique(QString,QString)));
    connect(this, SIGNAL(nouvelleMesureCharge(QString,QString)), infosBatterie, SLOT(traiterNouvelleMesureCharge(QString,QString)));
    connect(this, SIGNAL(nouvelleMesureCourant(QString,QString)), infosBatterie, SLOT(traiterNouvelleMesureCourant(QString,QString)));
    connect(this, SIGNAL(nouvelleMesureTension(QString,QString)), infosBatterie, SLOT(traiterNouvelleMesureTension(QString,QString)));
    connect(this, SIGNAL(nouvelleMesurePoids(QString,QString)), infosPoids, SLOT(traiterNouveauPoids(QString,QString)));
}

/**
 * \fn       connecterGestionMessageTraite
 * \brief    methode regroupant les conection des messages taité cenant des classe
 * \details  methode permetant les conection des messages traité des classe infos vers la classe ruche
 */

void Ruche::connecterGestionMessageTraite()
{
    qDebug() << Q_FUNC_INFO;
    connect(temperatureRuche,SIGNAL(temperatureInterieurEnvoye(double, QString)),this,SLOT(recevoirTemperatureInterieurTraite(double, QString)));
    connect(temperatureRuche,SIGNAL(temperatureExterieurEnvoye(double,QString)),this,SLOT(recevoirTemperatureExterieurTraite(double,QString)));
    connect(humiditeRuche,SIGNAL(humiditeInterieurEnvoye(double, QString)),this,SLOT(recevoirMesureHumiditeInterieurTraite(double, QString)));
    connect(humiditeRuche,SIGNAL(humiditeExterieurEnvoye(double,QString)),this,SLOT(recevoirMesureHumiditeExterieurTraite(double,QString)));
    connect(pressionAtmospheriqueRuche,SIGNAL(pressionAtmospheriqueEnvoye(double,QString)),this,SLOT(recevoirPressionAtmospheriqueTraite(double,QString)));
    connect(ensoleillementRuche,SIGNAL(ensoleillementEnvoye(double,QString)),this,SLOT(recevoirEnsoleillementTraite(double,QString)));
    connect(infosBatterie, SIGNAL(tensionEnvoye(double,QString)),this, SLOT(recevoirMesureTensionTraite(double,QString)));
    connect(infosBatterie, SIGNAL(courantEnvoye(double,QString)),this, SLOT(recevoirMesureCourantTraite(double,QString)));
    connect(infosBatterie, SIGNAL(chargeEnvoye(double,QString)), this, SLOT(recevoirMesureChargeTraite(double,QString)));
    connect(infosPoids, SIGNAL(poidsEnvoye(double,QString)), this, SLOT(recevoirMesurePoidsTraite(double,QString)));

    connect(temperatureRuche,SIGNAL(traitementTemperatureInterieurEnvoye(const double, const double, const double, int)), this, SLOT(recevoirTraitementTemperatureInterieur(const double,const double,const double, int)));
    connect(temperatureRuche,SIGNAL(traitementTemperatureExterieurEnvoye(const double, const double, const double, int)), this , SLOT(recevoirTraitementTemperatureExterieur(const double,const double,const double, int)));
    connect(humiditeRuche, SIGNAL(traitementHumiditeInterieurEnvoye(double,double,double,int)), this,SLOT(recevoirTraitementHumiditeInterieur(double,double,double,int)));
    connect(humiditeRuche, SIGNAL(traitementHumiditeExterieurEnvoye(double,double,double,int)), this,SLOT(recevoirTraitementHumiditeExterieur(double,double,double,int)));
    connect(pressionAtmospheriqueRuche, SIGNAL(traitementPressionAtmospheriqueEnvoye(double,double,double,int)), this, SLOT(recevoirTraitementPressionAtmospherique(double,double,double,int)));
    connect(ensoleillementRuche, SIGNAL(traitementEnsoleillementEnvoye(double,double,double,int)), this, SLOT(recevoirTraitementEnsoleillement(double,double,double,int)));
}

void Ruche::gererAlertes()
{
    qDebug() << Q_FUNC_INFO;
    connect(alertes, SIGNAL(envoiAlertesTemperatureInterieur(SeuilsAlertes)),this,SLOT(recevoirAlertesTemperatureInterieur(SeuilsAlertes)));
    connect(alertes, SIGNAL(envoiAlertesTemperatureExterieur(SeuilsAlertes)),this,SLOT(recevoirAlertesTemperatureExterieur(SeuilsAlertes)));
    connect(alertes, SIGNAL(envoiAlertesHumiditeInterieur(SeuilsAlertes)),this,SLOT(recevoirAlertesHumiditeInterieur(SeuilsAlertes)));
    connect(alertes, SIGNAL(envoiAlertesHumiditeExterieur(SeuilsAlertes)),this,SLOT(recevoirAlertesHumiditeExterieur(SeuilsAlertes)));
    connect(alertes, SIGNAL(envoiAlertesPressionAtmospherique(SeuilsAlertes)),this,SLOT(recevoirAlertesPressionAtmospherique(SeuilsAlertes)));
    connect(alertes, SIGNAL(envoiAlertesEnsoleillement(SeuilsAlertes, double)),this, SLOT(recevoirAlertesEnsoleillement(SeuilsAlertes, double)));
    connect(alertes, SIGNAL(envoiAlertesBatterie(SeuilsAlertes,double)),this, SLOT(recevoirAlertesBatterie(SeuilsAlertes, double)));
    connect(alertes, SIGNAL(envoiAlertesPoids(SeuilsAlertes)),this, SLOT(recevoirAlertesPoids(SeuilsAlertes)));
}

/**
 * \brief    slot recevant les données
 * \fn       Ruche::recevoirMessage
 * \details  slot permetant la reception des données et du nom de topic
 * \param    message    message reçu émis par le serveur ttn grace au potocole mqtt
 * \param    topic      nom du topic emettant la données
 */

void Ruche::recevoirMessage(const QByteArray &message, const QMqttTopicName &topic)
{   
    Q_UNUSED(topic)
    PortsTTN port = recupererNumeroDePort(message);
    extraireDonnees(port,message);
}

void Ruche::recevoirEtatClientConnexion(bool etat)
{
    emit etatClientConnexion(etat);
}

QString Ruche::extraireDonneesBrutes(const QByteArray &message)
{
    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();

    QString messageTraite = objetJSON.value(QString("payload_raw")).toString();
    qDebug() << Q_FUNC_INFO << "payload_raw=" << messageTraite;

    return messageTraite;
}

QString Ruche::extrairePoids(const QString &donneesBrutes)
{
    QByteArray donneesBrutesPoids;
    unsigned int poidsRuche = 0;
    unsigned char poidsRucheMSB;
    unsigned char poidsRucheLSB;

    donneesBrutesPoids = QByteArray::fromBase64(donneesBrutes.toLocal8Bit());
    poidsRucheMSB = donneesBrutesPoids.at(0);
    poidsRucheLSB = donneesBrutesPoids.at(1);
    poidsRuche = ((poidsRucheMSB << 8) | poidsRucheLSB); // poids en grammes

    qDebug("%s Poids MSB/LSB = %02X%02X", Q_FUNC_INFO, poidsRucheMSB, poidsRucheLSB);
    //qDebug() << Q_FUNC_INFO << "poidsRuche=" << poidsRuche;

    return QString::number((double)poidsRuche/(double)1000.); // en Kg
}

/**
 * \brief    recuperer le numero de port
 * \fn       Ruche::recupererNumeroDePort
 * \details  Méthode permetant de recuperer le nupéro de port en extrayant celle ci dans l'objet Json "ports"
 * \param    message    message reçu émis par le serveur ttn grace au potocole mqtt
 * \return   Un \e PortsTTN correspondant au portsttn extrait
 */

PortsTTN Ruche::recupererNumeroDePort(const QByteArray message)
{
    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();
    PortsTTN numeroPort = (PortsTTN)objetJSON.value(QString("port")).toInt();
    qDebug() << Q_FUNC_INFO << "numeroPort=" << numeroPort;

    // port valide ?
    if(numeroPort > portInconnu && numeroPort < nbPortsTTN)
        return numeroPort;
    return portInconnu;
}

/**
 * \brief    extait les données
 * \fn       Ruche::extraireDonnees
 * \details  extait les données avec le numéro de port spécifié par la méthode recupérerNomDePort
 * \param    port       correspond au numéro de port
 * \param    message    message reçu émis par le serveur ttn grace au potocole mqtt
 */

void Ruche::extraireDonnees(PortsTTN port, QByteArray message)
{
    qDebug() << Q_FUNC_INFO << "port=" << port << "message=" << message;
    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();

    // attributs ?
    QString donneesBrutes;

    switch (port)
    {
        case portMesureEnergie :
            donneesRuche.donneesBatterie.charge = recupererDonnees(message, "payload_fields", "charge");
            donneesRuche.donneesBatterie.courant = recupererDonnees(message, "payload_fields", "courant");
            donneesRuche.donneesBatterie.tension = recupererDonnees(message, "payload_fields", "tension");
            donneesRuche.donneesBatterie.horodatagecharge = recupererHorodatage(message,"metadata","time");
            donneesRuche.donneesBatterie.horodatagecourant = recupererHorodatage(message,"metadata","time");
            donneesRuche.donneesBatterie.horodatagetension = recupererHorodatage(message,"metadata","time");

            emit nouvelleMesureCharge(donneesRuche.donneesBatterie.charge,donneesRuche.donneesBatterie.horodatagecharge );
            emit nouvelleMesureCourant(donneesRuche.donneesBatterie.courant,donneesRuche.donneesBatterie.horodatagecourant);
            emit nouvelleMesureTension(donneesRuche.donneesBatterie.tension,donneesRuche.donneesBatterie.horodatagetension);
            insererDonneesPortBatterie();
            recupererLocalisationPasserelle(message);
            qDebug() << Q_FUNC_INFO << "charge = " << donneesRuche.donneesBatterie.charge << "tension = " << donneesRuche.donneesBatterie.tension << "courant = " << donneesRuche.donneesBatterie.courant;
            break;
        case portMesurePoids :
            donneesRuche.poids = recupererDonnees(message,"payload_fields","poids");
            donneesRuche.horodatagePoids = recupererHorodatage(message,"metadata","time");
            //donneesBrutes = extraireDonneesBrutes(message);
            //donneesRuche.poids = extrairePoids(donneesBrutes);
            qDebug() << Q_FUNC_INFO << "Mesure poids=" << donneesRuche.poids;
            emit nouvelleMesurePoids(donneesRuche.poids, donneesRuche.horodatagePoids);
            insererDonneesPortPoids();
            recupererLocalisationPasserelle(message);
            break;
        case portMesureRuche :        
            //qDebug() << Q_FUNC_INFO << "payload_fields:{\"temperature\"}=" << objetJSON.value("payload_fields").toObject().value("temperature");
            donneesRuche.temperatureInterieur = recupererDonnees(message, "payload_fields", "temperature");
            qDebug() << Q_FUNC_INFO << "Mesure temperatureInterieur=" << donneesRuche.temperatureInterieur;

            donneesRuche.horodatageTemperatureInterieur =  recupererHorodatage(message , "metadata", "time");
            qDebug() << Q_FUNC_INFO << "Horodatage temperatureInterieur=" << donneesRuche.horodatageTemperatureInterieur;
            emit nouvelleTemperatureInterieur(donneesRuche.temperatureInterieur, donneesRuche.horodatageTemperatureInterieur);
            donneesRuche.humiditeInterieur = recupererDonnees(message, "payload_fields", "humidite");
            donneesRuche.horodatageHumiditeInterieur = recupererHorodatage(message,"metadata","time");
            emit nouvelleHumiditeInterieur(donneesRuche.humiditeInterieur, donneesRuche.horodatageHumiditeInterieur);
            insererDonneesPortMesureRuche();
            recupererLocalisationPasserelle(message);
            break;
        case portMesureEnvironement :
            donneesRuche.temperatureExterieur = recupererDonnees(message,"payload_fields", "temperature");
            donneesRuche.horodatageTemperatureExterieur = recupererHorodatage(message,"metadata","time");
            emit nouvelleTemperatureExterieur(donneesRuche.temperatureExterieur, donneesRuche.horodatageTemperatureExterieur);

            donneesRuche.humiditeExterieur = recupererDonnees(message,"payload_fields", "humidite");
            donneesRuche.horodatageHumiditeExterieur = recupererHorodatage(message,"metadata","time");
            emit nouvelleHumiditeExterieur(donneesRuche.humiditeExterieur, donneesRuche.horodatageHumiditeExterieur);

            donneesRuche.pressionAtmospherique = recupererDonnees(message,"payload_fields", "pression");
            donneesRuche.horodatagePressionAtmospherique = recupererHorodatage(message,"metadata","time");
            emit nouvellePressionAtmospherique(donneesRuche.pressionAtmospherique, donneesRuche.horodatagePressionAtmospherique );
            insererDonneesPortMesureEnvironnement();
            recupererLocalisationPasserelle(message);
            break;

        case portMesureEnsoleillement :
            donneesRuche.ensoleillement = recupererDonnees(message,"payload_fields", "ensoleillement");
            donneesRuche.horodatageEnsoleillement = recupererHorodatage(message,"metadata","time");
            emit nouvelleMesureEnsoleillement(donneesRuche.ensoleillement, donneesRuche.horodatageEnsoleillement);
            insererDonneesPortEnsoleillement();
            recupererLocalisationPasserelle(message);
            break;
        default:
            break;
    }

}

void Ruche::recupererLocalisationPasserelle(QByteArray message)
{
    QString longitude;
    QString latitude;

    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();

    qDebug() << Q_FUNC_INFO << "latitude" << objetJSON.value("metadata").toObject().value("gateways").toArray().at(0).toObject().value("latitude").toDouble();
    qDebug() << Q_FUNC_INFO << "longitude" << objetJSON.value("metadata").toObject().value("gateways").toArray().at(0).toObject().value("longitude").toDouble();

    latitude = QString::number(objetJSON.value("metadata").toObject().value("gateways").toArray().at(0).toObject().value("latitude").toDouble());
    longitude = QString::number(objetJSON.value("metadata").toObject().value("gateways").toArray().at(0).toObject().value("longitude").toDouble());

    emit envoiLocalisationPasserelle(longitude, latitude);
}


/**
 * \brief    recuperer les données
 * \fn       Ruche::recupererDonnees
 * \details  Méthode permetant de recuperer les données en extrayant celle ci dans un sous objet Json
 * \param    message        message reçu message reçu émis par le serveur ttn grace au potocole mqtt
 * \param    objetJson      nom de l'objet Json
 * \param    sousObjetJson  nom du sous objet Json
 * \return   Un \e QString correspondant a la donnée
 */

QString Ruche::recupererDonnees(QByteArray message, QString objetJson, QString sousObjetJson)
{
    QString donnees;
    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();

    donnees = QString::number(objetJSON.value(objetJson).toObject().value(sousObjetJson).toDouble());
    return donnees;
}

/**
 * \brief    recuperer l'horodatage
 * \fn       Ruche::recupererHorodatage
 * \details  Méthode permetant de recuperer l'horodatage en extrayant celle ci dans un sous objet Json
 * \param    message        message reçu message reçu émis par le serveur ttn grace au potocole mqtt
 * \param    objetJson      nom de l'objet Json
 * \param    sousObjetJson  nom du sous objet Json
 * \return   Un \e QString correspondant a l'horodatage
 */

QString Ruche::recupererHorodatage(QByteArray message, QString objetJson, QString sousObjetJson)
{
    QString horodatageJSON;
    QJsonDocument documentJSON = QJsonDocument::fromJson(message);
    QJsonObject objetJSON = documentJSON.object();

    horodatageJSON = (objetJSON.value(objetJson).toObject().value(sousObjetJson).toString());

    // Formatage FR
    //QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageJSON.split(".").at(0), "yyyy-MM-ddTHH:mm:ss");
    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageJSON, Qt::ISODate).toLocalTime();

    return dateTimeHorodatage.toString("dd/MM/yyyy HH:mm:ss");
}


/**
 * \brief    slot recevant la temperature interieur traite
 * \fn       Ruche::recevoirTemperatureInterieurTraite
 * \details  cette fonction emet la valeur traitée a l'ihm
 * \param    temperatureInterieur    correspond aux mesures temperature traitée
 * \param    horodatage              correspond a l'horadatage de la temperature interieur
 */

void Ruche::recevoirTemperatureInterieurTraite(double temperatureInterieur, QString horodatage)
{
    qDebug() << Q_FUNC_INFO << "temperatureInterieur=" << temperatureInterieur;    
    alertes->appelerLesAlertes(alerteTemperatureInterieur);
    emit nouvelleMesureTemperatureInterieurTraite(temperatureInterieur, horodatage);

}

/**
 * \brief    slot recevant l'humidite interieur traite
 * \fn       recevoirMesureHumiditeInterieurTraite
 * \details  cette fonction émet la valeur traitée a l'ihm
 * \param    humiditeInterieurTraite    correspond aux mesures d'humidite traitée
 * \param    horodatage                 correspond a l'horadatage de la temperature interieur
 */

void Ruche::recevoirMesureHumiditeInterieurTraite(double humiditeInterieurTraite,QString horodatage)
{    
    qDebug() << Q_FUNC_INFO << "humiditeInterieurTraite=" << humiditeInterieurTraite;
    alertes->appelerLesAlertes(alerteHumiditeInterieur);
    emit nouvelleMesureHumiditeInterieurTraite(humiditeInterieurTraite, horodatage);
}

/**
 * \brief    slot recevant la temperature exterieur traite
 * \fn       Ruche::recevoirTemperatureExterieurTraite
 * \details  cette fonction emet la valeur traitée a l'ihm
 * \param    temperatureExterieur    correspond à la temperature extéreure traitée
 * \param    horodatage              correspond a l'horadatage de la temperature exterieur
 */

void Ruche::recevoirTemperatureExterieurTraite(double temperatureExterieur,QString horodatage)
{
    qDebug() << Q_FUNC_INFO << "temperature Exterieur =" << temperatureExterieur;
    emit nouvelleMesureTemperatureExterieurTraite(temperatureExterieur, horodatage);
    alertes->appelerLesAlertes(alerteTemperatureExterieur);
}

/**
 * \brief    slot recevant l'humidite exterieur traite
 * \fn       recevoirMesureHumiditeExterieurTraite
 * \details  cette fonction émet la valeur traitée a l'ihm
 * \param    humiditeExterieurTraite    correspond aux mesures d'humidite exterieur traitée
 * \param    horodatage                 corespond a horodatage de l'humidite exterieur  traitée
 */

void Ruche::recevoirMesureHumiditeExterieurTraite(double humiditeExterieurTraite,QString horodatage)
{
    emit nouvelleMesureHumiditeExterieurTraite(humiditeExterieurTraite, horodatage);
    alertes->appelerLesAlertes(alerteHumiditeExterieur);
}

/**
 * \brief    slot recevant la pression atmospherique traite
 * \fn       recevoirMesureHumiditeExterieurTraite
 * \details  cette fonction émet la valeur traitée a l'ihm
 * \param    pressionAtmospherique    correspond aux mesures de pression atmospherique traitée
 * \param    horodatage                 corespond a horodatage de la pression atmospherique exterieur  traitée
 */
void Ruche::recevoirPressionAtmospheriqueTraite(double pressionAtmospherique, QString horodatage)
{
    emit nouvellePressionAtmospheriqueTraite(pressionAtmospherique,horodatage);
    alertes->appelerLesAlertes(alertePressionAtmospherique);
}

void Ruche::recevoirMesureTensionTraite(double tension, QString horodatage)
{
    emit nouvelleMesureTension(tension,horodatage);
}

void Ruche::recevoirMesureCourantTraite(double courant, QString horodatage)
{
    emit nouvelleMesureCourant(courant,horodatage);
}

void Ruche::recevoirMesureChargeTraite(double charge, QString horodatage)
{
    emit nouvelleMesureCharge(charge,horodatage);
    alertes->appelerLesAlertes(alerteBatterie);
}

void Ruche::recevoirMesurePoidsTraite(double poids, QString horodatage)
{
    emit nouvelleMesurePoids(poids,horodatage);
    alertes->appelerLesAlertes(alertePoids);
}

/**
 * \brief    slot recevant l'ensoleillement traite
 * \fn       Ruche::recevoirEnsoleillementTraite
 * \details  cette fonction émet la valeur traitée a l'ihm
 * \param    ensoleillement    correspond aux mesures d'ensoleillement traitée
 * \param    horodatage                 corespond a horodatage de l'ensoleillement exterieur  traitée
 */

void Ruche::recevoirEnsoleillementTraite(double ensoleillement, QString horodatage)
{
    qDebug() << Q_FUNC_INFO;

    emit nouvelleMesureEnsoleillementTraite(ensoleillement,horodatage);
    alertes->appelerLesAlertes(alerteEnsoleillement);
}

void Ruche::recevoirAlertesTemperatureInterieur(SeuilsAlertes seuilsAlertes)
{
    qDebug() << Q_FUNC_INFO << seuilsAlertes;

    emit envoiAlertesTemperatureInterieur(seuilsAlertes);
}

void Ruche::recevoirAlertesTemperatureExterieur(SeuilsAlertes seuilsAlertes)
{
    qDebug() << Q_FUNC_INFO;

    emit envoiAlertesTemperatureExterieur(seuilsAlertes);
}

void Ruche::recevoirAlertesHumiditeInterieur(SeuilsAlertes seuilsAlertes)
{
    qDebug() << Q_FUNC_INFO;

    emit envoiAlertesHumiditeInterieur(seuilsAlertes);
}

void Ruche::recevoirAlertesHumiditeExterieur(SeuilsAlertes seuilsAlertes)
{
    emit envoiAlertesHumiditeExterieur(seuilsAlertes);
}

void Ruche::recevoirAlertesPressionAtmospherique(SeuilsAlertes seuilsAlertes)
{
    qDebug() << Q_FUNC_INFO;

    emit envoiAlertesPressionAtmospherique(seuilsAlertes);
}

void Ruche::recevoirAlertesPoids(SeuilsAlertes seuilsAlertes)
{
    emit envoiAlertesPoids(seuilsAlertes);
}

void Ruche::recevoirAlertesEnsoleillement(SeuilsAlertes seuilsAlertes, double niveauEnsoleillement)
{
    emit envoiAlertesEnsoleillement(seuilsAlertes, niveauEnsoleillement);
}

void Ruche::recevoirAlertesBatterie(SeuilsAlertes seuilsAlertes, double charge)
{
    emit envoiAlertesBatterie(seuilsAlertes, charge);
}

void Ruche::insererDonneesPortMesureRuche()
{

    QDateTime dateTimePortMesureRuche = QDateTime::fromString(temperatureRuche->getHorodatageTemperatureInterieur(),"dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "ruche" << donneesRucheTTN.at(0) << dateTimePortMesureRuche.toString("yyyy-MM-dd  HH:mm:ss") << "temperatureInterieur" <<  temperatureRuche->getTemperatureInterieur() << "humiditeInterieur" << humiditeRuche->getHumiditeInterieur();
    QString requete = "INSERT INTO MesuresRuche (MesuresRuche.idRuche, MesuresRuche.Temperature, MesuresRuche.Humidite, MesuresRuche.Horodatage) VALUES ('" + donneesRucheTTN.at(0) +"','" + donneesRuche.temperatureInterieur + "','" + donneesRuche.humiditeInterieur +  "','" + dateTimePortMesureRuche.toString("yyyy-MM-dd  HH:mm:ss") + "')";
    bdd->executer(requete);
}

void Ruche::insererDonneesPortMesureEnvironnement()
{
    QDateTime dateTimePortMesureEnvironnement = QDateTime::fromString(temperatureRuche->getHorodatageTemperatureExterieur(),"dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "ruche" << donneesRucheTTN.at(0) << dateTimePortMesureEnvironnement.toString("yyyy-MM-dd  HH:mm:ss");
    QString requete = "INSERT INTO MesuresEnvironnement(idRuche, Temperature, Humidite, Pression, Horodatage) VALUES ('" + donneesRucheTTN.at(0) + "','"  + donneesRuche.temperatureExterieur + "','" + donneesRuche.humiditeExterieur + "','" + donneesRuche.pressionAtmospherique + "','" + dateTimePortMesureEnvironnement.toString("yyyy-MM-dd  HH:mm:ss") + "')";
    bdd->executer(requete);
}

void Ruche::insererDonneesPortEnsoleillement()
{
    QDateTime dateTimePortMesureEnsoleillement = QDateTime::fromString(ensoleillementRuche->getHorodatageEnsoleillement(),"dd/MM/yyyy HH:mm:ss");
    QString requete = "INSERT INTO MesuresEnsoleillement(idRuche, Ensoleillement, Horodatage) VALUES ('" + donneesRucheTTN.at(0) + "','"  + donneesRuche.ensoleillement + "','" + dateTimePortMesureEnsoleillement.toString("yyyy-MM-dd  HH:mm:ss") + "')";
    bdd->executer(requete);

}

void Ruche::insererDonneesPortPoids()
{
    QDateTime dateTimePortMesurePoids = QDateTime::fromString(infosPoids->getHorodatagePoids(),"dd/MM/yyyy HH:mm:ss");
    QString requete = "INSERT INTO MesuresPoids(idRuche, Poids, Horodatage) VALUES ('" + donneesRucheTTN.at(0) + "','"  + donneesRuche.poids + "','" + dateTimePortMesurePoids.toString("yyyy-MM-dd  HH:mm:ss") + "')";
    bdd->executer(requete);
}

void Ruche::insererDonneesPortBatterie()
{
    QDateTime dateTimePortMesureBatterie = QDateTime::fromString(infosBatterie->getHorodatageBatterie(),"dd/MM/yyyy HH:mm:ss");
    QString requete = "INSERT INTO MesuresEnergie(idRuche, Charge, Tension, Courant, Horodatage) VALUES ('" + donneesRucheTTN.at(0) + "','"  + donneesRuche.donneesBatterie.charge + "','" + donneesRuche.donneesBatterie.tension + "','" + donneesRuche.donneesBatterie.courant + "','" + dateTimePortMesureBatterie.toString("yyyy-MM-dd  HH:mm:ss") + "')";
    bdd->executer(requete);
}

// ->
void Ruche::insererMesureHoraireRuche()
{
    if(mesureHoraireRuche.temperaturesRecues && mesureHoraireRuche.humiditesRecues)
    {
        QDate dateMesureRuche = QDate::currentDate();
        QTime heureMesureRuche(mesureHoraireRuche.heure, 0);

        QString requete = "INSERT INTO MesuresJournalieresRuche (MesuresJournalieresRuche.idRuche, MesuresJournalieresRuche.Temperature, MesuresJournalieresRuche.TemperatureMin, MesuresJournalieresRuche.TemperatureMax, MesuresJournalieresRuche.Humidite, MesuresJournalieresRuche.HumiditeMin, MesuresJournalieresRuche.HumiditeMax, MesuresJournalieresRuche.DateMesure, MesuresJournalieresRuche.HeureMesure) VALUES ('" + donneesRucheTTN.at(0) +"','" + QString::number(mesureHoraireRuche.temperatureInterieurMoyenne) + "','" + QString::number(mesureHoraireRuche.temperatureInterieurMinimum) + "','" + QString::number(mesureHoraireRuche.temperatureInterieurMaximum) + "','" + QString::number(mesureHoraireRuche.humiditeInterieurMoyenne) + "','" + QString::number(mesureHoraireRuche.humiditeInterieurMinimum) + "','" + QString::number(mesureHoraireRuche.humiditeInterieurMaximum) + "','" + dateMesureRuche.toString("yyyy-MM-dd") + "', '" + heureMesureRuche.toString("HH:mm:ss") + "')";
        bdd->executer(requete);
        mesureHoraireRuche.temperaturesRecues = false;
        mesureHoraireRuche.humiditesRecues = false;
    }
}

void Ruche::insererMesureHoraireEnvironnement()
{
    if(mesureHoraireEnvironement.temperaturesRecues && mesureHoraireEnvironement.humiditesRecues && mesureHoraireEnvironement.pressionAtmospheriqueRecues)
    {
        QDate dateMesureEnvironement = QDate::currentDate();
        QTime heureMesureEnvironement(mesureHoraireEnvironement.heure, 0);

        QString requete = "INSERT INTO MesuresJournalieresEnvironnement (MesuresJournalieresEnvironnement.idRuche, MesuresJournalieresEnvironnement.Temperature, MesuresJournalieresEnvironnement.TemperatureMin, MesuresJournalieresEnvironnement.TemperatureMax, MesuresJournalieresEnvironnement.Humidite, MesuresJournalieresEnvironnement.HumiditeMin, MesuresJournalieresEnvironnement.HumiditeMax, MesuresJournalieresEnvironnement.Pression, MesuresJournalieresEnvironnement.PressionMin, MesuresJournalieresEnvironnement.PressionMax, MesuresJournalieresEnvironnement.DateMesure, MesuresJournalieresEnvironnement.HeureMesure) VALUES ('" + donneesRucheTTN.at(0) +"','" + QString::number(mesureHoraireEnvironement.temperatureExterieurMoyenne) + "','" + QString::number(mesureHoraireEnvironement.temperatureExterieurMinimum) + "','" + QString::number(mesureHoraireEnvironement.temperatureExterieurMaximum) + "','" + QString::number(mesureHoraireEnvironement.humiditeExterieurMoyenne) + "','" + QString::number(mesureHoraireEnvironement.humiditeExterieurMinimum) + "','" + QString::number(mesureHoraireEnvironement.humiditeExterieurMaximum)+ "','" + QString::number(mesureHoraireEnvironement.pressionAtmospheriqueMoyenne) + "','"+QString::number(mesureHoraireEnvironement.pressionAtmospheriqueMinimum) + "', '" + QString::number(mesureHoraireEnvironement.pressionAtmospheriqueMaximum)+ "', '" + dateMesureEnvironement.toString("yyyy-MM-dd") + "', '" + heureMesureEnvironement.toString("HH:mm:ss") + "')";
        bdd->executer(requete);
        mesureHoraireEnvironement.temperaturesRecues = false;
        mesureHoraireEnvironement.humiditesRecues = false;
        mesureHoraireEnvironement.pressionAtmospheriqueRecues = false;
    }
}

void Ruche::insererMesureHoraireEnsoleillement()
{
    if(mesuresHorraireEnsoleillement.ensoleillementsRecues)
    {
        QDate dateMesureEnsoleillement= QDate::currentDate();
        QTime heureMesureEnsoleillement(mesuresHorraireEnsoleillement.heure, 0);

        QString requete = "INSERT INTO MesuresJournalieresEnsoleillement (MesuresJournalieresEnsoleillement.idRuche, MesuresJournalieresEnsoleillement.Ensoleillement, MesuresJournalieresEnsoleillement.EnsoleillementMin, MesuresJournalieresEnsoleillement.EnsoleillementMax, MesuresJournalieresEnsoleillement.DateMesure, MesuresJournalieresEnsoleillement.HeureMesure) VALUES ('" + donneesRucheTTN.at(0) +"','" + QString::number(mesuresHorraireEnsoleillement.ensoleillementsMoyen) + "','" + QString::number(mesuresHorraireEnsoleillement.ensoleillementsMinimum) + "','" + QString::number(mesuresHorraireEnsoleillement.ensoleillementsMaximum) + "','" + dateMesureEnsoleillement.toString("yyyy-MM-dd") + "', '" + heureMesureEnsoleillement.toString("HH:mm:ss") + "')";
        bdd->executer(requete);
        mesuresHorraireEnsoleillement.ensoleillementsRecues = false;
    }
}

void Ruche::insererMesureHorairePoids()
{

}

void Ruche::insererMesureHoraireBatterie()
{

}


void Ruche::recevoirTraitementTemperatureInterieur(const double temperatureInterieurMoyenne ,const double temperatureInterieurMinimum ,const double temperatureInterieurMaximum, int heure)
{
    qDebug() << Q_FUNC_INFO << "temperatureInterieurMoyenne" << temperatureInterieurMoyenne << "temperatureInterieurMinimum" << temperatureInterieurMinimum << "temperatureInterieurMaximum" << temperatureInterieurMaximum << "heure" << heure;
    mesureHoraireRuche.heure = heure;
    mesureHoraireRuche.temperatureInterieurMoyenne = temperatureInterieurMoyenne;
    mesureHoraireRuche.temperatureInterieurMinimum = temperatureInterieurMinimum;
    mesureHoraireRuche.temperatureInterieurMaximum = temperatureInterieurMaximum;
    mesureHoraireRuche.temperaturesRecues = true;
    insererMesureHoraireRuche();
    emit nouvelleMesureTemperatureInterieurTraiteParHeure();
}

void Ruche::recevoirTraitementTemperatureExterieur(const double temperatureExterieurMoyenne ,const double temperatureExterieurMinimum ,const double temperatureExterieurMaximum, int heure)
{
    mesureHoraireEnvironement.heure = heure;
    mesureHoraireEnvironement.temperatureExterieurMoyenne = temperatureExterieurMoyenne;
    mesureHoraireEnvironement.temperatureExterieurMinimum = temperatureExterieurMinimum;
    mesureHoraireEnvironement.temperatureExterieurMaximum = temperatureExterieurMaximum;
    mesureHoraireEnvironement.temperaturesRecues = true;
    insererMesureHoraireEnvironnement();
    emit nouvelleMesureTemperatureExterieurTraiteParHeure();
}

void Ruche::recevoirTraitementHumiditeInterieur(const double humiditeInterieurMoyenne ,const double humiditeInterieurMinimum ,const double humiditeInterieurMaximum, int heure)
{
    qDebug() << Q_FUNC_INFO << "humiditeInterieurMoyenne" << humiditeInterieurMoyenne << "humiditeInterieurMinimum" << humiditeInterieurMinimum << "humiditeInterieurMaximum" << humiditeInterieurMaximum << "heure" << heure;
    mesureHoraireRuche.heure = heure;
    mesureHoraireRuche.humiditeInterieurMoyenne = humiditeInterieurMoyenne;
    mesureHoraireRuche.humiditeInterieurMinimum = humiditeInterieurMinimum;
    mesureHoraireRuche.humiditeInterieurMaximum = humiditeInterieurMaximum;
    mesureHoraireRuche.humiditesRecues = true;
    insererMesureHoraireRuche();
    emit nouvelleMesureHumiditeInterieurTraiteParHeure();
}

void Ruche::recevoirTraitementHumiditeExterieur(const double humiditeExterieurMoyenne ,const double humiditeExterieurMinimum ,const double humiditeExterieurMaximum, int heure)
{
    mesureHoraireEnvironement.heure = heure;
    mesureHoraireEnvironement.humiditeExterieurMoyenne = humiditeExterieurMoyenne;
    mesureHoraireEnvironement.humiditeExterieurMinimum = humiditeExterieurMinimum;
    mesureHoraireEnvironement.humiditeExterieurMaximum = humiditeExterieurMaximum;
    mesureHoraireEnvironement.humiditesRecues = true;
    insererMesureHoraireEnvironnement();
    emit nouvelleMesureHumiditeExterieurTraiteParHeure();
}

void Ruche::recevoirTraitementPressionAtmospherique(const double pressionAtmospheriqueMoyenne, const double pressionAtmospheriqueMinimum, const double pressionAtmospheriqueMaximum, int heure)
{
    mesureHoraireEnvironement.heure = heure;
    mesureHoraireEnvironement.pressionAtmospheriqueMoyenne = pressionAtmospheriqueMoyenne;
    mesureHoraireEnvironement.pressionAtmospheriqueMinimum = pressionAtmospheriqueMinimum;
    mesureHoraireEnvironement.pressionAtmospheriqueMaximum = pressionAtmospheriqueMaximum;
    mesureHoraireEnvironement.pressionAtmospheriqueRecues = true;
    insererMesureHoraireEnvironnement();
    emit nouvelleMesurePressionAtmospheriqueTraiteParHeure();
}

void Ruche::recevoirTraitementEnsoleillement(const double ensoleillementsMoyen ,const double ensoleillementsMinimum ,const double ensoleillementsMaximum, int heure)
{
    mesuresHorraireEnsoleillement.heure = heure;
    mesuresHorraireEnsoleillement.ensoleillementsMoyen = ensoleillementsMoyen;
    mesuresHorraireEnsoleillement.ensoleillementsMinimum = ensoleillementsMinimum;
    mesuresHorraireEnsoleillement.ensoleillementsMaximum = ensoleillementsMaximum;
    mesuresHorraireEnsoleillement.ensoleillementsRecues = true;
    insererMesureHoraireEnsoleillement();
    emit nouvelleMesureEnsoleillementTraiteParHeure();
}
