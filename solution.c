#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void freeMem(int n, char** s)
{
    for (int i = 0; i < n; i++)
    {
        free(s[i]);
    }
    free(s);
}

int closeDirectory(DIR* dir)
{
    if(closedir(dir) == -1){
        perror("Failed to close!");
        return -1;
    } 
    return 0;
}

int closeFile(int fd)
{
    if(close(fd) == -1){
        printf("Could not close the file!");
        return 1;
    }
    return 0;
}

//functie care inverseaza un sir de caractere
char* strrev(char* str)
{
    if(!str || !*str)
    {
        return str;
    }

    int i = strlen(str)-1, j = 0;
    char ch;

    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

int list(char* path){
    char full_path[360];

    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(stat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 ))
            {
                printf("%s\n", full_path);}
        }
    }
    closeDirectory(dir); 
    return 0; 
}

int list_rec(char* path, int s){
    char full_path[360];
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    if(s == 1){
        printf("SUCCESS\n");
        s = 0;
    }

    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(lstat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 )){
                printf("%s\n", full_path);
                if(S_ISDIR(file_info.st_mode)){
                    list_rec(full_path, s);
                }
            }
         }
    }
    
    closeDirectory(dir); 

    return 0; 
}

int list_size(char* path, char* s){
    char sz[360], full_path[360];
    int size;
    strcpy(sz, s+13);
    size = atoi(sz);

    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(stat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 )){
                if(S_ISREG(file_info.st_mode) && (file_info.st_size < size)){
                    printf("%s\n", full_path);
                }
            }
        }
    }
    
    closeDirectory(dir); 

    return 0; 
}

int list_rec_size(char* path, char* s, int success){
    char full_path[360], sz[360];
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;
    int size;

    strcpy(sz, s+13);
    size = atoi(sz);


    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    if(success == 1){
        printf("SUCCESS\n");
        success = 0;
    }

    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(lstat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 )){
                if(S_ISREG(file_info.st_mode) && (file_info.st_size < size)){
                    printf("%s\n", full_path);
                }
                if(S_ISDIR(file_info.st_mode)){
                    list_rec_size(full_path, s, success);
                }
            }
        }
    }
    
    closeDirectory(dir);   

    return 0; 
}

int list_perm(char* path){
    char full_path[360];

    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    printf("SUCCESS\n");
    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(stat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 )){
                if((S_ISREG(file_info.st_mode) || S_ISDIR(file_info.st_mode)) && file_info.st_mode & S_IWUSR){
                    printf("%s\n", full_path);
                }
            }
        }
    }
    
    closeDirectory(dir);   

    return 0; 
}

int list_rec_perm(char* path, int success){
    char full_path[360];
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    if(success == 1){
        printf("SUCCESS\n");
        success = 0;
    }

    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);
        if(lstat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 )){
                if((S_ISREG(file_info.st_mode) || S_ISDIR(file_info.st_mode)) && file_info.st_mode & S_IWUSR){
                    printf("%s\n", full_path);
                }
                if(S_ISDIR(file_info.st_mode)){
                    list_rec_perm(full_path, success);
                }
            }
        }
    }
   
    closeDirectory(dir);  

    return 0; 
}


