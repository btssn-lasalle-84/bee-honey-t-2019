package fr.campus.laurainc.honeybee;

import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.EditText;

import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.sql.Array;
import java.sql.ResultSet;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static java.lang.String.valueOf;

/**
 * @class Ruche
 * @brief
 */
public class Ruche {
    private static final String TAG = "Ruche";
    private int idRuche;
    private String nom;
    private String description;
    private String deviceID;
    private String adresse;
    private double temperatureInt;
    private double humiditeInt;
    private double temperatureExt;
    private double humiditeExt;
    private double pression;
    private double ensoleillement;
    private int charge;
    private String latitude;
    private String longitude;
    private String dateDeMiseEnService;
    private double poids;
    private String Horodatage;
    private BaseDeDonnees bdd = null;
    private Handler handler = null;
    private ArrayList<String> listeChoixRuches;
    private ArrayList<String> listeChoixAppID;
    private int idTTNSelectionne;
    private LineGraphSeries<DataPoint> mSeriesTemperaturesInt;
    private LineGraphSeries<DataPoint> mSeriesTemperaturesExt;
    private LineGraphSeries<DataPoint> mSeriesHumiditeInt;
    private LineGraphSeries<DataPoint> mSeriesHumiditeExt;
    private LineGraphSeries<DataPoint> mSeriesPoids;
    private LineGraphSeries<DataPoint> mSeriesPression;
    private LineGraphSeries<DataPoint> mSeriesEnsoleillement;
    private Double temp_int_Basse;
    private Double temp_int_Moyenne;
    private Double temp_int_Haute;
    private Double hum_int_Basse;
    private Double hum_int_Moyenne;
    private Double hum_int_Haute;
    private Double poids_Basse;
    private Double poids_Moyenne;
    private Double poids_Haute;
    private Double pression_Basse;
    private Double pression_Moyenne;
    private Double pression_Haute;
    private Double ensoleillement_Basse;
    private Double ensoleillement_Moyenne;
    private Double ensoleillement_Haute;
    private Double temp_ext_Basse;
    private Double temp_ext_Moyenne;
    private Double temp_ext_Haute;
    private Double hum_ext_Basse;
    private Double hum_ext_Moyenne;
    private Double hum_ext_Haute;
    private String historiqueAlertes;
    private double temp_int_min;
    private double temp_int_max;
    private double temp_ext_min;
    private double temp_ext_max;
    private double hum_int_min;
    private double hum_int_max;
    private double hum_ext_min;
    private double hum_ext_max;
    private double pression_min;
    private double pression_max;
    private double poids_min;
    private double poids_max;
    private double ensoleillement_min;
    private double ensoleillement_max;
    private double charge_min;


    /**
     * @fn Ruche
     * @brief Constructeur par défaut de la classe Ruche
     */
    public Ruche() {
        Log.d(TAG, "Constructeur par défaut de la classe Ruche");
        listeChoixRuches = new ArrayList<String>();
        listeChoixAppID = new ArrayList<String>();
        if (HoneyBee.BDD) {
            bdd = BaseDeDonnees.getInstance();
            bdd.connecter();
        }
    }

    /**
     * @fn Ruche
     * @brief Constructeur par défaut de la classe Ruche
     */
    public Ruche(Handler handler) {
        Log.d(TAG, "Constructeur par défaut de la classe Ruche");
        this.handler = handler;
        listeChoixRuches = new ArrayList<String>();
        listeChoixAppID = new ArrayList<String>();
        mSeriesTemperaturesInt = new LineGraphSeries<>();
        mSeriesTemperaturesExt = new LineGraphSeries<>();
        mSeriesHumiditeInt = new LineGraphSeries<>();
        mSeriesHumiditeExt = new LineGraphSeries<>();
        mSeriesPoids = new LineGraphSeries<>();
        mSeriesPression = new LineGraphSeries<>();
        mSeriesEnsoleillement = new LineGraphSeries<>();
        historiqueAlertes = new String();
        if (HoneyBee.BDD) {
            bdd = BaseDeDonnees.getInstance();
            bdd.connecter();
        }
    }

