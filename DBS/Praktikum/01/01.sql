
-- 1.3
DROP TABLE IF EXISTS beispiel;

CREATE TABLE beispiel (
    name VARCHAR(50),
    alter INT
);


INSERT INTO beispiel (name, alter) VALUES ('Max', 20);

DROP TABLE IF EXISTS beispiel;

-- 1.4

CREATE TABLE Import (
    BestellID VARCHAR(20),
    KundenID VARCHAR(20),
    Bestelldatum VARCHAR(10),
    Betrag VARCHAR(20),
    Status VARCHAR(50),
    Titel VARCHAR(20),
    Vorname VARCHAR(50),
    Name VARCHAR(50),
    PLZ VARCHAR(10),
    Ort VARCHAR(100)
);

CREATE TABLE Kunde (
    KundenID VARCHAR(20) PRIMARY KEY,
    Titel VARCHAR(20),
    Vorname VARCHAR(50) NOT NULL,
    Name VARCHAR(50) NOT NULL,
    PLZ CHAR(5) CHECK (PLZ LIKE '[0-9][0-9][0-9][0-9][0-9]') NOT NULL,
    Ort VARCHAR(100) NOT NULL
);

CREATE TABLE Bestellung (
    BestellID VARCHAR(20) PRIMARY KEY,
    KundenID VARCHAR(20) NOT NULL,
    Bestelldatum DATE NOT NULL,
    Betrag DECIMAL(10, 2) NOT NULL,
    Status VARCHAR(20) NOT NULL,
    FOREIGN KEY (KundenID) REFERENCES Kunde(KundenID) ON DELETE CASCADE
);