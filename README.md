
---
title: "Tries"
course: AD3
year: 2022-2023
---

# Opgave project AD3

Dit jaar onderzoeken we in het project de verschillende soorten tries en wanneer je welke implementatie het best gebruikt. Als de strings die je zal opslaan veel gemeenschappelijk hebben (bijvoorbeeld: familienamen) dan kan je daarvan gebruik maken om opzoekingen te versnellen. Voor strings die willekeurig verdeeld zijn (bijvoorbeeld: hashes van wachtwoorden) zal zo'n optimalisatie niet toepasbaar zijn, maar misschien kan je dan op andere manieren optimaliseren.

## Functionele vereisten programma

Implementeer drie bibliotheken: `arraytrie`, `ternarytrie` en `customtrie` die elk de implementatie zijn van een soort trie:

- `arraytrie`: een implementatie van een trie waar in elke top de kinderen zijn opgeslagen in een array en waarbij paden gecomprimeerd worden, bijvoorbeeld zoals in **Patricia** tries uit de lesnota's, of op een andere manier.
- `ternarytrie`: een implementatie van een ternary trie waar elke top door een kleine binaire boom wordt voorgesteld en waarbij je geen paden hoeft te comprimeren.
- `customtrie`: een eigen implementatie van een trie die zo goed mogelijk presteert voor een gegeven dataset.


## Specificaties

### Programmeertaal

In de opleidingscommissie informatica (OCI) werd beslist dat, om meer ervaring met het programmeren in C te verwerven, het project van Algoritmen en Datastructuren 3 in C geïmplementeerd moet worden.
Het is met andere woorden de bedoeling je implementatie in C uit te voeren. Je implementatie moet voldoen aan de ANSI-standaard.
Je mag hiervoor dus gebruikmaken van de laatste features in C17, voor zover die ondersteund worden door de `C` compiler die gebruikt wordt voor dit project op SubGIT (GCC 11).

Voor het project kun je de standaard libraries gebruiken, externe libraries
zijn echter niet toegelaten. Het spreekt voor zich dat je normale, procedurele
C-code schrijft en geen platformspecifieke APIs (zoals bv. de Win32 API) of
features uit C++ gebruikt. Op Windows bestaat van enkele functies zoals `qsort`
een "safe" versie (in dit geval `qsort_s`), maar om je programma te kunnen
compileren op een Unix-systeem kun je die versie dus niet gebruiken. Let er ook op dat je bibliotheek geen geheugenlekken bevat.

**Wat je ontwikkelingsplatform ook mag zijn, controleer geregeld de output van
SubGIT om te verifiëren dat je programma ook bij ons compileert en slaagt voor de minimale testen!**

### Headerbestand

In de opgave zal je drie headerbestanden vinden die de C-functies beschrijven die je bibliotheek moet implementeren: `arraytrie.h`, `ternarytrie.h` en `customtrie.h`. Dit is voor elk van deze headerbestanden gelijkaardig:

- een functie `ArrayTrie* arraytrie_init();` die een nieuwe trie alloceert en initialiseert
- een functie `bool arraytrie_search(ArrayTrie* trie, char* string);` die de gegeven `string` opzoekt in de trie
- een functie `bool arraytrie_add(ArrayTrie* trie, char* string);` die de gegeven `string` toevoegt aan de trie
- een functie `bool arraytrie_remove(ArrayTrie* trie, char* string);` die de gegeven `string` verwijdert uit de trie
- een functie `size_t arraytrie_size(ArrayTrie* trie);` die teruggeeft hoeveel strings er in de huidige trie zitten
- een functie `void arraytrie_free(ArrayTrie* trie);` die de gegeven trie leegmaakt en het geheugen terug vrijgeeft

De prefix voor elke functie en de naam van de struct (hier: `arraytrie_` en `ArrayTrie`) verschilt natuurlijk per implementatie. Bekijk de headerbestanden voor meer details over wat de functies precies moeten doen.

### Datasets

In het mapje `data/` vind je uitleg over hoe je de datasets kan downloaden om je implmentatie mee te testen en te benchmarken. Dit kunnen vrij grote bestanden zijn, voeg deze bestanden dus niet toe aan je repository.

Tijdens het verbeteren zullen we gelijkaardige bestanden gebruiken om jullie tijd- en geheugengebruik mee te meten. Het loont dus zeker de moeite om voor deze bestanden te optimaliseren.

### Limieten

