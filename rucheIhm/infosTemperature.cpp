#include "infosTemperature.h"
#include <QDebug>
#include <QTime>
/**
  *
  * \file rucheIhm/infosTemperature.cpp
  *
  * \brief Définition de la classe InfosTemperature
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la classe InfosTemperature
 *
 * \fn InfosTemperature::InfosTemperature
 *
 * \details Définition des attributs temperatureInterieur, temperatureExterieur à 0 et les attributs temperatureExterieur horodatageTemperatureExterieur à ""
 *
 * \param parent QObject Adresse de l'objet Qt parent
 *
 */
InfosTemperature::InfosTemperature(QObject *parent):QObject(parent),temperatureInterieur(0),temperatureExterieur(0),horodatageTemperatureInterieur(""),horodatageTemperatureExterieur(""),heureCouranteTemperatureInterieur(-1),heureCouranteTemperatureExterieur(-1)
{
}

InfosTemperature::~InfosTemperature()
{
}

/**
 * \brief    getter  de l'attribut temperatureInterieur
 * \fn       InfosTemperature::getTemperatureInterieur
 * \return   Un \e double correspondant a la temperature intérieur
 */

double InfosTemperature::getTemperatureInterieur()
{
    return temperatureInterieur;
}

/**
 * \brief    setter de l'attribut temperatureInterieur
 * \fn       Ruche::setTemperatureInterieur
 * \param    temperatureInterieur    correspondant  l'attribut temperatureInterieur
 */

void InfosTemperature::setTemperatureInterieur(double temperatureInterieur)
{
    this->temperatureInterieur = temperatureInterieur;
}

/**
 * \brief    getter  de l'attribut horodatagetemperatureInterieur
 * \fn       InfosTemperature::getHorodatageTemperatureInterieur
 * \return   Un \e QString correspondant a l'horodatage temperature intérieur
 */

QString InfosTemperature::getHorodatageTemperatureInterieur()
{
    return horodatageTemperatureInterieur;
}

/**
 * \brief    setter de l'attribut horodatagetemperatureInterieur
 * \fn       Ruche::setHorodatageTemperatureInterieur
 * \param    horodatageTemperatureInterieur    correspondant  l'attribut horodatageTemperatureInterieur
 */

void InfosTemperature::setHorodatageTemperatureInterieur(const QString horodatageTemperatureInterieur)
{
    this->horodatageTemperatureInterieur = horodatageTemperatureInterieur;
}

/**
 * \brief    getter  de l'attribut temperatureExterieur
 * \fn       InfosTemperature::getTemperatureExterieur
 * \return   Un \e double correspondant a la  temperature exterieur
 */

double InfosTemperature::getTemperatureExterieur()
{
    return temperatureExterieur;
}

/**
 * \brief    setter de l'attribut temperatureExterieur
 * \fn       InfosTemperature::setTemperatureExterieur
 * \param    temperatureExterieur    correspondant  l'attribut TemperatureExterieur
 */

void InfosTemperature::setTemperatureExterieur(double temperatureExterieur)
{
    this->temperatureExterieur = temperatureExterieur;
}

/**
 * \brief    getter  de l'attribut horodatagetemperatureExterieur
 * \fn       InfosTemperature::getHorodatageTemperatureeExterieur
 * \return   Un \e QString correspondant a l'horodatage temperature exterieur
 */

QString InfosTemperature::getHorodatageTemperatureExterieur()
{
    return horodatageTemperatureExterieur;
}

/**
 * \brief    setter de l'attribut horodatageTemperatureExterieur
 * \fn       InfosTemperature::setHorodatageTemperatureExterieur
 * \param    horodatageTemperatureExterieur    correspondant  l'attribut horodatageTemperatureExterieur
 */

void InfosTemperature::setHorodatageTemperatureExterieur(const QString horodatageTemperatureExterieur)
{
    this->horodatageTemperatureExterieur = horodatageTemperatureExterieur;
}
/**
 * @brief slot qui traite la temperature interieur venant de la classe Ruche
 * \fn    InfosTemperature::traiterMoyenneHoraireTemperatureInterieur
 * @param horodatageTemperatureInterieur correspondant a l'horodatage de la mesure temperature interieur
 */

