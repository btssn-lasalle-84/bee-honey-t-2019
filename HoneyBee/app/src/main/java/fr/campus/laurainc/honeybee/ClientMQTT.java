package fr.campus.laurainc.honeybee;

import android.content.Context;
import android.util.Base64;
import android.util.Log;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.DisconnectedBufferOptions;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import org.json.JSONException;
import org.json.JSONObject;


public class ClientMQTT
{
    private static final String TAG = "ClientMQTT"; //!< le TAG de la classe pour les logs
    public MqttAndroidClient mqttAndroidClient;
    private String serverUri = "tcp://eu.thethings.network:1883";
    private String clientId = "mes_ruches";
    private String username = "mes_ruches"; // <ApplicationID>
    private String password = "ttn-account-v2.vC-aqMRnLLzGkNjODWgy81kLqzxBPAT8_mE-L7U2C_w";
    private String subscriptionTopic; //"mes_ruches/devices/ruche_1/up"
    private boolean connecte;
    private boolean inscrit;
    public static final int PORT_MESURE_ENERGIE = 1;
    public static final int PORT_MESURE_POIDS = 2;
    public static final int PORT_MESURE_RUCHE = 3;
    public static final int PORT_MESURE_ENVIRONEMENT = 4;
    public static final int PORT_MESURE_ENSOLEILLEMENT = 5;

