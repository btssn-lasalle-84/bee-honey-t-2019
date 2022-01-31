#include "infosBatterie.h"
#include <QDebug>

/**
  *
  * \file rucheIhm/infosBatterie.cpp
  *
  * \brief Définition de la classe InfosBatterie
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

InfosBatterie::InfosBatterie(QObject *parent) : QObject(parent), tension(0), courant(0), charge(0), horodatageBatterie("")
{ 
}

InfosBatterie::~InfosBatterie()
{

}

double InfosBatterie::getMesureBatterieCharge()
{
    return charge;
}

double InfosBatterie::getMesureBatterieCourant()
{
    return courant;
}

double InfosBatterie::getMesureBatterieTension()
{
    return tension;
}

QString InfosBatterie::getHorodatageBatterie()
{
    return horodatageBatterie;
}

void InfosBatterie::setHorodatageBatterie(QString horodatageBatterie)
{
    this->horodatageBatterie = horodatageBatterie;
}

void InfosBatterie::setMesuresBatterie(double tension, double courant, double charge)
{
    this->tension = tension;
    this->courant = courant;
    this->charge = charge;
}

void InfosBatterie::traiterNouvelleMesureCourant(QString nouveauCourant, QString horodatage)
{
    courant = nouveauCourant.toDouble();
    horodatageBatterie = horodatage;
    emit tensionEnvoye(courant,horodatage);
    qDebug() << Q_FUNC_INFO << "courant = " << courant;

}

void InfosBatterie::traiterNouvelleMesureCharge(QString nouvelleCharge, QString horodatage)
{
    charge = nouvelleCharge.toDouble();
    horodatageBatterie = horodatage;
    emit chargeEnvoye(charge,horodatage);
    qDebug() << Q_FUNC_INFO << "charge = " << charge;
}

void InfosBatterie::traiterNouvelleMesureTension(QString nouvelleTension, QString horodatage)
{
    tension = nouvelleTension.toDouble();
    horodatageBatterie = horodatage;
    emit tensionEnvoye(tension,horodatage);
    qDebug() << Q_FUNC_INFO << "tension = " << tension;
}