    /**
     * @param Nom String le nom de la ruche dans la table Ruche
     * @fn Ruche
     * @brief Constructeur de la classe Ruche à partir d'un Nom
     */
    public Ruche(String Nom) {
        Log.d(TAG, "Constructeur de la classe Ruche -> nom : " + Nom);
        this.nom = Nom;
        listeChoixRuches = new ArrayList<String>();
        listeChoixAppID = new ArrayList<String>();
        mSeriesTemperaturesInt = new LineGraphSeries<>();
        mSeriesTemperaturesExt = new LineGraphSeries<>();
        mSeriesHumiditeInt = new LineGraphSeries<>();
        mSeriesHumiditeExt = new LineGraphSeries<>();
        mSeriesPoids = new LineGraphSeries<>();
        mSeriesPression = new LineGraphSeries<>();
        mSeriesEnsoleillement = new LineGraphSeries<>();
        if (HoneyBee.BDD) {
            bdd = BaseDeDonnees.getInstance();
            bdd.connecter();
            // Récupères les informations de cette ruche dans la base de données
            recuperer(Nom);
        }
    }

    /**
     * @param Nom String le nom de la ruche dans la table Ruche
     * @fn Ruche
     * @brief Constructeur de la classe Ruche à partir d'un Nom
     */
    public Ruche(String Nom, Handler handler) {
        Log.d(TAG, "Constructeur de la classe Ruche -> nom : " + Nom);
        this.nom = Nom;
        listeChoixRuches = new ArrayList<String>();
        listeChoixAppID = new ArrayList<String>();
        mSeriesTemperaturesInt = new LineGraphSeries<>();
        mSeriesTemperaturesExt = new LineGraphSeries<>();
        mSeriesHumiditeInt = new LineGraphSeries<>();
        mSeriesHumiditeExt = new LineGraphSeries<>();
        mSeriesPoids = new LineGraphSeries<>();
        mSeriesPression = new LineGraphSeries<>();
        mSeriesEnsoleillement = new LineGraphSeries<>();

        this.handler = handler;
        if (HoneyBee.BDD) {
            bdd = BaseDeDonnees.getInstance();
            bdd.connecter();
            // Récupères les informations de cette ruche dans la base de données
            recuperer(nom);
        }
    }

    /**
     * @return String le nom de la ruche dans la table Ruche
     * @fn getNom
     * @brief Retourne le nom de la ruche
     */


    public String getNom() {
        return nom;
    }

    public double getTemperatureInt() {
        return temperatureInt;
    }

    public void setTemperatureInt(double temperatureInt) {
        this.temperatureInt = temperatureInt;
    }

    public double getPoids() {
        return poids / 1000;
    }

    public void setHorodatage(String horodatage) {
        Horodatage = horodatage;
    }

    public void setPoids(double poids) {
        this.poids = poids;
    }

    public double getHumiditeInt() {
        return humiditeInt;
    }

    public void setHumiditeInt(double humiditeInt) {
        this.humiditeInt = humiditeInt;
    }

    public double getTemperatureExt() { return temperatureExt;}

    public void setTemperatureExt(double temperatureExt) {
        this.temperatureExt = temperatureExt;
    }

    public double getHumiditeExt () { return humiditeExt;}

    public void setHumiditeExt(double humiditeExt) {
        this.humiditeExt = humiditeExt;
    }

    public double getPression () { return pression;}

    public void setPression(double pression) {
        this.pression = pression;
    }

    public double getEnsoleillement () { return ensoleillement;}

    public void setEnsoleillement(double ensoleillement) {
        this.ensoleillement = ensoleillement;
    }

    public int getCharge() {
        return charge;
    }

    public void setCharge(int charge) {
        this.charge = charge;
    }

    public String getLatitude () { return latitude;}

    public String getLongitude () { return longitude;}

