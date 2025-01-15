\i u5-1-create.sql

create or replace function f_produkt() returns trigger as 
$$
begin
  raise notice 'Aufruf von f_produkt wegen %', TG_OP;
  if new.hnr IS NOT NULL then
    if (select count(*) from hersteller where hnr = new.hnr) = 0 then
      raise exception 'hersteller % existiert nicht für neues oder geändertes produkt', new.hnr;
    end if;
  end if;
  
  return new;  
end;
$$ language 'plpgsql';


create or replace trigger t_produkt before insert or update on produkt for each row execute procedure f_produkt();

create or replace function f_hersteller() returns trigger as 
$$
-- declare
 -- bla record;
begin
  raise notice 'Aufruf von f_hersteller wegen %', TG_OP;
  
--   select * from produkt into bla where hnr = old.hnr;
  perform from produkt where hnr = old.hnr;
  
  if TG_OP = 'DELETE' then
    if FOUND then
      raise exception 'es gibt noch produkt von hersteller %', old.hnr;
    end if;
    return old;  
  end if;
    
       
  if TG_OP = 'UPDATE' then
    if old.hnr <> new.hnr AND FOUND then
      raise exception 'es gibt noch produkt von hersteller %', old.hnr;
    end if;
    return new;
  end if;
  
   
end;
$$ language 'plpgsql';


create or replace trigger t_hersteller before delete or update on hersteller for each row execute procedure f_hersteller();

table hersteller; table produkt;


-- cascade (c)
CREATE OR REPLACE FUNCTION trg_delete_cascade()
RETURNS TRIGGER AS $$
BEGIN
    DELETE FROM produkt
    WHERE hnr = OLD.hnr;  -- Lösche alle Produkte mit dem gleichen hnr
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_hersteller_delete
BEFORE DELETE ON hersteller
FOR EACH ROW
EXECUTE FUNCTION trg_delete_cascade();

CREATE OR REPLACE FUNCTION trg_delete_cascade()
RETURNS TRIGGER AS $$
BEGIN
    DELETE FROM produkt
    WHERE hnr = OLD.hnr;  -- Lösche alle Produkte mit dem gleichen hnr
    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_hersteller_delete
BEFORE DELETE ON hersteller
FOR EACH ROW
EXECUTE FUNCTION trg_delete_cascade();