    public ClientMQTT (String deviceID, String topic, Context context) // Constructeur
    {
        Log.v(TAG, "deviceID : " + deviceID + " topic : " + topic);
        setSubscriptionTopic(topic);
        connecte = false;
        inscrit = false;
        mqttAndroidClient = new MqttAndroidClient(context, serverUri, clientId);
        mqttAndroidClient.setCallback(new MqttCallbackExtended()
        {
            @Override
            public void connectComplete(boolean b, String s) {
                Log.w(TAG, s);
            }

            @Override
            public void connectionLost(Throwable throwable) {
                Log.w(TAG, "Connexion perdue avec le serveur");
                connecte = false;
            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                Log.w(TAG, mqttMessage.toString());
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {Log.w("mqtt", "Message envoyé");}
        });

        connecter();
    }

    public void setCallback(MqttCallbackExtended callback) {
        mqttAndroidClient.setCallback(callback);
    }

    private void connecter(){
        MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
        mqttConnectOptions.setAutomaticReconnect(true);
        mqttConnectOptions.setCleanSession(false);
        mqttConnectOptions.setUserName(username);
        mqttConnectOptions.setPassword(password.toCharArray());

        try {
            Log.d(TAG, "Connexion au serveur : " + serverUri);
            mqttAndroidClient.connect(mqttConnectOptions, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    DisconnectedBufferOptions disconnectedBufferOptions = new DisconnectedBufferOptions();
                    disconnectedBufferOptions.setBufferEnabled(true);
                    disconnectedBufferOptions.setBufferSize(100);
                    disconnectedBufferOptions.setPersistBuffer(false);
                    disconnectedBufferOptions.setDeleteOldestMessages(false);
                    mqttAndroidClient.setBufferOpts(disconnectedBufferOptions);
                    connecte = true;
                    souscrireTopic();
                    Log.d(TAG, "Connecté au serveur : " + serverUri);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.d(TAG, "Impossible de se connecter au serveur : " + serverUri + exception.toString());
                }
            });
        } catch (MqttException ex){
            ex.printStackTrace();
        }
    } // Méthode permettant de se connecter

    public void deconnecter() {
        try {
            if(estInscrit())
                desinscrireTopic();

            IMqttToken disconToken = mqttAndroidClient.disconnect();
            disconToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.d(TAG, "Déconnecté du serveur : " + serverUri);
                    connecte = false;
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.d(TAG, "Impossible de se déconnecter du serveur : " + serverUri + exception.toString());
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void souscrireTopic() {
        try {
            mqttAndroidClient.subscribe(subscriptionTopic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.w(TAG,"Abonné au topic " + subscriptionTopic);
                    inscrit = true;
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.w(TAG, "Impossible de s'abonner du topic");
                    inscrit = false;
                }
            });

        } catch (MqttException ex) {
            System.err.println("Exceptionst subscribing");
            ex.printStackTrace();
        }
    } // Abonnement à un topic

    public void desinscrireTopic() {
        try {
            IMqttToken unsubToken = mqttAndroidClient.unsubscribe(subscriptionTopic);
            unsubToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.w(TAG,"Désabonné du topic " + subscriptionTopic);
                    inscrit = false;
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.w(TAG, "Impossible de se désabonner du topic");
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public boolean estConnecte() {
        Log.w(TAG,"isConnected " + mqttAndroidClient.isConnected());
        Log.w(TAG,"connecte " + connecte);
        //return mqttAndroidClient.isConnected();
        return connecte;
    }

    public boolean estInscrit() {
        Log.w(TAG,"inscrit " + inscrit);
        return inscrit;
    }

    public void setSubscriptionTopic(String subscriptionTopic) {
        this.subscriptionTopic = subscriptionTopic;
    }

    public double extraireHumiditeInterieure(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double humidite = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            humidite = payloadFields.getDouble("humidite");
            Log.d(TAG, "Humidite intérieure : " + humidite + " %");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return humidite;
    }

    public double extraireTemperatureInterieure(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double temperature = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            temperature = payloadFields.getDouble("temperature");
            Log.d(TAG, "Température : " + temperature + " °C");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return temperature;
    }

    public int extrairePoids(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        int poids = 0;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            poids = payloadFields.getInt("poids");
            Log.d(TAG, "Poids : " + poids + " Kg");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return poids;
    }

    public String extraireHorodatage (MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        String date = "";

        try
        {
            JSONObject jsonObject = new JSONObject(message);
            date = jsonObject.getJSONObject("metadata").getString("time");
            date = date.substring(0, 10) + " " + date.substring(11, 19);
            Log.d(TAG, "Horodatage : " + date);

        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  date;
    }

    public double extraireTemperatureExterieure(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double temperatureExterieure = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            temperatureExterieure = payloadFields.getDouble("temperature");
            Log.d(TAG, "Temperature extérieure : " + temperatureExterieure + " °C");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  temperatureExterieure;
    }

    public double extraireHumiditeExterieure(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double humiditeExterieure = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            humiditeExterieure = payloadFields.getDouble("humidite");
            Log.d(TAG, "Humidité extérieure : " + humiditeExterieure + " %");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  humiditeExterieure;
    }

    public double extrairePression(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double pression = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            pression = payloadFields.getDouble("pression");
            Log.d(TAG, "Pression : " + pression + " hPa");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  pression;
    }

    public double extraireEnsoleillement(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double ensoleillement = 0.;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            ensoleillement = payloadFields.getDouble("ensoleillement");
            Log.d(TAG, "Ensoleillement : " + ensoleillement + " hPa");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  ensoleillement;
    }

    public int extraireCharge(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        int charge = 0;

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            JSONObject payloadFields = jsonObjet.getJSONObject("payload_fields");
            charge = payloadFields.getInt("charge");
            Log.d(TAG, "Charge : " + charge + " %");

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  charge;
    }

    /*public double extraireLongitude (MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();
        double longitude = 0;

        try
        {
            JSONObject jsonObject = new JSONObject(message);
            message = jsonObject.getJSONObject("metadata").getString("time");
            longitude = date.substring(0, 10) + " " + date.substring(11, 19);
            Log.d(TAG, "Horodatage : " + date);

            extraireHorodatage(mqttMessage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }

        return  longitude;
    }*/

    public double arrondir(double nombre,double nbApresVirgule)
    {
        return(double)((int)(nombre * Math.pow(10,nbApresVirgule) + .5)) / Math.pow(10,nbApresVirgule);
    }
}

