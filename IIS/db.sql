
SET NAMES utf8;
SET foreign_key_checks = 0;
SET time_zone = 'SYSTEM';
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';
DROP TABLE IF EXISTS `zamestnanec`;
DROP TABLE IF EXISTS `pobocka`;
DROP TABLE IF EXISTS `typuctu`;
DROP TABLE IF EXISTS `karta`;
DROP TABLE IF EXISTS `ucet`;
DROP TABLE IF EXISTS `operace`;
DROP TABLE IF EXISTS `klient`;
DROP TABLE IF EXISTS `klientDisponuje`;
DROP TABLE IF EXISTS `disponent`;
DROP TABLE IF EXISTS `admin`;


CREATE TABLE `typuctu`(`id_Typ` int(5) NOT NULL,
                      `Sluzby` varchar(60) NOT NULL,
                      PRIMARY KEY(`id_Typ`))
                      ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `pobocka`(`id_Pobocka` int(5) NOT NULL,
                     `Adresa` varchar(60) NOT NULL,
                      PRIMARY KEY(`id_Pobocka`))
                      ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `klient`(`MenoKlienta` varchar(30) NOT NULL,
                    `PriezviskoKlienta` varchar(40) NOT NULL,
                    `AdresaKlienta` varchar(60) NOT NULL,
                    `Telefon` varchar(60) NOT NULL,
                    `Email` varchar(80) NOT NULL,
                    `PrihlasovacieMeno` varchar(80) NOT NULL,
                    `PrihlasovacieHeslo` varchar(80) NOT NULL,
                     PRIMARY KEY(`PrihlasovacieMeno`)
                     )ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `disponent`(`MenoDisponenta` varchar(30),
                    `PriezviskoDisponenta` varchar(40),
                    `AdresaDisponenta` varchar(60),
                    `Telefon` varchar(60),
                    `Email` varchar(80),
                    `PrihlasovacieMeno` varchar(80) NOT NULL, #STACI MENO NOT NULL OSTATNE SA DOHLADA V KLIENTOVI
                    `PrihlasovacieHeslo` varchar(80),
                     PRIMARY KEY(`PrihlasovacieMeno`)                       
                     )ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `admin`(`id_Admin` int(5) NOT NULL,
                          `PrihlasovacieMeno` varchar(80) NOT NULL,
                          `PrihlasovacieHeslo` varchar(80) NOT NULL,
                           PRIMARY KEY (`id_Admin`)
              )ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `zamestnanec`(`id_Zamestnanec` int(5) NOT NULL,
                           `PriezviskoZamestnanca` varchar(30) NOT NULL,
                          `MenoZamestnanca` varchar(30) NOT NULL,
                          `AdresaZamestnanca` varchar(50) NOT NULL,
                          `id_Pob` int(5) NOT NULL,
                          `PrihlasovacieMeno` varchar(80) NOT NULL,
                          `PrihlasovacieHeslo` varchar(80) NOT NULL,
                           PRIMARY KEY (`id_Zamestnanec`),
                           KEY `id_Pob`(`id_Pob`),
                           CONSTRAINT `id_Zamesnanec_fk` FOREIGN KEY (`id_Pob`) REFERENCES `pobocka` (`id_Pobocka`) ON DELETE CASCADE ON UPDATE CASCADE
              )ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
CREATE TABLE `ucet`(`id_cislouctu` int(50) NOT NULL,
                  `IBAN` varchar(26) NOT NULL UNIQUE,
                  `AktivovaneSluzby` varchar(60) NOT NULL,
                  `id_type` int(5) NOT NULL,
                  `id_Klient` varchar(80) NOT NULL,
                  `id_Disponent` varchar(80),
                  `uctovny_zostatok` float(50) NOT NULL,
                  `Mena` varchar(20) NOT NULL,
                   PRIMARY KEY(`id_cislouctu`),
                   KEY `id_Klient` (`id_Klient`),
                   KEY `id_Disponent` (`id_Disponent`),
                   CONSTRAINT `id_cislouctu_fk` FOREIGN KEY (`id_Klient`) REFERENCES `klient` (`PrihlasovacieMeno`) ON DELETE CASCADE ON UPDATE CASCADE,
                   CONSTRAINT `id_cislouctu_fk1` FOREIGN KEY (`id_Disponent`) REFERENCES `disponent` (`PrihlasovacieMeno`) ON DELETE CASCADE ON UPDATE CASCADE)
                   ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;    
