/*
 * Dinu Daniel - 314CB
 */

#include "graf.h"

/*
 *  Functia de alocare pentru o structura trie.
 */

Trie AlocNode(void){
    Trie Node;
    Node = (Trie)malloc(sizeof(CTrie));

    if(!Node)  return NULL;

    int i;
    for(i = 0; i < CHAR_SIZE; i++){
        Node->v[i] = NULL;
    }

    Node->parent = NULL;
    Node->type = 0;
    Node->isLeaf = 0;

    return Node;
}

/*
 *  Functia de dezalocare pentru o structura trie.
 */

void FreeNode(Trie Node){
    int i;
    for(i = 0; i < CHAR_SIZE; i++){
        if(Node->v[i] != NULL){
            FreeNode(Node->v[i]);
        }
    }
    free(Node);
}

int CHAR_TO_INDEX(char character){
    if(character >= 'a' && character <= 'z'){
        return (int)character - (int)'a';
    }
    else if(character >= '0' && character <= '9'){
        return (int)character - (int)'0' + 26;
    }
    else if(character == '/'){
        return 36;
    }
    else if(character == '.'){
        return 37;
    }
    else{
        return 38;
    }
}

int INDEX_TO_CHAR(int i){
    if(i == 37){
        return '.';
    }
    else if(i == 38){
        return '*';
    }
    else if(i == 36){
        return '/';
    }
    else if(i > 25 && i < 36){
        return (char)((int)'0' + i - 26);
    }
    else{
        return (char)((int)'a' + i);
    }
}

/*
 *  Voi parcurge primul parametru si il voi imparti in 2 bucati
 * file_name - numele fisierului
 * root_path - calea catre acest
 * Spre exemplu: In /a/b/c: /a/b - root_path, c - fisierul
 */

void Split_Path(char *parameter, char *root_path, char *file_name){
    int i;
    int index = 0;
    int length = strlen(parameter);

    for(i = length - 1; i >= 0; i--){
        if(parameter[i] == '/' && index == 0){
            index = i;
        }
    }

    if(index == 0){
        root_path[0] = '\0';
        strcpy(file_name, parameter);
        return;
    }

    for(i = 0; i <= index; i++){
        root_path[i] = parameter[i];
    }
    
    root_path[index + 1] = 0;
    for(i = 0; i < length - index; i++){
        file_name[i] = parameter[i + index + 1];
    }
}

/*
 *  In cazul in care un nod are copii, inseamna ca nu este gol. 
 */

int Empty_directory(Trie current_node, char *key){
    Trie p = current_node;
    int i;

    for(i = 0; i < strlen(key); i++){
        if(p != NULL){
            p = p->v[CHAR_TO_INDEX(key[i])];
        }
        if(p != NULL){
            return 1;
        }
    }
    return 0;
}

/*
 *  Functia va parcurge structura, iar acolo unde nu s a alocat
 * inca un anumit caracter, il va introduce ca fiind fiul precedentului
 * nod din arborele de tip trie.
 */

void mkdir(Trie current_node, char *key, FILE *err){
    int level = 0;
    int length = strlen(key);
    int index;

    Trie p = current_node;

    if(current_node->parent == NULL && *(key) == '/'){
        level = 1;
    }

    for(; level < length; level++){
        index = CHAR_TO_INDEX(key[level]);

        if(!p->v[index]){
            p->v[index] = AlocNode();
            p->v[index]->parent = p;
        }
        p = p->v[index];
    }

    if(p->v[CHAR_TO_INDEX('/')] != NULL){
        fprintf(err, "%s: already exists\n", key);
        return;
    }

    p->v[CHAR_TO_INDEX('/')] = AlocNode();
    p->v[CHAR_TO_INDEX('/')]->parent = p;

    p->type = 2;
    p->isLeaf = 1;
}

/*
 *  In functie de parametrul primit, functia va cauta prin toata
 * structura daca exista directorul primit ca parametru, iar apoi
 * va fixa locatia pe acel director. Daca se va primi ca parametru
 * un fisier, variabila is_file va returna valoarea 1, iar daca
 * directorul pe care vrem sa ajungem nu exista in structura, va
 * fi returnat NULL. In cazul in care se ajunge, va fi returnat
 * nodul pe care a trebuit sa ne ducem.
 */