int parse(char* path, int to_print)
{
    int fd = open(path, O_RDONLY);

    if(fd == -1){
        printf("Could not open the file!");
        return -1;
    }

    lseek (fd, 0, SEEK_END); // cursorul se afla la sfarsitul header-ului
    
    lseek (fd, -1, SEEK_CUR); // cursorul se afla la inceputul lui MAGIC
    char magic;
    read (fd, &magic, 1);

    lseek (fd, -3, SEEK_END); //cursorul se afla la inceputul lui HEADER_SIZE
    short int header_size;
    read (fd, &header_size, 2);
    
    lseek (fd, -1 * header_size, SEEK_END); //cursorul se afla la inceputul header-ului
    short int version;
    read (fd, &version, 2);

    char no_of_sections; //cursorul se afla la inceputul NO_OF_SECTIONS
    read (fd, &no_of_sections, 1);

    char** sect_name = (char**)malloc(no_of_sections * sizeof(char*));

    int* sect_size= (int*)malloc(no_of_sections * sizeof(int));
    int* sect_type= (int*)malloc(no_of_sections * sizeof(int));

    for (int i = 0; i < no_of_sections; i++){
        sect_name[i] = (char*)malloc(20 * sizeof(char));

        //cursorul se afla la inceputul SECT_TYPE
        read (fd, sect_name[i], 20);
        read (fd, &sect_type[i], 4);
        lseek (fd, 4, SEEK_CUR);
        read (fd, &sect_size[i], 4);
         //cursorul se afla la sfarsitul sectiunii
    }

    int sect_type_values[6] = {51, 53, 29, 96, 11, 22};
    
    int ok = 1; //pentru a verifica daca SECT_TYPE apartine valorilor date
    for (int i = 0; i < no_of_sections ;i++)
    {
        int found = 0;
        for (int j = 0; j < 6; j++)
        {
            if (sect_type[i] == sect_type_values[j])
            {
                found = 1;
            }
        }
        if (found == 0) {
            ok = 0;
            break;
        }
    }

    //afisarile pe ecran se vor face doar cand comanda data este "parse" (to_print = 1)
    //daca avem nevoie de functia parse in interiorul altor functii => to_print = 0
        if (magic != 'A')
        {
            if (to_print == 1)
            printf("ERROR\nwrong magic\n");
            freeMem(no_of_sections, sect_name);
            free(sect_size);
            free(sect_type);
            closeFile(fd);
            return -1;
        }

        if (!(54 <= version && version <= 86))
        {
            if (to_print == 1)
            printf("ERROR\nwrong version\n");
            freeMem(no_of_sections, sect_name);
            free(sect_size);
            free(sect_type);
            closeFile(fd);
            return -1;
        }

        if (!(3 <= no_of_sections && no_of_sections <= 13))
        {
            if (to_print == 1)
            printf("ERROR\nwrong sect_nr\n");
            freeMem(no_of_sections, sect_name);
            free(sect_size);
            free(sect_type);
            closeFile(fd);
            return -1;
        }

        if (ok == 0)
        {
            if (to_print == 1)
            printf("ERROR\nwrong sect_types\n");
            freeMem(no_of_sections, sect_name);
            free(sect_size);
            free(sect_type);
            closeFile(fd);
            return -1;
        }

        if (to_print == 1)
        {
            printf("SUCCESS\n");
            printf("version=%d\n", version);
            printf("nr_sections=%d\n", no_of_sections);
            for (int i =0; i<no_of_sections;i++){
                printf("section%d: %s %d %d\n", i+1, sect_name[i], sect_type[i], sect_size[i]);
            }
        }
    
    freeMem(no_of_sections, sect_name);
    free(sect_size);
    free(sect_type);

    return 0;
}

int extract(char* path, char* sec, char* l)
{
    int section = atoi(sec);
    int line = atoi(l);
    int fd = open(path, O_RDONLY);

    if(fd == -1){
        printf("ERROR\ninvalid file\n");
        return -1;
    }

    lseek (fd, -3, SEEK_END); //cursorul se afla la inceputul lui HEADER_SIZE
    short int header_size;
    read (fd, &header_size, 2);
    
    lseek (fd, -1 * header_size, SEEK_END); //cursorul se afla la inceputul header-ului
    lseek (fd, 2, SEEK_CUR);

    char no_of_sections; //cursorul se afla la inceputul NO_OF_SECTIONS
    read (fd, &no_of_sections, 1);


    if ((section < 0) || (section > no_of_sections))
    {
        printf("ERROR\ninvalid section\n");
        return -1;
    }

    int sect_size, sect_offset;
    
    for (int i = 1; i <= no_of_sections; i++)
    {
        if (i == section)
        {
            //citim header-ul sectiunii la care vrem sa ajungem
            lseek(fd, 24, SEEK_CUR);
            read (fd, &sect_offset, 4);
            read (fd, &sect_size, 4);
            break;
        }
        else 
        {
            lseek(fd, 32, SEEK_CUR);
        }
    }


    lseek(fd, sect_offset, SEEK_SET); //cursorul se afla la inceputul sectiunii
    lseek(fd, sect_size, SEEK_CUR); //liniile se numara de la sfarsitul spre inceputul sectiunii
    char byte_read;
    int line_nr = 1, k = 0, found = 0;
    char* line_read = (char*)malloc(10000000 * sizeof(char));
    for (int i = 0; i<=sect_size && found == 0; i++)
    {
        // cautam linia pe care vrem sa o extragem numarand caracterele newline (\n)
        if(line_nr != line)
        {
            if(i == 0)
            {
                lseek(fd, -1, SEEK_CUR);
                read (fd, &byte_read, 1);
            }
            else
            {
                lseek(fd, -2, SEEK_CUR);
                read (fd, &byte_read, 1);
            }

            if (byte_read == '\n')
            {
                line_nr++;
            }
        }
        else
        {
            if(i == 0)
            {
                off_t cursor = lseek(fd, -1, SEEK_CUR);
                read (fd, &byte_read, 1);

                if (byte_read == '\n' || (cursor == sect_offset - 1)) //luam in considerare si ultima linie care s-ar putea sa nu fie separata prin \n
                {
                    found = 1;
                    cursor = 0;
                }
                else 
                {
                    line_read[k] = byte_read;
                    k++;
                }
            }
            else
            {
                off_t cursor1 = lseek(fd, -2, SEEK_CUR);
                read (fd, &byte_read, 1);

                if (byte_read == '\n' || (cursor1 == sect_offset - 1))
                {
                    found = 1;
                    cursor1 = 0;
                }
                else 
                {
                    line_read[k] = byte_read;
                    k++;
                }
            }
        }
    }

    if (found == 0)
    {
        printf("ERROR\ninvalid line\n");
        free(line_read);
        return -1;
    }
    else
    {
        //linia a fost citita in ordine inversa
        line_read[k] = 0;
        strrev(line_read);
    }
    
    printf("SUCCESS\n%s\n", line_read);
    free(line_read);

    return 0;
}

