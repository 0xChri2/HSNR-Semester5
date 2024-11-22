/*
 * Beispielumgebung für Übung 3
*/

drop table if exists lieferung;
drop table if exists produkt;
drop table if exists hersteller;

-- Tabellendefinitionen
create table hersteller (
  hnr   varchar(4),
  name  varchar(30),
  stadt varchar(30),
  primary key (hnr)
);
create table produkt (
 pnr   varchar(4),
 name  varchar(30),
 hnr   varchar(4) references hersteller(hnr),
 preis numeric(8,2),
 primary key (pnr)
);
create table lieferung (
  lnr   varchar(6),
  pnr   varchar(4) references produkt(pnr),
  datum date,
  menge numeric(8,2),
  primary key (lnr)
);



-- Testdaten
-- hersteller
insert into hersteller (hnr,name,stadt)
  values ('H1','Henkel','Düsseldorf');
insert into hersteller (hnr,name,stadt)
  values ('H2','Pelikan','Hannover');
insert into hersteller (hnr,name,stadt)
  values ('H3','Brause','Iserlohn');
insert into hersteller (hnr,name,stadt)
  values ('H4','Soennecken',NULL);
insert into hersteller (hnr,name,stadt)
  values ('H5','Faber-Castell',NULL);
insert into hersteller (hnr,name,stadt)
  values ('H6','Geha',NULL);
-- produkt
insert into produkt (pnr,name,preis,hnr)
  values ('P1','Pritt',2.50,'H1');
insert into produkt (pnr,name,preis,hnr)
  values ('P2','Uhu',2.70,'H1');
insert into produkt (pnr,name,preis,hnr)
  values ('P3','Tinte',3.20,'H2');
insert into produkt (pnr,name,preis,hnr)
  values ('P4','Füller',12.98,'H2');
insert into produkt (pnr,name,preis,hnr)
  values ('P5','Papier',5.90,'H4');
insert into produkt (pnr,name,preis,hnr)
  values ('P6','Bandzugfeder',2.30,'H3');
insert into produkt (pnr,name,preis,hnr)
  values ('P7','Bleistift',1.00,'H5');
-- lieferungen
insert into lieferung (lnr,pnr,datum,menge)
  values ('000001','P1',to_date('01012002','DDMMYYYY'),40.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000002','P1',to_date('15032002','DDMMYYYY'),42.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000003','P2',to_date('02012002','DDMMYYYY'),32.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000004','P3',to_date('05022002','DDMMYYYY'),10.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000005','P5',to_date('10022002','DDMMYYYY'),200.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000006','P7',to_date('20032002','DDMMYYYY'),150.0);
insert into lieferung (lnr,pnr,datum,menge)
  values ('000007','P5',to_date('20032002','DDMMYYYY'),50.0);
