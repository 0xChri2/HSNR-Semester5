-- Variante: ON UPDATE/DELETE SET NULL

drop table if exists produkt, hersteller;

create table hersteller (
  hnr varchar(4) PRIMARY KEY,
  name varchar(30),
  stadt varchar(30)
);

create table produkt (
  pnr varchar(4) PRIMARY KEY,
  name varchar(30),
  preis numeric(8,2),
  hnr varchar(4) -- normalerweise references hersteller.hnr
);

create or replace function t_produktcheck_func ()
  returns trigger as 
  $$
  begin    
    raise notice 'triggered by a % on product %', TG_OP, old.pnr;
    -- prüfe ob hnr NOT NULL
    if (new.hnr IS NOT NULL) then
      -- prüfe of hnr in hersteller enthalten ist
      -- da das ergebnis nicht weiterverwendet wird, muss perform statt select benutzt werden
	  -- ansonsten können Sie auch select ... into lokaleVariable ... verwenden
      perform from hersteller where hersteller.hnr = new.hnr;
      if (NOT found) then
        raise exception 'Es existiert kein hersteller-Datensatz mit der hnr %', new.hnr;
      end if;
    end if;
    
    return new;
  end;
  $$ 
  language 'plpgsql';

create trigger t_produktcheck
  before insert or update on produkt
  for each row execute procedure t_produktcheck_func();

create or replace function t_herstellercheck_func ()
  returns trigger as 
  $$  
  begin 
    raise notice 'triggered by a % on hersteller %', TG_OP, old.hnr;
    
	-- bei einem Update, das die hnr betrifft, oder bei einem DELETE müssen
	-- bei allen betroffenen Produkt-Datensätze das hnr-Attribut auf NULL gesetzt werden
    if old.hnr <> new.hnr or TP_OP = 'DELETE' then
	  update produkt set hnr = NULL where produkt.hnr = old.hnr;
	endif; 
    
    if (TG_OP = 'DELETE') then
      return old;
    elsif (TG_OP = 'UPDATE') then
      return new;
    end if;
  end;
  $$ 
  language 'plpgsql';

create trigger t_herstellercheck
  after update or delete on hersteller
  for each row execute procedure t_herstellercheck_func();








