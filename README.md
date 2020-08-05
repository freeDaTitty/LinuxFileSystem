# LinuxFileSystem
Virtual filesystem and some virtual commands

- graf.h

    Fisierul contine definitiile functiilor si al structurii.

    CHAR_SIZE este o variabila in care retin toate caracterele
care urmeaza a fi utilizate. De la a la z, de la 0 la 9, plus
caracterele speciale *, ., /

	Structura este compusa din 4 campuri prin care retin:
type - tipul acestuia. 1 - fisier, 2 director, 0 - orice altceva
isLeaf - 1 in cazul in care e frunza
parent retine parintele unui nod
v[CHAR_SIZE] contine toti copiii structurii de tip trie.

	Functiile ajutatoare:
 - despart o cale in subdiviziuni pentru o parcurgere mai usoara
a structurii
 - verific daca un director este gol
 - parcurg structura printr o indexare si, cum nu pot sa
folosesc caractere pe post de indexuri, le voi transpune
in ceva de care ma pot folosi.

graf-L.c

Fisierul contine implementarile tuturor functiilor necesare
pentru rezolvarea problemei.

	-Split_Path
Voi parcurge primul parametru si il voi imparti in 2 bucati
file_name - numele fisierului
root_path - calea catre acest
Spre exemplu: In /a/b/c: /a/b - root_path, c - fisierul

	-mkdir
Functia va parcurge structura, iar acolo unde nu s a alocat
inca un anumit caracter, il va introduce ca fiind fiul precedentului
nod din arborele de tip trie.

	-cd
In functie de parametrul primit, functia va cauta prin toata
structura daca exista directorul primit ca parametru, iar apoi
va fixa locatia pe acel director. Daca se va primi ca parametru
un fisier, variabila is_file va returna valoarea 1, iar daca
directorul pe care vrem sa ajungem nu exista in structura, va
fi returnat NULL. In cazul in care se ajunge, va fi returnat
nodul pe care a trebuit sa ne ducem.

	-pwd

Voi parcurge structura de la nodul curent si voi retine
parintele nodului respectiv. In cazul in care suntem in root,
parintele va fi NULL. Voi retine in variabila location fiecare
caracter care mi trebuie din structura si voi parcurge in continuare
fiecare parinte pana cand voi ajunge in locatia necesara. Daca locatia 
nu exista va fi afisat mesajul corespunzator. In variabila location
voi retine totul invers, deoarece parcurg de jos is sus structura, iar
la final voi afisa fiecare caracter de la coada la cap pentru ca afisajul
sa fie corespunzator.

	-touch
Functia touch se comporta la fel ca functia mkdir, numai ca
 * tipul structurii va fi 1, cel definit de mine pentru fisiere.
 */

	-ls

Voi incepe prin a traversa din nodul root al structurii, iar,
de fiecare data cand gasim un copil diferit de NULL, vom printa
in fisier "cheia parinte" a copilului nodului, iar apoi voi apela
functia recursiv pentru fiecare fiu pana cand va fi gasit un nod
de tip frunza.


	-rm

Se va parcurge structura in cautarea fisierului de eliminat.
In cazul in care este frunza, variabila ok va retine valoarea 1.
Aceeasi valoare o va primi in cazul in care copilul nodului nu este
caracterul '/' ce indica un faptul ca avem de a face cu un fisier, deci
il putem sterge. Functia se va opri daca s a ajuns la sfarsitul cautarilor.
Parcurgerea se va face din parinte in parinte cu verificarea fiecarui
copil.


	-cp

Functia va verifica daca parametrii sunt buni, iar apoi se va desparti
parametrul "key" in 2 bucati. file_path - calea catre fisier, si
file_name - numele fisierului si se va crea fisierul folosind functia
touch la nodul q. Nodul q va reprezenta directorul in care se va
copia fisierul, locatia acestuia fiind accesata folosind functia cd.


	-mv
Funcita mv se comporta la fel precum functia cp, cu mici exceptii precum
posibilitatea de a muta un folder, sau intr un fisier care sunt erori.
Se va parcurge structura, ma voi duce in directorul respectiv, iar, in
cazul in care este bun, se va efectua mutarea. Functia consta in stergerea
unui fisier dintr un loc si crearea acestuia in alt loc.