CREATE TABLE `operace`(`id_Operace` int(5) NOT NULL AUTO_INCREMENT,
                     `id_Klient` varchar(80) NOT NULL,
                     `id_ucet` int(50) NOT NULL,
                      `DatumVykonania` DATE NOT NULL,
                      `MenoVykonavatela` varchar(60) NOT NULL,
                      `PrevadzanaCiastka` int(50),
                      `PrevadzanaMena` varchar(20),
                      `CisloUctuPrijemcu` int(50),
                      `VyberovaCiastka` int(50),
                      `MiestoVyberu` varchar(50),
                      `PoplatokZaVyber` int(50),
                      `VyberovaMena` VARCHAR(30),
                      `VkladanaCiastka` int(50) ,
                      `MiestoVkladu` varchar(60),
                      `VkladanaMena` varchar(20),
                       PRIMARY KEY(`id_Operace`),
                       KEY `id_Klient` (`id_Klient`),
                       KEY `id_ucet` (`id_ucet`),
                       CONSTRAINT `id_Operace_fk` FOREIGN KEY (`id_Klient`) REFERENCES `klient` (`PrihlasovacieMeno`) ON DELETE CASCADE ON UPDATE CASCADE,
                       CONSTRAINT `id_Operace_fk1` FOREIGN KEY (`id_ucet`) REFERENCES `ucet` (`id_cislouctu`) ON DELETE CASCADE ON UPDATE CASCADE
                      )ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;

CREATE TABLE `karta`(`id_CisloKarty` int(50) NOT NULL,
                   `id_Klient` varchar(80) NOT NULL,
                   `id_ucet` int(50) NOT NULL,
                    `VerifikacnyKod` int(5) NOT NULL,
                    `DobaPlatnosti` DATE NOT NULL,
                    `DrzitelKarty` varchar(60) NOT NULL,
                     PRIMARY KEY(`id_CisloKarty`),
                     KEY `id_Klient` (`id_Klient`),
                     KEY `id_ucet` (`id_ucet`),
                     CONSTRAINT `id_CisloKarty_fk1` FOREIGN KEY (`id_Klient`) REFERENCES `klient` (`PrihlasovacieMeno`) ON DELETE CASCADE ON UPDATE CASCADE,
                     CONSTRAINT `id_CisloKarty_fk2` FOREIGN KEY (`id_ucet`) REFERENCES `ucet` (`id_cislouctu`) ON DELETE CASCADE ON UPDATE CASCADE)
                     ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;
