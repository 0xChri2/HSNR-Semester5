# Musterlösung Webentwicklung Probeklausur

## Aufgabe 1: Grundlagen & Konzepte (15 Punkte)

### a) Lastenheft vs. Pflichtenheft (4P)
- **Lastenheft**: Beschreibt die Anforderungen des Auftraggebers ("WAS wird benötigt?")
  - Enthält die Gesamtheit der Forderungen an die Lieferungen und Leistungen
  - Wird vom Auftraggeber erstellt
- **Pflichtenheft**: Beschreibt die technische Umsetzung ("WIE wird es umgesetzt?")
  - Detaillierte Beschreibung der Realisierung aller Anforderungen
  - Wird vom Auftragnehmer erstellt

### b) Web 3.0 Technologien (6P)
1. **Blockchain**:
   - Dezentrale, unveränderliche Datenbank
   - Ermöglicht transparente und sichere Transaktionen
   - Basis für Kryptowährungen und Smart Contracts

2. **Semantic Web**:
   - Maschinen können Bedeutung von Daten verstehen
   - Verbesserte Datenverlinkung und -interpretation
   - Ermöglicht intelligentere Suchergebnisse

3. **Künstliche Intelligenz**:
   - Personalisierte Benutzererfahrung
   - Automatisierte Datenanalyse
   - Verbesserte Sprachverarbeitung

### c) WWW 1.0 vs. aktuelles Web & Gründer (5P)
- **WWW 1.0**: 
  - Statische Webseiten
  - Nur Lese-Zugriff für Benutzer
  - Keine Interaktivität
  
- **Aktuelles Web**:
  - Dynamische, interaktive Inhalte
  - Benutzer können Inhalte erstellen und teilen
  - Social Media, Cloud-Computing, mobile Anwendungen

- **Gründer**: Tim Berners-Lee (1989)

## Aufgabe 2: Client-Server & Browser (20 Punkte)

### a) Skizze HTML-Datei Weg (8P)
```
[Client/Browser] <----HTTP Request---- [DNS Server]
       ↑                                   ↑
       |                                   |
       ↓                                   ↓
[Cache/RAM] <----HTTP Response---- [Webserver]
                  (HTML-Datei)
```

### b) Browser-Code vs. Linux-Kernel (4P)
- Browser-Code: ca. 300.000 Zeilen
- Linux-Kernel: ca. 70.000 Zeilen
- Gründe für Umfang:
  - Komplexe Rendering-Engine
  - Verschiedene Web-Standards
  - Sicherheitsmechanismen
  - Fehlertoleranz
  - Multimedia-Unterstützung

### c) Browser-Verarbeitungsprozess (8P)
1. **HTML-Parsing**:
   - Einlesen des HTML-Codes
   - Tokenisierung
   - Syntax-Überprüfung

2. **DOM-Tree-Erstellung**:
   - Hierarchische Strukturierung
   - Elementbeziehungen werden abgebildet

3. **CSSOM-Erstellung**:
   - CSS-Regeln werden verarbeitet
   - Styling-Informationen werden strukturiert

4. **Render-Tree-Konstruktion**:
   - Kombination aus DOM und CSSOM
   - Nur sichtbare Elemente
   - Layout-Berechnung

5. **Painting**:
   - Finale Darstellung auf dem Bildschirm

## Aufgabe 3: HTML & Fehlertoleranz (15 Punkte)

### a) HTML-Fehlertoleranz (5P)
```html
<div>
  <p>Hallo
  <strong>Welt</p>
  </div>
```
Browser-Verhalten:
- Fehlende schließende `<strong>`-Tag wird automatisch ergänzt
- Falsche Verschachtelung wird korrigiert
- Resultat wird sein:
```html
<div>
  <p>Hallo
    <strong>Welt</strong>
  </p>
</div>
```

### b) HTML-Versionen (4P)
- Erste Version: HTML 1.0 (1991/1992)
- Aktuellste Version: HTML5 (2014)

### c) Element vs. Attribut (6P)
- **Elemente**: Strukturelle Komponenten
  - Beispiele: `<p>`, `<div>`, `<a>`, `<img>`
  
- **Attribute**: Zusätzliche Eigenschaften von Elementen
  - Beispiele: `class="..."`, `id="..."`, `src="..."`, `href="..."`

## Aufgabe 4: JavaScript (25 Punkte)

### a) Code-Ausgabe (6P)
```javascript
console.log(5 + "3");     // "53" (String-Konkatenation)
console.log([] + {});     // "[object Object]" (Beide werden zu Strings konvertiert)
console.log(typeof NaN);  // "number" (NaN ist vom Typ number)
```

### b) JavaScript-Tabelle (10P)
```javascript
function createTable() {
    const table = document.createElement('table');
    
    for (let i = 0; i < 3; i++) {
        const row = document.createElement('tr');
        for (let j = 0; j < 3; j++) {
            const cell = document.createElement('td');
            cell.textContent = `Zelle ${i},${j}`;
            cell.onclick = function() {
                this.style.backgroundColor = 
                    this.style.backgroundColor ? '' : '#lightblue';
            };
            row.appendChild(cell);
        }
        table.appendChild(row);
    }
    
    document.body.appendChild(table);
}
```

