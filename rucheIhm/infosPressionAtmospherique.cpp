#include "infosPressionAtmospherique.h"

/**
  *
  * \file rucheIhm/infosPressionAtmospherique.cpp
  *
  * \brief Définition de la classe InfosPressionAtmospherique
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QDebug>
#include <QTime>

/**
 * \brief Constructeur de la classe InfosPressionAtmospherique
 *
 * \fn InfosTemperature::InfosTemperature
 *
 * \details Définition des attributs pressionAtmospherique à 0 et l'attribut horodatagePressionAtmospherique à ""
 *
 */
InfosPressionAtmospherique::InfosPressionAtmospherique(QObject *parent): QObject(parent), pressionAtmospherique(0), horodatagePressionAtmospherique(""),heureCourantePressionAtmospherique(-1)
{
}
/**
 * @brief destructeur de la classe InfosPressionAtmospherique
 * \fn InfosPressionAtmospherique::~InfosPressionAtmospherique
 */
InfosPressionAtmospherique::~InfosPressionAtmospherique()
{
}

/**
 * @brief getter de l'attribut pressionAtmospherique
 * \fn    InfosPressionAtmospherique::getPressionAtmospherique
 * @return \e double pressionAtmospherique
 */
double InfosPressionAtmospherique::getPressionAtmospherique()
{
    return pressionAtmospherique;
}

/**
 * @brief setter de l'attribut pressionAtmospherique
 * \fn    InfosPressionAtmospherique::setPressionAtmospherique
 * @param pressionAtmospherique corespondant a la pression atmospherique
 */
void InfosPressionAtmospherique::setPressionAtmospherique(double pressionAtmospherique)
{
    this->pressionAtmospherique = pressionAtmospherique;
}
/**
 * @brief setter de l'attribut horodatagePressionAtmospherique
 * \fn    InfosPressionAtmospherique::setHorodatagePressionAtmospherique
 * @param horodatagePressionAtmospherique corespondant a la horodatagePressionAtmospherique
 */
void InfosPressionAtmospherique::setHorodatagePressionAtmospherique(const QString horodatagePressionAtmospherique)
{
    this->horodatagePressionAtmospherique = horodatagePressionAtmospherique;
}
/**
 * @brief getter de l'attribut horodatagePressionAtmospherique
 * \fn InfosPressionAtmospherique::getHorodatagePressionAtmospherique
 * @return \e QString horodatagePressionAtmospherique
 */
QString InfosPressionAtmospherique::getHorodatagePressionAtmospherique() const
{
    return horodatagePressionAtmospherique;
}

/**
 * @brief slot qui traite la pression atmospherique
 * \fn InfosPressionAtmospherique::traiterNouvellePressionAtmospherique
 * @param pressionAtmospheriqueString       qui corespond a la pression atmospherique envoyé par la Ruche
 * @param horodatagePressionAtmospherique   correspond a l'horodatage de la mesure de la pression atmospherique
 */
void InfosPressionAtmospherique::traiterNouvellePressionAtmospherique(QString pressionAtmospheriqueString ,QString horodatagePressionAtmospherique)
{
    pressionAtmospherique = pressionAtmospheriqueString.toDouble();
    emit pressionAtmospheriqueEnvoye(pressionAtmospherique,horodatagePressionAtmospherique);

    QDateTime dateTimeHorodatage = QDateTime::fromString(horodatagePressionAtmospherique, "dd/MM/yyyy HH:mm:ss");
    qDebug() << Q_FUNC_INFO << "heureCourantePressionAtmospherique" << heureCourantePressionAtmospherique << dateTimeHorodatage.time().hour();
    if(heureCourantePressionAtmospherique == -1)
    {
        heureCourantePressionAtmospherique = dateTimeHorodatage.time().hour();
    }
    if(heureCourantePressionAtmospherique == dateTimeHorodatage.time().hour())
    {
        pressionAtmospheriques.append(pressionAtmospherique);
    }
    else if((heureCourantePressionAtmospherique+1)%24 == dateTimeHorodatage.time().hour())
    {
        calculerMoyenneHorairePressionAtmospherique();
        heureCourantePressionAtmospherique = dateTimeHorodatage.time().hour();
        pressionAtmospheriques.append(pressionAtmospherique);
    }
}

void InfosPressionAtmospherique::calculerMoyenneHorairePressionAtmospherique()
{
    double sommePressionAtmospherique= 0;
    double pressionAtmospheriqueMoyenne = 0;
    double pressionAtmospheriqueMinimum = 1200;
    double pressionAtmospheriqueMaximum = -999;

    // au moins 2 mesures
    if(pressionAtmospheriques.size() >= 2)
    {
        pressionAtmospheriqueMinimum = pressionAtmospheriques[0];
        pressionAtmospheriqueMaximum = pressionAtmospheriques[0];
        for (int i = 0; i < pressionAtmospheriques.size(); i++)
        {
            sommePressionAtmospherique += pressionAtmospheriques[i];

            if(pressionAtmospheriqueMinimum > pressionAtmospheriques[i])
            {
                pressionAtmospheriqueMinimum = pressionAtmospheriques[i];
            }

            if(pressionAtmospheriqueMaximum < pressionAtmospheriques[i])
            {
                pressionAtmospheriqueMaximum = pressionAtmospheriques[i];
            }
        }
    }
    qDebug() << Q_FUNC_INFO << pressionAtmospheriques;
    pressionAtmospheriqueMoyenne = sommePressionAtmospherique/ double(pressionAtmospheriques.size());
    emit traitementPressionAtmospheriqueEnvoye(pressionAtmospheriqueMoyenne, pressionAtmospheriqueMinimum ,pressionAtmospheriqueMaximum, heureCourantePressionAtmospherique);
    pressionAtmospheriques.clear();
}
