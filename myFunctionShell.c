#include "MyFunctionShell.h"

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

char *trim(char *str)
{
    while (*str && isspace(*str))
    {
        str++;
    }

    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1]))
    {
        len--;
    }

    str[len] = '\0';

    return str;
}

void logout(char *str)
{
    char *trimmed_str = trim(str);
    char *hostname = getHostname();

    if (strncmp(trimmed_str, "exit", 4) == 0)
    {
        printf("See you later %s!", hostname);
        puts(" ");
        printf("Exiting program...\n");
    }
    free(trimmed_str);
    exit(0);
}

void echo(char **argumnts)
{
    int i = 1;
    while (argumnts[i] != NULL)
        printf("%s ", argumnts[i++]);

    puts("");
}

char *recoverString(char **arguments, char *delim)
{
    char *recoverString = arguments[0];
    recoverString++;

    for (char **p = arguments; *p != NULL; p++)
    {
        char *s = *p;
        while (1)
        {
            if (*s == '\0')
            {
                *s = *delim;
                break;
            }
            s++;
            if (*s == '"')
            {
                *s = '\0';
                return recoverString;
            }
        }
    }
    recoverString[strlen(recoverString) - 1] = '\0';
    return recoverString;
}

void cd(char *path)
{
    if (chdir(path) != 0)
    {
        printf("Error: No such file or directory '%s'\n", path);
    }
}

void cp(const char *source_path, const char *destination_path)
{
    char ch;
    FILE *src, *des;

    if ((src = fopen(source_path, "r")) == NULL)
    {
        perror("Error opening source file");
        return;
    }

    if ((des = fopen(destination_path, "a")) == NULL)
    {
        fclose(src);
        perror("Error opening destination file");
        return;
    }

    while ((ch = fgetc(src)) != EOF)
        fputc(ch, des);

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
        printf("%s    ", entry->d_name);
    }
    puts(" ");
    closedir(directory);
}

void my_delete(char **str)
{
    char *path = str[1];

    if (path[0] == '"')
    {
        char *end_quote = strchr(path + 1, '"');
        if (end_quote != NULL)
        {
            int length = end_quote - path - 1;
            char *temp_path = malloc(length + 1);
            if (temp_path != NULL)
            {
                strncpy(temp_path, path + 1, length);
                temp_path[length] = '\0';
                path = temp_path;
            }
            else
            {
                printf("Error: Out of memory\n");
                return;
            }
        }
        else
        {
            printf("Error: Unclosed quotes\n");
            return;
        }
    }

    // delete file
    if (unlink(path) != 0)
    {
        printf("-myShell: unlink: %s: Aucun fichier ou répertoire de ce type\n", path);
    }

    if (path != str[1])
    {
        free(path);
    }
}

void systemCall(char **arg)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}

void mypipe(char **argv1, char **argv2)
{
    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            /* stdout now goes to pipe */
            /* child process does command */
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}
