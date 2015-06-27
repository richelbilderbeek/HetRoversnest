# Het Roversnest

Het Roversnest is een Nederlands tekstavontuur boek, een vertaling van 'City of Thieves' van Ian Livingstone.

 * [Wikipedia](https://nl.wikipedia.org/wiki/Het_Roversnest)

![Voorkant van Het Roversnest](BoekVoorkant.jpg)

## Ik wil helpen!

Top! Email Richel of je een collaborator kunt worden.

## Hoe kan ik helpen?

Bekijk de 'Issues'. Begin eerst een paar van de gemakkelijke te doen. 

Dit zijn de gemakkelijkere:
 * Issue #3: Hoofdstukken toevoegen
 * Issue #11: Hoofdstukken verbinden

Moeilijker
 * (Issue #1: Display op LCD)
 * (Issue #2: Input met knoppen)
 * Issue #6: Introverhaal erin

Moeilijkst:
 * Issue #4: Vecht engine
 * Issue #5: Karakter generatie
 * Issue #7: Inventaris erin
 * Issue #12: Test-Je-Geluk engine

## Wat zijn al die bestanden in de folder `Bestanden`?

De bestanden bevatten het verhaal van Het Roversnest.

## Waarom staan al die bestanden in de folder `Bestanden`?

Dit project is in eerste instantie bedoeld voor Arduino. 
En een Arduino heeft niet genoeg RAM en FLASH geheugen om een heel boek op te kunnen slaan. 
Een mini-SD kaart van 2 GB kan met gemak een heel boek opslaan. 
Daarom is staan er in de folder `Bestanden` bestanden die je op een SD kaart kunt kopieeren. 
Het programma `HetRoversnest.ino` (in de map `HetRoversnest`) leest deze dan uit.

# Hoe moet ik de bestanden invoeren?

Dit is afhankelijk van het type bestand.

### 0: Verhaalbestand

Een bestand waarin een tekst wordt getoond en de speler kan kiezen uit meerdere opties:

```
0
[tekst]
@[optie]:[hoodstuk]
```

 * 0: code van een verhaalbestand
 * [tekst]: de tekst, inclusief keuzes die de speler kan maken
 * [optie]: de optie de de speler kiest, bijvoorbeeld `1`
 * [hoofdstuk]: het hoofstuk waar de speler dan heen gaat

Een voorbeeld van een verhaalbestand is [1](Bestanden/1.txt).

### 1: Test-Je-Geluk 

Een bestand waarin een tekst wordt getoond, het geluk wordt getest, 
en er een tekst is bij pech of geluk en een hoofdstuk erna.

```
1
[tekst]
@0:[tekst bij pech]@[hoodstuk bij pech]
@1:[tekst bij geluk]@[hoodstuk bij geluk]
```

 * 1: code van een Test-Je-Geluk
 * [tekst]: de tekst tot het geluk bepaald wordt
 * [tekst bij pech]: de tekst die de speler ziet bij pech
 * [hoodstuk bij pech]: het hoofstuk waar de speler heen gaat bij pech
 * [tekst bij geluk]: de tekst die de speler ziet bij geluk
 * [hoodstuk bij geluk]: het hoofstuk waar de speler heen gaat bij geluk

Een voorbeeld van een Test-Je-Geluks is [49](Bestanden/49.txt).

### 2: Test-Je-Behendigheid

Een bestand waarin een tekst wordt getoond, de behendigheid wordt getest, 
en er een tekst is bij geen behendigheid of wel behendigheid en een hoofdstuk erna.

```
2
[tekst]
@0:[tekst bij geen behendigheid]@[hoodstuk bij geen behendigheid]
@0:[tekst bij wel behendigheid]@[hoodstuk bij wel behendigheid]
```

 * 1: code van een Test-Je-Geluk
 * [tekst]: de tekst tot het geluk bepaald wordt
 * [tekst bij geen behendigheid]: de tekst die de speler ziet bij pech
 * [hoodstuk bij pech]: het hoofstuk waar de speler heen gaat bij pech
 * [tekst bij geluk]: de tekst die de speler ziet bij geluk
 * [hoodstuk bij geluk]: het hoofstuk waar de speler heen gaat bij geluk

Een voorbeeld van een Test-Je-Behendigheid is ...

## Hoe gebruik ik git en GitHub?

Volg de eerste keer de volgende stappen:

 1. Installeer git
 2. Download de GitHub naar je harde schijf
 3. Wijzig de code
 4. Benoem de wijzigingen
 5. Download de nieuwste versie van de GitHub
 6. Upload de verbeterde code
 7. Ga naar stap 3

Start de keer erop eenmalig stap 5, start dan bij 3.

### 1. Installeer git

 * Windows en Mac: download Git Bash via `httsp://git-scm.com/downloads`, dat installeren
 * Linux: in terminal: `sudo apt-get install git`

### 2. Download de GitHub naar je harde schijf

Start Git Bash (Windows en Mac) of een terminal (Linux). Type dan:

```
git clone https://github.com/richelbilderbeek/HetRoversnest
```

### 3. Wijzig de code

Pas de code aan.

### 4. Benoem de wijzigingen

Start Git Bash (Windows en Mac) of een terminal (Linux). 

Ga in de folder van de GitHub, bijvoorbeeld:

```
cd HetRoversnest
```

dan:

```
git add --all :/
```

en

```
git commit -m "Hoofdstukken toevoegd"
```

Gefeliciteerd! Je hebt je eerste commit gemaakt!

### 5. Download de nieuwste versie van de GitHub

Voor je de benoemde wijzigingen gaat uploaden is het een goed idee eerst de GitHub te updaten:

Start Git Bash (Windows en Mac) of een terminal (Linux). 

Ga in de folder van de GitHub.

Doe dan:

```
git pull
```

### 6. Upload de verbeterde code

Start Git Bash (Windows en Mac) of een terminal (Linux). Ga in de folder van de GitHub.

Doe dan:

```
git push
```

Nu wordt jouw naam en wachtwoord gevraagd. Als je je wachtwoord intypt zul je geen tekens zien verschijnen.

Gefeliciteerd! Je hebt je eerste commit geupload!
