package fr.campus.laurainc.honeybee;

import android.animation.FloatArrayEvaluator;
import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.CardView;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.CursorAdapter;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ArrayAdapter;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONException;
import org.json.JSONObject;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;

import static java.lang.String.valueOf;

public class DashboardActivity extends AppCompatActivity
{
    private static final String TAG = "DashboardActivity"; //!< le TAG de la classe pour les logs
    // Base de données MySQL
    private BaseDeDonnees bdd = null; //!< l'objet permettant un accès à la base de données MySQL
    // Ruche
    Ruche ruche = null; //!< une ruche
    private ClientMQTT clientMQTT = null;
    private CardView cardView_Ruche; //!< la vue contenant les informations générales de la ruche
    private TextView txtTemperatureInt;
    private TextView txtPoids;
    private TextView txtHumiditeInt;
    private TextView txtTemperatureExt;
    private TextView txtHumiditeExt;
    private TextView txtPression;
    private TextView txtEnsoleillement;
    private TextView txtDescription;
    private TextView txtGPS;
    private TextView txtCharge;
    private TextView txtAlertes;
    private ImageView imgCharge;
    private ImageView carte;
    private Button graphique;
    private TextView donneesTTN;
    private TextView txtHorodatage;
    private FloatingActionButton boutonSupprimerRuche;

