package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import java.util.ArrayList;

import static java.lang.String.valueOf;

public class GestionAlertesActivity extends AppCompatActivity {

    private final String TAG ="GestionAlertesActivity";
    private EditText temp_int_min;
    private EditText temp_int_max;
    private EditText temp_ext_min;
    private EditText temp_ext_max;
    private EditText hum_int_min;
    private EditText hum_int_max;
    private EditText hum_ext_min;
    private EditText hum_ext_max;
    private EditText pression_min;
    private EditText pression_max;
    private EditText poids_min;
    private EditText poids_max;
    private EditText ensoleillement_min;
    private EditText ensoleillement_max;
    private EditText charge_min;
    private Spinner choixRuche;
    private Button btn_valider;
    private BaseDeDonnees bdd=null;
    private Ruche ruche;
    private ArrayList<String> mesRuches;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gestion_alertes);

        ruche = new Ruche(handler);
        ruche.recupererListeRuches();
        mesRuches =ruche.getListeRuches();

        temp_int_min = findViewById(R.id.a_temp_int_min);
        temp_int_max = findViewById(R.id.a_temp_int_max);
        temp_ext_min = findViewById(R.id.a_temp_ext_min);
        temp_ext_max = findViewById(R.id.a_temp_ext_max);
        hum_int_min = findViewById(R.id.a_hum_int_min);
        hum_int_max = findViewById(R.id.a_hum_int_max);
        hum_ext_min = findViewById(R.id.a_hum_ext_min);
        hum_ext_max = findViewById(R.id.a_hum_ext_max);
        pression_min = findViewById(R.id.a_pression_min);
        pression_max = findViewById(R.id.a_pression_max);
        poids_min = findViewById(R.id.a_poids_min);
        poids_max = findViewById(R.id.a_poids_max);
        ensoleillement_min = findViewById(R.id.a_ensoleillement_min);
        ensoleillement_max = findViewById(R.id.a_ensoleillement_max);
        charge_min = findViewById(R.id.a_charge_min);
        choixRuche = findViewById(R.id.choixRucheAlerteGestion);
        btn_valider = findViewById(R.id.btn_valider_alertes);

        btn_valider.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean tempintminValide = temp_int_min.getText().toString().equals("");
                boolean tempintmaxValide = temp_int_max.getText().toString().equals("");
                boolean tempextminValide = temp_ext_min.getText().toString().equals("");
                boolean tempextmaxValide = temp_ext_max.getText().toString().equals("");
                boolean humintminValide = hum_int_min.getText().toString().equals("");
                boolean humintmaxValide = hum_int_max.getText().toString().equals("");
                boolean humextminValide = hum_ext_min.getText().toString().equals("");
                boolean humextmaxValide = hum_ext_max.getText().toString().equals("");
                boolean pressionminValide = pression_min.getText().toString().equals("");
                boolean pressionmaxValide = pression_max.getText().toString().equals("");
                boolean poidsminValide = poids_min.getText().toString().equals("");
                boolean poidsmaxValide = poids_max.getText().toString().equals("");
                boolean ensoleillementminvalide = ensoleillement_min.getText().toString().equals("");
                boolean ensoleillementmaxValide = ensoleillement_max.getText().toString().equals("");
                boolean chargeminvalide = charge_min.getText().toString().equals("");


                final Intent actualiser = new Intent(GestionAlertesActivity.this, GestionAlertesActivity.class);
                if(tempintminValide || tempintmaxValide || tempextminValide || tempextmaxValide || humintminValide || humintmaxValide || humextminValide || humextmaxValide || pressionminValide || pressionmaxValide || poidsminValide || poidsmaxValide || ensoleillementminvalide || ensoleillementmaxValide || chargeminvalide)
                    Toast.makeText(getBaseContext(), "Veuillez saisir tout les champs", Toast.LENGTH_SHORT).show();
                else {
                    if (HoneyBee.BDD) {
                        bdd = BaseDeDonnees.getInstance();
                        bdd.connecter();
                        bdd.executerRequete("UPDATE IGNORE Seuils SET TemperatureIntMin='" + temp_int_min.getText() + "', TemperatureIntMax='" + temp_int_max.getText() + "', TemperatureExtMin='" + temp_ext_min.getText() + "', TemperatureExtMax='" + temp_ext_max.getText() + "', HumiditeIntMin='" + hum_int_min.getText() + "', HumiditeIntMax='" + hum_int_max.getText() + "', HumiditeExtMin='" + hum_ext_min.getText() + "', HumiditeExtMax='" + hum_ext_max.getText()+ "', PressionMin='" + pression_min.getText() + "', PressionMax='" + pression_max.getText() + "', PoidsMin='" + poids_min.getText() + "', PoidsMax='" + poids_max.getText()+ "', EnsoleillementMin='" + ensoleillement_min.getText()+ "', EnsoleillementMax='" + ensoleillement_max.getText()+ "', Charge='" + charge_min.getText() + "' WHERE idRuche='" + ruche.getIdRuche() +"'");
                        Toast.makeText(getBaseContext(), "Seuils de " + ruche.getNom() + " mise à jour !", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }

    private void afficherSeuils ()
    {
        temp_int_min.setText(valueOf(ruche.getTemp_int_min()));
        temp_int_max.setText(valueOf(ruche.getTemp_int_max()));
        temp_ext_min.setText(valueOf(ruche.getTemp_ext_min()));
        temp_ext_max.setText(valueOf(ruche.getTemp_ext_max()));
        hum_int_min.setText(valueOf(ruche.getHum_int_min()));
        hum_int_max.setText(valueOf(ruche.getHum_int_max()));
        hum_ext_min.setText(valueOf(ruche.getHum_ext_min()));
        hum_ext_max.setText(valueOf(ruche.getHum_ext_max()));
        pression_min.setText(valueOf(ruche.getPression_min()));
        pression_max.setText(valueOf(ruche.getPression_max()));
        poids_min.setText(valueOf(ruche.getPoids_min()));
        poids_max.setText(valueOf(ruche.getPoids_max()));
        ensoleillement_min.setText(valueOf(ruche.getEnsoleillement_min()));
        ensoleillement_max.setText(valueOf(ruche.getEnsoleillement_max()));
        charge_min.setText(valueOf(ruche.getCharge_min()));
    }


    private void afficherListeRuches() {
        mesRuches = ruche.getListeRuches();
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.item_spinner ,mesRuches);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        choixRuche.setAdapter(adapter);
        choixRuche.setSelection(0);
        //adapter.setNotifyOnChange(true);

        choixRuche.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id) {
                Toast.makeText(getBaseContext(), mesRuches.get(position), Toast.LENGTH_SHORT).show();
                Log.d(TAG, "position : " + position);
                ruche = new Ruche(mesRuches.get(position), handler);

            }
            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
                // TODO Auto-generated method stub
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
                case HoneyBee.REQUETE_SQL_RUCHE:
                    Log.d(TAG, "handleMessage -> REQUETE SQL RUCHE");
                    ruche.recupererSeuils(ruche.getIdRuche());
                    break;
                case HoneyBee.REQUETE_SQL_LISTE_RUCHES:
                    Log.d(TAG, "handleMessage -> REQUETE SQL LISTES RUCHES");
                    afficherListeRuches();
                    break;
                case HoneyBee.REQUETE_SQL_SEUILS:
                    Log.d(TAG, "handleMessage -> REQUETE SQL SEUILS");
                    afficherSeuils();
                    break;
            }
        }
    };


}
