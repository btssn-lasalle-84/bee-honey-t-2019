package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Layout;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GridLabelRenderer;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

import static java.lang.String.valueOf;

public class GraphActivity extends AppCompatActivity {

    private static final String TAG = "GraphActivity";
    private Ruche ruche;
    private GraphView graphiqueTemperatures;
    private LineGraphSeries<DataPoint> mSeriesTemperaturesInt;
    private LineGraphSeries<DataPoint> mSeriesTemperaturesExt;
    private LineGraphSeries<DataPoint> mSeriesHumiditeInt;
    private LineGraphSeries<DataPoint> mSeriesHumiditeExt;
    private LineGraphSeries<DataPoint> mSeriesPoids;
    private LineGraphSeries<DataPoint> mSeriesPression;
    private LineGraphSeries<DataPoint> mSeriesEnsoleillement;
    private Button iv_tem_int;
    private Button iv_tem_ext;
    private Button iv_humidite_int;
    private Button iv_humidite_ext;
    private Button iv_poids;
    private Button iv_ensoleillement;
    private Button iv_pression;
    //private ImageView iv_temps_reel;
    private boolean temp_int_afficher;
    private boolean temp_ext_afficher;
    private boolean humidite_int_afficher;
    private boolean humidite_ext_afficher;
    private boolean poids_afficher;
    private boolean ensoleillement_afficher;
    private boolean pression_afficher;
    private boolean temps_reel_afficher;
    private ClientMQTT clientMQTT;
    private TextView tv_valeurCourante;
    private TextView tv_donneesBas;
    private TextView tv_donneesMoyenne;
    private TextView tv_donneesHaut;
    private Spinner choixRuche;
    private ArrayList<String> mesRuches;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_graph);

        choixRuche = findViewById(R.id.choixRuche);
        ruche = new Ruche(handler);
        ruche.recupererListeRuches();

         graphiqueTemperatures = (GraphView) findViewById(R.id.graph);

        //graphiqueTemperatures.setTitleTextSize(40);
        //graphiqueTemperatures.setBackgroundResource(R.color.BleuFond);

        graphiqueTemperatures.getViewport().setYAxisBoundsManual(true);
        graphiqueTemperatures.getViewport().setMinY(0);
        graphiqueTemperatures.getViewport().setMaxY(50);

        graphiqueTemperatures.getViewport().setMinX(0);
        graphiqueTemperatures.getViewport().setMaxX(23);
        graphiqueTemperatures.getViewport().setXAxisBoundsManual(true);
        graphiqueTemperatures.getGridLabelRenderer().setLabelFormatter(new TimeAsXAxisLabelFormatter("H'h'")); // 0h 1h 2h ...
        graphiqueTemperatures.getGridLabelRenderer().setNumHorizontalLabels(12); // nb d'heures affichées sur l'axe X
        graphiqueTemperatures.getGridLabelRenderer().setGridColor(Color.BLACK);
        graphiqueTemperatures.getGridLabelRenderer().setVerticalLabelsColor(Color.BLACK);
        graphiqueTemperatures.getGridLabelRenderer().setHorizontalLabelsColor(Color.BLACK);
        graphiqueTemperatures.getGridLabelRenderer().setGridStyle(GridLabelRenderer.GridStyle.BOTH);

        // Légende
        graphiqueTemperatures.getLegendRenderer().setVisible(true);
        //graphiqueTemperatures.getLegendRenderer().setAlign(LegendRenderer.LegendAlign.TOP);
        graphiqueTemperatures.getLegendRenderer().setFixedPosition(1,48);
        graphiqueTemperatures.getLegendRenderer().setBackgroundColor(0);


        /*GraphView graph = (GraphView) findViewById(R.id.graph);
        LineGraphSeries<DataPoint> series = new LineGraphSeries<>(new DataPoint[] {
                new DataPoint(0, 1),
                new DataPoint(1, 5),
                new DataPoint(2, 3),
                new DataPoint(3, 2),
                new DataPoint(4, 6)
        });
        graph.addSeries(series);*/
    }

    private void initialiserCallBacks()
    {
        tv_valeurCourante.setText(valueOf(ruche.getTemperatureInt()) + "°C");
        tv_donneesBas.setText(valueOf(ruche.getTemp_int_Basse()) + "°C");
        tv_donneesHaut.setText(valueOf(ruche.getTemp_int_Haute()) + "°C");
        tv_donneesMoyenne.setText(String.format("%.2f °C", ruche.getTemp_int_Moyenne()));

        iv_tem_int.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(temp_int_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(40);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(5);
                    iv_tem_int.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getTemperatureInt()) + "°C");
                    tv_donneesBas.setText(valueOf(ruche.getTemp_int_Basse()) + "°C");
                    tv_donneesHaut.setText(valueOf(ruche.getTemp_int_Haute()) + "°C");
                    tv_donneesMoyenne.setText(String.format("%.2f °C", ruche.getTemp_int_Moyenne()));
                    temp_int_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesTemperaturesInt);

                }
            }
        });

        iv_tem_ext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(temp_ext_afficher)
                {
                   clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(40);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(5);
                    iv_tem_ext.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getTemperatureExt()) + "°C");
                    tv_donneesBas.setText(valueOf(ruche.getTemp_ext_Basse()) + "°C");
                    tv_donneesHaut.setText(valueOf(ruche.getTemp_ext_Haute()) + "°C");
                    tv_donneesMoyenne.setText(String.format("%.2f °C", ruche.getTemp_ext_Moyenne()));
                    temp_ext_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesTemperaturesExt);
                }
            }
        });

        iv_humidite_int.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(humidite_int_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(100);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(6);
                    iv_humidite_int.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getHumiditeInt()) + "%");
                    tv_donneesBas.setText(valueOf(ruche.getHum_int_Basse()) + "%");
                    tv_donneesHaut.setText(valueOf(ruche.getHum_int_Haute()) + "%");
                    tv_donneesMoyenne.setText(String.format("%.2f ", ruche.getHum_int_Moyenne())+ "%");
                    humidite_int_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesHumiditeInt);
                }
            }
        });

        iv_humidite_ext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(humidite_ext_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(100);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(6);
                    iv_humidite_ext.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getHumiditeExt()) + "%");
                    tv_donneesBas.setText(valueOf(ruche.getHum_ext_Basse()) + "%");
                    tv_donneesHaut.setText(valueOf(ruche.getHum_ext_Haute()) + "%");
                    tv_donneesMoyenne.setText(String.format("%.2f ", ruche.getHum_ext_Moyenne()) + "%");
                    humidite_ext_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesHumiditeExt);
                }
            }
        });

        iv_poids.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(poids_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(80);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(9);
                    iv_poids.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getPoids()) + "Kg");
                    tv_donneesBas.setText(valueOf(ruche.getPoids_Basse()) + "Kg");
                    tv_donneesHaut.setText(valueOf(ruche.getPoids_Haute()) + "Kg");
                    tv_donneesMoyenne.setText(String.format("%.2f Kg", ruche.getPoids_Moyenne()/1000));
                    poids_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesPoids);
                }
            }
        });

        iv_pression.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(pression_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(1100);
                    graphiqueTemperatures.getViewport().setMinY(900);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(5);
                    iv_pression.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getPression()) + "hPa");
                    tv_donneesBas.setText(valueOf(ruche.getPression_Basse()) + "hPa");
                    tv_donneesHaut.setText(valueOf(ruche.getPression_Haute()) + "hPa");
                    tv_donneesMoyenne.setText(String.format("%.2f hPa", ruche.getPression_Moyenne()));
                    pression_afficher = true;
                    graphiqueTemperatures.addSeries(mSeriesPression);
                }
            }
        });

        iv_ensoleillement.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(ensoleillement_afficher)
                {
                    clearSelection();
                }
                else
                {
                    clearSelection();
                    graphiqueTemperatures.getViewport().setMaxY(1000);
                    graphiqueTemperatures.getViewport().setMinY(0);
                    graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(6);
                    iv_ensoleillement.setBackgroundResource(R.color.BleuSelection);
                    tv_valeurCourante.setText(valueOf(ruche.getEnsoleillement()) + "Watt/m²");
                    tv_donneesBas.setText(valueOf(ruche.getEnsoleillement_Basse()) + "Watt/m²");
                    tv_donneesHaut.setText(valueOf(ruche.getEnsoleillement_Haute()) + "Watt/m²");
                    tv_donneesMoyenne.setText(String.format("%.2f Watt/m²", ruche.getEnsoleillement_Moyenne()));
                    ensoleillement_afficher = true;;
                    graphiqueTemperatures.addSeries(mSeriesEnsoleillement);
                }
            }
        });

    }

    private void initialisationElementsIHM ()
    {
        iv_tem_int = findViewById(R.id.btn_temp_int);
        iv_tem_ext = findViewById(R.id.btn_temp_ext);
        iv_humidite_int = findViewById(R.id.btn_humidite_int);
        iv_humidite_ext = findViewById(R.id.btn_humidite_ext);
        iv_poids = findViewById(R.id.btn_poids);
        iv_ensoleillement = findViewById(R.id.btn_ensoleillement);
        iv_pression = findViewById(R.id.btn_pression);
        tv_valeurCourante = findViewById(R.id.tv_valeurCourante);
        tv_donneesBas = findViewById(R.id.tv_donneesBas);
        tv_donneesMoyenne = findViewById(R.id.tv_donneesMoyenne);
        tv_donneesHaut = findViewById(R.id.tv_donneesHaut);
        //iv_temps_reel = findViewById(R.id.iv_temp_reel);

        temp_int_afficher = true;
        temp_ext_afficher = false;
        humidite_int_afficher = false;
        humidite_ext_afficher = false;
        poids_afficher = false;
        ensoleillement_afficher = false;
        pression_afficher = false;
        temps_reel_afficher = false;

        mSeriesTemperaturesInt = ruche.getmSeriesTemperaturesInt();
        graphiqueTemperatures.getViewport().setMaxY(40);
        graphiqueTemperatures.getGridLabelRenderer().setNumVerticalLabels(5);
        graphiqueTemperatures.removeAllSeries();
        graphiqueTemperatures.addSeries(mSeriesTemperaturesInt);
        mSeriesTemperaturesExt = ruche.getmSeriesTemperaturesExt();
        mSeriesHumiditeInt = ruche.getmSeriesHumiditeInt();
        mSeriesHumiditeExt = ruche.getmSeriesHumiditeExt();
        mSeriesPoids = ruche.getmSeriesPoids();
        mSeriesEnsoleillement = ruche.getmSeriesEnsoleillement();
        mSeriesPression = ruche.getmSeriesPression();

        mSeriesTemperaturesInt.setColor(Color.RED);
        mSeriesHumiditeExt.setColor(Color.RED);
        mSeriesHumiditeInt.setColor(Color.BLUE);
        mSeriesHumiditeExt.setColor(Color.BLUE);
        mSeriesPoids.setColor(Color.GREEN);
        mSeriesPression.setColor(Color.GRAY);
        mSeriesEnsoleillement.setColor(Color.BLACK);
        mSeriesTemperaturesInt.setTitle("°C");
        mSeriesTemperaturesExt.setTitle("°C");
        mSeriesHumiditeInt.setTitle("%");
        mSeriesHumiditeExt.setTitle("%");
        mSeriesPoids.setTitle("Kg");
        mSeriesPression.setTitle("hPa");
        mSeriesEnsoleillement.setTitle("Watt/m²");
        Log.d(TAG, "Initialisation des éléments de l'IHM");
    }

    private void afficherListeRuches() {
        mesRuches = ruche.getListeRuches();
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.item_spinner_graph ,mesRuches);
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
                /*if(clientMQTT != null)
                {
                    if(clientMQTT.estConnecte())
                        clientMQTT.deconnecter();

                }*/
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

    private void clearSelection ()
    {
        iv_tem_ext.setBackgroundResource(R.color.BleuFond);
        temp_ext_afficher = false;

        iv_tem_int.setBackgroundResource(R.color.BleuFond);
        temp_int_afficher = false;

        iv_humidite_int.setBackgroundResource(R.color.BleuFond);
        humidite_int_afficher = false;

        iv_humidite_ext.setBackgroundResource(R.color.BleuFond);
        humidite_ext_afficher = false;

        iv_poids.setBackgroundResource(R.color.BleuFond);
        poids_afficher = false;

        iv_pression.setBackgroundResource(R.color.BleuFond);
        pression_afficher = false;

        iv_ensoleillement.setBackgroundResource(R.color.BleuFond);
        ensoleillement_afficher = false;

        graphiqueTemperatures.removeAllSeries();

    }

    public double arrondir(double nombre,double nbApresVirgule)
    {
        return(double)((int)(nombre * Math.pow(10,nbApresVirgule) + .5)) / Math.pow(10,nbApresVirgule);
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
                case HoneyBee.REQUETE_SQL_LISTE_RUCHES:
                    afficherListeRuches();
                case HoneyBee.REQUETE_SQL_MESURES_RUCHES:
                    Log.d(TAG, "handleMessage -> REQUETE SQL MESURES RUCHE");
                    initialisationElementsIHM();
                    initialiserCallBacks();
                    break;
                case HoneyBee.REQUETE_SQL_RUCHE:
                    ruche.recupererMesuresJournalieresRuche(ruche.getIdRuche());
                    ruche.recupererMoyennes(ruche.getIdRuche());
            }
        }
    };
}
