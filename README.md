# Projet Informatique graphique 3D : The Maze Runner

## DeadLine: 1er juin 2023

## liste des t�ches: 

**obligatoire**: 
+ afficher une carte vierge
+ g�n�rer un terrain plat
+ g�n�rer les positions des murs de mani�re al�atoire
+ g�n�rer les murs avec texture
+ cr�ation des interruptions clavier/ modification de la position 
+ emp�cher de boucher sur murs 
+ g�n�rer message de d�but et message de fin 

**bonus**: 
+ g�n�rer bestioles 
+ g�n�rer lumi�re de nuit 

## 18/04/2023! 

ivan: g�n�rer sc�ne vierge + terrain 

h�l�ne: g�n�rer position al�atoire des murs + position personnage 

## 25/04/2023: 

ivan: g�n�ration graphique des murs sur position avec texture 

h�l�ne: interruptions clavier et mise � jour de la position personnage

## vacances: 

ivan: g�n�rer message d�but/fin

facile: afficher "let's go" eu d�but et � la fin message "f�licitations"

h�l�ne: emp�cher de bouger o� il y a un mur 

## 09/05/2023: 

ivan: lumi�re de nuit 

h�l�ne: cr�ation de cr�ature qui bouge 

## 16/05/2023: animation cr�ature dans le ciel 

ivan: 

h�l�ne: 

## 23/05/2023: 

ivan: 

h�l�ne: 

## 30/05/2023: 

ivan: 

h�l�ne: 

## A l'intention de ma ch�re H�l�ne : 
Pour que tout ce passe bien je te sugg�re d'effectuer les �tapes suivantes pendant la phase de configuration.


### En ce qui concerne le projet
1. Faire un `pull` (maj le repos local)
2. Dans les fichiers `maze_runner\Makefile` et `maze_runner\scripts\Makefile_emscripten` Tu dois changer 
```PATH_TO_CGP = ../cgp/library/```
En 
```PATH_TO_CGP = [your path]/cgp/library/```
3. Dans le fichier `maze_runner\CMakeList.txt` pareil il faudrait changer 
```set(PATH_TO_CGP "../cgp/library/" CACHE PATH "Relative path to CGP library location") 
```
En 
```set(PATH_TO_CGP "[your path]/cgp/library/" CACHE PATH "Relative path to CGP library location") 
```
4. L� tu peux faire comme on fait d'habitude chaque fois qu'on a un nouveau TD. Comme le rappel profite au croyant : 
* Ouvrir CMake
* Remplir le champ **Where is the source code** avec le path du dossier maze_runner disons `path\maze_runner`
* Remplir le champ **Where to build the binaries** avec `path\maze_runner\build`
* Cliquer sur **Configure**, etc...
L� on devrait �tre bons et avoir la m�me structure du dossier `maze_runner`.

### Pour le dev en local
1. Cr�er une branche pout toi soit sur l'interface graphique soit en utilisant
```
git branch helene
git checkout helene
```
2. Faire un rebase (aligner ta branche avec la branche main) soit gr�ce � l'interface graphique soit en utilisant
```
git rebase mainn
```
3. Coder ...
