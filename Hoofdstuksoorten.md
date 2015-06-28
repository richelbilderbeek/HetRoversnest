# Hoofdstuksoorten

### 0: Verhaalbestand

Een bestand waarin een tekst wordt getoond en de speler kan kiezen uit meerdere opties:

```
[tekst]
@0
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
[tekst]
@1
@0:[tekst bij pech]@[hoodstuk bij pech]
@1:[tekst bij geluk]@[hoodstuk bij geluk]
```

 * 1: code van een Test-Je-Geluk
 * [tekst]: de tekst tot het geluk bepaald wordt
 * [tekst bij pech]: de tekst die de speler ziet bij pech
 * [hoodstuk bij pech]: het hoofstuk waar de speler heen gaat bij pech
 * [tekst bij geluk]: de tekst die de speler ziet bij geluk
 * [hoodstuk bij geluk]: het hoofstuk waar de speler heen gaat bij geluk

Voorbeeld van een 'Test-Je-Geluk' hoofdstukken zijn

  * [15](Bestanden/15.txt).
  * [49](Bestanden/49.txt).

### 2: Test-Je-Behendigheid

Een bestand waarin een tekst wordt getoond, de behendigheid wordt getest, 
en er een tekst is bij geen behendigheid of wel behendigheid en een hoofdstuk erna.

```
[tekst]
@2
@0:[tekst bij geen behendigheid]@[hoodstuk bij geen behendigheid]
@0:[tekst bij wel behendigheid]@[hoodstuk bij wel behendigheid]
```

 * 2: code van een Test-Je-Behendigheid
 * [tekst]: de tekst tot het geluk bepaald wordt
 * [tekst bij geen behendigheid]: de tekst die de speler ziet bij pech
 * [hoodstuk bij pech]: het hoofstuk waar de speler heen gaat bij pech
 * [tekst bij geluk]: de tekst die de speler ziet bij geluk
 * [hoodstuk bij geluk]: het hoofstuk waar de speler heen gaat bij geluk

Een voorbeeld van een Test-Je-Behendigheid is [69](Bestanden/69.txt).

### 3: Wijziging van je status

Een bestand waarin een tekst wordt getoond en een status (conditie, behendigheid of geluk) wordt gewijzigd

```
[tekst]
@3
@[wat][plus of min][getal]
@[volgende hoofdstuk]
```

 * 3: code van een wijziging van je status
 * [tekst]: de tekst die getoond wordt
 * [wat]: de status die gewijzigd wordt: `D` = Behendigheid ('Dexterity'), `S` = Conditie ('Stamina'), `L` = Geluk ('Luck'), `I` = Voorwerp ('Item')
* [plus of min]: is of `+` of `-`
* [getal]: getal die aangeeft hoeveel de status veranderd
* [volgende hoofdstuk]: het nummer van het volgende hoofdstuk

Het [wat] gedeelte is wat ingewikkelder. Er kunnen meerdere dingen gebeuren, deze worden gescheiden met een komma. Ook kan er een vraag in gesteld worden ('Bezit je een malienkolder?') met een vraagteken. Ook kunnen er voorwerpen verloren worden

Enkele voorbeelden van [wat]:

 * `@D-1`: verlies 1 behendigheid
 * `@D-1,S-1`: verlies 1 behendigheid en een conditie
 * `@I01-`: verlies het schild 
 * `@I02+`: verkrijg de malienkolder
 * `@I02?D-2`: heb je een malienkolder, verlies dan twee behendigheidpunten
 * `@I02?I02-`: heb je een malienkolder, verlies dan je malienkolder
 * `@I02?D-2,@I02?I02-`: heb je een malienkolder, verlies dan twee behendigheidpunten en je malienkolder

Een voorbeeld van een Wijziging van je status is

 * [8](Bestanden/8.txt)
 * [11](Bestanden/11.txt)

Zie [nummers van de voorwerpen](Voorwerpnummers.md) voor de nummers van de voorwerpen.

### 4: Gevecht binnen een paar beurten

```
[tekst]
@4
@[naam monster(s)]
@[behendigheid]
@[conditie]
@[aantal rondes voordat je onderbroken wordt]
@[hoofdstuk bij onderbreking]
@[hoofdstuk bij klaar voor voor onderbreking]
```

Een voorbeeld van een Gevecht binnen een paar beurten is [10](Bestanden/10.txt)

### 5: Game over

```
[tekst]
@5
```

Een voorbeeld van een 'Game over' is [9](Bestanden/9.txt)

### 6: Heb je een voorwerp?

Een bestand waarin een tekst wordt getoond en afhankelijk
van het bezit van een voorwerp naar een ander hoofdstuk 
wordt gegaan

```
[tekst]
@6
@[voorwerpnummer]
@[hoofdstuk bij niet bezitten van voorwerp]
@[hoofdstuk bij wel bezitten van voorwerp]

```

 * [tekst]: de tekst die getoond wordt
 * 6: code van een 'Heb je een voorwerp?' hoofdstuk
 * [voorwerpnummer]: het nummer van het voorwerp
 * [hoofdstuk bij niet bezitten van voorwerp]: hoofdstuk waar je heen gaat als je het voorwerp niet hebt
 * [hoofdstuk bij wel bezitten van voorwerp]: hoofdstuk waar je heen gaat als je het voorwerp wel hebt

Een voorbeeld van een 'Heb je een voorwerp?' is

 * [14](Bestanden/14.txt)

Zie [nummers van de voorwerpen](Voorwerpnummers.md) voor de nummers van de voorwerpen.