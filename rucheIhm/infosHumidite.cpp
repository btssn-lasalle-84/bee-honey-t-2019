#include "infosHumidite.h"

/**
  *
  * \file rucheIhm/infosHumidite.cpp
  *
  * \brief Définition de la classe InfosHumidite
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
#include <QTime>
#include <QDebug>

/**
 * \brief Constructeur de la classe InfosHumidite
 *
 * \fn InfosHumidite::InfosHumidite
 *
 * \details Définition des attributs pressionAtmospherique à 0 et l'attribut horodatagePressionAtmospherique à ""
 *
 */

InfosHumidite::InfosHumidite(QObject *parent) :QObject(parent),humiditeInterieur(0),humiditeExterieur(0),horodatageHumiditeInterieur(""),horodatageHumiditeExterieur(""), heureCouranteHumiditeInterieur(-1), heureCouranteHumiditeExterieur(-1)
{
}

/**
 * @brief getter de l'attribut humiditeInterieur
 * \fn    InfosHumidite::getHumiditeInterieur
 * @return \e double corespondant a la valeur de l'attribut humiditeInterieur
 */

double InfosHumidite::getHumiditeInterieur()
{
    return humiditeInterieur;
}
/**
 * @brief setter de l'attribut humiditeInterieur
 * \fn    InfosHumidite::setHumiditeInterieur
 * @param humiditeInterieur corespondant a l'humidite interieur
 */

void InfosHumidite::setHumiditeInterieur(double humiditeInterieur)
{
    this->humiditeInterieur = humiditeInterieur;
}
/**
 * @brief getter de l'attribut horodatageHumiditeInterieur
 * \fn InfosHumidite::getHorodatageHumiditeInterieur
 * @return \e QString  horodatageHumiditeInterieur corespondant a l'attribut horodatageHumiditeInterieur
 */

QString InfosHumidite::getHorodatageHumiditeInterieur()const
{
    return horodatageHumiditeInterieur;
}
/**
 * @brief InfosHumidite::setHorodatageHumiditeInterieur
 * \fn InfosHumidite::setHorodatageHumiditeInterieur
 * @param horodatageHumiditeInterieur corespondant a l'horodatage humiddite interieur
 */

void InfosHumidite::setHorodatageHumiditeInterieur(const QString horodatageHumiditeInterieur)
{
    this->horodatageHumiditeInterieur = horodatageHumiditeInterieur;
}
/**
 * @brief getter de l'attribut humiditeExterieur
 * \fn InfosHumidite::getHumiditeExterieur
 * @return \e double la valeur de l'humiditeExterieur
 */

double InfosHumidite::getHumiditeExterieur()
{
    return humiditeExterieur;
}
/**
 * @brief setter de l'attribut humiditeExterieur
 * \fn    InfosHumidite::setHumiditeExterieur
 * @param humiditeExterieur corespondant a l'humidité l'exterieur
 */

void InfosHumidite::setHumiditeExterieur(double humiditeExterieur)
{
    this->humiditeExterieur = humiditeExterieur;
}

/**
 * @brief getter de l'attribut horodatageHumiditeExterieur
 * \fn    InfosHumidite::getHorodatageHumiditeExterieur
 * @return un \e QString correspondant a la valeur de l'attribut horodatageHumiditeExterieur
 */

QString InfosHumidite::getHorodatageHumiditeExterieur()const
{
    return horodatageHumiditeExterieur;
}

/**
 * @brief setter de l'attribut horodatageHumiditeExterieur
 * \fn    InfosHumidite::setHorodatageHumiditeExterieur
 * @param horodatageHumiditeExterieur
 */

void InfosHumidite::setHorodatageHumiditeExterieur(const QString horodatageHumiditeExterieur)
{
    this->horodatageHumiditeExterieur = horodatageHumiditeExterieur;
}

/**
 * @brief slot qui traite l'humidite Interieur
 * \fn InfosHumidite::traiterNouvelleHumiditeInterieur
 * @param humiditeInterieurstring       qui corespond a l'humidite interieur envoyé par la Ruche
 * @param horodatageHumiditeInterieur   correspond a l'horodatage de la mesure de l'humidité interieur
 */

