# Projet Informatique graphique 3D : The Maze Runner

## DeadLine: 1er juin 2023

## liste des tâches: 

**obligatoire**: 
+ afficher une carte vierge
+ générer un terrain plat
+ générer les positions des murs de manière aléatoire
+ générer les murs avec texture
+ création des interruptions clavier/ modification de la position 
+ empêcher de boucher sur murs 
+ générer message de début et message de fin 

**bonus**: 
+ générer bestioles 
+ générer lumière de nuit 

## 18/04/2023! 

ivan: générer scène vierge + terrain 

hélène: générer position aléatoire des murs + position personnage 

## 25/04/2023: 

ivan: génération graphique des murs sur position avec texture 

hélène: interruptions clavier et mise à jour de la position personnage

## vacances: 

ivan: générer message début/fin

facile: afficher "let's go" eu début et à la fin message "félicitations"

hélène: empêcher de bouger où il y a un mur 

## 09/05/2023: 

ivan: lumière de nuit 

hélène: création de créature qui bouge 

## 16/05/2023: animation créature dans le ciel 

ivan: 

hélène: 

## 23/05/2023: 

ivan: 

hélène: 

## 30/05/2023: 

ivan: 

hélène: 

## A l'intention de ma chère Hélène : 
Pour que tout ce passe bien je te suggère d'effectuer les étapes suivantes pendant la phase de configuration.


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
4. Là tu peux faire comme on fait d'habitude chaque fois qu'on a un nouveau TD. Comme le rappel profite au croyant : 
* Ouvrir CMake
* Remplir le champ **Where is the source code** avec le path du dossier maze_runner disons `path\maze_runner`
* Remplir le champ **Where to build the binaries** avec `path\maze_runner\build`
* Cliquer sur **Configure**, etc...
Là on devrait être bons et avoir la même structure du dossier `maze_runner`.

### Pour le dev en local
1. Créer une branche pout toi soit sur l'interface graphique soit en utilisant
```
git branch helene
git checkout helene
```
2. Faire un rebase (aligner ta branche avec la branche main) soit grâce à l'interface graphique soit en utilisant
```
git rebase mainn
```
3. Coder ...
