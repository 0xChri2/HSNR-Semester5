/* 
  Die folgenden Testfälle sollten Sie nach und nach überprüfen, indem Sie
  jeweils ein Statement ausführen und alle anderen auskommentieren.
  Der Startwerte-Block befüllt die Tabelle mit Initialwerten und kann in einem
  Schritt ausgeführt werden.
*/

-- Block 1
-- Startwerte für regal und ware, kann in einem Schritt ausgeführt werden
-- /*
INSERT INTO regal VALUES (1, 'A-5', 220, 0), (2, 'B-2', 120, 0), (3, 'C-37', 310, 0);

INSERT INTO ware VALUES (1, 1, 'M6-S', 'Schraube', 10, 5, 2.0, to_date('01.01.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (2, 1, 'M8-S', 'Schraube', 5, 10, 2.5, to_date('15.03.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (3, 1, 'M10-S', 'Schraube', 5, 15, 3.0, to_date('15.03.2024', 'DD.MM.YYYY'));

INSERT INTO ware VALUES (4, 2, 'M6-M', 'Mutter', 5, 5, 1.5, to_date('01.01.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (5, 2, 'M8-M', 'Mutter', 3, 10, 2, to_date('15.03.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (6, 2, 'M10-M', 'Mutter', 3, 15, 2.4, to_date('15.03.2024', 'DD.MM.YYYY'));

INSERT INTO ware VALUES (7, 3, 'R-T', 'Rohr', 10, 5, 5, to_date('01.06.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (8, 3, 'R-N', 'Rohr', 5, 20, 7.5, to_date('01.06.2024', 'DD.MM.YYYY'));
INSERT INTO ware VALUES (9, 3, 'R-W', 'Rohr', 10, 7, 4.9, to_date('01.07.2024', 'DD.MM.YYYY'));
-- */

-- Danach sollten die Tabellen wie folgt aussehen
/*
dbs000=> table ware; table regal; table warenhistorie;
 invnr | regalnr | warenname |   typ    | anzahl | platzproeinheit | preisproeinheit | datumverfuegbar 
-------+---------+-----------+----------+--------+-----------------+-----------------+-----------------
     1 |       1 | M6-S      | Schraube |     10 |               5 |            2.00 | 2024-01-01
     2 |       1 | M8-S      | Schraube |      5 |              10 |            2.50 | 2024-03-15
     3 |       1 | M10-S     | Schraube |      5 |              15 |            3.00 | 2024-03-15
     4 |       2 | M6-M      | Mutter   |      5 |               5 |            1.50 | 2024-01-01
     5 |       2 | M8-M      | Mutter   |      3 |              10 |            2.00 | 2024-03-15
     6 |       2 | M10-M     | Mutter   |      3 |              15 |            2.40 | 2024-03-15
     7 |       3 | R-T       | Rohr     |     10 |               5 |            5.00 | 2024-06-01
     8 |       3 | R-N       | Rohr     |      5 |              20 |            7.50 | 2024-06-01
     9 |       3 | R-W       | Rohr     |     10 |               7 |            4.90 | 2024-07-01
(9 Zeilen)

 regalnr | lage | platzgesamt | platzbelegt 
---------+------+-------------+-------------
       1 | A-5  |         220 |         175
       2 | B-2  |         120 |         100
       3 | C-37 |         310 |         220
(3 Zeilen)

 whnr | invnr | regalnr | warenname | preisalt | verfuegbaralt | operation | benutzername | geaendertam 
------+-------+---------+-----------+----------+---------------+-----------+--------------+-------------
(0 Zeilen)

*/




