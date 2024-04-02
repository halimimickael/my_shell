#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <dirent.h>

char *inputFromUser();
char **splitString(char *);
void getLocation();
char *my_strtok(char *, const char *);
char *getHostname();
void logout(char *);
void echo(char **);
char *myRecoverString(char **, char *);
void cd(char *);
void cp(const char *, const char *);
void get_dir();
char *trim(char *);
void my_delete(char **);
void systemCall(char **);
void move(char **);
void echoappend(char **);
