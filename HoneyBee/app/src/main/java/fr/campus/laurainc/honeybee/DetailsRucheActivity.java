package fr.campus.laurainc.honeybee;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

/**
 * @todo Renommer cettte classe
 */
public class DetailsRucheActivity extends AppCompatActivity {

    int idRuche;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_details);


    }

    public void setIdRuche(int idRuche) {
        this.idRuche = idRuche;
    }
}