We voorzien programma's `test_{arraytrie,ternary,custom}` die de
basisfunctionaliteit van elke trie uittest. Je moet tenminste voor al deze testen slagen om punten te krijgen voor de overeenkomstige trie. Natuurlijk is het ook de bedoeling om zelf ook testen te schrijven om garanderen dat jouw implementatie volledig correct is. Je mag hiervoor gebruik maken van de meegeleverde `test.h`-header van het framework [acutest](https://github.com/mity/acutest), maar dat is niet verplicht.

Het is niet toegestaan om bestanden aan te maken of sockets te openen vanuit je
bibliotheek. Je mag ook niet uitschrijven naar stdout of stderr, of stdin
uitlezen. Je programma moet single threaded zijn en mag dus niet forken.

We zullen je geheugengebruik meten met behulp van de `massif` tool in `valgrind`:

```sh
valgrind --tool=massif --stacks=yes --massif-out-file=memory.ms -- ./test_arraytrie
```

De bovenstaande lijn zal het testprogramma (`./test_arraytrie`) uitvoeren met de [Valgrind Massif](https://www.valgrind.org/docs/manual/ms-manual.html) tool, die zal een `memory.ms` bestand genereren die het geheugengebruik beschrijft. Je kunt dit bestand gewoon openen met een teksteditor of je kunt `ms_print` gebruiken om een ASCII art grafiekje te krijgen die het geheugengebruik doorheen de tijd toont. Met de [massif-visualizer](https://github.com/KDE/massif-visualizer) tool kun je een mooiere weergave krijgen (met kleuren en grafieken waar je op kunt klikken).

*Tips*:

- Compileer je programma met `-g` om debuginformatie aan te zetten zodat deze tools mooi kunnen tonen waar er geheugen gealloceerd is.
- De dumpbestanden van Valgrind Massif gebruiken de notatie `kB` voor 1024 byte, een correctere notatie zou kiB zijn.
- Valgrind heeft ook een [memcheck](https://www.valgrind.org/docs/manual/mc-manual.html) tool waarmee je memory leaks kunt opsporen.
- Je programma kan tot 100 keer trager worden tijdens het runnen in `valgrind`.

## Onderzoek

Voor dit project onderzoeken we wanneer je welke trie het best gebruikt voor de gegeven datasets op het vlak van geheugengebruik en snelheid. Bespreek dit uitgebreid: in verschillende situaties zullen andere tries beter presteren, of zal je sommige tries zelfs helemaal niet kunnen gebruiken. Geef telkens een duidelijke verklaring.

*Tip:* Als je metingen doet, compileer je natuurlijk met zo veel mogelijk optimalisaties aan (compileer met `-O3`)

## Verslag

Schrijf een bondig verslag over je werk met volgende componenten:

- [ ] De ontwerpbeslissingen die je nam voor de implementaties, in het bijzonder van je `customtrie`;
- [ ] Hoe je precies meer of minder geheugen gebruikt;
- [ ] Korte analyse van de tijdscomplexiteit van de bewerkingen van jouw datastructuren;
- [ ] Studie van de optimale trie voor de gegeven datasets, waarbij je een verklaring geeft waarom die trie beter werkt en die verklaring staaft met relevante benchmarks.

Maak je verslag niet onnodig lang.

## Indienen

### Directorystructuur

Je indiening moet volgende structuur hebben:

- `src/` bevat alle broncode, in de bestanden `extra-sources-{array,ternary,custom}trie` kan je extra bronbestanden meegeven die ook moeten gecompileerd worden

- `include/` bevat de headerbestanden die wij je gegeven hebben, deze mag je niet aanpassen. Je mag hier ook geen extra headerbestanden aan toevoegen: die voeg je toe in `src/`. De overeenkomstige `c`-bestanden van extra headerbestanden plaats je in de `extra-sources`-bestanden.

- `tests/` alle testcode (je moet testcode hebben, push geen grote datafiles
  naar SubGIT, plaats een scriptje dat deze genereert).

- `extra/verslag.pdf` bevat de elektronische versie van je verslag. In deze map
  kun je ook eventueel extra bijlagen plaatsen zoals de broncode van je verslag.

Je directory structuur ziet er dus ongeveer zo uit:

    |
    |-- extra/
    |   `-- verslag.pdf
    |-- src/
    |   |-- extra-sources-arraytrie (optioneel)
    |   |-- extra-sources-ternarytrie (optioneel)
    |   |-- extra-sources-customtrie (optioneel)
    |   `-- je broncode
    |-- include/
    |   |-- arraytrie.h
    |   |-- ternarytrie.h
    |   `-- customtrie.h
    |-- test/
    |   `-- je testcode
    `-- data/
        `-- onze datasets

Al je bestanden moeten als UTF-8 opgeslagen zijn.

### Compileren

De code zal bij het indienen gecompileerd worden met (ruwweg) onderstaande
opdracht door SubGIT met **GCC 11**.

```sh
gcc -shared -std=c18 -O3 -Wall -Werror -lm src/arraytrie.c -o libarraytrie.so
gcc -shared -std=c18 -O3 -Wall -Werror -lm src/ternarytrie.c -o libternarytrie.so
gcc -shared -std=c18 -O3 -Wall -Werror -lm src/customtrie.c -o libcustomtrie.so
```

De `Dockerfile` en bijhorende bronbestanden die SubGIT gebruikt, kunnen gepulld worden van `git@SubGIT.UGent.be:2022-2023/AD3/project-dockerfile`. Je kan de testen van SubGIT lokaal uitvoeren met de volgende commando's in deze repository:

```sh
docker build . -t ad3-project-2022-2023 # hoef je maar één keer te doen
docker run -it --rm --mount type=bind,source={PAD},destination=/submission,readonly ad3-project-2022-2023
```

Waarbij `{PAD}` vervangen dient te worden door het absolute pad naar de hoofdmap
(niet `src`) van je code.

#### Extra bronbestanden

Wanneer je extra bronbestanden wilt gebruiken en die wilt include-en, dan heb je twee keuzes:

##### Single compilation unit

```c
#include "common.c"
```

Je include de extra c-bestanden rechtstreeks zodat alles tot één objectbestand wordt gecompileerd. Deze techniek heet [Single compilation unit (SCU)](https://en.wikipedia.org/wiki/Single_compilation_unit) en zorgt ervoor dat de compiler soms beter kan optimaliseren.

Dit heeft als nadeel dat wanneer je iets in één van de bestanden aanpast, je alles opnieuw moet compileren. Dit project is klein genoeg dat dit weinig effect zal hebben.

Als je deze techniek toepast hoef je niets extra te doen.

##### Header-files

```c
#include "common.h"
```

Je include een extra headerbestand waardoor je je code opsplitst in meerdere objectbestanden. Dit wordt over het algemeen gezien als betere programmeerstijl en is waarschijnlijk hoe je aangeleerd hebt om met meerdere bestanden te werken in C.

Omdat je enkel de headerbestanden include, weet de compiler niet welke c-bestanden er nog gecompileerd moeten worden. Die moet je in dit geval oplijsten in een extra bestand `src/extra-sources-{array,ternary,custom}trie` met hun relatief pad ten opzichte van de `src`-map.

Bijvoorbeeld: stel dat je een `arraytrie` hebt die de extra bestanden `src/utilities.c` en `src/common/common.c` nodig hebben, dan maak je een bestand `src/extra-sources-arraytrie` aan met als inhoud:

```
utilities.c
common/common.c
```



### SubGIT

Het indienen gebeurt via het [SubGIT](https://SubGIT.ugent.be/) platform. Indien je hier nog geen account op hebt, dien je deze aan te maken.

#### Repository afhalen

```bash
git clone git@SubGIT.ugent.be:2022-2023/AD3/project/{studentnr} projectAD3
```

#### Opgave als `upstream` instellen

Je kunt de opgave en boilerplate voor het project afhalen door de opgave
repository als upstream met volgende commando's in de `projectAD3` map:

```bash
git remote add upstream git@SubGIT.UGent.be:2022-2023/AD3/project-assignment
git pull upstream master
```

(Je kunt niet pushen naar de upstream, alleen de lesgevers kunnen dat.)

#### Feedback

Als je pusht naar SubGIT, zul je in je terminal te zien krijgen of je code
voldoet aan de minimumvereisten. In dat geval krijg je bij het pushen de melding
dat het pushen geslaagd is:

```
remote: Acceptable submission
```

Je kunt geen code pushen naar de `master` branch als die niet compileert of niet
aan de minimale vereisten voldoet. Je kunt echter wel pushen naar andere
branches en daar zal je push wel aanvaard worden.

#### De `master` branch

De `master` branch op SubGIT stelt jouw indiening voor. Je kunt voor de deadline
zoveel pushen als je wilt. Zorg ervoor dat je voor de deadline zeker je finale
versie naar de **`master`** branch hebt gepusht, want enkel die zal verbeterd worden.

Nieuwe versies van git gaan de standaard branchnaam instellen op `main`. Dit zorgt echter voor problemen met SubGIT. Je kan je `main`-branch hernoemen naar `master` met het commando:

```bash
git branch -m main master
```

#### Pushen naar een andere branch

Het is sterk aangeraden om regelmatig te pushen naar SubGIT. Als je code niet aanvaard wordt op de `master`-branch, kan je een nieuwe branch aanmaken met het volgende commando:

```bash
git switch -c develop # hier is 'develop' de nieuwe branch naam
git push origin develop
```
Voor oudere versies van git kan bovenstaande `push` nog een foutmelding geven omdat er geen upstream is ingesteld, in dat geval moet je de optie `-u` meegeven om die in te stellen.


#### Controleren of je zeker goed hebt ingediend

Je kunt jouw indiening bekijken door jouw repository nog eens te clonen in een
andere map

```bash
cd eenAndereMap
git clone git@SubGIT.ugent.be:2021-2022/AD3/project/{studentnr} projectAD3Controle
```

### Deadlines en belangrijke data

Zorg ervoor dat je een tussentijdse versie hebt gepusht naar een
branch (niet noodzakelijk `master`, moet zelfs niet compileren) op SubGIT voor **woensdag 2022-11-23 om 17:00:00**. De versie die je hier
upload zal niet bekeken worden, we vragen je dit te doen om er zeker van zijn
dat iedereen kan indienen. Als je niets pusht voor deze deadline verlies je al
je punten voor het project.

De code die op **woensdag 2022-12-07 om 17:00:00** op de `master` branch staat is je
finale code indiening. Enkel code die op dat moment op de `master` branch staat,
wordt verbeterd. **Als er geen code op je `master` branch staat, krijg je nul op vier voor het
project.** Je kunt na deze deadline nog steeds pushen om wijzigingen aan te
brengen aan het verslag.

Na **woensdag 2022-12-14 om 17:00:00** kun je ook helemaal geen wijzigingen meer
aanbrengen aan je repo. Het verslag dat aanwezig is als PDF op de `master`
branch in `extra/verslag.pdf` is je finaal verslag.

## Algemene richtlijnen

- Schrijf efficiënte code, maar ga niet over-optimaliseren: **geef de voorkeur
  aan elegante, goed leesbare code**. Kies zinvolle namen voor methoden en
  variabelen en voorzie voldoende commentaar. Let op geheugenhygiëne: ga actief
  op zoek naar geheugenlekken en alloceer niet onnodig veel geheugen.
- Op SubGIT staat een Dockerfile waarmee je een Linux container kunt bouwen die
  jouw code compileert en minimale testen uitvoert. Als alle testen slagen zal 
  dit programma `Acceptable submission` uitschrijven als laatste regel en 
  stoppen met exit code 0. Code die hier niet aan voldoet, zal geweigerd worden
  door SubGIT en levert geen punten op (0/4 voor het project).
- Het project wordt gequoteerd op **4** van de 20 te behalen punten voor dit
  vak, en deze punten worden ongewijzigd overgenomen naar de tweede
  examenperiode.
- Projecten die ons niet (via de `master` branch op SubGIT) bereiken voor de
  deadline worden niet meer verbeterd: dit betekent het verlies van alle te
  behalen punten voor het project.
- Dit is een individueel project en dient dus door jou persoonlijk gemaakt te
  worden. Het is uiteraard toegestaan om andere studenten te helpen of om ideeën
  uit te wisselen, maar **het is ten strengste verboden code uit te wisselen**,
  op welke manier dan ook. Het overnemen van code beschouwen we als fraude (van
  **beide** betrokken partijen) en zal in overeenstemming met het
  examenreglement behandeld worden. Op het internet zullen ongetwijfeld ook
  (delen van) implementaties te vinden zijn. Het overnemen of aanpassen van
  dergelijke code is echter **niet toegelaten** en wordt gezien als fraude. (Als
  je je code op GitHub/Bitbucket/sourcehut/Gitlab/… plaatst moet je die privaat
  houden)
- Essentiële vragen worden **niet** meer beantwoord tijdens de week voor
  de laatste deadline.

## Vragen

Als je vragen hebt over de opgave of problemen ondervindt, dan kun je je vraag
stellen tijdens het practicum op dinsdagvoormiddag. Contacteer je ons per mail,
stuur dan ook je studentennummer en de branch waarop de code met het probleem
zit mee. Zo kunnen we onmiddellijk je code pullen. Stuur geen screenshots van
code of foutboodschappen.

