package fr.campus.laurainc.honeybee;

import android.content.Intent;
import android.graphics.Camera;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.util.Log;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.Circle;
import com.google.android.gms.maps.model.CircleOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.util.HashMap;
import java.util.Map;

import static java.lang.String.valueOf;

public class CarteActivity extends FragmentActivity implements OnMapReadyCallback {

    final String TAG = "CarteActivity";
    private GoogleMap mMap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_carte);


        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }


    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        Intent intent = getIntent();

        double latitude = 0.0;
        if (intent.hasExtra("latitude")){ // vérifie qu'une valeur est associée à la clé “edittext”
            latitude = Double.parseDouble(intent.getStringExtra("latitude").replaceAll("°", "")); // on récupère la valeur associée à la clé
            Log.v("Map", "Latitude :" + valueOf(latitude));
        }

        double longitude = 0.0;
        if (intent.hasExtra("longitude")){ // vérifie qu'une valeur est associée à la clé “edittext”
            longitude = Double.parseDouble(intent.getStringExtra("longitude").replaceAll("°", "")); // on récupère la valeur associée à la clé
            Log.v("Map", "Longitude" + valueOf(longitude));
        }

        // Add a marker in Sydney and move the camera
        LatLng sydney = new LatLng(latitude, longitude);
        mMap.addMarker(new MarkerOptions().position(sydney).title("Ruche"));
        mMap.moveCamera(CameraUpdateFactory.newLatLng(sydney));
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(sydney, 15));
        mMap.animateCamera(CameraUpdateFactory.zoomTo(10), 2000, null);
        Circle circle = mMap.addCircle(new CircleOptions()
                .center(new LatLng(latitude, longitude))
                .radius(10000)
                .strokeColor(Color.RED));
    }
    }


