package fr.campus.laurainc.honeybee;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class alertesActivity extends AppCompatActivity {

    private final String TAG = "alertesActivity";
    private TextView tv_hitoriqueAlertes;
    private Ruche ruche;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alertes);

        ruche = new Ruche(handler);

        tv_hitoriqueAlertes = findViewById(R.id.tv_historiqueAlertes);
        ruche.recupererHistoriqueAlertes();

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
                case HoneyBee.REQUETE_SQL_ALERTES:
                    Log.d(TAG, "handleMessage -> REQUETE SQL ALERTES");
                    tv_hitoriqueAlertes.setText(ruche.getHistoriqueAlertes());
                    break;
            }
        }
    };
}
