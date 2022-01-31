package fr.campus.laurainc.honeybee;

import android.app.Activity;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.ProgressBar;

import java.io.Serializable;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @class BaseDeDonnees
 * @brief Gestion d'une base de données MySQL (Singleton)
 * @author Thierry Vaira
 */
public class BaseDeDonnees implements Serializable
{
    private static final String TAG = "BaseDeDonnees"; //!< le TAG de la classe pour les logs
    public static Connection connexion = null; //!< objet de connexion à MySQL (null par défaut)
    public static final Lock mutex = new ReentrantLock(true); //!< mutex pour l'exécution concurrente de requêtes
    private Activity activite = null; //!< objet sur l'activite ayant créé l'objet BaseDeDonnees (si besoin pour UI)
    // Paramètres de connexion à la base de données
    private String url; //!< l'URL pointant sur la base de données d'un serveur MySQL
    private String username; //!< le nom du compte utilisateur (root par défaut)
    private String password; //!< le mot de passe du compte utilisateur (password par défaut)
    private static BaseDeDonnees bdd = null; //!< l'instance unique de BaseDeDonnees (Singleton)

    /**
     * @fn getInstance
     * @brief Retourne l'instance BaseDeDonnees
     * @return BaseDeDonnees l'instance BaseDeDonnees
     */
    public synchronized static BaseDeDonnees getInstance()
    {
        if(bdd == null)
            bdd = new BaseDeDonnees();
        return bdd;
    }

    /**
     * @fn getInstance
     * @brief Retourne l'instance BaseDeDonnees
     * @param activite Activity l'activité appelante
     * @return BaseDeDonnees l'instance BaseDeDonnees
     */
    public synchronized static BaseDeDonnees getInstance(Activity activite)
    {
        if(bdd == null)
            bdd = new BaseDeDonnees(activite);
        return bdd;
    }

    /**
     * @fn getInstance
     * @brief Retourne l'instance BaseDeDonnees
     * @param url String l'URL pointant sur la base de données d'un serveur MySQL
     * @param username String le nom d'utilisateur
     * @param password String le mot de passe de l'utilisateur
     * @return BaseDeDonnees l'instance BaseDeDonnees
     */
    public synchronized static BaseDeDonnees getInstance(String url, String username, String password)
    {
        if(bdd == null)
            bdd = new BaseDeDonnees(url, username, password);
        return bdd;
    }

    /**
     * @fn getInstance
     * @brief Retourne l'instance BaseDeDonnees
     * @param activite Activity l'activité appelante
     * @param url String l'URL pointant sur la base de données d'un serveur MySQL
     * @param username String le nom d'utilisateur
     * @param password String le mot de passe de l'utilisateur
     * @return BaseDeDonnees l'instance BaseDeDonnees
     */
    public synchronized static BaseDeDonnees getInstance(Activity activite, String url, String username, String password)
    {
        if(bdd == null)
            bdd = new BaseDeDonnees(activite, url, username, password);
        return bdd;
    }

    /**
     * @fn readResolve
     * @brief Retourne l'objet dans un flux (cf. Singleton)
     * @return BaseDeDonnees l'instance BaseDeDonnees
     */
    public Object readResolve()
    {
        return BaseDeDonnees.getInstance();
    }

    /**
     * @fn BaseDeDonnees
     * @brief Constructeur par défaut de la classe BaseDeDonnees
     */
    private BaseDeDonnees()
    {
    }

    /**
     * @fn BaseDeDonnees
     * @brief Constructeur de la classe BaseDeDonnees
     * @param activite Activity l'activité appelante
     */
    private BaseDeDonnees(Activity activite)
    {
        this.activite = activite;
    }

    /**
     * @fn BaseDeDonnees
     * @brief Constructeur de la classe BaseDeDonnees
     * @param url String l'URL pointant sur la base de données d'un serveur MySQL
     * @param username String le nom d'utilisateur
     * @param password String le mot de passe de l'utilisateur
     */
    private BaseDeDonnees(String url, String username, String password)
    {
        Log.v(TAG, "url=" + url);
        Log.v(TAG, "username=" + username);
        Log.v(TAG, "password=" + password);
        this.url = url;
        this.username = username;
        this.password = password;
        // on peut se connecter
        connecter();
    }

