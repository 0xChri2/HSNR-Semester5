#!/bin/bash

# Execute API call and store result
api_result=$(echo '{"select":["individual.age_years","display_name","individual.display_name"],"join":[["Relationship AS relationship","LEFT",["relationship.contact_id_b","=","id"]],["Individual AS individual","LEFT",["relationship.contact_id_a","=","individual.id"]]],"where":[["individual.age_years","<",16]]}' | cv api4 Household.get --in=json)

# Create CSV header
echo "Haushalt,Kind,Alter" > haushalte_mit_kindern.csv

# Process JSON and convert to CSV format
# Using jq to directly process the array structure
echo "$api_result" | jq -r '.[] | [.display_name, .["individual.display_name"], .["individual.age_years"]] | @csv' >> haushalte_mit_kindern.csv

# Sort and remove duplicate entries
sort -u haushalte_mit_kindern.csv -o haushalte_mit_kindern.csv

echo "Export wurde erstellt in: haushalte_mit_kindern.csv"

# Optional: Display the content of the CSV
echo -e "\nInhalt der CSV-Datei:"
cat haushalte_mit_kindern.csv