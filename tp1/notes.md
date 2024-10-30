## Q1
sgdt : 


## Q2 
bit de granularité : blocs de 4KO (plus grand) et si pas de bit de granularité (petis blocs)

## Q3
en gros comprendre ou est chaque selector (index selector) et voirque dans le descripteur de fichier correspondant, le seg type correspond au type de selector

## Q4
flat : code selector a acces a toute la memoire et data selector a acces a toute la mémoire donc pas de ségrégation 

## Q5
set_gdtr pour set l'adresse de gdt

## Q13
**why ss can't load a data segment of ring 3 (but can load data segment of ring 0)**

selon prof, GP quand on load un un semgent data dpl 3 alors que boot infini (a verifier)
pour tout ce qui est segment register et ce qu'on peut mettre dedans (chercher chapitre 3 volume 3 doc )

### pourquoi tss.esp = ebp ?
d'après doc, on push esp et ss et on récupère esp et ss du handler de TSS. 
tss spécifie esp a utiliser quand il y a une interruption et qu'on est en ring 3 (privelege switch)

**le deuxième rôle principal de tss est le hardware-assisted task switching**

### A retenir
there is space in the tss for 3 stack segments and 3 stack pointers (for privilige 0, 1, and 2 en fonction de là on switch de privilege)