    /**
     * @fn BaseDeDonnees
     * @brief Constructeur de la classe BaseDeDonnees
     * @param activite Activity l'activité appelante
     * @param url String l'URL pointant sur la base de données d'un serveur MySQL
     * @param username String le nom d'utilisateur
     * @param password String le mot de passe de l'utilisateur
     */
    private BaseDeDonnees(Activity activite, String url, String username, String password)
    {
        Log.v(TAG, "url=" + url);
        Log.v(TAG, "username=" + username);
        Log.v(TAG, "password=" + password);
        this.activite = activite;
        this.url = url;
        this.username = username;
        this.password = password;
        // on peut se connecter
        connecter();
    }

    /**
     * @fn setParametres
     * @brief Modifie les paramètres de connexion à la base de donnees et se reconnecte
     * @param url String l'URL pointant sur la base de données d'un serveur MySQL
     * @param username String le nom d'utilisateur
     * @param password String le mot de passe de l'utilisateur
     */
    public void setParametres(String url, String username, String password)
    {
        this.url = url;
        this.username = username;
        this.password = password;
        // reconnexion
        //reconnecter();
    }

    /**
     * @fn setActivite
     * @brief Fixe l'activité appelante associée à l'instance de Base de données
     * @param activite Activity l'activité appelante
     */
    public void setActivite(Activity activite)
    {
        this.activite = activite;
    }

    /**
     * @fn connecter
     * @brief Connexion à la base de données MySQL si pas déjà connecté
     * @return boolean vrai si la connexion a réussi sinon faux
     */
    public boolean connecter()
    {
        if(!HoneyBee.BDD)
            return false;

        try
        {
            if (!estConnecte())
            {
                ConnexionMySql connexionMySql = new ConnexionMySql();
                connexionMySql.execute();
                Log.d(TAG, "connecter -> retour : " + connexionMySql.get());
                return connexionMySql.get();
            }
            return true;
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
            Log.e(TAG, "connecter -> InterruptedException : " + e.toString());
            return false;
        }
        catch (ExecutionException e)
        {
            e.printStackTrace();
            Log.e(TAG, "connecter -> ExecutionException : " + e.toString());
            return false;
        }
    }

    /**
     * @fn reconnecter
     * @brief Reconnexion à la base de données MySQL
     * @return boolean vrai si la connexion a réussi sinon faux
     */
    public boolean reconnecter()
    {
        if(!HoneyBee.BDD)
            return false;
        deconnecter();
        return connecter();
    }

    /**
     * @fn deconnecter
     * @brief Déconnexion de la base de données MySQL
     * @return boolean vrai si la déconnexion a réussi sinon faux
     */
    public boolean deconnecter()
    {
        if (!HoneyBee.BDD)
            return false;

        try
        {
            if (connexion != null)
            {
                connexion.close();
                Log.d(TAG, "deconnecter -> retour : " + connexion.isClosed());
                return connexion.isClosed();
            }
            return true;
        }
        catch (SQLException e)
        {
            e.printStackTrace();
            Log.e(TAG, "deconnecter -> ExecutionException : " + e.toString());
            return false;
        }
    }

    /**
     * @fn estConnecte
     * @brief Retourne vrai si on est connecté à la base de données MySQL
     * @return boolean vrai si on est connecté à la base de données MySQL sinon faux
     */
    public boolean estConnecte()
    {
        if(!HoneyBee.BDD)
            return false;

        try
        {
            if(connexion != null)
                return !connexion.isClosed();
            else
                return false;
        }
        catch (SQLException e)
        {
            e.printStackTrace();
            return false;
        }
    }

    /**
     * @class ConnexionMySql
     * @brief Classe permettant de se connecter à MySQL en arrière-plan
     */
    private class ConnexionMySql extends AsyncTask<Void, Integer, Boolean>
    {
        private String messageConnexion = "";

