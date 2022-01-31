#ifndef PARAMETRES_H
#define PARAMETRES_H

/**
  *
  * \file rucheIhm/parametres.h
  *
  * \brief Paramètres généraux de l'application
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

// Titre de l'application
#define APP_TITRE       "Projet Ruche 2019"
//192.168.52.119
// Paramètres de connexion à la base de données
#define BDD_SERVEUR     "192.168.52.119"
#define BDD_USERNAME    "fmellah"
#define BDD_PASSWORD    "password"
#define BDD_NOMBASE     "ruches"

// Paramètres de connexion au serveur The Things Network
#define TTN_SERVEUR     "eu.thethings.network"
#define TTN_PORT        1883
#define TTN_USERNAME    "mes_ruches"        
#define TTN_PASSWORD    "ttn-account-v2.vC-aqMRnLLzGkNjODWgy81kLqzxBPAT8_mE-L7U2C_w"
#define TTN_TOPIC       "mes_ruches/devices/ruche_1/up"
#define TTN_EMAIL       "florentinmellah@gmail.com"

// Paramètres d'envoi des mails d'alerte
#define USER_GMAIL      QLatin1String("bee.honey.bts@gmail.com")
#define PASSWORD_GMAIL  QLatin1String("ruches123")

// Seuils par défaut des alertes
#define TEMPERATURE_INTERIEUR_SEUIL_MAX     35.0
#define TEMPERATURE_INTERIEUR_SEUIL_MIN     25.
#define HUMIDITE_INTERIEUR_SEUIL_MAX        30.
#define HUMIDITE_INTERIEUR_SEUIL_MIN        20.
#define TEMPERATURE_EXTERIEUR_SEUIL_MAX     35.
#define TEMPERATURE_EXTERIEUR_SEUIL_MIN     5.
#define HUMIDITE_EXTERIEUR_SEUIL_MAX        35.
#define HUMIDITE_EXTERIEUR_SEUIL_MIN        20.
#define PRESSION_ATMOSPHERIQUE_SEUIL_MIN    1000.
#define PRESSION_ATMOSPHERIQUE_SEUIL_MAX    1200.
#define POIDS_SEUIL_MAX                     100.
#define POIDS_SEUIL_MIN                     35.
#define ENSOLEILLEMENT_SEUIL_MAX            1000.
#define ENSOLEILLEMENT_SEUIL_MIN            10.
#define BATTERIE_SEUIL_MIN                  25.


typedef enum
{
    tropHaut = 0,
    tropBas = 1,
    bon = 2,

} SeuilsAlertes;

typedef enum
{
    alerteTemperatureInterieur = 0,
    alerteTemperatureExterieur = 1,
    alerteHumiditeInterieur = 2,
    alerteHumiditeExterieur = 3,
    alertePressionAtmospherique = 4,
    alertePoids = 5,
    alerteEnsoleillement = 6,
    alerteBatterie = 7,
    toutesLesAlertes = 8,
} TypeAlertes;

typedef enum
{
    portInconnu = 0,
    portMesureEnergie = 1,
    portMesurePoids,
    portMesureRuche,
    portMesureEnvironement,
    portMesureEnsoleillement,
    portVol,
    nbPortsTTN
} PortsTTN;

#endif // PARAMETRES_H
