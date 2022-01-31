package fr.campus.laurainc.honeybee;

import android.app.DatePickerDialog;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.TextInputEditText;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TabHost;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.Calendar;

public class NouvelleRucheActivity extends AppCompatActivity {

    public final static String TAG = "NouvelleRucheActivity";
    private EditText edNomRuche;
    private EditText edDescription;
    private EditText edAdresse;
    private EditText edLongitude;
    private EditText edLatitude;
    private EditText edDeviceID;
    private Spinner choixAppID;
    private Spinner choixRuche;
    private TextView tv_date;
    private Button btnAjouterRuche;
    private DatePickerDialog.OnDateSetListener fenetreSelectionDate;
    private BaseDeDonnees bdd = null;
    private ArrayList<String> AppID;
    private Ruche rucheUtilitaire;
    private int idTTN;
    private Button btnSupprimerRuche;
    private Ruche ruche;
    private ArrayList<String> mesRuches;
    private Button btnModifierRuche;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nouvelle_ruche);
        mesRuches = new ArrayList<String>();

        ruche = new Ruche(handler);
        ruche.recupererListeRuches();

        recupererElementsIHM();
        initialiserMenuOnglets();
        final Intent actualiser = new Intent(NouvelleRucheActivity.this, NouvelleRucheActivity.class);

       tv_date.setOnClickListener(new View.OnClickListener() {
           @Override
           public void onClick(View v) {
               Calendar calendrier = Calendar.getInstance();
               int annee = calendrier.get(Calendar.YEAR);
               int mois = calendrier.get(Calendar.MONTH);
               int jour = calendrier.get(Calendar.DAY_OF_MONTH);

               DatePickerDialog dialog = new DatePickerDialog(NouvelleRucheActivity.this, android.R.style.Theme_DeviceDefault_Light_Dialog, fenetreSelectionDate, annee, mois, jour);

               dialog.getWindow().setBackgroundDrawable(new ColorDrawable(Color.WHITE));
               dialog.show();
           }
       });

       fenetreSelectionDate = new DatePickerDialog.OnDateSetListener() {
           @Override
           public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
               month = month +1;
               Log.d(TAG, "Date sélectionnée " + dayOfMonth + "/" + month + "/" + year );

               String date = year + "-" + month + "-" + dayOfMonth;
               tv_date.setText(date);

           }
       };

       btnAjouterRuche.setOnClickListener(new View.OnClickListener() {
           @Override
           public void onClick(View v) {
               boolean DevicedIDValide = edDeviceID.getText().toString().equals("");
               boolean nomValide = edNomRuche.getText().toString().equals("");
               boolean descriptionValide = edDescription.getText().toString().equals("");
               boolean adresseValide = edAdresse.getText().toString().equals("");
               boolean longitudeValide = edLongitude.getText().toString().equals("");
               boolean latitudeValide = edLatitude.getText().toString().equals("");
               boolean dateDeMiseEnServiceValide = tv_date.getText().toString().equals("");



               if(DevicedIDValide || nomValide || descriptionValide || adresseValide || longitudeValide || latitudeValide || dateDeMiseEnServiceValide)
                   Toast.makeText(getBaseContext(), "Veuillez saisir tout les champs", Toast.LENGTH_SHORT).show();
               else {
                   if (HoneyBee.BDD) {
                       bdd = BaseDeDonnees.getInstance();
                       bdd.connecter();
                       bdd.executerRequete("INSERT INTO Ruche (idTTN, Nom, Description, DateMiseEnService, Adresse, Longitude, Latitude, DeviceID) VALUES('1', '" + edNomRuche.getText() + "','" + edDescription.getText() + "','" + tv_date.getText() + "','" + edAdresse.getText() + "','" + edLongitude.getText() + "','" + edLatitude.getText() + "','" + edDeviceID.getText() + "')");
                       edDeviceID.setText("");
                       edNomRuche.setText("");
                       edDescription.setText("");
                       edAdresse.setText("");
                       edLongitude.setText("");
                       edLatitude.setText("");
                       tv_date.setText("Choisir une date");
                       finish();
                       startActivity(actualiser);
                       Toast.makeText(getBaseContext(), edNomRuche.getText() + " ajoutée !", Toast.LENGTH_SHORT).show();
                   }
               }
           }
       });

        btnModifierRuche.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean DevicedIDValide = edDeviceID.getText().toString().equals("");
                boolean nomValide = edNomRuche.getText().toString().equals("");
                boolean descriptionValide = edDescription.getText().toString().equals("");
                boolean adresseValide = edAdresse.getText().toString().equals("");
                boolean longitudeValide = edLongitude.getText().toString().equals("");
                boolean latitudeValide = edLatitude.getText().toString().equals("");
                boolean dateDeMiseEnServiceValide = tv_date.getText().toString().equals("Choisir une date");

                if(DevicedIDValide || nomValide || descriptionValide || adresseValide || longitudeValide || latitudeValide || dateDeMiseEnServiceValide)
                    Toast.makeText(getBaseContext(), "Veuillez saisir tout les champs", Toast.LENGTH_SHORT).show();
                else {
                    if (HoneyBee.BDD) {
                        bdd = BaseDeDonnees.getInstance();
                        bdd.connecter();
                        bdd.executerRequete("UPDATE IGNORE Ruche SET Nom='" + edNomRuche.getText() + "', Description='" + edDescription.getText() + "', DateMiseEnService='" + tv_date.getText() + "', Adresse='" + edAdresse.getText() + "', Longitude='" + edLongitude.getText() + "', Latitude='" + edLatitude.getText() + "', DeviceID='" + edDeviceID.getText() + "'");
                        Toast.makeText(getBaseContext(), edNomRuche.getText() + " modifiée !", Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });

        btnSupprimerRuche.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (HoneyBee.BDD) {
                        bdd = BaseDeDonnees.getInstance();
                        bdd.connecter();
                        bdd.supprimerRuche(ruche.getIdRuche());
                        Toast.makeText(getBaseContext(), edNomRuche.getText() + " supprimée !", Toast.LENGTH_SHORT).show();
                        finish();
                        startActivity(actualiser);
                        choixRuche.setSelection(1);
                    }
                }
        });

    }

    private void afficherListeRuches() {
        mesRuches.add(0,"Nouvelle ruche");
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
                ruche = new Ruche(mesRuches.get(position), handler);
                if (mesRuches.get(position).toString().equals("Nouvelle ruche"))
                {
                    btnAjouterRuche.setClickable(true);
                    btnModifierRuche.setClickable(false);
                    btnSupprimerRuche.setClickable(false);
                    btnSupprimerRuche.setBackgroundResource(R.color.btnindispo);
                    btnModifierRuche.setBackgroundResource(R.color.btnindispo);
                    btnAjouterRuche.setBackgroundResource(R.color.btndispo);

                    edDeviceID.setText("");
                    edNomRuche.setText("");
                    edDescription.setText("");
                    edAdresse.setText("");
                    edLongitude.setText("");
                    edLatitude.setText("");
                    tv_date.setText("Choisir une date");
                }
                else
                {
                    btnAjouterRuche.setClickable(false);
                    btnModifierRuche.setClickable(true);
                    btnSupprimerRuche.setClickable(true);
                    btnSupprimerRuche.setBackgroundResource(R.color.btndispo);
                    btnModifierRuche.setBackgroundResource(R.color.btndispo);
                    btnAjouterRuche.setBackgroundResource(R.color.btnindispo);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
                // TODO Auto-generated method stub
            }
        });
    }

    private void actualiserAffichage ()
    {
        edDeviceID.setText(ruche.getDeviceID());
        edNomRuche.setText(ruche.getNom());
        edDescription.setText(ruche.getDescription());
        edAdresse.setText(ruche.getAdresse());
        edLongitude.setText(ruche.getLongitude());
        edLatitude.setText(ruche.getLatitude());
        tv_date.setText(ruche.getDateDeMiseEnService());
    }

    private void recupererElementsIHM()
    {
        edNomRuche = (EditText) findViewById(R.id.edNomRuche);
        edDescription = (EditText) findViewById(R.id.edDesciption);
        edAdresse = (EditText) findViewById(R.id.edAdresseRuche);
        edLongitude = (EditText) findViewById(R.id.edLongitude);
        edLatitude = (EditText) findViewById(R.id.edLatitude);
        edDeviceID = (EditText) findViewById(R.id.edDeviceID);
        tv_date = (TextView) findViewById(R.id.tv_date);
        btnAjouterRuche = (Button) findViewById(R.id.btnAjouterRuche);
        btnModifierRuche = findViewById(R.id.btnModifierRuche);
        btnSupprimerRuche = findViewById(R.id.btnSuprrimerRuche);
        choixAppID = (Spinner) findViewById(R.id.spinner_AppID);
        choixRuche = findViewById(R.id.choixRucheAjout);

    }

    private void initialiserMenuOnglets()
    {
        rucheUtilitaire = new Ruche(handler);
        Log.d(TAG, "Ruche utilitaire créée");
        rucheUtilitaire.recupererChoixChAppID();
        Log.d(TAG, "Recuperation de la liste des App ID");
    }

    private void initialiserChoixAppID() {
        AppID = rucheUtilitaire.getListeChoixAppID();
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, AppID);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        choixAppID.setAdapter(adapter);
        choixAppID.setSelection(0);

        choixAppID.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id)
            {
                Log.d(TAG, "position : " + position);
                Log.d(TAG, "AppID Sélectionné : " + AppID.get(position));
                rucheUtilitaire.recupererIdTTN(AppID.get(position));
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
                // TODO Auto-generated method stub
            }
        });
    }

    private void getIdTTN()
    {
        idTTN = rucheUtilitaire.getIdTTNSelectionne();
        Log.d(TAG, "IdTTN :" + idTTN);
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
                case HoneyBee.REQUETE_SQL_LISTE_RUCHES:
                    Log.d(TAG, "handleMessage -> REQUETE SQL LISTE RUCHES");
                    afficherListeRuches();
                    initialiserChoixAppID();
                    break;
                case HoneyBee.REQUETE_SQL_RUCHE:
                    actualiserAffichage();
                    break;
            }
        }
    };
}
