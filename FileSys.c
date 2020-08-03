/*
 * Dinu Daniel - 314CB
 */

#include "graf-L.c"

int main(){
    int n;

    FILE *input_file;
    input_file = fopen("input.in", "r");

    if(!input_file){
        fprintf(stderr, "%s", "Can't open input file!\n");
        return(-1);
    }

    FILE *output_file;
    output_file = fopen("output.out", "w");

    if(!output_file){
        fprintf(stderr, "%s", "Can't open output file!\n");
        return(-1);
    }

    FILE *err;
    err = fopen("error.out", "w");

    if(!err){
        return(-1);
    }

    /*
     *  Se va aloca nodul root, acesta fiind radacina structurii, iar nodul
     * curent va porni exact din acesta. In continuare voi lua fiecare linie
     * in parte, pe langa aceasta si o copie pentru a nu strica continutul
     * acestora, se va desparti linia in subdiviziuni pentru a vedea ce
     * functii avem de apelat si ce parametrii vor avea.
     */

    Trie root;
    root = AlocNode();

    Trie current_node = root;

    char *line;
    line = calloc(80, sizeof(char));

    if(!line)  return(-1);

    fscanf(input_file, "%d\n", &n);
    while(n != 0){
        fgets(line, 80, input_file);
        char copy[80];

        char *key;
        
        char *line_copy = strdup(line);
        if(line_copy[strlen(line_copy) - 1] == '\n'){
            line_copy[strlen(line_copy) - 1] = 0;
        }
        strcpy(copy, line);

        /*
         *  Newline urile vor fi scoase pentru a nu incurca
         * rezolvarea problemei.
         */

        if(n == 0){
            key = strtok(line, "\n");
            if(key[strlen(key) - 1] == '\n' && strlen(key) < 4){
                key[strlen(key) - 1] = 0;
            }
        }
        else{
          key = strtok(line_copy, " ");
        }
        int is_file = 0;
        if(strcmp(key, "mkdir") == 0){
            while(key != NULL){

                /*
                 *  Daca numarul de '/' din parametrii este mai mare
                 * de 1 inseamna ca avem o cale complexa pe care o
                 * despartim ca sa verificam daca anumite foldere intermediare
                 * exista deja, iar daca nu exista inseamna ca nu voi putea
                 * crea tot ce este necesar. In cazul unei cai /a/b/c,
                 * daca folderele /a/b nu exista, atunci /c nu va putea fi
                 * facut.
                 */

                int nr = 0;
                key = strtok(NULL, " ");

                if(key != NULL){
                    int i;
                    for(i = 0; i < strlen(key); i++){
                        if(key[i] == '/'){
                            nr++;
                        }
                    }

                    char root_path[100] = {'\0'};
                    char file_name[100] = {'\0'};

                    if(nr > 1){
                        Trie node = current_node;
                        Split_Path(key, root_path, file_name);
                        root_path[strlen(root_path) - 1] = 0;
                        if(cd(node, root_path, &is_file) == NULL){
                            fprintf(err, "%s: No such file or directory\n", 
                            key);
                        }
                        else{
                            Trie node_to_make = current_node;
                            if(*(key) == '/'){
                                node_to_make = root;
                            }
                            mkdir(node_to_make, key, err);
                        }
                    }
                    else{
                        Trie node_to_make = current_node;
                        if(*(key) == '/'){
                            node_to_make = root;
                        }
                        mkdir(node_to_make, key, err);
                    }
                }
            }
        }
        else if(strcmp(key, "touch") == 0){
            while(key != NULL){
                key = strtok(NULL, " ");
                if(key != NULL){
                    Trie node_to_touch = current_node;
                    if(*(key) == '/'){
                        node_to_touch = root;
                    }
                    touch(node_to_touch, key, err);
                }
            }
        }
        else if(strcmp(key, "cd") == 0){
            key = strtok(NULL, " ");
            if(key == NULL){
                fprintf(err, "cd: Missing operand\n");
            }
            else{
                char *key2 = strtok(NULL, " ");
                if(key2 == NULL){
                    if(cd(current_node, key, &is_file) == NULL){
                        if(is_file == 1){
                            fprintf(err, "%s: Not a directory\n", key);
                        }
                        else{
                            fprintf(err, "%s: No such file or directory\n", 
                            key);
                        }
                    }
                    else{                    
                        current_node = cd(current_node, key, &is_file);
                    }
                }
                else{
                    fprintf(err, "cd: too many arguments\n");
                }
            }
        }
        else if(strcmp(copy, "pwd") == 0 || strcmp(key, "pwd") == 0){
            pwd(current_node, output_file);
        }
        else if(strcmp(key, "ls") == 0 || strcmp(copy, "ls") == 0){

            /*
             *  In functie de parametrii comenzii se va parcurge structura
             * si va fi afisat continutul acesteia din zona respectiva.
             */

            key = strtok(NULL, " ");
            char str[20];
            if(key == NULL){
                ls(current_node, str, output_file, 0, 0);
            }
            else if(strcmp(key, "-F") == 0){
                key = strtok(NULL, " ");
                if(key == NULL){
                    ls(current_node, str, output_file, 1, 0);
                }
                else{
                    Trie node_to_ls = current_node;
                    node_to_ls = cd(node_to_ls, key, &is_file);
                    if(node_to_ls != NULL){
                        ls(node_to_ls, str, output_file, 1, 0);
                    }
                }
            }
            else{
                Trie node_to_ls = current_node;
                node_to_ls = cd(node_to_ls, key, &is_file);
                ls(node_to_ls, str, output_file, 0, 0);
            }
            fprintf(output_file, "\n");
        }
        else if(strcmp(key, "rm") == 0){
            key = strtok(NULL, " ");

            while(key != NULL){
                char root_path[100] = {'\0'};
                char file_name[100] = {'\0'};

                Split_Path(key, root_path, file_name);
                root_path[strlen(root_path) - 1] = 0;

                Trie node = current_node;
                node = cd(node, root_path, &is_file);
                if(node == NULL){
                    fprintf(err, "%s: No such file or directory\n", key);
                }
                else{
                    rm(node, file_name, err);
                }
                key = strtok(NULL, " ");
            }
        }
        else if(strcmp(key, "mv") == 0){
            char *first = strtok(NULL, " ");
            char *second = strtok(NULL, " ");

            if(first && second){
                mv(root, current_node, first, second, err);
            }
            else{
                fprintf(err, "mv %s: missing operand\n", first);
            }
        }
        else if(strcmp(key, "cp") == 0){
            char *first = strtok(NULL, " ");
            char *second = strtok(NULL, " ");

            if(first && second){
                cp(root, current_node, first, second, err);
            }
            else{
                fprintf(err, "cp %s: missing operand\n", first);
            }
        }
        else if(strcmp(key, "rmdir") == 0){
            char *parameter = strtok(NULL, " ");
            while(parameter != NULL){
                Trie node_to_rm = current_node;
                node_to_rm = cd(node_to_rm, parameter, &is_file);

                if(node_to_rm == NULL){
                    fprintf(err, "%s: No such file or directory\n", parameter);
                }
                else if(is_file == 1){
                    fprintf(err, "%s: Not a directory\n", parameter);
                }
                else{
                    if(Empty_directory(node_to_rm, parameter) == 1){
                        fprintf(err, "%s: Directory not empty\n", parameter);
                    }
                }

                parameter = strtok(NULL, " ");
            }
        }
        else{
            fprintf(err, "%s: command not found\n", key);
        }
        free(line_copy);
        n--;
    }

    fclose(input_file);
    fclose(output_file);
    fclose(err);
    FreeNode(root);
    free(line);

    return 0;
}