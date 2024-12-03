#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "db.h"

using namespace std;

void print_usage() {
    cerr << "Verwendung: hpdb <connect-options> commandfile" << endl;
    cerr << "Connect Optionen:" << endl;
    cerr << "  -u user     : User name" << endl;
    cerr << "  -c credential : Password" << endl;
    cerr << "  -h host     : Hostname des database server" << endl;
    cerr << "  -p port     : Port des database server" << endl;
    cerr << "  -d database : Database name" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        print_usage();
        return 1;
    }

    // Argumente parsen
    string user, password, host, port, dbname, infile_path;
    for (int i = 1; i < argc - 1; i += 2) {
        string arg(argv[i]);
        if (arg == "-u") user = argv[i+1];
        else if (arg == "-c") password = argv[i+1];
        else if (arg == "-h") host = argv[i+1];
        else if (arg == "-p") port = argv[i+1];
        else if (arg == "-d") dbname = argv[i+1];
        else {
            print_usage();
            return 1;
        }
    }

    // Letztes Argument ist der Command file path
    infile_path = argv[argc - 1];
    ifstream infile(infile_path.c_str());
    if (!infile.good()) {
        cerr << "Error: Cannot open command file " << infile_path << endl;
        return 1;
    }

    // Connect to database
    if (db_login(user, password, host, port, dbname) != 0) {
        cerr << "Error: Database login failed" << endl;
        return 1;
    }

    // Begin transaction
    if (db_begin() != 0) {
        cerr << "Error: Could not begin transaction" << endl;
        db_logout();
        return 1;
    }

    // Read command file
    string line;
    vector<string> values;

    try {
        while (getline(infile, line)) {
            // Skip empty lines
            if (line.empty()) continue;

            // Clear previous values
            values.clear();

            // Parse line into values
            istringstream stream(line);
            string value;
            while (stream >> value) {
                values.push_back(value);
            }

            // Process command
            if (values.empty()) continue;

            string cmd = values[0];

            // n = Create empty database tables "produkt" and "hersteller"
            if (cmd == "n") {
                // Create new empty database tables
                if (db_drop_table("produkt") != 0 || 
                    db_drop_table("hersteller") != 0 ||
                    db_create_table_hersteller() != 0 || 
                    db_create_table_produkt() != 0) {
                    throw runtime_error("Error creating new database");
                }
            }
            // ih (hnr, name, stadt) = Fügt neues Tupel mit Werten zu Hersteller hinzu
            else if (cmd == "ih") {
                // Insert or update manufacturer
                if (values.size() != 4) {
                    throw runtime_error("Invalid ih command: " + line);
                }
                int check = db_check_hnr(values[1]);
                if (check == -1) {
                    throw runtime_error("Error checking manufacturer");
                }
                if (check == 0) {
                    // Insert new manufacturer
                    if (db_insert_hersteller(values[1], values[2], values[3]) != 0) {
                        throw runtime_error("Error inserting manufacturer");
                    }
                } else {
                    // Update existing manufacturer
                    if (db_update_hersteller(values[1], values[2], values[3]) != 0) {
                        throw runtime_error("Error updating manufacturer");
                    }
                }
            }
            // ip (pnr, name, preis, hnr) = Fügt neues Tupel mit Werten zu Produkt hinzu
            else if (cmd == "ip") {
                // Insert or update product
                if (values.size() != 5) {
                    throw runtime_error("Invalid ip command: " + line);
                }
                int check = db_check_pnr(values[1]);
                if (check == -1) {
                    throw runtime_error("Error checking product");
                }
                if (check == 0) {
                    // Insert new product
                    if (db_insert_produkt(values[1], values[2], values[3], values[4]) != 0) {
                        throw runtime_error("Error inserting product");
                    }
                } else {
                    // Update existing product
                    if (db_update_produkt(values[1], values[2], values[3], values[4]) != 0) {
                        throw runtime_error("Error updating product");
                    }
                }
            }
            // i (hnr, hname, stadt, pnr, pname, preis) = Fügt neues Tupel zu Hersteller und Produkt hinzu
            else if (cmd == "i") {
                // Combined insert
                if (values.size() != 7) {
                    throw runtime_error("Invalid i command: " + line);
                }
                // First, insert/update manufacturer
                int check_hnr = db_check_hnr(values[1]);
                if (check_hnr == -1) {
                    throw runtime_error("Error checking manufacturer");
                }
                if (check_hnr == 0) {
                    if (db_insert_hersteller(values[1], values[2], values[3]) != 0) {
                        throw runtime_error("Error inserting manufacturer");
                    }
                } else {
                    if (db_update_hersteller(values[1], values[2], values[3]) != 0) {
                        throw runtime_error("Error updating manufacturer");
                    }
                }

                // Then, insert/update product
                int check_pnr = db_check_pnr(values[4]);
                if (check_pnr == -1) {
                    throw runtime_error("Error checking product");
                }
                if (check_pnr == 0) {
                    if (db_insert_produkt(values[4], values[5], values[6], values[1]) != 0) {
                        throw runtime_error("Error inserting product");
                    }
                } else {
                    if (db_update_produkt(values[4], values[5], values[6], values[1]) != 0) {
                        throw runtime_error("Error updating product");
                    }
                }
            }
            // dh hnr = Löscht Hersteller mit hnr
            else if (cmd == "dh") {
                if (values.size() != 2) {
                    throw runtime_error("Invalid dh command: " + line);
                }
                if (db_delete_hersteller(values[1]) != 0) {
                    throw runtime_error("Error deleting manufacturer");
                }
            }
            // dp pnr = Löscht Produkt mit pnr
            else if (cmd == "dp") {
                if (values.size() != 2) {
                    throw runtime_error("Invalid dp command: " + line);
                }
                if (db_delete_produkt(values[1]) != 0) {
                    throw runtime_error("Error deleting product");
                }
            }
            // ch = Zählt Hersteller
            else if (cmd == "ch") {
                int count = db_count("hersteller");
                if (count == -1) {
                    throw runtime_error("Error counting manufacturers");
                }
                cout << "Anzahl Hersteller: " << count << endl;
            }
            // cp = Zählt Produkte
            else if (cmd == "cp") {
                int count = db_count("produkt");
                if (count == -1) {
                    throw runtime_error("Error counting products");
                }
                cout << "Anzahl Produkte: " << count << endl;
            }
            else {
                throw runtime_error("Unknown command: " + line);
            }
        }

        if (db_commit() != 0) {
            throw runtime_error("Error committing transaction");
        }
    }
    catch (const exception& e) {
        // Zurückschreiben und ausloggen
        cerr << "Error: " << e.what() << endl;
        db_rollback();
        db_logout();
        return 1;
    }

    db_logout();
    return 0;
}