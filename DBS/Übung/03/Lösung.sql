Select * FROM Hersteller WHERE stadt IS NULL or stadt = '';

Select * FRIN lieferung WHERE datum >= to_date('01.01.2002', 'DD.MM.YYYY') AND <= to_date('31.12.2002', 'DD.MM.YYYY');

# Oder mit BETWEEN
Select * FRIN lieferung WHERE datum BETWEEN to_date('01.01.2002', 'DD.MM.YYYY') AND to_date('31.12.2002', 'DD.MM.YYYY');

Select * FROM lieferung
WHERE to_char(datum, 'YYYY-MM') = '2002-01';

SELECT * FROM lieferung
WHERE EXTRACT(YEAR FROM datum) = 2002 AND EXTRACT(MONTH FROM datum) = 1;

Select l.*, p.name FROM lieferung l, produkt p
WHERE l.pnr = p.pnr AND to_char(datum, 'YYYY-MM') = '2002-01';

SELECT * FROM lieferung l JOIN produkt p ON l.pnr = p.pnr
WHERE to_char(datum, 'YYYY-MM') = '2002-01';

Select p.pnr, p.name, h.hnr, h.name 
FROM produkt p, hersteller h
WHERE p.hnr = h.hnr;

# mit join 
Select p.pnr, p.name, h.hnr, h.name
FROM produkt p JOIN hersteller h ON p.hnr = h.hnr;


Select h.hnr, h.name , 