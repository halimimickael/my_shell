#include "MyFunctionShell.h"

char *my_strtok(char *str, const char *delimitation)
{
    static char *next_token = NULL;
    char *token_start;
    if (str != NULL)
        next_token = str;

    if (next_token == NULL || *next_token == '\0')
        return NULL;

    // Ignore delimiters at the beginning
    while (*next_token != '\0')
    {
        int is_delimitation = 0;
        for (const char *d = delimitation; *d != '\0'; ++d)
        {
            if (*next_token == *d)
            {
                is_delimitation = 1;
                break;
            }
        }
        if (!is_delimitation)
            break;
        next_token++;
    }

    // If we reach the end of the string, return NULL
    if (*next_token == '\0')
        return NULL;

    token_start = next_token;

    // Find the end of the token
    while (*next_token != '\0')
    {
        int is_delimitation = 0;

        for (const char *d = delimitation; *d != '\0'; ++d)
        {
            if (*next_token == *d)
            {
                is_delimitation = 1;
                break;
            }
        }
        if (is_delimitation)
            break;
        next_token++;
    }

    // If a delimiter is found, replace it with '\0'
    if (*next_token != '\0')
    {
        *next_token = '\0';
        next_token++;
    }

    return token_start;
}

char *inputFromUser()
{
    char ch;
    int size = 0;
    char *str = (char *)malloc((size + 1) * sizeof(char));
    // stdin = hello hello\n
    while ((ch = getchar()) != '\n')
    {
        *(str + size) = ch;
        size++;
        str = (char *)realloc(str, size + 1);
    }
    *(str + size) = '\0';
    return str;
}

char **splitString(char *str)
{

    char *subStr;
    subStr = my_strtok(str, " ");
    int argumentSize = 2;
    int index = 0;
    char **argumentArray = (char **)malloc(argumentSize * sizeof(char *));
    *(argumentArray + index) = subStr;
    while ((subStr = my_strtok(NULL, " ")) != NULL)
    {
        index++;
        argumentSize++;
        argumentArray = (char **)realloc(argumentArray, argumentSize * sizeof(char *));
        *(argumentArray + index) = subStr;
    }
    *(argumentArray + index + 1) = NULL;

    return argumentArray;
}

char *getHostname()
{
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("Error getting hostname");
        exit(EXIT_FAILURE);
    }

    struct hostent *host_entry;
    host_entry = gethostbyname(hostname);
    if (host_entry == NULL)
    {
        perror("Error getting host entry");
        exit(EXIT_FAILURE);
    }

    strcpy(hostname, host_entry->h_name);
    return strdup(hostname);
}

void getLocation()
{
    char location[256];
    if (getcwd(location, sizeof(location)) == NULL)
    {
        puts("Error");
        return;
    }

    char *hostname = getHostname();
    printf("\033[1;31m");
    printf("%s@%s", getenv("USER"), hostname);
    printf("\033[0m");
    printf(":");
    printf("\033[1;35m");
    printf("%s", location);
    printf("\033[0m");
    printf("$");

    free(hostname);
}

void logout(char *str)
{
    char *hostname = getHostname();
    printf("See you later %s!", hostname);
    puts(" ");
    free(str);
    exit(0);
}

void echo(char **argumnts)
{
    int i = 1;
    while (argumnts[i] != NULL)
        printf("%s ", argumnts[i++]);

    puts("");
}

void cd(char **path)
{
    if (chdir(path[1]) != 0)
        printf("bash: cd: %s: No such file or directory\n", path[1]);
}

void cp(char **arguments)
{
    char ch;
    FILE *src, *des;

    // Ouvrir le fichier source en mode lecture
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Error: Unable to open source file.");
        return;
    }

    // Ouvrir le fichier de destination en mode ajout
    if ((des = fopen(arguments[2], "a")) == NULL)
    {
        fclose(src); // Fermer le fichier source avant de quitter
        puts("Error: Unable to open destination file.");
        return;
    }

    // Copier le contenu du fichier source vers le fichier de destination
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, des);

    // Fermer les fichiers après la copie
    fclose(src);
    fclose(des);
}
void get_dir()
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(".");

    if (directory == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(directory);
}
