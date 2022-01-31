package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * @class MainActivity
 * @brief Activité principale de l'application (Thread UI)
 * @author Clément Laurain
 */
public class MainActivity extends AppCompatActivity
{
    private static final String TAG = "MainActivity"; //!< le TAG de la classe pour les logs
    // Base de données MySQL
    private BaseDeDonnees bdd = null; //!< l'objet permettant un accès à la base de données MySQL
    // Paramètres de connexion à la base de données
    private String url; //!< l'URL pointant sur la base de données d'un serveur MySQL
    private String hostname = "192.168.52.119"; //!< l'adresse IP du serveur MySQL
    private String basename = "ruches"; //!< le nom de la la base de données du serveur MySQL
    private String username = "fmellah"; //!< le nom du compte utilisateur (root par défaut)
    private String password = "password"; //!< le mot de passe du compte utilisateur (password par défaut)
    private EditText ed_usernmame;
    private EditText ed_password;
    private Button btn_seConnecter;
    private TextView tv_erreur;

    private LinearLayout m_linearLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ed_usernmame = findViewById(R.id.ed_username);
        ed_password = findViewById(R.id.ed_password);
        btn_seConnecter = findViewById(R.id.btn_seConnecter);
        tv_erreur = findViewById(R.id.tv_erreur);

        // Initialise l'url pour la connexion à la base de données MySQL
        url = "jdbc:mysql://" + hostname + "/" + basename;
        // Récupère l'instance de BaseDeDonnees
        bdd = BaseDeDonnees.getInstance(this, url, username, password);
        bdd.connecter();
        /**
         * @todo Affichage de l'état de connexion MySQL ?
         */

        // Fenêtre d'accès aux ruches
        final Intent homeActivity = new Intent(MainActivity.this, homeActivity.class);
       btn_seConnecter.setOnClickListener(new View.OnClickListener() {
           @Override
           public void onClick(View v) {
               if(ed_usernmame.getText().toString().equals(username) && ed_password.getText().toString().equals((password)))
               {
                   startActivity(homeActivity);
               }
               else
               {
                    tv_erreur.setVisibility(View.VISIBLE);
               }
           }
       });

        // Fenêtre de paramétrage de l'application
        /*FloatingActionButton btn_Parametres = findViewById(R.id.btn_Parametres);
        btn_Parametres.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Crée et démarre une activité
                Intent intent = new Intent(MainActivity.this, ParametresHoneyBeeActivity.class);
                // Passage de données
                intent.putExtra("hostname", hostname);
                intent.putExtra("basename", basename);
                intent.putExtra("username", username);
                intent.putExtra("password", password);
                //startActivity(intent);
                startActivityForResult(intent, HoneyBee.ID_Intent_ParametresHoneyBee);
            }
        });*/
    }

    /**
     * @fn onActivityResult
     * @brief
     */
    protected void onActivityResult(int requestCode, int resultCode, Intent intent)
    {
        //Log.d(TAG, "requestCode=" + requestCode);
        //Log.d(TAG, "resultCode=" + resultCode);
        if (requestCode == HoneyBee.ID_Intent_ParametresHoneyBee)
        {
            switch(resultCode)
            {
                case RESULT_CANCELED:
                    // rien à faire ?
                    break;
                case RESULT_OK:
                    // Récupère les paramètres
                    hostname = intent.getStringExtra("hostname");
                    basename = intent.getStringExtra("basename");
                    username = intent.getStringExtra("username");
                    password = intent.getStringExtra("password");
                    // Recrée l'URL JDBC MySQL
                    url = "jdbc:mysql://" + hostname + "/" + basename;
                    Log.v(TAG, "url=" + url);
                    Log.v(TAG, "username=" + username);
                    Log.v(TAG, "password=" + password);
                    bdd.setParametres(url, username, password);
                    bdd.reconnecter();

                    /**
                     * @todo Gestion d'autres paramètres ? MQTT ? ...
                     */
                    break;
            }
        }
    }
}
