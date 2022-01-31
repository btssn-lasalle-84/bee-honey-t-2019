#ifndef ALERTES_H
#define ALERTES_H

#include <QObject>
#include "parametres.h"

/**
  *
  * \file rucheIhm/alertes.h
  *
  * \brief La classe des alertes
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosHumidite;
class InfosTemperature;
class InfosPressionAtmospherique;
class InfosBatterie;
class InfosPoids;
class InfosEnsoleillement;
class ReglagesAlertesIhm;
class BaseDeDonnees;
class Ruche;

/**
  *
  * \class Alertes
  *
  * \brief La classe des alertes
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
class Alertes : public QObject
{
    Q_OBJECT

    private :
        QString idRuche;
        QString nomRuche;
        InfosTemperature *infosTemperature;
        InfosHumidite *infosHumidite;
        InfosPressionAtmospherique *infosPressionAtmospherique;
        InfosEnsoleillement *infosEnsoleillement;
        InfosBatterie *infosBatterie;
        InfosPoids *infosPoids;
        SeuilsAlertes seuilsAlertes;        
        BaseDeDonnees *bdd; //!< agrégation de l'objet BaseDeDonnees

        // Seuils
        double seuilTemperatureInterieurMin;
        double seuilTemperatureInterieurMax;
        double seuilHumiditeInterieurMin;
        double seuilHumiditeInterieurMax;
        double seuilTemperatureExterieurMin;
        double seuilTemperatureExterieurMax;
        double seuilHumiditeExterieurMin;
        double seuilHumiditeExterieurMax;
        double seuilPoidsMin;
        double seuilPoidsMax;
        double seuilEnsoleillement;
        double seuilPressionAtmospherique;
        double seuilBatterie;

    public:
        explicit Alertes(QString idRuche, QString nomRuche, QObject *parent = 0);
        ~Alertes();
        QString getIdRuche();
        double getSeuilTemperatureInterieurMin();
        double getSeuilTemperatureInterieurMax();
        double getSeuilHumiditeInterieurMin();
        double getSeuilHumiditeInterieurMax();
        double getSeuilTemperatureExterieurMin();
        double getSeuilTemperatureExterieurMax();
        double getSeuilHumiditeExterieurMin();
        double getSeuilHumiditeExterieurMax();
        double getSeuilPoidsMin();
        double getSeuilPoidsMax();
        double getSeuilPressionAtmospherique();
        double getSeuilEnsoleillement();
        double getSeuilBatterie();

        void setInfosTemperature(InfosTemperature *infosTemperature);
        void setInfosHumidite(InfosHumidite *infosHumidite);
        void setInfosPressionAtmospherique(InfosPressionAtmospherique *infosPressionAtmospherique);
        void setInfosPoids(InfosPoids *infosPoids);
        void setInfosBatterie(InfosBatterie *infosBatterie);
        void setInfosEnsoleillement(InfosEnsoleillement *infosEnsoleillement);

        void alertesTemperatureInterieur();
        void alertesTemperatureExterieur();
        void alertesHumiditeInterieur();
        void alertesHumiditeExterieur();
        void alertesPressionAtmospherique();
        void alertesPoids();
        void alertesEnsoleillement();
        void alertesBatterie();
        void appelerLesAlertes(TypeAlertes typeAlertes);
        void envoyerMailAlerte(QString email, QString objet, QString message);

    signals :
        void envoiAlertesTemperatureInterieur(SeuilsAlertes);
        void envoiAlertesTemperatureExterieur(SeuilsAlertes);
        void envoiAlertesHumiditeInterieur(SeuilsAlertes);
        void envoiAlertesHumiditeExterieur(SeuilsAlertes);
        void envoiAlertesPressionAtmospherique(SeuilsAlertes);
        void envoiAlertesPoids(SeuilsAlertes);
        void envoiAlertesEnsoleillement(SeuilsAlertes, double);
        void envoiAlertesBatterie(SeuilsAlertes, double);

    public slots:
        void recevoirSeuilTemperatureInterieurMin(QString seuil);
        void recevoirSeuilTemperatureInterieurMax(QString seuil);
        void recevoirSeuilHumiditeInterieurMin(QString seuil);
        void recevoirSeuilHumiditeInterieurMax(QString seuil);
        void recevoirSeuilTemperatureExterieurMin(QString seuil);
        void recevoirSeuilTemperatureExterieurMax(QString seuil);
        void recevoirSeuilHumiditeExterieurMin(QString seuil);
        void recevoirSeuilHumiditeExterieurMax(QString seuil);
        void recevoirSeuilPoidsMin(QString seuil);
        void recevoirSeuilPoidsMax(QString seuil);
        void recevoirSeuilPressionAtmospherique(QString seuil);
        void recevoirSeuilEnsoleillement(QString seuil);
        void recevoirSeuilBatterie(QString seuil);
};

#endif // ALERTES_H
