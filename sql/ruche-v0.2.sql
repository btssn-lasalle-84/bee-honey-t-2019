--
-- Base de données: `ruche`
--

DROP DATABASE IF EXISTS `ruche`;

CREATE DATABASE IF NOT EXISTS `ruche`;

USE `ruche`;

-- --------------------------------------------------------

--
-- Structure de la table `Apiculteur`
--

CREATE TABLE IF NOT EXISTS `Apiculteur` (
  `idApiculteur` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  `Prenom` varchar(64) NOT NULL,
  `Identifiant` varchar(255) DEFAULT NULL,
  `MotDePasse` varchar(255) DEFAULT NULL,
  `Email` varchar(64) NOT NULL,  
  PRIMARY KEY (`idApiculteur`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Apiculteur` (`Nom`,`Prenom`,`Identifiant`,`MotDePasse`,`Email`) VALUES('Vaira','Thierry','tvaira','5f4dcc3b5aa765d61d8327deb882cf99','tvaira@free.fr');
INSERT INTO `Apiculteur` (`Nom`,`Prenom`,`Identifiant`,`MotDePasse`,`Email`) VALUES('Beaumont','Jerome','jbeaumont','','beaumont@lasalle84.org');

-- --------------------------------------------------------

--
-- Structure de la table `Ruche`
--

CREATE TABLE IF NOT EXISTS `Ruche` (
  `idRuche` int(11) NOT NULL AUTO_INCREMENT,
  `idApiculteur` int(11) NOT NULL,
  `idTTN` int(11) NOT NULL,
  `Nom` varchar(255) NOT NULL,
  `Description` varchar(255) DEFAULT NULL,
  `DateMiseEnService` date NOT NULL,
  `Adresse` varchar(17) DEFAULT NULL,
  `Longitude` varchar(15) DEFAULT NULL,
  `Latitude` varchar(15) DEFAULT NULL,
  `DeviceID` varchar(64) NOT NULL,
  PRIMARY KEY (`idRuche`),
  CONSTRAINT Ruche_fk_1 FOREIGN KEY (`idApiculteur`) REFERENCES Apiculteur(`idApiculteur`) ON DELETE CASCADE,
  CONSTRAINT Ruche_fk_2 FOREIGN KEY (`DeviceID`) REFERENCES RucheTTN(`DeviceID`) ON DELETE CASCADE    
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- --------------------------------------------------------
-- TODO renommer ce DeviceID !
-- --------------------------------------------------------

INSERT INTO `Ruche` (`idTTN`,`Nom`,`Description`,`DateMiseEnService`,`Adresse`,`Longitude`,`Latitude`,`DeviceID`) VALUES('1','Ruche 1','...','2015-03-29','','-4.5°', '43.9°','ruche_1');


-- --------------------------------------------------------

--
-- Structure de la table `RucheTTN`
--

-- --------------------------------------------------------
-- TODO problème doublon sur idRuche (une "idRuche" ne peut être associé qu'une seule fois et à un seul DeviceID)
-- --------------------------------------------------------

	CREATE TABLE IF NOT EXISTS `RucheTTN` (
	  `DeviceID` varchar(64) NOT NULL,
	  `PortTTNMesuresEnvironnement` int(11) DEFAULT NULL,
	  `PortTTNMesuresRuche` int(11) DEFAULT NULL,
	  `PortTTNMesuresPoids` int(11) DEFAULT NULL,
	  `PortTTNMesuresEnergie` int(11) DEFAULT NULL,
	  PRIMARY KEY (`DeviceID`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;


	-- ----------------------------œ----------------------------
	-- TODO renommer ce DeviceID !
	-- --------------------------------------------------------

	INSERT INTO `Ruche` (`DeviceID`,`PortTTNMesuresEnvironnement`,`PortTTNMesuresRuche`,`PortTTNMesuresPoids`,`PortTTNMesuresEnergie`) VALUES('capteur_temperature','4', '3', '2', '1');


	-- --------------------------------------------------------

	--
	-- Structure de la table `TTN`
	--

	CREATE TABLE IF NOT EXISTS `TTN` (
	  `idTTN` int(11) NOT NULL AUTO_INCREMENT,
	  `idApiculteur` int(11) NOT NULL,
	  `Hostname` varchar(64) NOT NULL,
	  `Port` int(11) NOT NULL,
	  `Username` varchar(64) NOT NULL,
	  `Password` varchar(255) DEFAULT NULL,
	  `ApplicationID` varchar(64) NOT NULL,
	  `Identifiant` varchar(255) DEFAULT NULL,
	  `MotDePasse` varchar(255) DEFAULT NULL,
	  `Email` varchar(64) NOT NULL,  
	  PRIMARY KEY (`idTTN`),
	  CONSTRAINT TTN_fk_1 FOREIGN KEY (`idApiculteur`) REFERENCES Apiculteur(`idApiculteur`) ON DELETE CASCADE
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	INSERT INTO `TTN` (`Hostname`,`Port`,`Username`,`Password`,`ApplicationID`,`Identifiant`,`MotDePasse`,`Email`) VALUES('eu.thethings.network',1883,'mes_ruches','ttn-account-v2.vC-aqMRnLLzGkNjODWgy81kLqzxBPAT8_mE-L7U2C_w','mes_ruches','fmellah','ruches123','florentinmellah@gmail.com');


-- --------------------------------------------------------

--
-- Structure de la table `Seuils`
--

CREATE TABLE IF NOT EXISTS `Seuils` (
  `idSeuils` int(11) NOT NULL AUTO_INCREMENT,
  `idRuche` int(11) NOT NULL,
  `TemperatureMin` int(11) NOT NULL,
  `TemperatureMax` int(11) NOT NULL,
  `HumiditeMin` int(11) NOT NULL,
  `HumiditeMax` int(11) NOT NULL,
  `PoidsMin` int(11) NOT NULL,
  `PoidsMax` int(11) NOT NULL,
  PRIMARY KEY (`idSeuils`),
  CONSTRAINT Seuils_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `Seuils` (`idRuche`, `TemperatureMin`, `TemperatureMax`, `HumiditeMin`, `HumiditeMax`, `PoidsMin`, `PoidsMax`) VALUES('1','20','30','20','45','2000','6000');

-- --------------------------------------------------------
-- TODO insérer les numéro de port des capteurs pour MQTT (TTN)
-- --------------------------------------------------------

--
-- Structure de la table `MesuresEnvironnement`
--

CREATE TABLE IF NOT EXISTS `MesuresEnvironnement` (
  `idMesuresEnvironnement` int(11) NOT NULL AUTO_INCREMENT,
  `idRuche` int(11) NOT NULL,
  `Ensoleillement` int(11) NOT NULL,
  `Temperature` int(11) NOT NULL,
  `Humidite` int(11) NOT NULL,
  `Pression` int(11) NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idMesuresEnvironnement`),
  CONSTRAINT MesuresEnvironnement_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','750','26','35','1100','2018-11-14 06:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','755','26','35','1100','2018-11-14 07:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','775','27','38','1101','2018-11-14 08:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','800','27','37','1101','2018-11-14 09:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','800','26','37','1101','2018-11-14 10:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','800','26','35','1100','2018-11-14 11:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','810','25','35','1100','2018-11-14 12:00:00');
INSERT INTO `MesuresEnvironnement` (`idRuche`,`Ensoleillement`,`Temperature`,`Humidite`,`Pression`,`Horodatage`) VALUES('1','810','25','35','1100','2018-11-14 13:00:00');

-- --------------------------------------------------------

--
-- Structure de la table `MesuresRuche`
--

CREATE TABLE IF NOT EXISTS `MesuresRuche` (
  `idMesuresRuche` int(11) NOT NULL AUTO_INCREMENT, 
  `idRuche` int(11) NOT NULL,
  `Temperature` int(11) NOT NULL,
  `Humidite` int(11) NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idMesuresRuche`),
  CONSTRAINT MesuresRuche_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','28','20','2018-11-14 06:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','29','20','2018-11-14 07:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','29','20','2018-11-14 08:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','29','20','2018-11-14 09:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','29','25','2018-11-14 10:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','29','25','2018-11-14 11:00:00');
INSERT INTO `MesuresRuche` (`idRuche`,`Temperature`,`Humidite`,`Poids`,`Horodatage`) VALUES('1','30','25','2018-11-14 12:00:00');

-- --------------------------------------------------------

--
-- Structure de la table `MesuresPoids`
--

CREATE TABLE IF NOT EXISTS `MesuresPoids` (
  `idMesuresPoids` int(11) NOT NULL AUTO_INCREMENT, 
  `idRuche` int(11) NOT NULL,
  `Poids` int(11) NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idMesuresPoids`),
  CONSTRAINT MesuresPoids_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5500','2018-11-14 06:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5510','2018-11-14 07:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5510','2018-11-14 08:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5510','2018-11-14 09:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5520','2018-11-14 10:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5510','2018-11-14 11:00:00');
INSERT INTO `MesuresPoids` (`idRuche`,`Poids`,`Horodatage`) VALUES('1','5510','2018-11-14 12:00:00');

-- --------------------------------------------------------

--
-- Structure de la table `MesuresEnergie`
--

CREATE TABLE IF NOT EXISTS `MesuresEnergie` (
  `idMesuresEnergie` int(11) NOT NULL AUTO_INCREMENT, 
  `idRuche` int(11) NOT NULL,
  `Charge` int(11) NOT NULL,
  `Tension` int(11) NOT NULL,
  `Courant` int(11) NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idMesuresEnergie`),
  CONSTRAINT MesuresEnergie_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `MesuresEnergie` (`idRuche`,`Charge`,`Tension`,`Courant`,`Horodatage`) VALUES('1','90','12','150','2018-11-14 06:00:00');

-- --------------------------------------------------------

--
-- Structure de la table `TypeAlerte`
--

CREATE TABLE IF NOT EXISTS `TypeAlerte` (
  `idType` int(11) NOT NULL AUTO_INCREMENT,
  `Nom` varchar(64) NOT NULL,
  PRIMARY KEY (`idType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

INSERT INTO `TypeAlerte` (`Nom`) VALUES
('Temperature'),
('Humidite'),
('Poids');

-- --------------------------------------------------------

--
-- Structure de la table `Alertes`
--

CREATE TABLE IF NOT EXISTS `Alertes` (
  `idAlerte` int(11) NOT NULL AUTO_INCREMENT,
  `idRuche` int(11) NOT NULL,
  `idType` int(11) NOT NULL,  
  `Description` text CHARACTER SET utf8 NOT NULL,
  `Horodatage` datetime NOT NULL,
  PRIMARY KEY (`idAlerte`),
  CONSTRAINT Alertes_fk_1 FOREIGN KEY (`idRuche`) REFERENCES Ruche(`idRuche`) ON DELETE CASCADE,
  CONSTRAINT Alertes_fk_2 FOREIGN KEY (`idType`) REFERENCES TypeAlerte(`idType`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Contenu de la table `Alertes`
--

INSERT INTO `Alertes` (`idRuche`, `idType`, `Description`, `Horodatage`) VALUES
('1', '1', '...', '2018-10-03 17:00:00');
