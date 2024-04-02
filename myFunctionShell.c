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

    if (*next_token == '\0')
        return NULL;

    token_start = next_token;

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

char *myRecoverString(char **arguments, char *delim)
{
    char *recoveredString = arguments[0] + 1;

    char **p = arguments;
    while (*p != NULL)
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
                return recoveredString;
            }
        }
        p++;
    }

    recoveredString[strlen(recoveredString) - 1] = '\0';
    return recoveredString;
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

void my_delete(char **args)
{
    if (args[1] == NULL)
    {
        printf("Usage: delete [file_path]\n");
        return;
    }

    char *path = strdup(args[1]);

    if (*path == '"')
    {
        path = myRecoverString(args + 1, " ");
    }

    if (unlink(path) != 0)
    {
        printf("Error: Failed to delete '%s'\n", path);
    }
    else
    {
        printf("Deleted '%s' successfully\n", path);
    }

    free(path);
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
    int pipefd[2];
    pid_t pid;

    // Create pipe
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork child process
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Execute the first command
        if (execvp(argv1[0], argv1) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        // Execute the second command
        if (execvp(argv2[0], argv2) == -1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
}

void move(char **arguments)
{
    if (arguments[1] == NULL || arguments[2] == NULL)
    {
        if (arguments[1] == NULL)
            printf("mv: source file missing\n");
        else
            printf("mv: destination directory missing after '%s'\n", arguments[1]);
        return;
    }

    if (arguments[3] != NULL)
    {
        printf("mv: too many arguments\n");
        return;
    }

    char *filename = arguments[1];
    char *last_slash = filename;
    while (*filename != '\0')
    {
        if (*filename == '/')
        {
            last_slash = filename + 1;
        }
        filename++;
    }

    char *destination_path = malloc(strlen(arguments[2]) + strlen(last_slash) + 2);
    if (destination_path == NULL)
    {
        perror("mv: memory allocation failed");
        return;
    }
    sprintf(destination_path, "%s/%s", arguments[2], last_slash);

    int result = rename(arguments[1], destination_path);

    if (result != 0)
    {
        printf("mv: failed to move '%s' to '%s'\n", arguments[1], arguments[2]);
    }
    else
    {
        printf("'%s' successfully moved to '%s'\n", arguments[1], arguments[2]);
    }

    free(destination_path);
}

void echoappend(char **arguments)
{
    char *fileName = NULL;

    // Find the file name
    for (char **arg = arguments + 1; *arg != NULL; arg++)
    {
        if (**arg == '>')
        {
            fileName = *(arg + 1);
            if (*fileName == '"')
            {
                fileName = myRecoverString(arguments + (arg - arguments + 1), " ");
            }
            break;
        }
    }

    if (fileName == NULL)
    {
        printf("File name not provided.\n");
        return;
    }

    // Open file in append mode
    FILE *file = fopen(fileName, "a");
    if (file == NULL)
    {
        printf("File '%s' does not exist. Creating new file.\n", fileName);
        // If file doesn't exist, open in write mode to create a new file
        file = fopen(fileName, "w");
        if (file == NULL)
        {
            printf("Error creating file '%s'.\n", fileName);
            return;
        }
    }

    // Write arguments to file
    for (char **arg = arguments + 1; *arg != NULL && **arg != '>'; arg++)
    {
        fprintf(file, "%s ", *arg);
    }

    fclose(file);
}
