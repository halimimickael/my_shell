
#include "MyShell.h"
#include "MyFunctionShell.h"

int main()
{
    welcome();
    while (1)
    {
        getLocation();
        char *str = inputFromUser();

        if (strcmp(str, "exit") == 0)
            logout(str);
        char **argumnts = splitString(str);
        if (strcmp(str, "echo") == 0)
            echo(argumnts);
        if (strcmp(str, "cd") == 0)
            cd(argumnts);
        if (strcmp(str, "cp") == 0)
            cp(argumnts);
        if (strcmp(str, "dir") == 0)
            get_dir();

        free(str);
        free(argumnts);
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