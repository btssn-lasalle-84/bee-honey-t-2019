package fr.campus.laurainc.honeybee;

import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.ArrayList;

public class DebugActivity extends AppCompatActivity {

    private final String TAG = "DebugActivity";
    private TextView donneesTTN;
    private Spinner choixRuche;
    private ClientMQTT clientMQTT=null;
    private Ruche ruche;
    private ArrayList<String> mesRuches;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug);

        choixRuche = findViewById(R.id.spinner);
        donneesTTN = findViewById(R.id.tv_trameTTN);

        donneesTTN.setTypeface(Typeface.MONOSPACE);

        ruche = new Ruche(handler);
        ruche.recupererListeRuches();
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

    private void afficherInformationsRuche ()
    {
        String topic = "mes_ruches/devices/" + ruche.getDeviceID() + "/up";
        clientMQTT = new ClientMQTT(ruche.getDeviceID(), topic, getApplicationContext());
        communiquerTTN(ruche.getDeviceID());
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
                donneesTTN.setText(donneesTTN.getText().toString() + "\n" + "\n" + "\n" + "\n" + mqttMessage.toString());
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });
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