    public String getDescription() {
        return description;
    }

    public String getDateDeMiseEnService() {
        return dateDeMiseEnService;
    }

    public String getDeviceID() {
        return deviceID;
    }

    /**
     * @param handler Handler le gestionnaire de messages du thread UI
     * @fn setHandler
     * @brief Fixe le gestionnaire de messages du thread UI
     */
    public void setHandler(Handler handler) {
        this.handler = handler;
    }

    /**
     * @return Nom String le nom de la ruche dans la table Ruche
     * @fn recuperer
     * @brief Récupère dans la table Ruche l'enregistrement correspondant au nom passé en argument
     */
    public void recuperer(final String Nom) {
        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        boolean trouve = false;
                        String requeteRucheInfo = "SELECT * FROM Ruche WHERE Nom='" + Nom + "'";
                        Log.d(TAG, "Requete : " + requeteRucheInfo);
                        Statement statement = bdd.connexion.createStatement();
                        ResultSet resultSet = statement.executeQuery(requeteRucheInfo);
                        if (resultSet.first()) {
                            idRuche = resultSet.getInt("idRuche");
                            nom = resultSet.getString("Nom");
                            description = resultSet.getString("Description");
                            deviceID = resultSet.getString("DeviceID");
                            description = resultSet.getString("Description");
                            longitude = resultSet.getString("Longitude");
                            latitude = resultSet.getString("Latitude");
                            dateDeMiseEnService = resultSet.getString("DateMiseEnService");
                            adresse = resultSet.getString("Adresse");

                            Log.v(TAG, "BDD -> idRuche : " + idRuche);
                            Log.v(TAG, "BDD -> Nom : " + nom);
                            Log.v(TAG, "BDD -> Description : " + description);
                            Log.d(TAG, "BDD -> DeviceID : " + deviceID);
                            Log.v(TAG, "BDD -> Longitude : " + longitude);
                            Log.v(TAG, "BDD -> Latitude : " + latitude);
                            Log.v(TAG, "BDD -> Date de mise en service : " + dateDeMiseEnService);
                            Log.v(TAG, "BDD -> Adresse : " + adresse);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        String requeteMesuresInfoInt = "SELECT * FROM `MesuresRuche` WHERE `idRuche`='" + idRuche + "' ORDER BY `Horodatage` DESC";
                        Log.d(TAG, "Requete : " + requeteMesuresInfoInt);
                        ResultSet resultSet2 = statement.executeQuery(requeteMesuresInfoInt);

                        if (resultSet2.first()) {
                            temperatureInt = resultSet2.getInt("Temperature");
                            humiditeInt = resultSet2.getInt("Humidite");
                            Log.v(TAG, "BDD -> Temperature Intérieure: " + temperatureInt);
                            Log.v(TAG, "BDD -> Humidite Intérieure: " + humiditeInt);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        String requetePoids = "SELECT `Poids` FROM `MesuresPoids` WHERE `idRuche`='" + idRuche + "' ORDER BY `Horodatage` DESC";
                        Log.d(TAG, "Requete : " + requetePoids);
                        ResultSet resultSet3 = statement.executeQuery(requetePoids);

                        if (resultSet3.first()) {
                            poids = resultSet3.getInt("Poids");
                            Log.v(TAG, "BDD -> Poids : " + poids);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        String requeteMesuresExt = "SELECT * FROM `MesuresEnvironnement` WHERE `idRuche`='" + idRuche + "' ORDER BY `Horodatage` DESC";
                        Log.d(TAG, "Requete : " + requeteMesuresExt);
                        ResultSet resultSet4 = statement.executeQuery(requeteMesuresExt);

                        if (resultSet4.first()) {
                            temperatureExt = resultSet4.getInt("Temperature");
                            humiditeExt = resultSet4.getInt("Humidite");
                            pression = resultSet4.getInt("Pression");
                            Log.v(TAG, "BDD -> Temperature Extérieure : " + temperatureExt);
                            Log.v(TAG, "BDD -> Humidite Extérieure : " + humiditeExt);
                            Log.v(TAG, "BDD -> Pression : " + pression);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        String requeteEnsoleillement = "SELECT `Ensoleillement` FROM `MesuresEnsoleillement` WHERE `idRuche`='" + idRuche + "' ORDER BY `Horodatage` DESC";
                        Log.d(TAG, "Requete : " + requeteEnsoleillement);
                        ResultSet resultSet5 = statement.executeQuery(requeteEnsoleillement);

                        if (resultSet5.first()) {
                            ensoleillement = resultSet5.getInt("Ensoleillement");
                            Log.v(TAG, "BDD -> Ensoleillement : " + ensoleillement);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        String requeteCharge = "SELECT `Charge` FROM `MesuresEnergie` WHERE `idRuche`='" + idRuche + "' ORDER BY `Horodatage` DESC";
                        Log.d(TAG, "Requete : " + requeteCharge);
                        ResultSet resultSet6 = statement.executeQuery(requeteCharge);

                        if (resultSet6.first()) {
                            charge = resultSet6.getInt("Charge");
                            Log.v(TAG, "BDD -> Charge : " + charge);
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        if (handler != null && trouve) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_RUCHE;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            }
                            else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public void recupererListeRuches() {

        listeChoixRuches.clear();

        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        String requeteListesRuches = "SELECT Nom FROM `Ruche`";
                        Log.d(TAG, "Requete : " + requeteListesRuches);
                        Statement statement = bdd.connexion.createStatement(
                        ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);
                        ResultSet resultSet3 = statement.executeQuery(requeteListesRuches);

                        while (resultSet3.next()) {
                            listeChoixRuches.add(resultSet3.getString("Nom"));
                            int row = resultSet3.getRow();
                            Log.v(TAG, "BDD -> Données contenues dans la ligne " + row);
                            Log.v(TAG, "BDD -> listeRuches : " + listeChoixRuches.get(row-1));
                        }

                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_LISTE_RUCHES;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            } else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public ArrayList<String> getListeRuches() {
        return listeChoixRuches;
    }

    public void recupererChoixChAppID ()
    {
        listeChoixAppID.clear();

        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        String requeteListesAppID = "SELECT `ApplicationID`FROM `TTN`";
                        Log.d(TAG, "Requete : " + requeteListesAppID);
                        Statement statement = bdd.connexion.createStatement(
                                ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);
                        ResultSet resultSet3 = statement.executeQuery(requeteListesAppID);

                        while (resultSet3.next()) {
                            listeChoixAppID.add(resultSet3.getString("ApplicationID"));
                            int row = resultSet3.getRow();
                            Log.v(TAG, "BDD -> Données contenues dans la ligne " + row);
                            Log.v(TAG, "BDD -> listeChoixAppID : " + listeChoixAppID.get(row-1));
                        }

                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_LISTE_RUCHES;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            } else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public ArrayList<String> getListeChoixAppID ()
    {
        return listeChoixAppID;
    }

    public void recupererIdTTN(final String ApplicationID)
    {
        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                    boolean trouve = false;
                    String requeteIdTTN = "SELECT idTTN FROM TTN WHERE ApplicationID=\"" + ApplicationID+ "\"";
                    Log.d(TAG, "Requete : " + requeteIdTTN);
                    Statement statement = bdd.connexion.createStatement();
                    ResultSet resultSet = statement.executeQuery(requeteIdTTN);
                    if (resultSet.first()) {
                        idTTNSelectionne = resultSet.getInt("idTTN");
                        Log.v(TAG, "BDD -> idTTN : " + idTTNSelectionne);
                        trouve = true;
                    }
                    else
                        Log.i(TAG, "Aucun résultat obtenu !");

                    if (handler != null && trouve) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_IDTTN;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            }
                            else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public int getIdTTNSelectionne() {
        return idTTNSelectionne;
    }

    public int getIdRuche() {
        return idRuche;
    }

    public void recupererMoyennes (final int idRuche)
    {
        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        boolean trouve=false;
                        String requeteMoyenneTempInt = "SELECT AVG(Temperature) FROM MesuresRuche WHERE idRuche='" + idRuche + "'";
                        String requeteMoyenneTempExt = "SELECT AVG(Temperature) FROM MesuresEnvironnement WHERE idRuche='" + idRuche + "'";
                        String requeteMoyenneHumInt = "SELECT AVG(Humidite) FROM MesuresRuche WHERE idRuche='" + idRuche + "'";
                        String requeteMoyenneHumExt = "SELECT AVG(Humidite) FROM MesuresEnvironnement WHERE idRuche='" + idRuche + "'";
                        String requeteMoyennePression = "SELECT AVG(Pression) FROM MesuresEnvironnement WHERE idRuche='" + idRuche + "'";
                        String requeteMoyenneEnsoleillement = "SELECT AVG(Ensoleillement) FROM MesuresEnsoleillement WHERE idRuche='" + idRuche + "'";
                        String requeteMoyennePoids = "SELECT AVG(Poids) FROM MesuresPoids WHERE  idRuche='" + idRuche + "'";
                        Statement statement = bdd.connexion.createStatement();

                        ResultSet resultSet = statement.executeQuery(requeteMoyenneTempInt);
                        if (resultSet.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyenneTempInt);
                            temp_int_Moyenne = resultSet.getDouble("AVG(Temperature)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        ResultSet resultSet2 = statement.executeQuery(requeteMoyenneTempExt);
                        if (resultSet2.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyenneTempExt);
                            temp_ext_Moyenne = resultSet2.getDouble("AVG(Temperature)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        ResultSet resultSet3 = statement.executeQuery(requeteMoyenneHumInt);
                        if (resultSet3.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyenneHumInt);
                            hum_int_Moyenne = resultSet3.getDouble("AVG(Humidite)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        ResultSet resultSet4 = statement.executeQuery(requeteMoyenneHumExt);
                        if (resultSet4.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyenneHumExt);
                            hum_ext_Moyenne = resultSet4.getDouble("AVG(Humidite)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        ResultSet resultSet5 = statement.executeQuery(requeteMoyenneEnsoleillement);
                        if (resultSet5.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyenneEnsoleillement);
                            ensoleillement_Moyenne = resultSet5.getDouble("AVG(Ensoleillement)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");

                        ResultSet resultSet6 = statement.executeQuery(requeteMoyennePoids);
                        if (resultSet6.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyennePoids);
                            poids_Moyenne = resultSet6.getDouble("AVG(Poids)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");


                        ResultSet resultSet7 = statement.executeQuery(requeteMoyennePression);
                        if (resultSet7.first())
                        {
                            Log.d(TAG, "Requete :" + requeteMoyennePression);
                            pression_Moyenne = resultSet7.getDouble("AVG(Pression)");
                            trouve = true;
                        }
                        else
                            Log.i(TAG, "Aucun résultat obtenu !");




                        if (handler != null && trouve) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_IDTTN;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            }
                            else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }

    }

    public void recupererSeuils (final int idRuche)
    {
        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        boolean trouve=false;
                        String requeteSeuils = "SELECT * FROM Seuils WHERE idRuche='" + idRuche + "'";

                        Statement statement = bdd.connexion.createStatement();

                        ResultSet resultSet = statement.executeQuery(requeteSeuils);
                        if (resultSet.first())
                        {
                            temp_int_min = resultSet.getDouble("TemperatureIntMin");
                            temp_int_max = resultSet.getDouble("TemperatureIntMax");
                            temp_ext_min = resultSet.getDouble("TemperatureExtMin");
                            temp_ext_max = resultSet.getDouble("TemperatureExtMax");
                            hum_int_min = resultSet.getDouble("HumiditeIntMin");
                            hum_int_max = resultSet.getDouble("HumiditeIntMax");
                            hum_ext_min = resultSet.getDouble("HumiditeExtMin");
                            hum_ext_max = resultSet.getDouble("HumiditeExtMax");
                            poids_min = resultSet.getDouble("PoidsMin");
                            poids_max = resultSet.getDouble("PoidsMax");
                            pression_min = resultSet.getDouble("PressionMin");
                            pression_max = resultSet.getDouble("PressionMax");
                            ensoleillement_min = resultSet.getDouble("EnsoleillementMin");
                            ensoleillement_max = resultSet.getDouble("EnsoleillementMax");
                            charge_min = resultSet.getDouble("Charge");


                            trouve = true;
                        }


                        if (handler != null && trouve) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_SEUILS;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            }
                            else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }


    }

    public  void recupererHistoriqueAlertes ()
    {
        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        boolean trouve=false;
                        String requeteHitoriqueAlertes = "SELECT * FROM Alertes WHERE idRuche='6'";
                        Statement statement = bdd.connexion.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);

                        ResultSet resultSet = statement.executeQuery(requeteHitoriqueAlertes);
                        Log.d(TAG, "Requete :" + requeteHitoriqueAlertes);
                        while (resultSet.next()) {
                            historiqueAlertes += resultSet.getString("Description") + " " + resultSet.getString("Horodatage")+"\n";
                            trouve = true;
                        }

                        Log.d(TAG, "Alertes : " + historiqueAlertes);


                        if (handler != null && trouve) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ALERTES;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            }
                            else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }

    }

    public void recupererMesuresJournalieresRuche(final int idRuche) {

        if (!HoneyBee.BDD)
            return;

        if (bdd.estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {
                    bdd.mutex.lock();
                    try {
                        Calendar calendar = Calendar.getInstance();
                        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd");
                        final String aujourdhui = simpleDateFormat.format(calendar.getTime());
                        String requeteMesuresJournalieresRuche = "SELECT * FROM MesuresJournalieresRuche WHERE DateMesure='" + aujourdhui + "' AND idRuche='" + idRuche + "' ORDER BY HeureMesure ASC";
                        String requeteMesuresJournalieresPoids = "SELECT * FROM MesuresJournalieresPoids WHERE DateMesure='" + aujourdhui + "' AND idRuche='" + idRuche + "' ORDER BY HeureMesure ASC";
                        String requeteMesuresJournalieresEnvironnement = "SELECT * FROM MesuresJournalieresEnvironnement WHERE DateMesure='" + aujourdhui + "' AND idRuche='" + idRuche + "' ORDER BY HeureMesure ASC";
                        String requeteMesuresJournalieresEnsoleillement = "SELECT * FROM MesuresJournalieresEnsoleillement WHERE DateMesure='" + aujourdhui + "' AND idRuche='" + idRuche + "' ORDER BY HeureMesure ASC";
                        Statement statement = bdd.connexion.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY);


                        ResultSet resultSet = statement.executeQuery(requeteMesuresJournalieresRuche);

                        while (resultSet.next()) {
                            Log.d(TAG, "Requete : " + requeteMesuresJournalieresRuche);
                            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                            Date heureMesure = sdf.parse(resultSet.getString("HeureMesure"));
                            calendar.setTime(heureMesure);
                            int heure = calendar.get(Calendar.HOUR_OF_DAY);

                            Log.v(TAG, "BDD -> Temperature " + resultSet.getDouble("Temperature"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesTemperaturesInt.appendData(new DataPoint(heure, resultSet.getDouble("Temperature")), true, 24);
                            temp_int_Basse = resultSet.getDouble("TemperatureMin");
                            temp_int_Haute = resultSet.getDouble("TemperatureMax");

                            Log.v(TAG, "BDD -> Humidite " + resultSet.getDouble("Humidite"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesHumiditeInt.appendData(new DataPoint(heure, resultSet.getDouble("Humidite")), true, 24);
                            hum_int_Basse = resultSet.getDouble("HumiditeMin");
                            hum_int_Haute = resultSet.getDouble("HumiditeMax");
                        }


                        ResultSet resultSet2 = statement.executeQuery(requeteMesuresJournalieresPoids);

                        while (resultSet2.next()) {
                            Log.d(TAG, "Requete : " + requeteMesuresJournalieresPoids);
                            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                            Date heureMesure = sdf.parse(resultSet2.getString("HeureMesure"));
                            calendar.setTime(heureMesure);
                            int heure = calendar.get(Calendar.HOUR_OF_DAY);

                            Log.v(TAG, "BDD -> Poids " + resultSet2.getDouble("Poids"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet2.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesPoids.appendData(new DataPoint(heure, resultSet2.getDouble("Poids")), true, 24);
                            poids_Basse= resultSet2.getDouble("PoidsMin");
                            poids_Haute = resultSet2.getDouble("PoidsMax");
                        }


                        ResultSet resultSet3 = statement.executeQuery(requeteMesuresJournalieresEnvironnement);

                        while (resultSet3.next()) {
                            Log.d(TAG, "Requete : " + requeteMesuresJournalieresEnvironnement);
                            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                            Date heureMesure = sdf.parse(resultSet3.getString("HeureMesure"));
                            calendar.setTime(heureMesure);
                            int heure = calendar.get(Calendar.HOUR_OF_DAY);

                            Log.v(TAG, "BDD -> Temperature " + resultSet3.getDouble("Temperature"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet3.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesTemperaturesExt.appendData(new DataPoint(heure, resultSet3.getDouble("Temperature")), true, 24);
                            temp_ext_Basse = resultSet3.getDouble("TemperatureMin");
                            temp_ext_Haute = resultSet3.getDouble("TemperatureMax");

                            Log.v(TAG, "BDD -> Humidite " + resultSet3.getDouble("Humidite"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet3.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesHumiditeExt.appendData(new DataPoint(heure, resultSet3.getDouble("Humidite")), true, 24);
                            hum_ext_Basse = resultSet3.getDouble("HumiditeMin");
                            hum_ext_Haute = resultSet3.getDouble("HumiditeMax");

                            Log.v(TAG, "BDD -> Pression " + resultSet3.getDouble("Pression"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet3.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesPression.appendData(new DataPoint(heure, resultSet3.getDouble("Pression")), true, 24);
                            pression_Basse = resultSet3.getDouble("PressionMin");
                            pression_Haute = resultSet3.getDouble("PressionMax");
                        }


                        ResultSet resultSet4 = statement.executeQuery(requeteMesuresJournalieresEnsoleillement);

                        while (resultSet4.next()) {
                            Log.d(TAG, "Requete : " + requeteMesuresJournalieresEnsoleillement);
                            SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
                            Date heureMesure = sdf.parse(resultSet4.getString("HeureMesure"));
                            calendar.setTime(heureMesure);
                            int heure = calendar.get(Calendar.HOUR_OF_DAY);

                            Log.v(TAG, "BDD -> Ensoleillement " + resultSet4.getDouble("Ensoleillement"));
                            Log.v(TAG, "BDD -> HeureMesure : " + resultSet4.getString("HeureMesure"));
                            Log.v(TAG, "BDD -> Heure : " + heure);
                            mSeriesEnsoleillement.appendData(new DataPoint(heure, resultSet4.getDouble("Ensoleillement")), true, 24);
                            ensoleillement_Basse = resultSet4.getDouble("EnsoleillementMin");
                            ensoleillement_Haute = resultSet4.getDouble("EnsoleillementMax");
                        }


                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_MESURES_RUCHES;
                            if (handler.sendMessage(msg)) {
                                Log.v(TAG, "BDD -> recuperer : message envoyé");
                            } else {
                                Log.e(TAG, "BDD -> recuperer : message non envoyé !");
                            }
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "BDD -> exception : " + e.toString());
                        if (handler != null) {
                            Message msg = Message.obtain();
                            msg.what = HoneyBee.REQUETE_SQL_ERREUR;
                            handler.sendMessage(msg);
                        }
                    } finally {
                        bdd.mutex.unlock();
                    }
                }
            });

            // Démarrage
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public LineGraphSeries<DataPoint> getmSeriesTemperaturesInt() {
        Log.d("Graph Données", valueOf(mSeriesTemperaturesInt));
        return mSeriesTemperaturesInt;
    }

    public LineGraphSeries<DataPoint> getmSeriesTemperaturesExt() {
        return mSeriesTemperaturesExt;
    }

    public LineGraphSeries<DataPoint> getmSeriesHumiditeInt() {
        return mSeriesHumiditeInt;
    }

    public LineGraphSeries<DataPoint> getmSeriesHumiditeExt() {
        return mSeriesHumiditeExt;
    }

    public LineGraphSeries<DataPoint> getmSeriesEnsoleillement() {
        return mSeriesEnsoleillement;
    }

    public LineGraphSeries<DataPoint> getmSeriesPoids() {
        return mSeriesPoids;
    }

    public LineGraphSeries<DataPoint> getmSeriesPression() {
        return mSeriesPression;
    }

    public Double getEnsoleillement_Basse() {
        return ensoleillement_Basse;
    }

    public Double getEnsoleillement_Haute() {
        return ensoleillement_Haute;
    }

    public Double getEnsoleillement_Moyenne() {
        return ensoleillement_Moyenne;
    }

    public Double getHum_ext_Basse() {
        return hum_ext_Basse;
    }

    public Double getHum_ext_Haute() {
        return hum_ext_Haute;
    }

    public Double getHum_ext_Moyenne() {
        return hum_ext_Moyenne;
    }

    public Double getHum_int_Basse() {
        return hum_int_Basse;
    }

    public Double getHum_int_Haute() {
        return hum_int_Haute;
    }

    public Double getHum_int_Moyenne() {
        return hum_int_Moyenne;
    }

    public Double getPoids_Basse() {
        return poids_Basse;
    }

    public Double getPoids_Haute() {
        return poids_Haute;
    }

    public Double getPoids_Moyenne() {
        return poids_Moyenne;
    }

    public Double getPression_Basse() {
        return pression_Basse;
    }

    public Double getPression_Haute() {
        return pression_Haute;
    }

    public Double getPression_Moyenne() {
        return pression_Moyenne;
    }

    public Double getTemp_ext_Basse() {
        return temp_ext_Basse;
    }

    public Double getTemp_ext_Haute() {
        return temp_ext_Haute;
    }

    public Double getTemp_ext_Moyenne() {
        return temp_ext_Moyenne;
    }

    public Double getTemp_int_Basse() {
        return temp_int_Basse;
    }

    public Double getTemp_int_Haute() {
        return temp_int_Haute;
    }

    public Double getTemp_int_Moyenne() {
        return temp_int_Moyenne;
    }

    public String getHistoriqueAlertes() {
        return historiqueAlertes;
    }

    public String getAdresse() {
        return adresse;
    }

    public double getCharge_min() {
        return charge_min;
    }

    public double getEnsoleillement_max() {
        return ensoleillement_max;
    }

    public double getEnsoleillement_min() {
        return ensoleillement_min;
    }

    public double getHum_ext_max() {
        return hum_ext_max;
    }

    public double getHum_ext_min() {
        return hum_ext_min;
    }

    public double getHum_int_max() {
        return hum_int_max;
    }

    public double getHum_int_min() {
        return hum_int_min;
    }

    public double getPoids_max() {
        return poids_max;
    }

    public double getPoids_min() {
        return poids_min;
    }

    public double getPression_max() {
        return pression_max;
    }

    public double getPression_min() {
        return pression_min;
    }

    public double getTemp_ext_max() {
        return temp_ext_max;
    }

    public double getTemp_ext_min() {
        return temp_ext_min;
    }

    public double getTemp_int_max() {
        return temp_int_max;
    }

    public double getTemp_int_min() {
        return temp_int_min;
    }
}