    public Spinner choixRuche;
    private ArrayList<String> mesRuches;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dash_board);

        // Récupère les éléments de la vue d'une Ruche dans l'IHM
        recupererElementsVue();

        // Instancie une ruche
        ruche = new Ruche(handler);
        ruche.recupererListeRuches();
    }

    private void recupererElementsVue() {
        cardView_Ruche = findViewById(R.id.CardView_Ruche);
        txtTemperatureInt = (TextView) this.findViewById(R.id.txt_temp_int);
        txtPoids = (TextView) this.findViewById(R.id.txtPoids);
        txtHumiditeInt = (TextView) this.findViewById(R.id.txt_humidite_int);
        txtTemperatureExt = (TextView) this.findViewById(R.id.txt_temp_ext);
        txtHumiditeExt = (TextView) this.findViewById(R.id.txt_humidite_ext);
        txtPression = (TextView) this.findViewById(R.id.txt_pression);
        txtEnsoleillement = (TextView) findViewById(R.id.txt_ensoleillement);
        txtDescription = (TextView) findViewById(R.id.tv_description);
        txtGPS = (TextView) findViewById(R.id.tv_GPS);
        txtHorodatage = (TextView) this.findViewById(R.id.txt_horodatage);
        txtCharge = (TextView) this.findViewById(R.id.charge);
        choixRuche = (Spinner) findViewById(R.id.choixRuche);
        imgCharge = (ImageView) findViewById(R.id.image_charge);
        carte = (ImageView) findViewById(R.id.btn_carte);


        final Intent map = new Intent(DashboardActivity.this, CarteActivity.class);
        carte.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                map.putExtra("longitude", ruche.getLongitude());
                map.putExtra("latitude", ruche.getLatitude());
                Log.d(TAG, "Longitude : " + ruche.getLongitude() + "  /   Longitude :" + " " + ruche.getLatitude());
                startActivity(map);
            }
        });

    }

    private void afficherListeRuches() {
        mesRuches = ruche.getListeRuches();
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.item_spinner ,mesRuches);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        choixRuche.setAdapter(adapter);
        choixRuche.setSelection(0);
        //adapter.setNotifyOnChange(true);

        choixRuche.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id)
            {
                Toast.makeText(getBaseContext(), mesRuches.get(position), Toast.LENGTH_SHORT).show();
                Log.d(TAG, "position : " + position);
                if(clientMQTT != null)
                {
                    if(clientMQTT.estConnecte())
                        clientMQTT.deconnecter();

                }
                // Instancie une ruche
                ruche = new Ruche(mesRuches.get(position), handler);
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
                // TODO Auto-generated method stub
            }
        });
    }


    /**
     * @fn afficherInformationsRuche
     * @brief Affichage des informations d'une ruche dans la vue
     */
    private void afficherInformationsRuche()
    {
        if(ruche != null)
        {
            Log.d(TAG, "Ruche : " + ruche.getNom());
            afficherCharge(ruche.getCharge());
            afficherInfosGenerale(ruche.getDescription(), ruche.getLongitude(), ruche.getLatitude());
            afficherMesuresInt(ruche.getTemperatureInt(), ruche.getHumiditeInt(), ruche.getPoids());
            Log.d(TAG, "Affichage des données intérieures");
            afficherMesuresExt(ruche.getTemperatureExt(), ruche.getHumiditeExt(), ruche.getPression(), ruche.getEnsoleillement());
            Log.d(TAG, "Affichage des données extérieures");
            String topic = "mes_ruches/devices/" + ruche.getDeviceID() + "/up";
            clientMQTT = new ClientMQTT(ruche.getDeviceID(), topic, getApplicationContext());
            communiquerTTN(ruche.getDeviceID());
        }
    }



    private void communiquerTTN(String deviceID)
    {
        Log.d(TAG, "deviceID : " + deviceID);
        clientMQTT.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean b, String s) {

            }

            @Override
            public void connectionLost(Throwable throwable) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                Log.d(TAG, "Topic : " + topic);
                Log.d(TAG, "Message : " + mqttMessage.toString());

                // débogage IHM
                traiterMessage(mqttMessage);
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });
    }

    private void traiterMessage(MqttMessage mqttMessage)
    {
        final String message = mqttMessage.toString();

        try
        {
            JSONObject jsonObjet = new JSONObject(message);
            int port = jsonObjet.getInt("port");
            String Horodatage = clientMQTT.extraireHorodatage(mqttMessage);
            Log.d(TAG, "Port : " + port);
            switch(port)
            {
                case ClientMQTT.PORT_MESURE_POIDS:
                    int poids = clientMQTT.extrairePoids(mqttMessage); // poids en grammes
                    afficherPoids(poids, Horodatage);
                    ruche.setPoids(poids);

                    break;
                case ClientMQTT.PORT_MESURE_RUCHE:
                    double temperatureInterieure = clientMQTT.extraireTemperatureInterieure(mqttMessage);
                    double humiditeInterieure = clientMQTT.extraireHumiditeInterieure(mqttMessage);
                    afficherMesuresInt(temperatureInterieure, humiditeInterieure, Horodatage);
                    ruche.setTemperatureInt(temperatureInterieure);
                    ruche.setHumiditeInt(temperatureInterieure);
                    break;

                case ClientMQTT.PORT_MESURE_ENERGIE:
                    int charge = clientMQTT.extraireCharge(mqttMessage);
                    afficherCharge(charge, Horodatage);
                    ruche.setCharge(charge);
                    break;

                case ClientMQTT.PORT_MESURE_ENVIRONEMENT:
                    double temperatureExterieure = clientMQTT.extraireTemperatureExterieure(mqttMessage);
                    double humiditeExterieure = clientMQTT.extraireHumiditeExterieure(mqttMessage);
                    double pression = clientMQTT.extrairePression(mqttMessage);
                    afficherMesuresExt(humiditeExterieure, temperatureExterieure, pression, Horodatage);
                    ruche.setTemperatureExt(temperatureExterieure);
                    ruche.setHumiditeExt(humiditeExterieure);
                    ruche.setPression(pression);
                    break;

                case ClientMQTT.PORT_MESURE_ENSOLEILLEMENT:
                    double ensoleillement = clientMQTT.extraireEnsoleillement(mqttMessage);
                    txtEnsoleillement.setText(valueOf(ensoleillement) + " Watt/m²");
                    ruche.setEnsoleillement(ensoleillement);

                default:
                    break;
            }
            ruche.setHorodatage(Horodatage);
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }
    }

    private void afficherMesuresInt(double temperatureInterieure, double humiditeInterieure, String Horodatage)
    {
        String horodatageFormate = getHorodatageFormate(Horodatage);
        txtTemperatureInt.setText(valueOf(temperatureInterieure) + " °C");
        txtHumiditeInt.setText(valueOf(humiditeInterieure) + " %");
        txtHorodatage.setText(horodatageFormate);
    }

    private void afficherMesuresInt(double temperatureInterieure, double humiditeInterieure, double poids)
    {
        txtTemperatureInt.setText(valueOf(temperatureInterieure) + " °C");
        txtHumiditeInt.setText(valueOf(humiditeInterieure) + " %");
        txtPoids.setText(valueOf(poids) + " Kg");
    }

    private void afficherMesuresExt(double temperatureExterieure, double humiditeExterieure, double pression, double ensoleillement)
    {
        txtTemperatureExt.setText(valueOf(temperatureExterieure) + " °C");
        txtHumiditeExt.setText(valueOf(humiditeExterieure) + " %");
        txtPression.setText(valueOf(pression) + " hPa");
        txtEnsoleillement.setText(valueOf(ensoleillement) + " Watt/m²");
    }

    private void afficherInfosGenerale (String description, String longitude, String latitude)
    {
        txtDescription.setText(description);
        txtGPS.setText(longitude + "°, " + latitude + "°");
    }

    private void afficherCharge (int charge)
    {
        txtCharge.setText(valueOf(charge) + " %");
        if (charge > 85)
            imgCharge.setImageResource(R.drawable.battery1);
        else if (charge <= 85 && charge > 60)
            imgCharge.setImageResource(R.drawable.battery);
        else if (charge <= 60 && charge > 35)
            imgCharge.setImageResource(R.drawable.battery3);
        else
            imgCharge.setImageResource(R.drawable.battery2);
    }

    private String getHorodatageFormate(String horodatage)
    {
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss", Locale.FRENCH);
        df.setTimeZone(TimeZone.getTimeZone("UTC"));
        Date date = null;
        String horodatageFormate = horodatage;
        try
        {
            date = df.parse(horodatage);
            df.setTimeZone(TimeZone.getDefault());
            horodatageFormate = df.format(date);
            Log.d(TAG, "horodatageFormate : " + horodatageFormate);
        }
        catch (ParseException e)
        {
            e.printStackTrace();
        }
        return horodatageFormate;
    }

    private void afficherCharge (int charge, String horodatage)
    {
        String horodatageFormate = getHorodatageFormate(horodatage);

        txtCharge.setText(valueOf(charge) + " %");
        txtHorodatage.setText(horodatageFormate);
        if (charge > 85)
            imgCharge.setImageResource(R.drawable.battery1);
        else if (charge <= 85 && charge > 60)
            imgCharge.setImageResource(R.drawable.battery);
        else if (charge <= 60 && charge > 35)
            imgCharge.setImageResource(R.drawable.battery3);
        else
            imgCharge.setImageResource(R.drawable.battery2);
    }

    private void afficherPoids(int Poids, String Horodatage)
    {
        String horodatageFormate = getHorodatageFormate(Horodatage);
        double poidsKg = clientMQTT.arrondir(((double)Poids / 1000.), 1);
        txtPoids.setText(valueOf(poidsKg) + " Kg");
        txtHorodatage.setText(horodatageFormate);
    }

    private void afficherMesuresExt(double Humidite, double Temperature, double Pression, String Horodatage)
    {
        String horodatageFormate = getHorodatageFormate(Horodatage);
        txtHumiditeExt.setText(valueOf(Humidite) + " %");
        txtTemperatureExt.setText(valueOf(Temperature) + " °C");
        txtPression.setText(valueOf(Pression) + " hPa");
        txtHorodatage.setText(horodatageFormate);
    }

    final private Handler handler = new Handler()
    {
        /**
         * @fn handleMessage
         * @brief Gestion des messages en provenance des threads
         */
        public void handleMessage(Message msg)
        {
        super.handleMessage(msg);
        switch (msg.what)
        {
            case HoneyBee.REQUETE_SQL_ERREUR:
                Log.d(TAG, "handleMessage -> REQUETE SQL ERREUR");
                break;
            case HoneyBee.REQUETE_SQL_OK:
                Log.d(TAG, "handleMessage -> REQUETE SQL OK");
                break;
            case HoneyBee.REQUETE_SQL_LISTE_RUCHES:
                Log.d(TAG, "handleMessage -> REQUETE SQL LISTE RUCHES");
                afficherListeRuches();
                break;
            case HoneyBee.REQUETE_SQL_RUCHE:
                Log.d(TAG, "handleMessage -> REQUETE SQL RUCHE");
                afficherInformationsRuche();
                break;
        }
        }
    };
}
