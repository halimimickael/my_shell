
#include "MyShell.h"
#include "MyFunctionShell.h"

int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char *str = inputFromUser();

        if (strncmp(str, "exit", 4) == 0)
        {
            logout(str);
            break;
        }

        char **arguments = splitString(str);
        char *command = arguments[0];

        if (strcmp(command, "echo") == 0)
            echo(arguments);
        else if (strcmp(command, "cd") == 0)
            cd(arguments);
        else if (strcmp(command, "cp") == 0)
        {
            if (arguments[1] == NULL || arguments[2] == NULL)
                printf("Usage: cp source_file destination_file\n");
            else
                cp(arguments[1], arguments[2]);
        }
        else if (strcmp(command, "dir") == 0)
            get_dir();
        else
            printf("Unknown command: %s\n", command);

        free(str);
        free(arguments);
    }

    return 0;
}

void welcome()
{
    char *logo[] = {
        "                          ╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭╮╱╱╱╱╭╮╭╮            \n",
        "                          ╱╱╱╱╱╱╱╱┃┃╱╱╱╱╱╱╱╱╱╱╱╱╱╭╯╰╮╱╱╱╱╱╱╱╱╱╱╱╱╱╱┃┃╱╱╱╱┃┃┃┃            \n",
        "                          ╭╮╭╮╭┳━━┫┃╭━━┳━━┳╮╭┳━━╮╰╮╭╋━━╮╭╮╭┳╮╱╭╮╭━━┫╰━┳━━┫┃┃┃            \n",
        "                          ┃╰╯╰╯┃┃━┫┃┃╭━┫╭╮┃╰╯┃┃━┫╱┃┃┃╭╮┃┃╰╯┃┃╱┃┃┃━━┫╭╮┃┃━┫┃┃┃            \n",
        "                          ╰╮╭╮╭┫┃━┫╰┫╰━┫╰╯┃┃┃┃┃━┫╱┃╰┫╰╯┃┃┃┃┃╰━╯┃┣━━┃┃┃┃┃━┫╰┫╰╮           \n",
        "                          ╱╰╯╰╯╰━━┻━┻━━┻━━┻┻┻┻━━╯╱╰━┻━━╯╰┻┻┻━╮╭╯╰━━┻╯╰┻━━┻━┻━╯           \n",
        "                          ╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╭━╯┃                          \n",
        "                          ╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╱╰━━╯                          \n",
        "                          -------------------\033[1m Mickael Halimi -----------------           \n",
        '\0'};

    int i = 0;
    purple();
    green();
    do
    {
        printf("%s", logo[i++]);
        if (i % 2 != 0 || i > 26)
            red();
        else
            purple();
    } while (logo[i]);
    reset();
    puts("\n");
}

void blue()
{
    printf("\033[1;34m");
}
void green()
{
    printf("\033[1;32m");
}
void purple()
{
    printf("\033[1;35m");
}
void red()
{
    printf("\033[1;31m");
}
void reset()
{
    printf("\033[0m");
}
