#ifndef RUCHEIHM_H
#define RUCHEIHM_H

/**
  *
  * \file rucheIhm/rucheIhm.h
  *
  * \brief Déclaration de la classe RucheIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QtWidgets>
#include <QVector>
#include <qwt_plot_marker.h>
#include "alertes.h"

class Ruche;
class BaseDeDonnees;
class NouvelleRucheIhm;
class ReglagesAlertesIhm;
class QwtPlotCurve;

namespace Ui {
class RucheIhm;
}

/**
  *
  * \class RucheIhm
  *
  * \brief La classe IHM
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 0.1
  *
  */

class RucheIhm : public QWidget
{
    Q_OBJECT

public:
    explicit RucheIhm(QWidget *parent = 0);
    ~RucheIhm();

    void insererMesuresEnsoleillementTableau(QVector<QStringList> mesuresJournalieresEnsoleillement);

private:
    Ui::RucheIhm *ui; //!< agrégation de la partie graphique de l'IHM
    Ruche *maRuche; //!< association de l'objet Ruche
    BaseDeDonnees *bdd; //!< agrégation de l'objet BaseDeDonnees
    NouvelleRucheIhm *nouvelleRuche;  //!< l'ihm pour créer une nouvelle ruche
    ReglagesAlertesIhm *reglagesAlertesIhm;  //!< l'ihm pour régler les seuils d'une ruche
    QVector<QStringList> mesRuches;  //!< tableau des informations sur les ruches
    QwtPlotCurve *courbeTemperatureInterieur;
    QwtPlotCurve *courbeTemperatureExterieur;
    QwtPlotCurve *courbeHumiditeInterieur;
    QwtPlotCurve *courbeHumiditeExterieur;
    QwtPlotCurve *courbePressionAtmospherique;
    QwtPlotCurve *courbeEnsoleillement;

    void initialiserThermometre() const;
    void initialiserWidgets();
    void inititialiserCourbes();
    void reinititialiserCourbes();
    void initialiserTemperatureInterieurCourbe();
    void initialiserTemperatureExterieurCourbe();
    void initialiserHumiditeInterieurCourbe();
    void initialiserHumiditeExterieurCourbe();
    void initialiserPressionAtmospheriqueCourbe();
    void initialiserEnsoleillementCourbe();
    void insererMesuresRucheTableau(QVector<QStringList> mesuresJournalieresRuche);
    void insererMesuresEnvironementTableau(QVector<QStringList> mesuresJournalieresEnvironement);
    void effacerTableau();
    void deconnecterSignaux();

public slots:
    void recevoirEtatClientConnexion(bool etat);
    /**********************************SLOT AFFICHAGE*********************************/
    void afficherTemperatureInterieur(double temperatureInterieur, QString horodatage);
    void afficherTemperatureExterieur(double temperatureExterieur, QString horodatage);
    void afficherHumiditeInterieur(double humiditeInterieur, QString horodatage);
    void afficherHumiditeExterieur(double humiditeExterieur, QString horodatage);
    void afficherPressionAtmospherique(double pressionAtmospherique,QString horodatage);
    void afficherEnsoleillement(double ensoleillement,QString horodatage);
    void afficherBatterie(double charge, QString horodatage);
    void afficherPoids(double poids, QString horodatage);

    void afficherAlertesTemperatureInterieur(SeuilsAlertes typeAlerte);
    void afficherAlertesTemperatureExterieur(SeuilsAlertes typeAlerte);
    void afficherAlertesHumiditeInterieur(SeuilsAlertes typeAlerte);
    void afficherAlertesHumiditeExterieur(SeuilsAlertes typeAlerte);
    void afficherAlertesPressionAtmospherique(SeuilsAlertes typeAlerte);
    void afficherAlertesPoids(SeuilsAlertes typeAlerte);
    void afficherAlertesEnsoleillement(SeuilsAlertes typeAlerte, double mesure);
    void afficherAlertesBatterie(SeuilsAlertes typeAlerte, double mesure);
    /**********************************SLOT Courbes*********************************/
    void afficherMesuresJournalieresRuche();
    void afficherMesuresJournalieresEnvironement();
    void afficherMesuresJournalieresEnsoleillement();
    /**********************************SLOT TABLEAU********************************/
    void afficherMesuresJournalieresSelectionnee();

    void afficherLocalisationPasserelle(QString longitude, QString latitude);

    void quitter();

signals:
    void nouvelleDonnesRuche(QString nom, QString prenom, QString email, QString nomTopic);

private slots:
    void selectionnerRuche(int numeroRuche);
    void creerNouvelleRuche();
    void ouvrirReglagesAlertes();
    void mettreAjourListeRuches();
    void supprimerRuche();
};

#endif // RUCHEIHM_H
