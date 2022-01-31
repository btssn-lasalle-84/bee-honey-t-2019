#include "communication.h"
#include <QMessageBox>
#include <QDebug>

/**
  *
  * \file rucheIhm/communication.cpp
  *
  * \brief Définition de la classe Communication
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

/**
 * \brief Constructeur de la classe Communication
 *
 * \fn Communication::Communication
 *
 * \param parent QObject Adresse de l'objet Qt parent (0 = fenêtre principale)
 *
 * \details Définition des attributs client , abonement  et nomTopic a 0 (=null)
 *
 */
Communication::Communication(QStringList donneesRucheTTN, QObject *parent) : QObject(parent), client(0), abonnement(0), donneesRucheTTN(donneesRucheTTN)
{
    nomTopic = donneesRucheTTN.at(6) + "/devices/" + donneesRucheTTN.at(2) + "/up";
    qDebug()<< Q_FUNC_INFO << "donneesRucheTTN" << donneesRucheTTN  << "nomTopic" << nomTopic;
    client = new QMqttClient();    
    connect(client, SIGNAL(stateChanged(ClientState)), this, SLOT(changementEtatTTN()));
    connect(client, SIGNAL(errorChanged(ClientError)), this, SLOT(erreurClientTTN()));
    connect(client, SIGNAL(connected()), this, SLOT(connecteTTN()));
    connect(client, SIGNAL(disconnected()), this, SLOT(deconnecteTTN()));
    connect(client, SIGNAL(messageReceived(const QByteArray &, const QMqttTopicName &)), this, SLOT(receptionner(const QByteArray &, const QMqttTopicName &)));
    connect(client, SIGNAL(pingResponseReceived()), this, SLOT(reponsePingTTN()));
    connecterTTN();
}
/**
 * @brief destructeur de la classse Communication
 * \fn Communication::~Communication
 */
Communication::~Communication()
{
    deconnecterTTN();
    delete abonnement;
    delete client;
    qDebug() << Q_FUNC_INFO;
}

void Communication::connecterTTN()
{
    qDebug()<< Q_FUNC_INFO << donneesRucheTTN.at(4) << donneesRucheTTN.at(5).toInt()  << donneesRucheTTN.at(6) << donneesRucheTTN.at(7);
    client->setHostname(donneesRucheTTN.at(4));
    client->setPort(donneesRucheTTN.at(5).toInt());
    client->setUsername(donneesRucheTTN.at(6));
    client->setPassword(donneesRucheTTN.at(7));
    client->connectToHost();
}

void Communication::deconnecterTTN()
{
    client->disconnectFromHost();
}

/**
 * @brief slot permetant la connection au serveur ttn
 * \fn Communication::connecteTTN
 */
void Communication::connecteTTN()
{
    qDebug() << Q_FUNC_INFO << nomTopic;
    // Le client est maintenant connecté
    emit etatClientConnexion(true); // pour l'IHM
    // Souscription à un topic :
    abonnement = client->subscribe(nomTopic);
    if (!abonnement)
    {
        qDebug() << Q_FUNC_INFO << "Impossible de s'abonner au broker TTN !";
        QMessageBox::critical(0, QString::fromUtf8(APP_TITRE), QString::fromUtf8("Impossible de s'abonner au broker The Things Network!"));
    }
}

/**
 * @brief slot permetant la deconnection au serveur ttn
 * \fn Communication::deconnecteTTN
 */
void Communication::deconnecteTTN()
{
    qDebug()<< Q_FUNC_INFO;
    // Le client est maintenant déconnecté
    emit etatClientConnexion(false); // pour l'IHM
}

/**
 * @brief slot permetant la reception des donnée grace au protocole mqtt
 * \param message   correspondant au données recu
 * \param topic     correspondant au nom de topic
 * \fn Communication::receptionner
 */

void Communication::receptionner(const QByteArray &message, const QMqttTopicName &topic)
{
    //qDebug() << Q_FUNC_INFO << topic;
    emit messageRecu(message, topic);
}

/**
 * @brief slot permetant d'effectuer un ping vers le serveur ttn
 * \fn Communication::reponsePingTTN
 */

void Communication::reponsePingTTN()
{
    // Pour les tests (requestPing())
    qDebug()<< Q_FUNC_INFO;
}

/**
 * @brief slot permetant de recevoir l'etat du serveur ttn
 * \fn Communication::changementEtatTTN
 */

void Communication::changementEtatTTN()
{
    // Pour le debug
    QString message;
    switch(client->state())
    {
        case 0: message = "Déconnecté"; break;
        case 1: message = "En cours de connexion"; break;
        case 2: message = "Connecté"; break;
    }
    qDebug()<< Q_FUNC_INFO << "client MQTT" << message << "state=" << client->state();
}

/**
 * @brief slot permetant de recevoir les erreur rencontré lors de la connection au serveur ttn
 * \fn Communication::erreurClientTTN
 */

void Communication::erreurClientTTN()
{

        /*QMqttClient::NoError                0	No error occurred.
        QMqttClient::InvalidProtocolVersion	1	The broker does not accept a connection using the specified protocol version.
        QMqttClient::IdRejected             2	The client ID is malformed. This might be related to its length.
        QMqttClient::ServerUnavailable      3	The network connection has been established, but the service is unavailable on the broker side.
        QMqttClient::BadUsernameOrPassword	4	The data in the username or password is malformed.
        QMqttClient::NotAuthorized          5	The client is not authorized to connect.
        QMqttClient::TransportInvalid       256	The underlying transport caused an error. For example, the connection might have been interrupted unexpectedly.
        QMqttClient::ProtocolViolation      257	The client encountered a protocol violation, and therefore closed the connection.
        QMqttClient::UnknownError           258	An unknown error occurred.
        QMqttClient::Mqtt5SpecificError     259	The error is related to MQTT protocol level 5. A reason code might provide more details.*/

    /**
     * \todo Faire une boîte de dialogue d'information sur l'erreur rencontrée
     */
    qDebug()<< Q_FUNC_INFO << "code erreur" << client->error();
}