Trie cd(Trie current_node, char *key, int *is_file){
    Trie p = current_node;

    int i;
    int ok = 0;

    
    if(*(key) == '/'){
        while(p->parent != NULL){
            p = p->parent;
        }
        if(strlen(key) > 1){
            for(i = 1; i < strlen(key); i++){
                p = p->v[CHAR_TO_INDEX(key[i])];
                if(p == NULL){
                    return NULL;
                }
            }
            p = p->v[CHAR_TO_INDEX('/')];
        }
    }
    else{
        for(i = 0; i < strlen(key);){
            if(*(key + i) == '.' && *(key + i + 1) == '.'){
                if (p->parent == NULL) {
                    return NULL;
                }
                p = p->parent;
                while(p->parent && (p->parent->v[CHAR_TO_INDEX('/')] != p)){
                    p = p->parent;
                }
                i += 2;
                if(*(key + i) == '/'){
                    i++;
                }
            }
            else{
                if(p == p->v[CHAR_TO_INDEX('/')] && p->parent != NULL){
                    p = p->v[CHAR_TO_INDEX(key[i])];
                }
                if(p != p->v[CHAR_TO_INDEX('/')]){
                    p = p->v[CHAR_TO_INDEX(key[i])];
                    i++;
                }
                if(p == NULL){
                    return NULL;
                }
                ok = 1;
                
            }
        }
        if(ok && key[strlen(key) - 1] != '/'){
            p = p->v[CHAR_TO_INDEX('/')];
        }
    }
    if(p == NULL){
        *is_file = 1;
    }
    return p;
}

/*
 *  Voi parcurge structura de la nodul curent si voi retine
 * parintele nodului respectiv. In cazul in care suntem in root,
 * parintele va fi NULL. Voi retine in variabila location fiecare
 * caracter care mi trebuie din structura si voi parcurge in continuare
 * fiecare parinte pana cand voi ajunge in locatia necesara. Daca locatia 
 * nu exista va fi afisat mesajul corespunzator. In variabila location
 * voi retine totul invers, deoarece parcurg de jos is sus structura, iar
 * la final voi afisa fiecare caracter de la coada la cap pentru ca afisajul
 * sa fie corespunzator.
 */

void pwd(Trie current_node, FILE *output_file){
    Trie p = current_node;
    Trie parent;

    if(current_node->parent == NULL){
        parent = NULL;
    }
    else{
        parent = current_node->parent;
    }
    
    char *location = (char *)calloc(80, sizeof(char));
    int location_index = 0;

    int i;
    while(parent != NULL){
        for(i = 0; i < CHAR_SIZE; i++){
            if(parent->v[i] == p){
                location[location_index] = INDEX_TO_CHAR(i);
                location_index++;
                break;   
            }
        }
        p = parent;
        parent = p->parent;
    }

    location[location_index] = '/';
    for(i = strlen(location) - 1; i >= 0; i--){
        if(i == 0 && location[i] == '/' && strlen(location) > 1){
            break;
        }
        fprintf(output_file, "%c", *(location + i));
    }
    fprintf(output_file, "\n");

    free(location);
}

/*
 *  Functia touch se comporta la fel ca functia mkdir, numai ca
 * tipul structurii va fi 1, cel definit de mine pentru fisiere.
 */

void touch(Trie current_node, char *key, FILE *err){
    int level = 0;
    int length = strlen(key);
    int index;

    Trie p = current_node;

    if(current_node->parent == NULL && *(key) == '/'){
         level = 1;
    }

    for(; level < length; level++){
        index = CHAR_TO_INDEX(key[level]);

        if(!p->v[index]){
            p->v[index] = AlocNode();
            p->v[index]->parent = p;
        }
        p = p->v[index];
    }
    if(p->type == 1){
        fprintf(err, "%s: already exists\n​", key);
        return;
    }

    p->type = 1;
    p->isLeaf = 1;
}

/*
 *  Voi incepe prin a traversa din nodul root al structurii, iar,
 * de fiecare data cand gasim un copil diferit de NULL, vom printa
 * in fisier "cheia parinte" a copilului nodului, iar apoi voi apela
 * functia recursiv pentru fiecare fiu pana cand va fi gasit un nod
 * de tip frunza.
 */

void ls(Trie current_node, char *key, FILE *output_file, int type, int level){
    if(current_node->isLeaf == 1)  
    {
        if(type == 1){
           if(current_node->type != 1){
               key[level] = '/';
           }
           else{
               key[level] = '*';
           }
            key[level + 1] = '\0';
        }
        else{
            key[level] = '\0';
        }
        if(key[0] != '\0'){
            fprintf(output_file, "%s ", key);
        }
    }
  
    int i; 
    for(i = 0; i < CHAR_SIZE; i++)  
    { 
        if(current_node->v[i] && INDEX_TO_CHAR(i) != '/')  
        {
            key[level] = INDEX_TO_CHAR(i);
            ls(current_node->v[i], key, output_file, type, level + 1); 
        } 
    }
}

