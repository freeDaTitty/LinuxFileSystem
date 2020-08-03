/*
 * Dinu Daniel - 314CB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *   CHAR_SIZE este o variabila in care retin toate caracterele
 * care urmeaza a fi utilizate. De la a la z, de la 0 la 9, plus
 * caracterele speciale *, ., /
 */

#define CHAR_SIZE 39

/*
 *  Structura este compusa din 4 campuri prin care retin:
 * type - tipul acestuia. 1 - fisier, 2 director, 0 - orice altceva
 * isLeaf - 1 in cazul in care e frunza
 * parent retine parintele unui nod
 * v[CHAR_SIZE] contine toti copiii structurii de tip trie.
 */

typedef struct TrieNode{
    int type; //1 - fisier, 2 - director
    int isLeaf; //1 - frunza
    struct TrieNode* parent;
    struct TrieNode* v[CHAR_SIZE];
}CTrie, *Trie, **ATrie;



Trie AlocNode();
void FreeNode(Trie Node);



void mkdir(Trie current_node, char *key, FILE *err);
Trie cd(Trie current_node, char *key, int *is_file);
void pwd(Trie current_node, FILE *output_file);
void touch(Trie current_node, char *key, FILE *err);
void ls(Trie current_node, char *key, FILE *output_file, int type, int level);
void rm(Trie current_node, char*key, FILE *err);
void cp(Trie root, Trie current_node, char *key, char *dir, FILE *err);
void mv(Trie root, Trie current_node, char *key, char *dir, FILE *err);

/*
 *  Aceste functii ajutatoare ma vor ajuta sa:
 * - despart o cale in subdiviziuni pentru o parcurgere mai usoara
 * a structurii
 * - verific daca un director este gol
 * - parcurg structura printr o indexare si, cum nu pot sa
 * folosesc caractere pe post de indexuri, le voi transpune
 * in ceva de care ma pot folosi.
 */


void Split_Path(char *parameter, char *root_path, char *file_name);
int Empty_directory(Trie current_node, char *key);
int CHAR_TO_INDEX(char character);
int INDEX_TO_CHAR(int i);