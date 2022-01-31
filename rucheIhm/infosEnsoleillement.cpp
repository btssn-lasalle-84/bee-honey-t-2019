#include "infosEnsoleillement.h"

/**
  *
  * \file rucheIhm/infosEnsoleillement.cpp
  *
  * \brief Définition de la classe InfosEnsoleillement
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QTime>
#include <QDebug>

/**
 * \brief Constructeur de la classe InfosEnsoleillement
 *
 * \fn InfosEnsoleillement::InfosEnsoleillement
 *
 * \details Définition des attributs ensoleillement à 0 et l'attribut horodatageEnsoleillement à ""
 *
 */
InfosEnsoleillement::InfosEnsoleillement(QObject *parent) :QObject(parent), ensoleillement(0.), horodatageEnsoleillement(""), heureCouranteEnsoleillement(-1)
{
}

/**
 * @brief getter de l'attribut ensoleillement
 * \fn InfosEnsoleillement::getEnsoleillement
 * @return un \e double correspondant à la valeur de l'attibut ensoleillement
 */

double InfosEnsoleillement::getEnsoleillement()
{
    return ensoleillement;
}

/**
 * @brief setter de l'attribut ensoleillement
 * \fn InfosEnsoleillement::setEnsoleillement
 * @param ensoleillement
 */

void InfosEnsoleillement::setEnsoleillement(double ensoleillement)
{
    this->ensoleillement = ensoleillement;
}

/**
 * @brief getter de l'attibut horodatageEnsoleillement
 * \fn InfosEnsoleillement::getHorodatageEnsoleillement
 * @return un QString correspondant a la valeur de l'attribut horodatageEnsoleillement
 */

QString InfosEnsoleillement::getHorodatageEnsoleillement() const
{
    return horodatageEnsoleillement;
}

/**
 * @brief setter de l'attribut horodatageEnsoleillement
 * \fn    InfosEnsoleillement::setHorodatageEnsoleillement
 * @param horodatageEnsoleillement
 */

void InfosEnsoleillement::setHorodatageEnsoleillement(const QString horodatageEnsoleillement)
{
    this->horodatageEnsoleillement = horodatageEnsoleillement;
}

/**
 * @brief slot qui traite l'ensoleillement
 * \fn InfosEnsoleillement::traiterNouvelleEnsoleillement
 * @param ensoleillementString       qui corespond a l'ensoleillement envoyé par la Ruche
 * @param horodatageEnsoleillement   correspond a l'horodatage de la mesure de l'ensoleillement
 */

void InfosEnsoleillement::traiterNouvelleEnsoleillement(QString ensoleillementString, QString horodatageEnsoleillement)
{
    ensoleillement = ensoleillementString.toDouble();
    this->horodatageEnsoleillement = horodatageEnsoleillement;
    emit ensoleillementEnvoye(ensoleillement,horodatageEnsoleillement);

    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageEnsoleillement, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCouranteEnsoleillement" << heureCouranteEnsoleillement << dateTimeHorodatage.time().hour();
    if(heureCouranteEnsoleillement == -1)
    {
        heureCouranteEnsoleillement = dateTimeHorodatage.time().hour();
    }
    if(heureCouranteEnsoleillement == dateTimeHorodatage.time().hour())
    {
        ensoleillements.append(ensoleillement);
    }
    else if((heureCouranteEnsoleillement+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHoraireEnsoleillement();
        heureCouranteEnsoleillement = dateTimeHorodatage.time().hour();
        ensoleillements.append(ensoleillement);
    }
}

void InfosEnsoleillement::calculerMoyenneHoraireEnsoleillement()
{
    double sommeEnsoleillement= 0;
    double ensoleillementMoyen = 0;
    double ensoleillementMinimum = 999;
    double ensoleillementMaximum = -999;

    // au moins 2 mesures
    if(ensoleillements.size() >= 2)
    {
        ensoleillementMinimum = ensoleillements[0];
        ensoleillementMaximum = ensoleillements[0];
        for (int i = 0; i < ensoleillements.size(); i++)
        {
            sommeEnsoleillement += ensoleillements[i];

            if(ensoleillementMinimum > ensoleillements[i])
            {
                ensoleillementMinimum = ensoleillements[i];
            }

            if(ensoleillementMaximum < ensoleillements[i])
            {
                ensoleillementMaximum = ensoleillements[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << ensoleillements;
    ensoleillementMoyen = sommeEnsoleillement/ double(ensoleillements.size());
    emit traitementEnsoleillementEnvoye(ensoleillementMoyen, ensoleillementMinimum ,ensoleillementMaximum, heureCouranteEnsoleillement);
    ensoleillements.clear();
}

