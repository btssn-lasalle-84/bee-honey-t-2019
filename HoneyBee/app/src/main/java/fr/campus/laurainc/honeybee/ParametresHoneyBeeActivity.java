package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

/**
 * @class ParametresHoneyBeeActivity
 * @brief Activité de paramétrage de l'application
 * @todo D'autres paramètres ? MQTT ? ...
 */
public class ParametresHoneyBeeActivity extends AppCompatActivity
{
    private static final String TAG = "ParametresHoneyBeeActivity"; //!< le TAG de la classe pour les logs
    // Paramètres de connexion à la base de données
    private String hostname; //!< l'adress IP du serveur MySQL
    private String basename = "ruche"; //!< le nom de la la base de données du serveur MySQL
    private String username = "root"; //!< le nom du compte utilisateur (root par défaut)
    private String password = "password"; //!< le mot de passe du compte utilisateur (password par défaut)
    // les éléments de l'IHM pour les paramètres de connexion MySQL
    EditText edtHostname;
    EditText edtBasename;
    EditText edtUsername;
    EditText edtPassword;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_parametres_honey_bee);

        Intent intent = getIntent();
        hostname = intent.getStringExtra("hostname");
        basename = intent.getStringExtra("basename");
        username = intent.getStringExtra("username");
        password = intent.getStringExtra("password");

        edtHostname = (EditText) this.findViewById(R.id.edtHostname);
        edtBasename = (EditText) this.findViewById(R.id.edtBasename);
        edtUsername = (EditText) this.findViewById(R.id.edtUsername);
        edtPassword = (EditText) this.findViewById(R.id.edtPassword);

        edtHostname.setText(hostname, TextView.BufferType.EDITABLE);
        edtBasename.setText(basename, TextView.BufferType.EDITABLE);
        edtUsername.setText(username, TextView.BufferType.EDITABLE);
        edtPassword.setText(password, TextView.BufferType.EDITABLE);
    }

    /**
     * @fn valider
     * @brief Quitte l'activité en appliquant les paramètres
     * @param view View la vue associée
     */
    public void valider(View view) // Ok
    {
        Intent intent = new Intent();
        hostname = edtHostname.getText().toString();
        basename = edtBasename.getText().toString();
        username = edtUsername.getText().toString();
        password = edtPassword.getText().toString();
        intent.putExtra("hostname", hostname);
        intent.putExtra("basename", basename);
        intent.putExtra("username", username);
        intent.putExtra("password", password);
        setResult(RESULT_OK, intent);
        finish();
    }

    /**
     * @fn annuler
     * @brief Quitte l'activité sans prendre en compte les paramètres
     * @param view View la vue associée
     */
    public void annuler(View view) // Annuler
    {
        Intent intent = new Intent();
        setResult(RESULT_CANCELED, intent);
        finish();
    }
}
