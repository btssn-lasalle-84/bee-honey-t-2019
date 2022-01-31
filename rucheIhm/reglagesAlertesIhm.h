#ifndef REGLAGESALERTESIHM_H
#define REGLAGESALERTESIHM_H

#include <QDialog>
#include "alertes.h"

/**
  *
  * \file rucheIhm/reglagesAlertesIhm.h
  *
  * \brief La classe ReglagesAlertesIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

class BaseDeDonnees;

namespace Ui {
class ReglagesAlertesIhm;
class Alertes;
}

/**
  *
  * \class ReglagesAlertesIhm
  *
  * \brief Déclaration de la classe ReglagesAlertesIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
class ReglagesAlertesIhm : public QDialog
{
    Q_OBJECT

public:
    explicit ReglagesAlertesIhm(QWidget *parent = 0);
    ~ReglagesAlertesIhm();

    void setAlertes(Alertes *alertes);

protected:
      void showEvent(QShowEvent *ev);

private:
    Ui::ReglagesAlertesIhm *ui;
    Alertes *alertes;
    BaseDeDonnees *bdd; //!< agrégation de l'objet BaseDeDonnees

public slots :
    void recevoirReglagesAlertes();

signals :
    void envoiSeuilTemperatureInterieurMin(QString seuil);
    void envoiSeuilTemperatureInterieurMax(QString seuil);
    void envoiSeuilHumiditeInterieurMin(QString seuil);
    void envoiSeuilHumiditeInterieurMax(QString seuil);
    void envoiSeuilTemperatureExterieurMin(QString seuil);
    void envoiSeuilTemperatureExterieurMax(QString seuil);
    void envoiSeuilHumiditeExterieurMin(QString seuil);
    void envoiSeuilHumiditeExterieurMax(QString seuil);
    void envoiSeuilEnsoleillementMin(QString seuil);
    void envoiSeuilPressionAtmospheriqueMin(QString seuil);
    void envoiSeuilPoidsMin(QString seuil);
    void envoiSeuilPoidsMax(QString seuil);
};

#endif // REGLAGESALERTESIHM_H
