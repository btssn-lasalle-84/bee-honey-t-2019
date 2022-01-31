package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class homeActivity extends AppCompatActivity {

    private LinearLayout layout_tableauDeBord;
    private LinearLayout layout_graphs;
    private LinearLayout layout_map;
    private LinearLayout layout_alertes;
    private LinearLayout layout_gestionAlertes;
    private LinearLayout layout_gestionRuches;
    private LinearLayout layout_debug;
    private ImageView img_disconnect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        layout_tableauDeBord = findViewById(R.id.layout_tableauDeBord);
        layout_graphs = findViewById(R.id.layout_graphs);
        layout_alertes = findViewById(R.id.layout_alertes);
        layout_gestionAlertes = findViewById(R.id.layout_gestionAlertes);
        layout_gestionRuches = findViewById(R.id.layout_gestionRuche);
        layout_debug = findViewById(R.id.layout_debug);
        img_disconnect = findViewById(R.id.img_disconnect);


        final Intent tableauDeBord = new Intent(homeActivity.this, DashboardActivity.class);
        layout_tableauDeBord.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(tableauDeBord);
            }
        });

        final Intent graphique = new Intent(homeActivity.this, GraphActivity.class);
        layout_graphs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(graphique);
            }
        });


        final Intent gestionRuche = new Intent(homeActivity.this, NouvelleRucheActivity.class);
        layout_gestionRuches.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(gestionRuche);
            }
        });
        final Intent disconnect = new Intent(homeActivity.this, MainActivity.class);
        img_disconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(disconnect);
            }
        });
        final Intent gestionAlertes = new Intent(homeActivity.this, GestionAlertesActivity.class);
        layout_gestionAlertes.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(gestionAlertes);
            }
        });

        final Intent alertes = new Intent(homeActivity.this, alertesActivity.class);
        layout_alertes.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(alertes);
            }
        });

        final Intent debug = new Intent(homeActivity.this, DebugActivity.class);
        layout_debug.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                startActivity(debug);
            }
        });

    }
}
