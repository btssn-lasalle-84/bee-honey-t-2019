#ifndef RUCHE_H
#define RUCHE_H

/**
  *
  * \file rucheIhm/ruche.h
  *
  * \brief Déclaration de la classe Ruche
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QString>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include "parametres.h"

class InfosEnsoleillement;
class InfosHumidite;
class InfosPressionAtmospherique;
class InfosTemperature;
class InfosBatterie;
class InfosPoids;
class Communication;
class BaseDeDonnees;
class Alertes;

// Les structures de données pour les mesures
/**
  *
  * \struct DonneesBatterie
  *
  * \brief structure de données pour les mesures de la batterie
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
struct DonneesBatterie
{
    QString tension;
    QString horodatagetension;
    QString courant;
    QString horodatagecourant;
    QString charge;
    QString horodatagecharge;
};

// La structure de données pour les mesures horodatées
/**
  *
  * \struct DonneesRuche
  *
  * \brief structure de données pour les mesures horodatées
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
struct DonneesRuche
{
    DonneesBatterie donneesBatterie;
    QString poids;
    QString horodatagePoids;
    QString temperatureInterieur;
    QString horodatageTemperatureInterieur;
    QString humiditeInterieur;
    QString horodatageHumiditeInterieur;
    QString temperatureExterieur;
    QString horodatageTemperatureExterieur;
    QString humiditeExterieur;
    QString horodatageHumiditeExterieur;
    QString pressionAtmospherique;
    QString horodatagePressionAtmospherique;
    QString ensoleillement;
    QString horodatageEnsoleillement;
};

/**
  *
  * \struct MesureHoraireRuche
  *
  * \brief structure de données pour les mesures horaires
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
struct MesureHoraireRuche
{
    int heure;
    bool temperaturesRecues = false;
    double temperatureInterieurMoyenne;
    double temperatureInterieurMinimum;
    double temperatureInterieurMaximum;
    bool humiditesRecues = false;
    double humiditeInterieurMoyenne;
    double humiditeInterieurMinimum;
    double humiditeInterieurMaximum;
};

/**
  *
  * \struct MesureHoraireEnvironement
  *
  * \brief structure de données pour les mesures horaires
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
struct MesureHoraireEnvironement
{
    int heure;
    bool temperaturesRecues = false;
    double temperatureExterieurMoyenne;
    double temperatureExterieurMinimum;
    double temperatureExterieurMaximum;
    bool humiditesRecues = false;
    double humiditeExterieurMoyenne;
    double humiditeExterieurMinimum;
    double humiditeExterieurMaximum;
    double pressionAtmospheriqueMoyenne;
    double pressionAtmospheriqueMinimum;
    double pressionAtmospheriqueMaximum;
    bool pressionAtmospheriqueRecues = false;
};

/**
  *
  * \struct MesuresHoraireEnsoleillement
  *
  * \brief structure de données pour les mesures horaires
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
struct MesuresHoraireEnsoleillement
{
    int heure;
    double ensoleillementsMoyen;
    double ensoleillementsMinimum;
    double ensoleillementsMaximum;
    bool ensoleillementsRecues = false;
};

/**
  *
  * \class Ruche
  *
  * \brief La classe Ruche
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class Ruche : public QObject
{
    Q_OBJECT

    public:
        Ruche(QStringList donneesRucheTTN, QObject *parent = 0);
        ~Ruche();

        Alertes* getAlertes();
        QString getIdRuche();

    private:
        InfosEnsoleillement *ensoleillementRuche;
        InfosHumidite *humiditeRuche;
        InfosPressionAtmospherique *pressionAtmospheriqueRuche;
        InfosTemperature *temperatureRuche;
        InfosBatterie *infosBatterie;
        InfosPoids *infosPoids;
        Communication *communicationRuche;
        BaseDeDonnees *bdd; //!< agrégation de l'objet BaseDeDonnees
        Alertes *alertes;
        QStringList donneesRucheTTN;
        DonneesRuche donneesRuche;
        MesureHoraireRuche mesureHoraireRuche;
        MesureHoraireEnvironement mesureHoraireEnvironement;
        MesuresHoraireEnsoleillement mesuresHorraireEnsoleillement;

        void connecterGestionMessageTraite();
        void connecterGestionMessageBrut();
        QString extraireDonneesBrutes(const QByteArray &message);
        QString extrairePoids(const QString &donneesBrutes);
        PortsTTN recupererNumeroDePort(QByteArray message);
        void extraireDonnees(PortsTTN port, QByteArray message);
        QString recupererDonnees(QByteArray message, QString objetJson, QString sousObjetJson);
        QString recupererHorodatage(QByteArray message, QString objetJson, QString sousObjetJson);
        void gererAlertes();
        void insererDonneesPortMesureRuche();
        void insererDonneesPortMesureEnvironnement();
        void insererDonneesPortEnsoleillement();
        void insererDonneesPortPoids();
        void insererDonneesPortBatterie();
        void insererMesureHoraireRuche();
        void insererMesureHoraireEnvironnement();
        void insererMesureHoraireEnsoleillement();
        void insererMesureHorairePoids();
        void insererMesureHoraireBatterie();
        void recupererLocalisationPasserelle(QByteArray message);

    public slots:        
        void recevoirMessage(const QByteArray &message, const QMqttTopicName &topic);
        void recevoirEtatClientConnexion(bool);
        /********************Slots Mesure****************************/
        void recevoirTemperatureInterieurTraite(double temperatureInterieur,QString horodatage);
        void recevoirMesureHumiditeInterieurTraite(double humiditeInterieurTraite,QString horodatage);
        void recevoirTemperatureExterieurTraite(double temperatureExterieur, QString horodatage);
        void recevoirMesureHumiditeExterieurTraite(double humiditeExterieurTraite, QString horodatage);
        void recevoirEnsoleillementTraite(double ensoleillement,QString horodatage);
        void recevoirPressionAtmospheriqueTraite(double pressionAtmospherique,QString horodatage);
        void recevoirMesureTensionTraite(double tension, QString);
        void recevoirMesureCourantTraite(double courant, QString);
        void recevoirMesureChargeTraite(double charge, QString);
        void recevoirMesurePoidsTraite(double poids, QString);

        /********************Slots calcul Moyenne****************************/
        void recevoirTraitementTemperatureInterieur(const double temperatureInterieurMoyenne , const double temperatureInterieurMinimum , const double temperatureInterieurMaximum, int heure);
        void recevoirTraitementTemperatureExterieur(const double temperatureExterieurMoyenne ,const double temperatureExterieurMinimum ,const double temperatureExterieurMaximum, int heure);
        void recevoirTraitementHumiditeInterieur(const double temperatureInterieurMoyenne ,const double temperatureInterieurMinimum ,const double temperatureInterieurMaximum, int heure);
        void recevoirTraitementHumiditeExterieur(const double humiditeExterieursMoyenne , const double humiditeExterieursMinimum , const double humiditeExterieursMaximum, int heure);
        void recevoirTraitementPressionAtmospherique(const double pressionAtmospheriqueMoyenne, const double pressionAtmospheriqueMinimum, const double pressionAtmospheriqueMaximum, int heure);
        void recevoirTraitementEnsoleillement(const double ensoleillementsMoyen , const double ensoleillementsMinimum , const double temperatureExterieurMaximum, int heure);
        /*void recevoirTraitementCharge();
        void recevoirTraitementPoids();*/

        /********************Slots Alertes****************************/
        void recevoirAlertesTemperatureInterieur(SeuilsAlertes seuilsAlertes);
        void recevoirAlertesTemperatureExterieur(SeuilsAlertes seuilsAlertes);
        void recevoirAlertesHumiditeInterieur(SeuilsAlertes seuilsAlertes);
        void recevoirAlertesHumiditeExterieur(SeuilsAlertes seuilsAlertes);
        void recevoirAlertesPressionAtmospherique(SeuilsAlertes seuilsAlertes);
        void recevoirAlertesPoids(SeuilsAlertes);
        void recevoirAlertesEnsoleillement(SeuilsAlertes, double niveauEnsoleillement);
        void recevoirAlertesBatterie(SeuilsAlertes, double);

    signals:
        /********************SIGNAUX VERS CLASSE INFOS****************************/
        void nouvelleMesurePoids(QString poids , QString horodatagePoids); //!< signal vers la classe InfosPoids
        void nouvelleTemperatureInterieur(QString temperatureInterieur,QString horodatage); //!< signal vers la classe InfosTemperature
        void nouvelleHumiditeInterieur(QString humiditeInterieur,QString horodatage); //!< signal vers la classe InfosHumidite
        void nouvelleTemperatureExterieur(QString temperatureExterieur,QString horodatage); //!< signal vers la classe InfosTemperature
        void nouvelleHumiditeExterieur(QString humiditeExterieur,QString horodatage); //!< signal vers la classe InfosHumidite
        void nouvellePressionAtmospherique(QString pressionAtmospherique,QString horodatage); //!< signal vers la classe InfosPressionAtmospherique
        void nouvelleMesureEnsoleillement(QString ensoleillement,QString horodatage); //!< signal vers la classe InfosEnsoleillement
        void nouvelleMesureCharge(QString charge, QString horodatage);
        void nouvelleMesureCourant(QString courant, QString horodatage);
        void nouvelleMesureTension(QString tension, QString horodatage);

        /********************** SIGNAUX VERS IHM **********************************/
        void nouvelleMesureTemperatureInterieurTraite(double temperatureInterieurTraite ,QString horodatage); //!< signal vers la classe RucheIhm
        void nouvelleMesureHumiditeInterieurTraite(double humiditeInterieurTraite ,QString horodatage); //!< signal vers la classe RucheIhm
        void nouvelleMesureTemperatureExterieurTraite(double temperatureExterieur, QString horodatage);//!< signal vers la classe RucheIhm
        void nouvelleMesureHumiditeExterieurTraite(double humiditeExterieurTraite, QString horodatage);//!< signal vers la classe RucheIhm
        void nouvelleMesureEnsoleillementTraite(double ensoleillement, QString horodatage);//!< signal vers la classe RucheIhm
        void nouvellePressionAtmospheriqueTraite(double pressionAtmospherique, QString horodatage);//!< signal vers la classe RucheIhm
        void nouvelleMesureTension(double tension, QString horodatage);
        void nouvelleMesureCourant(double courant, QString horodatage);
        void nouvelleMesureCharge(double charge, QString horodatage);
        void nouvelleMesurePoids(double poids,  QString horodatage);
        void nouvelleMesureTemperatureInterieurTraiteParHeure();
        void nouvelleMesureTemperatureExterieurTraiteParHeure();
        void nouvelleMesureHumiditeInterieurTraiteParHeure();
        void nouvelleMesureHumiditeExterieurTraiteParHeure();
        void nouvelleMesurePressionAtmospheriqueTraiteParHeure();
        void nouvelleMesureEnsoleillementTraiteParHeure();

        void envoiAlertesTemperatureInterieur(SeuilsAlertes);
        void envoiAlertesTemperatureExterieur(SeuilsAlertes);
        void envoiAlertesHumiditeInterieur(SeuilsAlertes);
        void envoiAlertesHumiditeExterieur(SeuilsAlertes);
        void envoiAlertesPressionAtmospherique(SeuilsAlertes);
        void envoiAlertesPoids(SeuilsAlertes);
        void envoiAlertesEnsoleillement(SeuilsAlertes, double);
        void envoiAlertesBatterie(SeuilsAlertes, double);
        void etatClientConnexion(bool connexion);

        void envoiLocalisationPasserelle(QString longitude, QString latitude);
};

#endif // RUCHE_H

