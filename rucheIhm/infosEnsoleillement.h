#ifndef INFOSENSOLEILLEMENT_H
#define INFOSENSOLEILLEMENT_H

/**
  *
  * \file rucheIhm/infosEnsoleillement.h
  *
  * \brief Déclaration de la classe InfosEnsoleillement
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
  * \class InfosEnsoleillement
  *
  * \brief La classe InfosEnsoleillement
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class InfosEnsoleillement : public QObject
{
    Q_OBJECT
    private:
        double ensoleillement; //!< valeur courante de l'ensoleillement en w/m²
        QString horodatageEnsoleillement; //!< horodatage de la l'ensoleillement
        QVector <double> ensoleillements;
        int heureCouranteEnsoleillement;
        void calculerMoyenneHoraireEnsoleillement();


    public:
        InfosEnsoleillement(QObject *parent);
        double getEnsoleillement();
        void setEnsoleillement(double ensoleillement);
        QString getHorodatageEnsoleillement()const;
        void setHorodatageEnsoleillement(const QString horodatageEnsoleillement);

    public slots:
        void traiterNouvelleEnsoleillement(QString ensoleillementString, QString horodatageEnsoleillement);

    signals:
        void ensoleillementEnvoye(double ensoleillement ,QString horodatage);
        void traitementEnsoleillementEnvoye(double ensoleillementMoyen, double ensoleillementMinimum, double ensoleillementMaximum, int heure);
};

#endif // INFOSENSOLEILLEMENT_H

