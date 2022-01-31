#ifndef INFOSBATTERIE_H
#define INFOSBATTERIE_H

/**
  *
  * \file rucheIhm/infosBatterie.h
  *
  * \brief Déclaration de la classe InfosBatterie
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QString>
#include <QtCore/QObject>

/**
  *
  * \class InfosBatterie
  *
  * \brief Déclaration de la classe InfosBatterie
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosBatterie : public QObject
{
    Q_OBJECT

private :
    double tension;
    double courant;
    double charge;
    QString horodatageBatterie;

public:
    InfosBatterie(QObject *parent);
    ~InfosBatterie();

    double getMesureBatterieCharge();
    double getMesureBatterieCourant();
    double getMesureBatterieTension();
    QString getHorodatageBatterie();
    void setHorodatageBatterie(QString horodatageBatterie);
    void setMesuresBatterie(double tension, double courant, double charge);

signals:
    void tensionEnvoye(const double tension,QString);
    void courantEnvoye(const double courant,QString);
    void chargeEnvoye(const double charge,QString);

public slots:
    void traiterNouvelleMesureCourant(QString nouveauCourant, QString horodatage);
    void traiterNouvelleMesureCharge(QString nouvelleCharge, QString horodatage);
    void traiterNouvelleMesureTension(QString nouvelleTension, QString horodatage);
};

#endif // INFOSBATTERIE_H
