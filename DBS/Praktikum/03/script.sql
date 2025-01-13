Create Table regal (
    regalnr Numeric primary key,
    lage varchar(5),
    platzgesamt Integer not null,
    platzbelegt interger Default 0
);

create table ware(
    invnr interger primary key,
    regalnr interger not null,
    warenname varchar(50),
    typ varchar(30),
    anzahl interger,
    platzproeoinheit interger,
    preisproeinheit numeric(8,2),
    datumverfuegbar date Default current_date,
    foreign key (regalnr) references regal(regalnr)
);

create table warehistory (
    invnr interger,
    regalnr interger,
    warenname varchar(50),
    typ varchar(30),
    anzahl interger,
    platzproeoinheit interger,
    preisproeinheit numeric(8,2),
    datumverfuegbar date,
    datumverkauft date Default current_date,
    foreign key (regalnr) references regal(regalnr)
);

Create or replace function tigger_ware_modification()
returns trigger as $$