        /**
         * @fn doInBackground
         * @brief Méthode de connexion qui s’exécute dans un autre thread
         */
        @Override
        protected Boolean doInBackground(Void... unused)
        {
            try
            {
                // chargement du pilote JDBC MySQL
                publishProgress(1);
                Class.forName("com.mysql.jdbc.Driver").newInstance();
            }
            catch (Exception e)
            {
                e.printStackTrace();
                Log.e(TAG, "doInBackground -> exception : " + e.toString());
                messageConnexion = "Erreur connexion MySQL !";
            }
            try
            {
                publishProgress(50);
                if (!estConnecte())
                {
                    connexion = DriverManager.getConnection(url, username, password);
                }
                publishProgress(75);
                Log.d(TAG, "doInBackground -> connecte : " + !connexion.isClosed());
                if(!connexion.isClosed())
                    messageConnexion = "Connexion MySQL réussie !";
                publishProgress(100);
                return !connexion.isClosed();
            }
            catch (SQLException e)
            {
                e.printStackTrace();
                Log.e(TAG, "doInBackground -> SQLException : " + e.getMessage());
                Log.e(TAG, "doInBackground -> SQLState : " + e.getSQLState());
                Log.e(TAG, "doInBackground -> VendorError : " + e.getErrorCode());
                messageConnexion = "Erreur connexion MySQL !";
                publishProgress(100);
                return false;
            }
        }

        /**
         * @fn onProgressUpdate
         * @brief Méthode permettant d'indiquer la progression de la tâche d'arrière plan (s’exécute dans le thread UI)
         * @details doInBackground peut appeler publishProgress() pour indiquer l'avancement du traitement ce qui aura pour effet d'appeler automatiquement onProgressUpdate()
         */
        @Override
        protected void onProgressUpdate(Integer... progress)
        {
            Log.v(TAG, "onProgressUpdate -> progression : " + progress[0]);
            if(activite != null)
            {
                // Ici on peut accéder à la partie UI d'une activité
                // et/ou appeler une méthode d'une activité
                //((MainActivity)activite).afficherProgression(progress[0]);
            }
        }

        /**
         * @fn onPreExecute
         * @brief Méthode appelée lorsque doInBackground a terminé (s’exécute dans le thread UI)
         */
        @Override
        protected void onPostExecute(Boolean result)
        {
            Log.d(TAG, "onPostExecute -> result : " + result);
            Log.d(TAG, "onPostExecute -> message : " + messageConnexion);
            if(result)
            {
                if(activite != null)
                {
                    // Ici on peut accéder à la partie UI d'une activité
                    // et/ou appeler une méthode d'une activité
                }
            }
            else
            {
                if(activite != null)
                {
                    // Ici on peut accéder à la partie UI d'une activité
                    // et/ou appeler une méthode d'une activité
                }
            }
        }
    }

    /**
     * @fn executerRequete
     * @brief Méthode qui exécute une requête UPDATE, INSERT ou DELETE en arrière-plan
     */

    public void executerRequete(final String requete)
    {
        if(!HoneyBee.BDD)
            return;

        if(estConnecte())
        {
            // Seulement pour les requêtes UPDATE, INSERT ou DELETE
            if(!requete.contains("UPDATE") && !requete.contains("INSERT") && !requete.contains("DELETE"))
                return;

            // Exécute la requête dans un thread
            Thread requeteBDD = new Thread(new Runnable()
            {
                public void run()
                {
                    mutex.lock();
                    try
                    {
                        Log.d(TAG, "executerRequete -> requete : " + requete);
                        Statement statement = connexion.createStatement();
                        statement.executeUpdate(requete);
                    }
                    catch (Exception e)
                    {
                        e.printStackTrace();
                        Log.e(TAG, "executerRequete -> exception : " + e.toString());
                    }
                    finally
                    {
                        mutex.unlock();
                    }
                }
            });

            // Démarrage de l'exécution de la requête
            Log.v("BDD", "Requete OK = " + requete);
            requeteBDD.start();
        }
        else
        {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }

    public void supprimerRuche(final int idRuche) {
        if (!HoneyBee.BDD)
            return;

        final String requete = "DELETE FROM Ruche WHERE idRuche='" + idRuche + "';";
        if (estConnecte()) {
            Thread requeteBDD = new Thread(new Runnable() {
                public void run() {

                    mutex.lock();
                    try {
                        Log.d(TAG, "executerRequete -> requete : " + requete);
                        Statement statement = connexion.createStatement();
                        statement.executeUpdate(requete);
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "executerRequete -> exception : " + e.toString());
                    } finally {
                        mutex.unlock();
                    }
                }
            });

            Log.v("BDD", "Requete OK = " + requete);
            requeteBDD.start();
        } else {
            Log.w(TAG, "Pas de connexion MySQL !");
        }
    }}