### c) innerHTML vs. textContent (4P)
- **innerHTML**:
  - Interpretiert HTML-Tags
  - Kann sicherheitsrelevant sein (XSS)
  - Für HTML-Inhalte

- **textContent**:
  - Behandelt alles als Text
  - Sicherer
  - Für reinen Text

### d) Bild-Attribut ändern (5P)
```javascript
const img = document.getElementById("myImage");
if (img) {
    img.src = "neue-url.jpg";
}
```

## Aufgabe 5: CSS (25 Punkte)

### a) Box-Model & Box-Sizing (8P)
```
┌──────── Margin ────────┐
│  ┌─── Border ───┐     │
│  │  ┌─ Padding ┐│     │
│  │  │  Content ││     │
│  │  └─────────┘│     │
│  └─────────────┘     │
└────────────────────────┘
```
- **content-box**: Width/Height bezieht sich nur auf Content
- **border-box**: Width/Height inkludiert Padding und Border

### b) Block vs. Inline vs. Inline-Block (6P)
- **Block**: 
  - Neue Zeile
  - Volle Breite
  - Beispiel: `<div>`

- **Inline**:
  - Im Textfluss
  - Keine Höhe/Breite
  - Beispiel: `<span>`

- **Inline-Block**:
  - Im Textfluss
  - Mit Höhe/Breite
  - Beispiel: `<button>`

### c) CSS-Selektoren (4P)
```css
div > p           /* Direkte p-Kinder von div */
div + p           /* p direkt nach div */
.class1.class2    /* Element mit beiden Klassen */
div[attr="value"] /* div mit Attribut attr="value" */
```

### d) CSS-Spezifität (7P)
Höchste Spezifität: `#header`
- #header: 100 Punkte
- .navigation > li: 11 Punkte
- div.active[data-role="tab"]: 22 Punkte

## Aufgabe 6: Responsive Design (15 Punkte)

### a) Responsive Web Design (5P)
- Definition: Webdesign, das sich an verschiedene Bildschirmgrößen anpasst
- Kernkomponenten:
  1. Flexible Grids
  2. Flexible Medien
  3. Media Queries

### b) Media Query (5P)
```css
@media screen and (min-width: 768px) and (max-width: 1024px) {
    /* Tablet-spezifische Styles */
}
```

### c) Grid vs. Flexbox (5P)
- **Grid**: 
  - Zweidimensionales Layout
  - Beispiel: Komplettes Seitenlayout

- **Flexbox**:
  - Eindimensionales Layout
  - Beispiel: Navigationmenü

## Aufgabe 7: Sicherheit & HTTP (20 Punkte)

### a) TLS-Handshake (6P)
1. Client sendet "Client Hello" mit unterstützten Verschlüsselungsmethoden
2. Server antwortet mit "Server Hello" und Zertifikat
3. Client überprüft Zertifikat und sendet symmetrischen Schlüssel
4. Server bestätigt verschlüsselte Verbindung
5. Sichere Kommunikation beginnt

### b) Forward vs. Reverse Proxy (8P)
**Forward Proxy**:
```
[Client] → [Forward Proxy] → [Internet] → [Server]
```
- Schützt Client-Identität
- Filtert ausgehenden Traffic

**Reverse Proxy**:
```
[Client] → [Internet] → [Reverse Proxy] → [Server]
```
- Schützt Server-Identität
- Lastverteilung
- Caching

### c) HTTP-Statuscodes (6P)
- 200: Erfolg (2xx - Erfolgreiche Operationen)
- 301: Umleitung (3xx - Weiterleitungen)
- 404: Nicht gefunden (4xx - Client-Fehler)
- 500: Server-Fehler (5xx - Server-Fehler)

## Aufgabe 8: DOM & AJAX (15 Punkte)

### a) DOM-Navigation (8P)
1. `appendChild()`: Fügt Kind-Element hinzu
2. `parentNode`: Greift auf Eltern-Element zu
3. `childNodes`: Liste aller Kind-Elemente
4. `nextSibling`: Nächstes Element auf gleicher Ebene

### b) AJAX & HTTP-Requests (7P)
- **AJAX**: Asynchronous JavaScript and XML
  - Ermöglicht asynchrone Serveranfragen
  - Ohne Seiten-Neuladen

**HTTP-Request-Typen**:
- GET: Daten abrufen
- POST: Daten senden
- PUT: Daten aktualisieren
- DELETE: Daten löschen

## Bonus-Aufgabe (5 Punkte)
**Hugo vs. WordPress**:
- **Hugo**:
  - Statischer Site-Generator
  - Keine Datenbank nötig
  - Schnellere Ladezeiten
  - Sicherer durch weniger Angriffsfläche

- **WordPress**:
  - Dynamisches CMS
  - Datenbank-basiert
  - Mehr Funktionalität
  - Benutzerfreundlicher Editor