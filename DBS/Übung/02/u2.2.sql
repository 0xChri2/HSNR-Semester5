DROP Table IF EXISTS person, plzort;

CREATE Table plzort (
    plz CHAR(5) CHECK (plz SIMILAR TO '\d{5}') PRIMARY KEY,
    ort VARCHAR(50) NOT NULL
);

-- \! (Befehle ausführen)

--\d tabellen anzeigen lassen

--- \i (Datei einlesen)

CREATE Table person (
    nr INT PRIMARY KEY,
    name VARCHAR(50),
    vorname VARCHAR(50),
    UNIQUE (name, vorname),
    plz CHAR(5) REFERENCES plzort(plz) ON UPDATE CASCADE ON DELETE NO ACTION,
    strasse VARCHAR(50) NOT NULL
    -- FOREIGN KEY (plz) REFERENCES adresse(plz, strasse)
);

INSERT INTO plzort VAlUES('47805', 'Krefeld');
INSERT INTO person VALUES(1, 'Gans', 'Gustav', '47805', 'Kölner Strasse 100');

UPDATE plzort SET plz = '47804' WHERE plz = '47805';
--- ON UPDATE CASCADE bedeutet: Wenn sich die PLZ ändert, dann wird die PLZ in der Tabelle person auch geändert
--- ON UPDATE SET NULL
-- ON UPDATE NO ACTION

ALTER TABLE plz ALTER COLUMN plz TYPE CHAR(5) Using to_char(plz, '000000');

ALTER TABLE person ADD COLUMN nachname VARCHAR;
UPDATE person SET nachname = name;
ALTER TABLE person DROP COLUMN name;

SELECT nr from person 
WHERE strasse is NULL or plz is NULL OR name is NULL OR vorname is NULL;

update person SET name = name || 'bla';
