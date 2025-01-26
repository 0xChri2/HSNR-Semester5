-- Erstellen der Tabelle regal
CREATE TABLE regal (
    regalnr INTEGER PRIMARY KEY,
    lage VARCHAR(5),
    platzgesamt INTEGER,
    platzbelegt INTEGER
);

-- Erstellen der Tabelle ware
CREATE TABLE ware (
    invnr INTEGER PRIMARY KEY,
    regalnr INTEGER REFERENCES regal(regalnr),
    warenname VARCHAR(30),
    typ VARCHAR(30),
    anzahl INTEGER,
    platzproeinheit INTEGER,
    preisproeinheit NUMERIC(8,2),
    datumverfuegbar DATE
);

-- Erstellen der Tabelle warenhistorie
CREATE TABLE warenhistorie (
    id SERIAL PRIMARY KEY,
    invnr INTEGER,
    regalnr INTEGER,
    warenname VARCHAR(30),
    alter_preis NUMERIC(8,2),
    altes_datum DATE,
    operation VARCHAR(6),
    benutzer VARCHAR(30),
    aenderungszeitpunkt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

---

-- 1. Trigger für die Validierungsregeln (wird zuerst ausgeführt)
CREATE OR REPLACE FUNCTION waren_validierung_func()
RETURNS TRIGGER AS $$
DECLARE
    verfuegbarer_platz INTEGER;
    waren_im_regal INTEGER;
    neues_regal INTEGER;
BEGIN
    -- INSERT Validierungen
    IF (TG_OP = 'INSERT') THEN
        -- Regel 2: Wenn kein Regal angegeben
        IF (NEW.regalnr IS NULL) THEN
            SELECT r.regalnr 
            INTO neues_regal
            FROM regal r
            WHERE (r.platzgesamt - r.platzbelegt) >= (NEW.anzahl * NEW.platzproeinheit)
            AND (SELECT COUNT(*) FROM ware w WHERE w.regalnr = r.regalnr) < 4
            ORDER BY r.regalnr
            LIMIT 1;

            IF neues_regal IS NULL THEN
                RAISE EXCEPTION 'Kein passendes Regal mit ausreichend Platz gefunden';
            END IF;

            NEW.regalnr := neues_regal;
            RAISE NOTICE 'Ware wird in Regal % eingefügt', neues_regal;
        END IF;

        -- Regel 3: Prüfe Anzahl unterschiedlicher Waren
        SELECT COUNT(*) INTO waren_im_regal
        FROM ware
        WHERE regalnr = NEW.regalnr;

        IF waren_im_regal >= 4 THEN
            RAISE EXCEPTION 'Maximal 4 unterschiedliche Waren pro Regal erlaubt';
        END IF;

        -- Regel 1: Prüfe verfügbaren Platz
        SELECT (platzgesamt - platzbelegt) INTO verfuegbarer_platz
        FROM regal
        WHERE regalnr = NEW.regalnr;

        IF (NEW.anzahl * NEW.platzproeinheit > verfuegbarer_platz) THEN
            RAISE EXCEPTION 'Nicht genügend Platz im Regal verfügbar';
        END IF;
    END IF;

    -- UPDATE Validierungen
    IF (TG_OP = 'UPDATE') THEN
        -- Regel 4: Prüfungen
        IF (NEW.regalnr != OLD.regalnr) THEN
            RAISE EXCEPTION 'Waren können nicht in ein anderes Regal umsortiert werden';
        END IF;

        IF (NEW.anzahl < 0) THEN
            RAISE EXCEPTION 'Die Anzahl darf nicht negativ sein';
        END IF;

        IF (NEW.anzahl > OLD.anzahl) THEN
            SELECT (platzgesamt - platzbelegt + (OLD.anzahl * OLD.platzproeinheit)) 
            INTO verfuegbarer_platz 
            FROM regal 
            WHERE regalnr = NEW.regalnr;

            IF (NEW.anzahl * NEW.platzproeinheit > verfuegbarer_platz) THEN
                RAISE EXCEPTION 'Nicht genügend Platz im Regal verfügbar';
            END IF;
        END IF;

        -- Regel 5: Prüfung des Verfügbarkeitsdatums
        IF (NEW.datumverfuegbar < OLD.datumverfuegbar) THEN
            NEW.datumverfuegbar := OLD.datumverfuegbar;
            RAISE NOTICE 'Verfügbarkeitsdatum kann nicht auf ein früheres Datum geändert werden';
            
            -- Prüfe ob sich sonst nichts geändert hat
            IF (NEW.invnr = OLD.invnr AND 
                NEW.regalnr = OLD.regalnr AND 
                NEW.warenname = OLD.warenname AND 
                NEW.typ = OLD.typ AND 
                NEW.anzahl = OLD.anzahl AND 
                NEW.platzproeinheit = OLD.platzproeinheit AND 
                NEW.preisproeinheit = OLD.preisproeinheit AND 
                NEW.datumverfuegbar != OLD.datumverfuegbar) THEN
                RETURN NEW;
            END IF;
        END IF;
    END IF;

    IF (TG_OP = 'DELETE') THEN
        -- Prüfe ob das Regal existiert
        IF NOT EXISTS (SELECT 1 FROM regal WHERE regalnr = OLD.regalnr) THEN
            RAISE EXCEPTION 'Regal % nicht gefunden - Platzbelegung kann nicht aktualisiert werden', OLD.regalnr;
        END IF;
        RETURN OLD;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- 2. Trigger für die Aktualisierung der Regalkapazität
CREATE OR REPLACE FUNCTION waren_regal_update_func()
RETURNS TRIGGER AS $$
BEGIN
    IF (TG_OP = 'INSERT') THEN
        UPDATE regal 
        SET platzbelegt = platzbelegt + (NEW.anzahl * NEW.platzproeinheit)
        WHERE regalnr = NEW.regalnr;
    ELSIF (TG_OP = 'UPDATE') THEN
        UPDATE regal 
        SET platzbelegt = platzbelegt - (OLD.anzahl * OLD.platzproeinheit) + (NEW.anzahl * NEW.platzproeinheit)
        WHERE regalnr = NEW.regalnr;
    ELSIF (TG_OP = 'DELETE') THEN
        UPDATE regal 
        SET platzbelegt = platzbelegt - (OLD.anzahl * OLD.platzproeinheit)
        WHERE regalnr = OLD.regalnr;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- 3. Trigger für die Warenhistorie
CREATE OR REPLACE FUNCTION waren_historie_func()
RETURNS TRIGGER AS $$
BEGIN
    IF (TG_OP = 'UPDATE') THEN
        -- Nur einfügen wenn sich relevante Werte geändert haben
        IF (OLD.preisproeinheit != NEW.preisproeinheit OR 
            OLD.datumverfuegbar != NEW.datumverfuegbar OR
            OLD.anzahl != NEW.anzahl) THEN
            
            INSERT INTO warenhistorie (invnr, regalnr, warenname, alter_preis, altes_datum, operation, benutzer)
            VALUES (
                OLD.invnr,
                OLD.regalnr,
                OLD.warenname,
                OLD.preisproeinheit,
                OLD.datumverfuegbar,
                TG_OP,
                CURRENT_USER
            );
        END IF;
        RETURN NEW;
    ELSIF (TG_OP = 'DELETE') THEN
        INSERT INTO warenhistorie (invnr, regalnr, warenname, alter_preis, altes_datum, operation, benutzer)
        VALUES (
            OLD.invnr,
            OLD.regalnr,
            OLD.warenname,
            OLD.preisproeinheit,
            OLD.datumverfuegbar,
            TG_OP,
            CURRENT_USER
        );
        RETURN OLD;
    END IF;

    RETURN NULL; -- Dies wird nie erreicht, da wir nur UPDATE und DELETE behandeln
END;
$$ LANGUAGE plpgsql;

-- Trigger in der richtigen Reihenfolge erstellen
DROP TRIGGER IF EXISTS a_waren_validierung ON ware;
DROP TRIGGER IF EXISTS b_waren_regal_update ON ware;
DROP TRIGGER IF EXISTS c_waren_historie ON ware;

CREATE TRIGGER a_waren_validierung
    BEFORE INSERT OR UPDATE OR DELETE ON ware
    FOR EACH ROW
    EXECUTE FUNCTION waren_validierung_func();

CREATE TRIGGER b_waren_regal_update
    AFTER INSERT OR UPDATE OR DELETE ON ware
    FOR EACH ROW
    EXECUTE FUNCTION waren_regal_update_func();

CREATE TRIGGER c_waren_historie
    AFTER UPDATE OR DELETE ON ware
    FOR EACH ROW
    EXECUTE FUNCTION waren_historie_func();