void InfosTemperature::traiterMoyenneHoraireTemperatureInterieur(QString horodatageTemperatureInterieur)
{
    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageTemperatureInterieur, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCouranteTemperatureInterieur" << heureCouranteTemperatureInterieur << dateTimeHorodatage.time().hour();
    if(heureCouranteTemperatureInterieur == -1)
    {
        heureCouranteTemperatureInterieur = dateTimeHorodatage.time().hour();
    }
    if(heureCouranteTemperatureInterieur == dateTimeHorodatage.time().hour())
    {
        temperaturesInterieurs.append(temperatureInterieur);
    }
    else if((heureCouranteTemperatureInterieur+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHoraireTemperatureInterieur();
        heureCouranteTemperatureInterieur = dateTimeHorodatage.time().hour();
        temperaturesInterieurs.append(temperatureInterieur);
    }
}

void InfosTemperature::traiterNouvelleTemperatureInterieur(QString temperatureInterieurString, QString horodatageTemperatureInterieur)
{
    qDebug() << Q_FUNC_INFO << "temperatureInterieurString" << temperatureInterieurString << horodatageTemperatureInterieur;
    this->horodatageTemperatureInterieur = horodatageTemperatureInterieur;
    temperatureInterieur = temperatureInterieurString.toDouble();    
    emit temperatureInterieurEnvoye(temperatureInterieur,horodatageTemperatureInterieur);

    traiterMoyenneHoraireTemperatureInterieur(horodatageTemperatureInterieur);
}

/**
 * @brief slot qui traite la temperature exterieur venant de la classe Ruche
 * \fn    InfosTemperature::traiterNouvelleTemperatureExterieur
 * @param temperatureExterieurString    correspondant à la mesure temperature exterieur
 * @param horodatageTemperatureExterieur correspondant a l'horodatage de la mesure temperature exterieur
 */

void InfosTemperature::traiterNouvelleTemperatureExterieur(QString temperatureExterieurString, QString horodatageTemperatureExterieur)
{
    temperatureExterieur = temperatureExterieurString.toDouble();
    this->horodatageTemperatureExterieur = horodatageTemperatureExterieur;
    emit temperatureExterieurEnvoye(temperatureExterieur,horodatageTemperatureExterieur);

    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatageTemperatureExterieur, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCouranteTemperatureExterieur" << heureCouranteTemperatureExterieur << dateTimeHorodatage.time().hour();
    if(heureCouranteTemperatureExterieur == -1)
    {
        heureCouranteTemperatureExterieur = dateTimeHorodatage.time().hour();
    }
    if(heureCouranteTemperatureExterieur == dateTimeHorodatage.time().hour())
    {
        temperaturesExterieurs.append(temperatureExterieur);
    }
    else if((heureCouranteTemperatureExterieur+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHoraireTemperatureExterieur();
        heureCouranteTemperatureExterieur = dateTimeHorodatage.time().hour();
        temperaturesExterieurs.append(temperatureExterieur);
    }
}
/**
 * @brief methode permettant de calculer la moyenne des temperatures interieurs sur une heure
 * \fn    InfosTemperature::calculerMoyenneHoraireTemperatureInterieur
 */

void InfosTemperature::calculerMoyenneHoraireTemperatureInterieur()
{
    double sommeTemperatureInterieur = 0;
    double temperatureInterieurMoyenne = 0;
    double temperatureInterieurMinimum = 999;
    double temperatureInterieurMaximum = -999;

    // au moins 2 mesures
    if(temperaturesInterieurs.size() >= 2)
    {
        temperatureInterieurMinimum = temperaturesInterieurs[0];
        temperatureInterieurMaximum = temperaturesInterieurs[0];
        for (int i = 0; i < temperaturesInterieurs.size(); i++)
        {
            sommeTemperatureInterieur += temperaturesInterieurs[i];

            if(temperatureInterieurMinimum > temperaturesInterieurs[i])
            {
                temperatureInterieurMinimum = temperaturesInterieurs[i];
            }

            if(temperatureInterieurMaximum < temperaturesInterieurs[i])
            {
                temperatureInterieurMaximum = temperaturesInterieurs[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << temperaturesInterieurs;
    temperatureInterieurMoyenne = sommeTemperatureInterieur / double(temperaturesInterieurs.size());
    qDebug() << Q_FUNC_INFO << "temperatureInterieurMoyenne=" << temperatureInterieurMoyenne << "temperatureInterieurMinimum=" << temperatureInterieurMinimum << "temperatureInterieurMaximum=" << temperatureInterieurMaximum;
    emit traitementTemperatureInterieurEnvoye(temperatureInterieurMoyenne, temperatureInterieurMinimum , temperatureInterieurMaximum, heureCouranteTemperatureInterieur);
    temperaturesInterieurs.clear();
}

/**
 * @brief methode permettant de calculer la moyenne des temperatures exterieurs sur une heure
 * \fn    InfosTemperature::calculerMoyenneHoraireTemperatureExterieur
 */

void InfosTemperature::calculerMoyenneHoraireTemperatureExterieur()
{
    double sommeTemperatureExterieur = 0;
    double temperatureExterieurMoyenne = 0;
    double temperatureExterieurMinimum = 999;
    double temperatureExterieurMaximum = -999;

    // au moins 2 mesures
    if(temperaturesExterieurs.size() >= 2)
    {
        temperatureExterieurMinimum = temperaturesExterieurs[0];
        temperatureExterieurMaximum = temperaturesExterieurs[0];
        for (int i = 0; i < temperaturesExterieurs.size(); i++)
        {
            sommeTemperatureExterieur += temperaturesExterieurs[i];

            if(temperatureExterieurMinimum > temperaturesExterieurs[i])
            {
                temperatureExterieurMinimum = temperaturesExterieurs[i];
            }

            if(temperatureExterieurMaximum < temperaturesExterieurs[i])
            {
                temperatureExterieurMaximum = temperaturesExterieurs[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << temperaturesExterieurs;
    temperatureExterieurMoyenne = sommeTemperatureExterieur / double(temperaturesExterieurs.size());
    qDebug() << Q_FUNC_INFO << "temperatureExterieurMoyenne=" << temperatureExterieurMoyenne << "temperatureExterieurMinimum=" << temperatureExterieurMinimum << "temperatureExterieurMaximum=" << temperatureExterieurMaximum;
    emit traitementTemperatureExterieurEnvoye(temperatureExterieurMoyenne, temperatureExterieurMinimum, temperatureExterieurMaximum, heureCouranteTemperatureExterieur);
    temperaturesExterieurs.clear();
}

