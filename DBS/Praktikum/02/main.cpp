#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "db.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Kommandozeilenargumente verarbeiten
    if (argc < 7) {
        cerr << "Usage: hpdb -u <user> -c <password> -h <host> -p <port> -d <dbname> <commandfile>" << endl;
        return 1;
    }

    string user, password, host, port, dbname, infile_path;
    for (int i = 1; i < argc - 1; i += 2) {
        string arg = argv[i];
        if (arg == "-u") user = argv[i + 1];
        else if (arg == "-c") password = argv[i + 1];
        else if (arg == "-h") host = argv[i + 1];
        else if (arg == "-p") port = argv[i + 1];
        else if (arg == "-d") dbname = argv[i + 1];
    }
    infile_path = argv[argc - 1];

    

    // Verbindung herstellen
    if (db_login(user, password, host, port, dbname) != 0) {
        cerr << "Datenbankverbindung fehlgeschlagen." << endl;
        return 1;
    }

    // Transaktion starten
    if (db_begin() != 0) {
        cerr << "Transaktion konnte nicht gestartet werden." << endl;
        db_logout();
        return 1;
    }

    ifstream infile(infile_path.c_str());
    if (!infile.good()) {
        cerr << "Fehler: Command-Datei konnte nicht geöffnet werden." << endl;
        db_rollback();
        db_logout();
        return 1;
    }

    string line;
    vector<string> values;

    // Verarbeitung der Befehlsdatei
    while (getline(infile, line)) {
        string value;
        istringstream stream(line);
        values.clear();

        while (stream >> value) {
            values.push_back(value);
        }

        if (values.empty()) continue;

        // Befehle verarbeiten
        if (values[0] == "n") {
            db_drop_table("produkt");
            db_drop_table("hersteller");
            db_create_table_hersteller();
            db_create_table_produkt();
        } else if (values[0] == "ih" && values.size() == 4) {
            db_insert_hersteller(values[1], values[2], values[3]);
        } else if (values[0] == "ip" && values.size() == 5) {
            db_insert_produkt(values[1], values[2], values[3], values[4]);
        } else if (values[0] == "ch") {
            cout << "Anzahl Hersteller: " << db_count("hersteller") << endl;
        } else if (values[0] == "cp") {
            cout << "Anzahl Produkte: " << db_count("produkt") << endl;
        } else if (values[0] == "dh" && values.size() == 2) {
            db_delete_hersteller(values[1]);
        } else if (values[0] == "dp" && values.size() == 2) {
            db_delete_produkt(values[1]);
        }
        else {
            cerr << "Unbekannter oder ungültiger Befehl: " << line << endl;
        }
    }

    // Transaktion abschließen
    if (db_commit() != 0) {
        cerr << "Fehler beim Commit. Rollback wird ausgeführt." << endl;
        db_rollback();
    }

    db_logout();
    return 0;
}
