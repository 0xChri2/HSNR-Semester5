#!/bin/bash

create_record() {
    local json=$1
    local result=$(echo "$json" | cv api4 $2.create --in=json)
    echo "$result" | grep -o '"id": [0-9]*' | awk '{print $2}'
}

declare -A ids

echo "Starting data creation process..."
echo "================================"

echo "Creating households..."

household_mueller=$(create_record '{"values":{
    "display_name":"Haushalt Müller",
    "household_name":"Haushalt Müller",
    "sort_name":"Mueller Household",
    "street_address":"Hauptstraße 123",
    "city":"Berlin",
    "postal_code":"10115"
}}' "Household")
echo "Created Haushalt Müller with ID: $household_mueller"
ids[household_mueller]=$household_mueller

household_schmidt=$(create_record '{"values":{
    "display_name":"Haushalt Schmidt",
    "household_name":"Haushalt Schmidt",
    "sort_name":"Schmidt Household",
    "street_address":"Waldweg 45",
    "city":"Berlin",
    "postal_code":"10587"
}}' "Household")
echo "Created Haushalt Schmidt with ID: $household_schmidt"
ids[household_schmidt]=$household_schmidt

household_weber=$(create_record '{"values":{
    "display_name":"Haushalt Weber",
    "household_name":"Haushalt Weber",
    "sort_name":"Weber Household",
    "street_address":"Lindenallee 78",
    "city":"Berlin",
    "postal_code":"12489"
}}' "Household")
echo "Created Haushalt Weber with ID: $household_weber"
ids[household_weber]=$household_weber

echo "Creating individuals..."

# Individual records
mueller_thomas=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Thomas",
    "last_name":"Müller",
    "birth_date":"1980-05-15",
    "gender_id":2
}}' "Contact")
echo "Created Thomas Müller with ID: $mueller_thomas"
ids[mueller_thomas]=$mueller_thomas

mueller_sandra=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Sandra",
    "last_name":"Müller",
    "birth_date":"1982-08-23",
    "gender_id":1
}}' "Contact")
echo "Created Sandra Müller with ID: $mueller_sandra"
ids[mueller_sandra]=$mueller_sandra

mueller_lisa=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Lisa",
    "last_name":"Müller",
    "birth_date":"2016-03-10",
    "gender_id":1
}}' "Contact")
echo "Created Lisa Müller with ID: $mueller_lisa"
ids[mueller_lisa]=$mueller_lisa

# Additional Individuals
schmidt_michael=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Michael",
    "last_name":"Schmidt",
    "birth_date":"1975-11-30",
    "gender_id":2
}}' "Contact")
echo "Created Michael Schmidt with ID: $schmidt_michael"
ids[schmidt_michael]=$schmidt_michael

schmidt_julia=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Julia",
    "last_name":"Schmidt",
    "birth_date":"1978-04-18",
    "gender_id":1
}}' "Contact")
echo "Created Julia Schmidt with ID: $schmidt_julia"
ids[schmidt_julia]=$schmidt_julia

schmidt_max=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Max",
    "last_name":"Schmidt",
    "birth_date":"2017-07-22",
    "gender_id":2
}}' "Contact")
echo "Created Max Schmidt with ID: $schmidt_max"
ids[schmidt_max]=$schmidt_max

schmidt_emma=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Emma",
    "last_name":"Schmidt",
    "birth_date":"2019-12-05",
    "gender_id":1
}}' "Contact")
echo "Created Emma Schmidt with ID: $schmidt_emma"
ids[schmidt_emma]=$schmidt_emma

weber_andreas=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Andreas",
    "last_name":"Weber",
    "birth_date":"1983-09-12",
    "gender_id":2
}}' "Contact")
echo "Created Andreas Weber with ID: $weber_andreas"
ids[weber_andreas]=$weber_andreas

weber_karen=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Karen",
    "last_name":"Weber",
    "birth_date":"1985-02-28",
    "gender_id":1
}}' "Contact")
echo "Created Karen Weber with ID: $weber_karen"
ids[weber_karen]=$weber_karen

weber_paul=$(create_record '{"values":{
    "contact_type":"Individual",
    "first_name":"Paul",
    "last_name":"Weber",
    "birth_date":"2015-06-15",
    "gender_id":2
}}' "Contact")
echo "Created Paul Weber with ID: $weber_paul"
ids[weber_paul]=$weber_paul

echo "Creating relationships..."

create_relationship() {
    local person_id=$1
    local household_id=$2
    local name=$3
    echo "Creating relationship for $name..."
    create_record "{\"values\":{
        \"relationship_type_id\":8,
        \"contact_id_a\":$person_id,
        \"contact_id_b\":$household_id
    }}" "Relationship"
}

create_relationship ${ids[mueller_thomas]} ${ids[household_mueller]} "Thomas Müller"
create_relationship ${ids[mueller_sandra]} ${ids[household_mueller]} "Sandra Müller"
create_relationship ${ids[mueller_lisa]} ${ids[household_mueller]} "Lisa Müller"

create_relationship ${ids[schmidt_michael]} ${ids[household_schmidt]} "Michael Schmidt"
create_relationship ${ids[schmidt_julia]} ${ids[household_schmidt]} "Julia Schmidt"
create_relationship ${ids[schmidt_max]} ${ids[household_schmidt]} "Max Schmidt"
create_relationship ${ids[schmidt_emma]} ${ids[household_schmidt]} "Emma Schmidt"

create_relationship ${ids[weber_andreas]} ${ids[household_weber]} "Andreas Weber"
create_relationship ${ids[weber_karen]} ${ids[household_weber]} "Karen Weber"
create_relationship ${ids[weber_paul]} ${ids[household_weber]} "Paul Weber"

echo "Data creation completed!"
echo "Summary of created records:"
echo "- 3 Households"
echo "- 11 Individuals"
echo "- 10 Relationships"