INSERT INTO `pobocka`(`id_Pobocka`, `Adresa`) VALUES 
(001,'Brno'),
(002,'Olomouc'),
(003,'Praha')ON DUPLICATE KEY UPDATE `id_Pobocka` = VALUES(`id_Pobocka`), `Adresa` = VALUES(`Adresa`);                     
INSERT INTO `zamestnanec` (`id_Zamestnanec`, `PriezviskoZamestnanca`, `MenoZamestnanca`, `AdresaZamestnanca`,`id_Pob`, `PrihlasovacieMeno`, `PrihlasovacieHeslo`) VALUES
( 001,'Veres','Marcel','Brno',001, 'Marcelino', 'pejprino'),
( 002,'Sevcik','Peter','Brno',001, 'KuvA', 'kasino'),
( 003,'Blasko','Peter','Ostrava',003, 'Nepoznam', 'ebekamarat'),
( 004,'Mokos','Andrej','Olomouc',002, 'Andrejko', 'mokosko'),
( 005,'Petroff','Adam','Kyjov',001, 'RockJe', 'thebest'),
( 006,'Novak' ,'Jan','Hradec Kralove',002, 'Spatne', 'priezvisko')ON DUPLICATE KEY UPDATE `id_Zamestnanec` = VALUES(`id_Zamestnanec`), `PriezviskoZamestnanca` = VALUES(`PriezviskoZamestnanca`), `MenoZamestnanca` = VALUES(`MenoZamestnanca`), `AdresaZamestnanca` = VALUES(`AdresaZamestnanca`), `id_Pob` = VALUES(`id_Pob`);      INSERT INTO `typuctu` (`id_Typ`, `Sluzby`) VALUES(001,'Povolene precerpanie,BezplatnyVyber,StudentskyUcet');
INSERT INTO `typuctu` (`id_Typ`, `Sluzby`) VALUES
(002,'Povolene precerpanie,SporiaciUrok'),
(003,'Povolene precerpanie')ON DUPLICATE KEY UPDATE `id_Typ` = VALUES(`id_Typ`), `Sluzby` = VALUES(`Sluzby`);;
INSERT INTO `klient` (`MenoKlienta`, `PriezviskoKlienta`, `AdresaKlienta`, `Telefon`, `Email`, `PrihlasovacieMeno`, `PrihlasovacieHeslo` ) VALUES
('Martin','Nizner','Hlohovecka 395/51 Sokolovce 92231','+421915565026','niznermartin96@gmail.com', 'gruf', 'blafo'),
('Jan','Gula','Skrivanova 327 Brno 62800','+421955423421','jangula@gmail.com', 'Jamal', '159753'),
('Robert','Misura','Piestanska 451/87 Povazany 91626','+421940453781','misurarobert77@gmail.com', 'kvitko','jehoheslo'),
('Novak','Michal',' 215 Praha 14000','+420925544007','mnovak45@gmail.com', 'spatnemeno','nevadimisko'),
('Hurtik','Lukas','A.Trajana 25 Piestany 92101','+421907541782','hurtiklukas@gmail.com', 'hulk', 'hahatourcite'),
('Bednar','Pavel','Druzebni 542/14 Ostrava 72526','+420902413714','pbednar@seznam.cz', 'neznaju', 'takeho')ON DUPLICATE KEY UPDATE `MenoKlienta` = VALUES(`MenoKlienta`), `PriezviskoKlienta` = VALUES(`PriezviskoKlienta`), `AdresaKlienta` = VALUES(`AdresaKlienta`), `Telefon` = VALUES(`Telefon`), `Email` = VALUES(`Email`);      
INSERT INTO `ucet` (`id_CisloUctu`, `IBAN`, `AktivovaneSluzby`,`id_type`,`id_Klient`,`uctovny_zostatok`,`Mena`) VALUES
( 77889653,'CZ6508000000192000145399' , 'PovolenePrecerpanie',001,'gruf',700,'EUR'),
( 77889785,'CZ3155000000001043006511' , 'StudentskyUcet, BezplatnyVyber',001,'Jamal',3000,'EUR'),
( 65627653,'CZ0201000000199216760237' , 'PovolenePrecerpanie, SporiaciUrok',002,'kvitko',2,'EUR'),
( 74129921,'CZ2306000000000176039275' , 'PovolenePrecerpanie',002,'gruf',300,'EUR'),
( 70214589,'CZ1520100000002500218857' , 'StudentskyUcet',001,'spatnemeno',0,'EUR'),
( 74245658,'CZ4301000000000085636621' , 'PovolenePrecerpanie',003,'neznaju',5,'EUR')ON DUPLICATE KEY UPDATE `id_CisloUctu` = VALUES(`id_CisloUctu`), `IBAN` = VALUES(`IBAN`), `AktivovaneSluzby` = VALUES(`AktivovaneSluzby`), `id_type` = VALUES(`id_type`), `id_Klient` = VALUES(`id_Klient`), `uctovny_zostatok` = VALUES(`uctovny_zostatok`), `Mena` = VALUES(`Mena`);
INSERT INTO `karta` (`id_CisloKarty`,`VerifikacnyKod`,`DobaPlatnosti`,`DrzitelKarty`, `id_Klient`, `id_ucet`) VALUES
(23468745,5588,'25.03.2018','MartinNizner','gruf',77889653),
(58964323,1622,'25.04.2019','JanGula','Jamal',77889785),
(56879795,4872,'01.09.2020','RobertMisura','kvitko',65627653),
(25123456,5412,'25.07.2019','MichalNovak','spatnemeno',70214589),
(77121565,1245,'12.01.2020','PavelBednar','neznaju',74245658),
(85412135,1478,'04.08.2021','MartinNizner','gruf',74129921)ON DUPLICATE KEY UPDATE `id_CisloKarty` = VALUES(`id_CisloKarty`), `VerifikacnyKod` = VALUES(`VerifikacnyKod`), `DobaPlatnosti` = VALUES(`DobaPlatnosti`), `DrzitelKarty` = VALUES(`DrzitelKarty`), `id_Klient` = VALUES(`id_Klient`), `id_ucet` = VALUES(`id_ucet`);
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `VyberovaCiastka`, `VyberovaMena`, `PoplatokZaVyber`, `MiestoVyberu`,`id_Klient`,`id_ucet`) VALUES 
('03.04.2017' , 'MartinNizner' , 500 , 'EUR', 0, 'Brno','gruf',77889653)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `VyberovaCiastka` = VALUES(`VyberovaCIastka`), `VyberovaMena` = VALUES(`VyberovaMena`), `PoplatokZaVyber` = VALUES(`PoplatokZaVyber`), `MiestoVyberu` = VALUES(`MiestoVyberu`), `id_Klient` = Values(`id_Klient`), `id_ucet` = values(`id_ucet`);
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `CisloUctuPrijemcu`, `PrevadzanaCiastka`, `PrevadzanaMena`, `id_Klient`,`id_ucet`) VALUES 
('02.01.2016' , 'JanGula', 66785235 ,2000, 'Kc','Jamal',77889785)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `CisloUctuPrijemcu` = VALUES(`CisloUctuPrijemcu`), `PrevadzanaCiastka` = VALUES(`PrevadzanaCiastka`), `PrevadzanaMena` = Values(`PrevadzanaMena`), `id_Klient` = VALUES(`id_Klient`), `id_ucet` = values(`id_ucet`);;
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `VkladanaCiastka`, `MiestoVkladu`, `VkladanaMena`, `id_Klient`,`id_ucet`) VALUES 
('24.12.2014' , 'RobertMisura' , 1500,'Praha','USD','kvitko',65627653)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `VkladanaCiastka` = VALUES(`VkladanaCiastka`), `MiestoVkladu` = VALUES(`VkladanaMena`), `id_Klient` = VALUES(`id_Klient`), `id_ucet` = values(`id_ucet`);
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `VkladanaCiastka`, `MiestoVkladu`, `VkladanaMena`, `id_Klient`,`id_ucet`) VALUES 
('01.09.2017' , 'MartinNisura' , 2000,'Olomouc','EUR','gruf',74129921)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `VkladanaCiastka` = VALUES(`VkladanaCiastka`), `MiestoVkladu` = VALUES(`VkladanaMena`), `id_Klient` = VALUES(`id_Klient`), `id_ucet` = values(`id_ucet`);
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `VyberovaCiastka`, `VyberovaMena`, `PoplatokZaVyber`, `MiestoVyberu`, `id_Klient`,`id_ucet`) VALUES 
('04.05.2017' , 'MartinNizner' , 100 , 'EUR', 10, 'Praha','gruf',77889653)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `VyberovaCiastka` = VALUES(`VyberovaCIastka`), `VyberovaMena` = VALUES(`VyberovaMena`), `PoplatokZaVyber` = VALUES(`PoplatokZaVyber`), `MiestoVyberu` = VALUES(`MiestoVyberu`), `id_Klient` = Values(`id_Klient`), `id_ucet` = values(`id_ucet`);
INSERT INTO `operace` (`DatumVykonania`, `MenoVykonavatela`, `CisloUctuPrijemcu`,`PrevadzanaCiastka`, `PrevadzanaMena`, `id_Klient`,`id_ucet`) VALUES 
('12.11.2015' , 'JanGula', 7666785235 ,10000, 'Kc','Jamal',77889785),
('08.05.2016' , 'MartinNizner', 5626785235 ,20, 'EUR','gruf',77889653)ON DUPLICATE KEY UPDATE `DatumVykonania` = VALUES(`DatumVykonania`), `MenoVykonavatela` = VALUES(`MenoVykonavatela`), `CisloUctuPrijemcu` = VALUES(`CisloUctuPrijemcu`), `PrevadzanaCiastka` = VALUES(`PrevadzanaCiastka`), `PrevadzanaMena` = Values(`PrevadzanaMena`), `id_Klient` = VALUES(`id_Klient`), `id_ucet` = values(`id_ucet`);
INSERT INTO `admin` (`id_Admin`,`PrihlasovacieMeno`, `PrihlasovacieHeslo`) VALUES
(0,'gulaj', 'jan')ON DUPLICATE KEY UPDATE `id_Admin` = VALUES(`id_Admin`), `PrihlasovacieMeno` = VALUES(`PrihlasovacieMeno`), `PrihlasovacieHeslo` = VALUES(`PrihlasovacieHeslo`);
 