void InfosHumidite::traiterNouvelleHumiditeInterieur(QString humiditeInterieurstring, QString horodatageHumiditeInterieur)
{
    humiditeInterieur = humiditeInterieurstring.toDouble();
    emit humiditeInterieurEnvoye(humiditeInterieur,horodatageHumiditeInterieur);

    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageHumiditeInterieur, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCouranteHumiditeInterieur" << heureCouranteHumiditeInterieur << dateTimeHorodatage.time().hour();
    if(heureCouranteHumiditeInterieur == -1)
    {
        heureCouranteHumiditeInterieur = dateTimeHorodatage.time().hour();
    }
    if(heureCouranteHumiditeInterieur == dateTimeHorodatage.time().hour())
    {
        humiditeInterieurs.append(humiditeInterieur);
    }
    else if((heureCouranteHumiditeInterieur+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHoraireHumiditeInterieur();
        heureCouranteHumiditeInterieur = dateTimeHorodatage.time().hour();
        humiditeInterieurs.append(humiditeInterieur);
    }
}

/**
 * @brief slot qui traite l'humidite Exterieur
 * \fn InfosHumidite::traiterNouvelleHumiditeExterieur
 * @param humiditeExterieurstring       qui corespond a l'humidite exterieur envoyé par la Ruche
 * @param horodatageHumiditeExterieur   qui correspond a l'horodatage de la mesure de l'humidité exterieur
 */


void InfosHumidite::traiterNouvelleHumiditeExterieur(QString humiditeExterieurstring, QString horodatageHumiditeExterieur)
{
    humiditeExterieur = humiditeExterieurstring.toDouble();
    emit humiditeExterieurEnvoye(humiditeExterieur,horodatageHumiditeExterieur);


    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageHumiditeExterieur, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCouranteHumiditeExterieur" << heureCouranteHumiditeExterieur << dateTimeHorodatage.time().hour();
    if(heureCouranteHumiditeExterieur == -1)
    {
        heureCouranteHumiditeExterieur = dateTimeHorodatage.time().hour();
    }
    if(heureCouranteHumiditeExterieur == dateTimeHorodatage.time().hour())
    {
        humiditeExterieurs.append(humiditeExterieur);
    }
    else if((heureCouranteHumiditeExterieur+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHoraireHumiditeExterieur();
        heureCouranteHumiditeExterieur = dateTimeHorodatage.time().hour();
        humiditeExterieurs.append(humiditeExterieur);
    }
}

void InfosHumidite::calculerMoyenneHoraireHumiditeInterieur()
{
    double sommeHumiditeInterieur = 0;
    double humiditeInterieurMoyenne = 0;
    double humiditeInterieurMinimum = 999;
    double humiditeInterieurMaximum = -999;

    // au moins 2 mesures
    if(humiditeInterieurs.size() >= 2)
    {
        humiditeInterieurMinimum = humiditeInterieurs[0];
        humiditeInterieurMaximum = humiditeInterieurs[0];
        for (int i = 0; i < humiditeInterieurs.size(); i++)
        {
            sommeHumiditeInterieur += humiditeInterieurs[i];

            if(humiditeInterieurMinimum > humiditeInterieurs[i])
            {
                humiditeInterieurMinimum = humiditeInterieurs[i];
            }

            if(humiditeInterieurMaximum < humiditeInterieurs[i])
            {
                humiditeInterieurMaximum = humiditeInterieurs[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << humiditeInterieurs;
    qDebug() << Q_FUNC_INFO << "humiditeInterieurMoyenne=" << humiditeInterieurMoyenne << "humiditeInterieurMinimum=" << humiditeInterieurMinimum << "humiditeInterieurMaximum=" << humiditeInterieurMaximum;
    humiditeInterieurMoyenne = sommeHumiditeInterieur / double(humiditeInterieurs.size());
    emit traitementHumiditeInterieurEnvoye(humiditeInterieurMoyenne, humiditeInterieurMinimum, humiditeInterieurMaximum, heureCouranteHumiditeInterieur);
    humiditeInterieurs.clear();
}

/**
 * @brief methode permettant de calculer la moyenne des temperatures exterieurs sur une heure
 * \fn    InfosTemperature::calculerMoyenneHoraireTemperatureExterieur
 */

void InfosHumidite::calculerMoyenneHoraireHumiditeExterieur()
{
    double sommeHumiditeExterieur = 0;
    double humiditeExterieurMoyenne = 0;
    double humiditeExterieurMinimum = 999;
    double humiditeExterieurMaximum = -999;

    // au moins 2 mesures
    if(humiditeExterieurs.size() >= 2)
    {
        humiditeExterieurMinimum = humiditeExterieurs[0];
        humiditeExterieurMaximum = humiditeExterieurs[0];
        for (int i = 0; i < humiditeExterieurs.size(); i++)
        {
            sommeHumiditeExterieur += humiditeExterieurs[i];

            if(humiditeExterieurMinimum > humiditeExterieurs[i])
            {
                humiditeExterieurMinimum = humiditeExterieurs[i];
            }

            if(humiditeExterieurMaximum < humiditeExterieurs[i])
            {
                humiditeExterieurMaximum = humiditeExterieurs[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << humiditeExterieurs;
    qDebug() << Q_FUNC_INFO << "humiditeExterieurMoyenne=" << humiditeExterieurMoyenne << "humiditeExterieurMinimum=" << humiditeExterieurMinimum << "humiditeExterieurMaximum=" << humiditeExterieurMaximum;
    humiditeExterieurMoyenne = sommeHumiditeExterieur / double(humiditeExterieurs.size());
    emit traitementHumiditeExterieurEnvoye(humiditeExterieurMoyenne, humiditeExterieurMinimum, humiditeExterieurMaximum, heureCouranteHumiditeExterieur);
    humiditeExterieurs.clear();
}


