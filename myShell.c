
#include "MyShell.h"
#include "MyFunctionShell.h"
int main()
{
    welcome();
    getLocation();
    while (1)
    {
        char *str = inputFromUser();
        puts(str);

        char **argumnts = splitString(str);

        for (int i = 0; argumnts[i] != NULL; i++)
        {
            printf("index %d -> ", i);

            puts(argumnts[i]);
        }
        free(str);
        free(argumnts);
        break;
    }

    return 0;
}
void welcome()
{
    char *logo[] = {
        "                                                                                                          \n",
        "               ___       __   __         ___    ___  __                   __        ___                   \n",
        "         |  | |__  |    /  ` /  \  |\/| |__      |  /  \     |\/| \ /    /__` |__| |__  |    |            \n",
        "-------------------------------------\033[1m Mickael Halimi-----------------------------------------------\n",
        "         |/\| |___ |___ \__, \__/  |  | |___     |  \__/     |  |  |     .__/ |  | |___ |___ |___         \n",
        '\0'};

    int i = 0;
    purple();
    // printf();
    green();
    do
    {
        printf("%s", logo[i++]);
        if (i % 2 != 0 || i > 26)
            green();
        else
            blue();
    } while (logo[i]);
    reset();
    puts("\n");
}

void blue()
{
    printf("\033[0;34m");
}
void green()
{
    printf("\033[0;32m");
}
void purple()
{
    printf("\033[0;35m");
}
void reset()
{
    printf("\033[0m");
}