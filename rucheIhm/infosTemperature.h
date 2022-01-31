#ifndef INFOSTEMPERATURE_H
#define INFOSTEMPERATURE_H

/**
  *
  * \file rucheIhm/infosTemperature.h
  *
  * \brief Déclaration de la classe InfosTemperature
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
  * \class InfosTemperature
  *
  * \brief La classe InfosTemperature
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosTemperature : public QObject
{
    Q_OBJECT

  private:
    double temperatureInterieur; //!< temperature interieur en degrée Celsius
    double temperatureExterieur; //!< temperature exterieuren degrée Celsius
    QString horodatageTemperatureInterieur; //!< horodatage de la temperature interieur
    QString horodatageTemperatureExterieur; //!< horodatage de la temperature exterieur
    QVector <double> temperaturesInterieurs;
    QVector <double> temperaturesExterieurs;
    int heureCouranteTemperatureInterieur;
    int heureCouranteTemperatureExterieur;
    void calculerMoyenneHoraireTemperatureInterieur();
    void calculerMoyenneHoraireTemperatureExterieur();

    void traiterMoyenneHoraireTemperatureInterieur(QString horodatageTemperatureInterieur);

public:
    InfosTemperature(QObject *parent);
    ~InfosTemperature();

    double getTemperatureInterieur();
    void setTemperatureInterieur(double temperatureInterieur);
    QString getHorodatageTemperatureInterieur();
    void setHorodatageTemperatureInterieur(const QString horodatageTemperatureInterieur);

    double getTemperatureExterieur();
    void setTemperatureExterieur(double temperatureExterieur);
    QString getHorodatageTemperatureExterieur();
    void setHorodatageTemperatureExterieur(const QString horodatageTemperatureExterieur);

  public slots:
    void traiterNouvelleTemperatureInterieur(QString temperatureInterieur,QString horodatageTemperatureInterieur);
    void traiterNouvelleTemperatureExterieur(QString temperatureExterieurString, QString horodatageTemperatureExterieur);

  signals:
    void temperatureInterieurEnvoye(const double temperatureInterieur,QString horodatage); //!< signal vers la classe Ruche
    void temperatureExterieurEnvoye(const double temperatureExterieur,QString horodatage); //!< signal vers la classe Ruche
    void traitementTemperatureInterieurEnvoye(const double temperatureInterieurMoyenne, const double temperatureInterieurMinimum, const double temperatureInterieurMaximum, int heure);
    void traitementTemperatureExterieurEnvoye(const double temperatureExterieurMoyenne, const double temperatureExterieurMinimum, const double temperatureExterieurMaximum, int heure);

};

#endif // INFOSTEMPERATURE_H

