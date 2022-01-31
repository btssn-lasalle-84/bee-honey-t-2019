#ifndef INFOSPRESSIONATMOSPHERIQUE_H
#define INFOSPRESSIONATMOSPHERIQUE_H

/**
  *
  * \file rucheIhm/infosPressionAtmospherique.h
  *
  * \brief Déclaration de la classe InfosPressionAtmospherique
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
  * \class InfosPressionAtmospherique
  *
  * \brief La classe InfosPressionAtmospherique
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosPressionAtmospherique: public QObject
{
    Q_OBJECT

    private:
        double pressionAtmospherique; //!< la pression atmospherique (unité ?)
        QString horodatagePressionAtmospherique; //!< horodatage de la pression atmospherique
        QVector <double> pressionAtmospheriques;
        int heureCourantePressionAtmospherique;
        void calculerMoyenneHorairePressionAtmospherique();

    public:
        InfosPressionAtmospherique(QObject *parent);
        ~InfosPressionAtmospherique();
        double getPressionAtmospherique();
        void setPressionAtmospherique(double pressionAtmospherique);
        QString getHorodatagePressionAtmospherique()const;
        void setHorodatagePressionAtmospherique(const QString horodatagePressionAtmospherique);

    public slots:
        void traiterNouvellePressionAtmospherique(QString pressionAtmospheriqueString , QString horodatagePressionAtmospherique);

    signals:
        void pressionAtmospheriqueEnvoye(double pressionAtmospherique,QString horodatage);
        void traitementPressionAtmospheriqueEnvoye(const double pressionAtmospheriqueMoyenne, const double pressionAtmospheriqueMinimum, const double pressionAtmospheriqueMaximum, int heure);
};

#endif // INFOSPRESSIONATMOSPHERIQUE_H