/*
 *  Se va parcurge structura in cautarea fisierului de eliminat.
 * In cazul in care este frunza, variabila ok va retine valoarea 1.
 * Aceeasi valoare o va primi in cazul in care copilul nodului nu este
 * caracterul '/' ce indica un faptul ca avem de a face cu un fisier, deci
 * il putem sterge. Functia se va opri daca s a ajuns la sfarsitul cautarilor.
 * Parcurgerea se va face din parinte in parinte cu verificarea fiecarui
 * copil.
 */

void rm(Trie current_node, char *key, FILE *err){
    Trie p = current_node;
    Trie parent = p;
    int i;
    int ok = 0;

    for(i = 0; i < strlen(key); i++){
        if(p)
            p = p->v[CHAR_TO_INDEX(key[i])];
        if(!p){
            fprintf(err, "%s: No such file or directory\n", key);
            return;
        }
    }

    int k = 0;

    if(p->type == 1){
        if(p->isLeaf == 1){
            ok = 1;
        }
        else{
            k = 1;
            p->type = 0;
        }
    } 
    else{
        if(p->v[CHAR_TO_INDEX('/')] != NULL) {
            ok = 1;
        }
    }

    if(ok == 1){
        int stop = 0;
        while (stop == 0 ) {
            parent = p->parent;
            if (parent == NULL) {
                break;
            }
            if (parent->v[CHAR_TO_INDEX('/')] == p) {
                FreeNode(p);
                parent->v[CHAR_TO_INDEX('/')] = NULL;
                break;
            }
            for(i = 0; i < CHAR_SIZE; i++){
                
                if(parent->v[i] != NULL && parent->v[i] != p){
                    stop = 1;
                }
                if(parent->v[i] == p){
                    FreeNode(p);
                    parent->v[i] = NULL;
                }
            }
            p = parent;
        }
    }
    else{
        if(k == 0)
            fprintf(err, "%s: No such file or directory\n", key);
    }
}

/*
 *  Functia va verifica daca parametrii sunt buni, iar apoi se va desparti
 * parametrul "key" in 2 bucati. file_path - calea catre fisier, si
 * file_name - numele fisierului si se va crea fisierul folosind functia
 * touch la nodul q. Nodul q va reprezenta directorul in care se va
 * copia fisierul, locatia acestuia fiind accesata folosind functia cd.
 */

void cp(Trie root, Trie current_node, char *key, char *dir, FILE *err){
    int i = 0;    
    int is_file = 0;
    Trie p = current_node;
    Trie q = cd(current_node, dir, &is_file);

    if(*(key) == '/'){
        p = root;
        i = 1;
    }

    for(; i < strlen(key); i++){
        p = p->v[CHAR_TO_INDEX(key[i])];
        if(!p){
            fprintf(err, "%s: No such file or directory\n", key);
            return;
        }
    }
    if(p->type == 2){
        fprintf(err, "%s: No such file or directory\n", key);
        return;
    }
    
    if(q == NULL){
        fprintf(err, "%s: No such file or directory\n", dir);
        return;
    }

    if(p->type == 0){
        fprintf(err, "%s: No such file or directory\n", key);
        return;
    }

    char file_path[100] = {'\0'};
    char file_name[100] = {'\0'};

    Split_Path(key, file_path, file_name);
    touch(q, file_name, err);
}

/*
 *  Funcita mv se comporta la fel precum functia cp, cu mici exceptii precum
 * posibilitatea de a muta un folder, sau intr un fisier care sunt erori.
 * Se va parcurge structura, ma voi duce in directorul respectiv, iar, in
 * cazul in care este bun, se va efectua mutarea. Functia consta in stergerea
 * unui fisier dintr un loc si crearea acestuia in alt loc.
 */

void mv(Trie root, Trie current_node, char *key, char *dir, FILE *err){
    int i = 0;    
    int is_file = 0;
    Trie p = current_node;
    Trie q = cd(current_node, dir, &is_file);

   if(is_file == 1){
        fprintf(err, "%s: Not a directory\n", dir);
        return;
    }

    if(*(key) == '/'){
        p = root;
        i = 1;
    }
    for(; i < strlen(key); i++){
        p = p->v[CHAR_TO_INDEX(key[i])];
        if(!p){
            fprintf(err, "%s: No such file or directory\n", key);
            return;
        }
    }
    if(p->type == 2){
        fprintf(err, "%s: No such file or directory\n", key);
        return;
    }
    
    if(q == NULL){
        fprintf(err, "%s: No such file or directory\n", dir);
        return;
    }

    if(p->type == 0){
        fprintf(err, "%s: No such file or directory\n", key);
        return;
    }
    
    char file_path[100] = {'\0'};
    char file_name[100] = {'\0'};

    Split_Path(key, file_path, file_name);
    touch(q, file_name, err);

    p = cd(current_node, file_path, &is_file);
    rm(p, file_name, err);
}