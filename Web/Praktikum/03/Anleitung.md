# Anleitung zur Installation von CiviCRM in Wordpress auf einer VM mit Docker

## Voraussetzungen
- Debian-basierte virtuelle Maschine (VM) mit Docker installiert.
- Keine direkte Internetverbindung auf der VM (Dateien werden manuell übertragen).
- Ziel: Installation von CiviCRM als Plugin in Wordpress.

---

## Schritte

### 1. Wordpress mit Docker installieren

#### 1.1 Docker-Container erstellen
Führen Sie folgende Befehle aus, um Wordpress und die dazugehörige Datenbank zu starten:

```bash
docker network create wordpress-net

docker run --name wordpress-db --network wordpress-net -e MYSQL_ROOT_PASSWORD=strongpassword -e MYSQL_DATABASE=wordpress -e MYSQL_USER=wordpress -e MYSQL_PASSWORD=wordpresspass -d mariadb:latest

docker run --name wordpress --network wordpress-net -p 8080:80 -e WORDPRESS_DB_HOST=wordpress-db -e WORDPRESS_DB_USER=wordpress -e WORDPRESS_DB_PASSWORD=wordpresspass -e WORDPRESS_DB_NAME=wordpress -d wordpress:latest
```

#### 1.2 Zugriff auf Wordpress
Öffnen Sie einen Browser und gehen Sie zu:
```
http://<VM_IP>:8080
```
Folgen Sie den Anweisungen, um Wordpress zu konfigurieren.

---

### 2. CiviCRM herunterladen und bereitstellen

#### 2.1 Herunterladen der ZIP-Datei
Die Datei wird von der offiziellen URL heruntergeladen. Falls die VM keinen Internetzugang hat, führen Sie die folgenden Schritte aus:

1. **Auf einem lokalen Rechner:**
   ```bash
   curl -o civicrm.zip https://storage.googleapis.com/civicrm/civicrm-stable/stable/civicrm-stable-wordpress.zip
   ```

2. **Übertragen Sie die Datei zur VM:**
   ```bash
   scp civicrm.zip <USER>@<VM_IP>:/path/to/destination
   ```

3. **Kopieren Sie die Datei in den Wordpress-Container:**
   ```bash
   docker cp civicrm.zip wordpress:/var/www/html/wp-content/plugins/
   ```

---

### 3. Installation von CiviCRM im Docker-Container

#### 3.1 Zugriff auf den Container
Melden Sie sich am Wordpress-Container an:
```bash
docker exec -it wordpress bash
```

#### 3.2 Entpacken der Datei
Entpacken Sie die Datei im Plugin-Verzeichnis:
```bash
apt-get update && apt-get install -y unzip
unzip /var/www/html/wp-content/plugins/civicrm.zip -d /var/www/html/wp-content/plugins/
```

#### 3.3 Berechtigungen setzen
Setzen Sie die korrekten Berechtigungen für die Dateien:
```bash
chown -R www-data:www-data /var/www/html/wp-content/plugins/civicrm
```

---

### 4. Aktivieren des Plugins

1. Melden Sie sich in Wordpress an.
2. Gehen Sie zu `Plugins` > `Installierte Plugins`.
3. Aktivieren Sie das Plugin **CiviCRM**.

---

### 5. Testdaten erstellen

#### 5.1 Zugang zur CiviCRM-Oberfläche
1. Navigieren Sie zu `Administer` > `System Settings` > `Directories` und konfigurieren Sie die erforderlichen Pfade.
2. Erstellen Sie Testdaten für Haushalte und Personen:
   - **Haushalte:** Namen, Adressen.
   - **Personen:** Namen, Geburtsdaten.
   - **Beziehungen:** Personen zu Haushalten zuweisen.

---

### 6. Export der Daten

#### 6.1 Zugriff auf die CiviCRM-API
1. Gehen Sie zu `Support` > `Developer` > `API Explorer v4`.
2. Nutzen Sie die API, um Haushalte mit Kindern (0–16 Jahre) zu finden.
3. Exportieren Sie die Daten im gewünschten Format (CSV, JSON etc.).

#### 6.2 Skripte verwenden
Alternativ können Sie ein Skript (z. B. Python) schreiben, das API-Abfragen durchführt und die Ergebnisse exportiert. Beispiel für eine Python-API-Abfrage:

```python
import requests

url = "http://<WP_URL>/wp-json/civicrm/v4"
data = {
    "entity": "Household",
    "action": "get",
    "where": {"children.age": {"between": [0, 16]}},
    "fields": ["household_name", "address", "children"]
}

response = requests.post(url, json=data)
print(response.json())
```

---

### 7. Dokumentation und Abgabe

#### 7.1 Dokumentation erstellen
- **Installationsdokumentation:** Beschreiben Sie die Schritte zur Installation von Wordpress und CiviCRM.
- **Skripte:** Fügen Sie den Quellcode Ihrer API- oder SQL-Skripte hinzu.
- **Testdokumentation:** Dokumentieren Sie die Testdaten und Ergebnisse.

#### 7.2 Abgabe
- Pushen Sie die Dokumentation und den Quellcode in Ihr Git-Projekt.
- Stellen Sie sicher, dass das System zum Testzeitpunkt zugänglich ist.
