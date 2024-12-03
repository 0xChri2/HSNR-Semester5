/*
 * Beispielumgebung für Übung 4
 */

drop table if exists person;

-- Tabellendefinition
CREATE TABLE person (
  nr       INT  PRIMARY KEY,
  name     VARCHAR(30),
  geburt   DATE,
  adresse1 VARCHAR(30),
  adresse2 VARCHAR(30)
);

-- Testdaten
insert into person (nr,name,geburt,adresse1,adresse2)
  values (1,'Sepp Meier',to_date('01021955','ddmmyyyy'),'Ottostr. 2','80808 München');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (2,'Sepp Meier',to_date('01021955','ddmmyyyy'),'Ottostr. 2','80808 München');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (3,'Melitta Beutel',to_date('21031966','ddmmyyyy'),'Kaffeweg 3','47906 Kempen');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (4,'Sepp Meier',to_date('01021955','ddmmyyyy'),'Ottostr. 2','80808 München');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (5,'Melitta Beutel',to_date('21031966','ddmmyyyy'),'Kaffeweg 3','47906 Kempen');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (6,'Dr. A. Narcho',to_date('15081961','ddmmyyyy'),'Oktoberweg 15','47906 Kempen');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (7,'Frank N. Stein',to_date('18111951','ddmmyyyy'),'Reagenzglas 8','47805 Krefeld');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (8,'Frank N. Stein',to_date('18111951','ddmmyyyy'),'Reagenzglas 8','47805 Krefeld');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (9,'Frank N. Stein',to_date('18111951','ddmmyyyy'),'Reagenzglas 8','47805 Krefeld');
insert into person (nr,name,geburt,adresse1,adresse2)
  values (10,'Frank N. Stein',to_date('18111951','ddmmyyyy'),'Reagenzglas 8','47805 Krefeld');