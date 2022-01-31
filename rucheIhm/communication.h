#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
  *
  * \file rucheIhm/communication.h
  *
  * \brief Déclaration de la classe Communication
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include "parametres.h"

/**
  *
  * \class Communication
  *
  * \brief La classe Communication
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class Communication : public QObject
{
    Q_OBJECT

    public:
        Communication(QStringList donneesRucheTTN, QObject *parent = 0);
        ~Communication();

        void connecterTTN();
        void deconnecterTTN();

    private:
        QMqttClient *client;
        QMqttSubscription *abonnement;        
        QStringList donneesRucheTTN;
        QString nomTopic;
        PortsTTN portsTTn;

    public slots:
        void connecteTTN();
        void deconnecteTTN();
        void receptionner(const QByteArray &message, const QMqttTopicName &topic);
        void reponsePingTTN();
        void changementEtatTTN();
        void erreurClientTTN();

    signals:
        void etatClientConnexion(bool connexion);
        void messageRecu(const QByteArray &message, const QMqttTopicName &topic);
};

#endif // COMMUNICATION_H

