1# Webentwicklung Probeklausur 

## Aufgabe 1: Grundlagen & Konzepte (15 Punkte)

a) Was ist der Unterschied zwischen einem Lastenheft und einem Pflichtenheft? (4P)

b) Beschreiben Sie drei wesentliche Technologien des Web 3.0 und erläutern Sie kurz deren Bedeutung. (6P)

c) Erklären Sie den Unterschied zwischen dem WWW 1.0 und dem aktuellen Web. Wer gilt als Gründer des WWW? (5P)

## Aufgabe 2: Client-Server & Browser (20 Punkte)

a) Zeichnen Sie eine Skizze, die den Weg einer HTML-Datei vom Server zum Client darstellt. Beschriften Sie alle wichtigen Komponenten. (8P)

b) Der Browser-Code ist deutlich umfangreicher als der Linux-Kernel. Erklären Sie, warum das so ist und nennen Sie die ungefähren Größenordnungen. (4P)

c) Beschreiben Sie den Prozess, wie ein Browser eine HTML-Seite verarbeitet (vom HTML-Code bis zur Darstellung). Nennen Sie dabei die wichtigsten Schritte wie DOM-Tree und Render-Tree. (8P)

## Aufgabe 3: HTML & Fehlertoleranz (15 Punkte)

a) Gegeben ist folgender HTML-Code mit Fehlern. Wie wird der Browser damit umgehen? Erklären Sie die Fehlertoleranz.
```html
<div>
  <p>Hallo
  <strong>Welt</p>
  </div>
```
(5P)

b) Nennen Sie die erste HTML-Version (mit Jahr) und die aktuellste Version (mit Jahr). (4P)

c) Was ist der Unterschied zwischen einem HTML-Element und einem HTML-Attribut? Geben Sie jeweils zwei Beispiele. (6P)

## Aufgabe 4: JavaScript (25 Punkte)

a) Was ist die Ausgabe folgenden Codes? Erklären Sie das Ergebnis.
```javascript
console.log(5 + "3");
console.log([] + {});
console.log(typeof NaN);
```
(6P)

b) Erstellen Sie eine Tabelle mit JavaScript. Die Tabelle soll 3x3 Felder haben und bei Klick auf eine Zelle soll sich deren Hintergrundfarbe ändern. (10P)

c) Was ist der Unterschied zwischen innerHTML und textContent? Wann sollte man welches verwenden? (4P)

d) Schreiben Sie Code, der ein Bild mit der ID "myImage" findet und dessen src-Attribut ändert. (5P)

## Aufgabe 5: CSS (25 Punkte)

a) Erklären Sie das Box-Model anhand einer beschrifteten Skizze. Was ist der Unterschied zwischen border-box und content-box? (8P)

b) Was ist der Unterschied zwischen Block-, Inline- und Inline-Block-Elementen? Geben Sie jeweils ein Beispiel. (6P)

c) Gegeben sind folgende CSS-Selektoren. Was bedeuten sie?
```css
div > p
div + p
.class1.class2
div[attr="value"]
```
(4P)

d) Erklären Sie das Konzept der CSS-Spezifität. Welcher dieser Selektoren hat die höchste Spezifität?
```css
#header
.navigation > li
div.active[data-role="tab"]
```
(7P)

## Aufgabe 6: Responsive Design (15 Punkte)

a) Was ist Responsive Web Design? Welche drei Kernkomponenten werden dafür benötigt? (5P)

b) Schreiben Sie eine Media Query für Tablets (Bildschirmbreite zwischen 768px und 1024px). (5P)

c) Wann verwendet man CSS Grid und wann Flexbox? Nennen Sie jeweils einen typischen Anwendungsfall. (5P)

## Aufgabe 7: Sicherheit & HTTP (20 Punkte)

a) Erklären Sie den TLS-Handshake in 4-5 Schritten. (6P)

b) Was sind die Unterschiede zwischen Forward Proxy und Reverse Proxy? Zeichnen Sie für beide eine Skizze. (8P)

c) Ordnen Sie diese HTTP-Statuscodes den richtigen Kategorien zu: 404, 500, 301, 200 (6P)

## Aufgabe 8: DOM & AJAX (15 Punkte)

a) Nennen und erklären Sie vier Methoden zur DOM-Navigation (z.B. appendChild). (8P)

b) Was ist AJAX? Welche HTTP-Request-Typen gibt es und wofür werden sie verwendet? (7P)

## Bonus-Aufgabe (5 Punkte)
Erklären Sie den Unterschied zwischen Hugo und WordPress als Content Management Systeme.