int isValid(char* path)
{
    int fd = open(path, O_RDONLY);

    if(fd == -1){
        return -1;
    }

    lseek (fd, -3, SEEK_END); //cursorul se afla la inceputul lui HEADER_SIZE
    short int header_size;
    read (fd, &header_size, 2);
    
    lseek (fd, -1 * header_size, SEEK_END); //cursorul se afla la inceputul header-ului
    lseek (fd, 2, SEEK_CUR);

    char no_of_sections; //cursorul se afla la inceputul NO_OF_SECTIONS
    read (fd, &no_of_sections, 1);

    int sect_size;
    
    for (int i = 1; i <= no_of_sections; i++)
    {
        lseek(fd, 28, SEEK_CUR);
        read (fd, &sect_size, 4);
        if(sect_size > 1304 || sect_size < 0)
        {
            return 0;
        }
    }   
    return 1;
}

int findall(char* path, int s)
{
    char full_path[360];
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    struct stat file_info;

    dir = opendir(path);
    if(dir == NULL){
        printf("\nERROR\ninvalid directory path\n");
        return -1;
    }

    if(s == 1){
        printf("SUCCESS\n");
        s = 0;
    }

    while ((entry = readdir(dir)) != NULL){
        snprintf(full_path, 360, "%s/%s", path, entry -> d_name);

        if(lstat(full_path, &file_info) == 0){
            if((strncmp(entry->d_name, ".", 1) != 0) && (strncmp(entry->d_name, "..", 2) != 0 ))
            {
                if(S_ISDIR(file_info.st_mode))
                {
                    findall(full_path, s);
                }
                if((S_ISREG(file_info.st_mode)) && (isValid(full_path) == 1) && (parse(full_path, 0)==0))
                {
                    printf("%s\n", full_path);
                }
                
            }
         }
    }
    
    closeDirectory(dir); 

    return 0; 
}

int main(int argc, char **argv)
{
    if(argc == 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("25475\n");
        }
    }

    if(argc == 3){
        if(strcmp(argv[1], "list") == 0){
            list(argv[2]+5);
        }

        if(strcmp(argv[1], "parse") == 0){
            parse(argv[2]+5, 1);
        }

        if(strcmp(argv[1], "findall") == 0){
            findall(argv[2]+5, 1);
        }
    }
        
    if(argc == 4){
        if((strcmp(argv[1], "list") == 0) && (strcmp(argv[2], "recursive") == 0)){
            list_rec(argv[3]+5, 1);
        }

        if((strcmp(argv[1], "list") == 0) && (strncmp(argv[2], "size_smaller", 12) == 0)){
            list_size(argv[3]+5, argv[2]);
        }

        if((strcmp(argv[1], "list") == 0) && (strcmp(argv[2], "has_perm_write") == 0)){
            list_perm(argv[3]+5);
        }
    }

    if(argc == 5){
        if((strcmp(argv[1], "list") == 0) && (strcmp(argv[2], "recursive") == 0) && (strncmp(argv[3], "size_smaller", 12) == 0)){
            list_rec_size(argv[4]+5, argv[3], 1);
        }

        if((strcmp(argv[1], "list") == 0) && (strcmp(argv[3], "recursive") == 0) && (strncmp(argv[2], "size_smaller", 12) == 0)){
            list_rec_size(argv[4]+5, argv[2], 1);
        }

        if(((strcmp(argv[1], "list") == 0) && (strcmp(argv[2], "recursive") == 0) && (strcmp(argv[3], "has_perm_write") == 0)) || ((strcmp(argv[1], "list") == 0) && (strcmp(argv[3], "recursive") == 0) && (strcmp(argv[2], "has_perm_write") == 0))){
            list_rec_perm(argv[4]+5, 1);
        }

        if((strcmp(argv[1], "extract") == 0) && (strncmp(argv[3], "section", 7) == 0) && (strncmp(argv[4], "line", 4) == 0))
        {
            extract(argv[2]+5, argv[3]+8, argv[4]+5);
        }
    }

    return 0;
}