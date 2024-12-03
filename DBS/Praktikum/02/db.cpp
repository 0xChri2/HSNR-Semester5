#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "db.h"

using namespace std;

// Global database connection
PGconn *conn = nullptr;

// Database login
int db_login(const string &user, const string &password, const string &host, 
             const string &port, const string &dbname) {
    string connstr = "user=" + user + 
                     " password=" + password + 
                     " host=" + host + 
                     " port=" + port + 
                     " dbname=" + dbname;

    conn = PQconnectdb(connstr.c_str());
    
    // Check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        conn = nullptr;
        return -1;
    }
    return 0;
}

// Database logout
void db_logout() {
    if (conn) {
        PQfinish(conn);
        conn = nullptr;
    }
}

// Begin transaction
int db_begin() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Begin transaction failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Commit transaction
int db_commit() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Commit transaction failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Rollback transaction
int db_rollback() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, "ROLLBACK");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Rollback transaction failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Create "hersteller" table
int db_create_table_hersteller() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, 
        "CREATE TABLE hersteller ("
        "hnr VARCHAR(4) PRIMARY KEY,"
        "name VARCHAR(30) NOT NULL,"
        "stadt VARCHAR(30) NOT NULL)"
    );
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Create hersteller table failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Create "produkt" table
int db_create_table_produkt() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, 
        "CREATE TABLE produkt ("
        "pnr VARCHAR(4) PRIMARY KEY,"
        "name VARCHAR(30) NOT NULL,"
        "preis NUMERIC(8,2) NOT NULL,"
        "hnr VARCHAR(4) REFERENCES hersteller(hnr) ON DELETE CASCADE)"
    );
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Create produkt table failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Drop table
int db_drop_table(const string &tablename) {
    if (!conn) return -1;
    
    string query = "DROP TABLE IF EXISTS " + tablename;
    PGresult *res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Drop table failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Check if "hersteller" exists
int db_check_hnr(const string &hnr) {
    if (!conn) return -1;
    
    string query = "SELECT COUNT(*) FROM hersteller WHERE hnr = $1";
    const char *paramValues[1] = { hnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 1, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Check manufacturer failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    int count = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    
    return count > 0 ? 1 : 0;
}

// Check if "produkt" exists
int db_check_pnr(const string &pnr) {
    if (!conn) return -1;
    
    string query = "SELECT COUNT(*) FROM produkt WHERE pnr = $1";
    const char *paramValues[1] = { pnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 1, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Check product failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    int count = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    
    return count > 0 ? 1 : 0;
}

// Count table rows
int db_count(const string &tablename) {
    if (!conn) return -1;
    
    string query = "SELECT COUNT(*) FROM " + tablename;
    PGresult *res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Count table rows failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    int count = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    
    return count;
}

// Insert produkt
int db_insert_produkt(const string &pnr, const string &name, const string &preis, const string &hnr) {
    if (!conn) return -1;
    
    string query = "INSERT INTO produkt (pnr, name, preis, hnr) VALUES ($1, $2, $3, $4)";
    const char *paramValues[4] = { pnr.c_str(), name.c_str(), preis.c_str(), hnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 4, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Insert product failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Insert hersteller
int db_insert_hersteller(const string &hnr, const string &name, const string &stadt) {
    if (!conn) return -1;
    
    string query = "INSERT INTO hersteller (hnr, name, stadt) VALUES ($1, $2, $3)";
    const char *paramValues[3] = { hnr.c_str(), name.c_str(), stadt.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 3, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Insert manufacturer failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Update produkt
int db_update_produkt(const string &pnr, const string &name, const string &preis, const string &hnr) {
    if (!conn) return -1;
    
    string query = "UPDATE produkt SET name = $2, preis = $3, hnr = $4 WHERE pnr = $1";
    const char *paramValues[4] = { pnr.c_str(), name.c_str(), preis.c_str(), hnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 4, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Update product failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Update hersteller
int db_update_hersteller(const string &hnr, const string &name, const string &stadt) {
    if (!conn) return -1;
    
    string query = "UPDATE hersteller SET name = $2, stadt = $3 WHERE hnr = $1";
    const char *paramValues[3] = { hnr.c_str(), name.c_str(), stadt.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 3, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Update manufacturer failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Delete produkt
int db_delete_produkt(const string &pnr) {
    if (!conn) return -1;
    
    string query = "DELETE FROM produkt WHERE pnr = $1";
    const char *paramValues[1] = { pnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 1, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Delete product failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Delete hersteller (und abhängige Produkte)
int db_delete_hersteller(const string &hnr) {
    if (!conn) return -1;
    
    string query = "DELETE FROM hersteller WHERE hnr = $1";
    const char *paramValues[1] = { hnr.c_str() };
    
    PGresult *res = PQexecParams(conn, query.c_str(), 1, NULL, paramValues, NULL, NULL, 0);
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Delete manufacturer failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}

// Delete all data from both tables
int db_delete() {
    if (!conn) return -1;
    
    PGresult *res = PQexec(conn, 
        "DELETE FROM produkt;"
        "DELETE FROM hersteller;"
    );
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Delete all data failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return -1;
    }
    
    PQclear(res);
    return 0;
}