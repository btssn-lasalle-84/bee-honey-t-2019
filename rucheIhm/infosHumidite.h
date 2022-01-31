#ifndef INFOSHUMIDITE_H
#define INFOSHUMIDITE_H

/**
  *
  * \file rucheIhm/infosHumidite.h
  *
  * \brief Déclaration de la classe InfosHumidite
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QString>
#include <QtCore/QObject>
#include <QVector>

/**
  *
  * \class InfosHumidite
  *
  * \brief La classe InfosHumidite
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosHumidite : public QObject
{
    Q_OBJECT

    private:
        double humiditeInterieur; //!< humidité interieur en pourcentage
        double humiditeExterieur; //!< humidité exterieur en pourcentage
        QString horodatageHumiditeInterieur; //!< horodatage de l'humidité interieur
        QString horodatageHumiditeExterieur; //!< horodatage de l'humidité exterieur
        QVector <double> humiditeInterieurs;
        QVector <double> humiditeExterieurs;
        int heureCouranteHumiditeInterieur;
        int heureCouranteHumiditeExterieur;
        void calculerMoyenneHoraireHumiditeInterieur();
        void calculerMoyenneHoraireHumiditeExterieur();

    public:
        InfosHumidite(QObject * parent);
        double getHumiditeInterieur();
        void setHumiditeInterieur(double humiditeInterieur);
        QString getHorodatageHumiditeInterieur() const;
        void setHorodatageHumiditeInterieur(const QString horodatageHumiditeInterieur);
        double getHumiditeExterieur();
        void setHumiditeExterieur(double humiditeExterieur);
        QString getHorodatageHumiditeExterieur() const;
        void setHorodatageHumiditeExterieur(const QString horodatageHumiditeExterieur);

    public slots:
        void traiterNouvelleHumiditeInterieur(QString humiditeInterieurstring, QString horodatageHumiditeInterieur);
        void traiterNouvelleHumiditeExterieur(QString humiditeExterieurstring, QString horodatageHumiditeExterieur);

    signals:
        void humiditeInterieurEnvoye(const double humiditeInterieur,QString horodatage); //!< signal vers la classe Ruche
        void humiditeExterieurEnvoye(const double humiditeExterieur,QString horodatage);
        void traitementHumiditeInterieurEnvoye(const double temperatureInterieurMoyenne ,const double temperatureInterieurMinimum ,const double temperatureInterieurMaximum, int heure);
        void traitementHumiditeExterieurEnvoye(const double temperatureExterieurMoyenne ,const double temperatureExterieurMinimum ,const double temperatureExterieurMaximum, int heure);

};

#endif // INFOSHUMIDITE_H

