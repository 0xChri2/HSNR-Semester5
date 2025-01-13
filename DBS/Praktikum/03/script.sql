Create Table hersteller (
    hnr VARCHAR(4),
    name VARCHAR(30),
    stadt VARCHAR(30),
    PRIMARY KEY (hnr)
);

Create Table produkt (
    pnr VARCHAR(4),
    name VARCHAR(30),
    hnr VARCHAR(4),
    preis NUMERIC(8,2),
    PRIMARY KEY (pnr)
):

create table lieferung(
    
)