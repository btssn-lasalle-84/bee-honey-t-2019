#ifndef NOUVELLERUCHEIHM_H
#define NOUVELLERUCHEIHM_H

/**
  *
  * \file rucheIhm/nouvelleRucheIhm.cpp
  *
  * \brief La classe NouvelleRucheIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */

#include <QDialog>

class BaseDeDonnees;

namespace Ui {
class NouvelleRucheIhm;
}

/**
  *
  * \class NouvelleRucheIhm
  *
  * \brief Déclaration de la classe NouvelleRucheIhm
  *
  * \author Florentin Mellah, Enzo Rossi
  *
  * \version 1.1
  *
  */
class NouvelleRucheIhm : public QDialog
{
    Q_OBJECT

public:
    explicit NouvelleRucheIhm(QWidget *parent = 0);
    ~NouvelleRucheIhm();

private:
    Ui::NouvelleRucheIhm *ui;
    BaseDeDonnees *bdd; //!< agrégation de l'objet BaseDeDonnees
    QVector<QStringList> listesTTN;
    void nettoyerZoneDeTexte();

private slots:
    void recevoirDonneeAjoutRuche();
    void annulerDonneesNouvelleRuche();

signals:
};

#endif // NOUVELLERUCHEIHM_H
