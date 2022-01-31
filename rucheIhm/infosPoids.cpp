#include "infosPoids.h"
#include <QDebug>

/**
  *
  * \file rucheIhm/infosPoids.cpp
  *
  * \brief Défénition de la classe InfosPoids
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

InfosPoids::InfosPoids(QObject *parent) : QObject(parent), poids(0) , horodatagePoids("")
{
}

InfosPoids::~InfosPoids()
{
}

double InfosPoids::getPoids()
{
    return poids;
}

void InfosPoids::setPoids(double poids)
{
    this->poids = poids;
}

void InfosPoids::traiterNouveauPoids(QString nouveauPoids, QString horodatagePoids)
{
    this->horodatagePoids = horodatagePoids;
    poids = nouveauPoids.toDouble() / 1000;
    emit poidsEnvoye(poids,horodatagePoids);
    qDebug() << Q_FUNC_INFO << "poids = " << poids;
}

QString InfosPoids::getHorodatagePoids()
{
    return horodatagePoids;
}
void InfosPoids::setHorodatagePoids(QString horodatagePoids)
{
    this->horodatagePoids = horodatagePoids;
}
