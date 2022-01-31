#ifndef INFOSPOIDS_H
#define INFOSPOIDS_H

/**
  *
  * \file rucheIhm/infosPoids.h
  *
  * \brief Déclaration de la classe InfosPoids
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
  * \class InfosPoids
  *
  * \brief Déclaration de la classe InfosPoids
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosPoids : public QObject
{
    Q_OBJECT

private :
    double poids;
    QString horodatagePoids;

public:
     InfosPoids(QObject *parent);
     ~InfosPoids();

     double getPoids();
     void setPoids(double poids);
     QString getHorodatagePoids();
     void setHorodatagePoids(QString horodatagePoids);

signals:
    void poidsEnvoye(const double poids,QString);

public slots:
    void traiterNouveauPoids(QString nouveauPoids, QString horodatagePoids);
};

#endif // INFOSPOIDS_H
