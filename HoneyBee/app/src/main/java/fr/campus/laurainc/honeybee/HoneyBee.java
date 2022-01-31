package fr.campus.laurainc.honeybee;

/**
 * @class HoneyBee
 * @brief Paramètres globaux de l'application
 * @author Thierry Vaira
 */
public class HoneyBee
{
    // ID Intent
    public final static int ID_Intent_ParametresHoneyBee = 1; //!< l'ID de l'Intent ParametresHoneyBeeActivity
    // Base de données MySQL
    public final static boolean BDD = true; //!< si vrai l'application peut utiliser la base de données MySQL (utile en debug)
    public final static int REQUETE_SQL_ERREUR = -1;
    public final static int REQUETE_SQL_OK = 0;
    public final static int REQUETE_SQL_LISTE_RUCHES = 1;
    public final static int REQUETE_SQL_RUCHE = 2;
    public final static int REQUETE_SQL_IDTTN =3;
    public final static int REQUETE_SQL_MESURES_RUCHES = 4;
    public final static int REQUETE_SQL_ALERTES = 5;
    public final static int REQUETE_SQL_SEUILS =6;
}
