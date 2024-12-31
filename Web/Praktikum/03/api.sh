#!/bin/bash

# API Query
api_result=$(echo '{
  "select": [
    "display_name",
    "address.street_address",
    "address.city",
    "address.postal_code",
    "individual.display_name",
    "individual.age_years"
  ],
  "join": [
    ["Relationship AS relationship", "LEFT", ["relationship.contact_id_b", "=", "id"]],
    ["Individual AS individual", "LEFT", ["relationship.contact_id_a", "=", "individual.id"]],
    ["Address AS address", "LEFT", ["address.contact_id", "=", "id"]]
  ],
  "where": [
    ["individual.age_years", "<=", 16]
  ]
}' | cv api4 Household.get --in=json)

# CSV Header erstellen
echo "Haushalt,Straße,Stadt,PLZ,Kind,Alter" > haushalte_mit_kindern.csv

# Response in CSV umwandeln
echo "$api_result" | jq -r '.[] | [
  .display_name,
  .["address.street_address"],
  .["address.city"],
  .["address.postal_code"],
  .["individual.display_name"],
  .["individual.age_years"]
] | @csv' >> haushalte_mit_kindern.csv

# Sortieren und Duplikate entfernen
sort -u haushalte_mit_kindern.csv -o haushalte_mit_kindern.csv

echo "Export erstellt in: haushalte_mit_kindern.csv"