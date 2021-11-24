#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void listCommand();
void listPrivCommand();
void helpCommand();
void main()
{
    char command[128];
    printf("\nCommand line interface started, enter \"help\" for all available commands \n");

    //get current working directory
    char cwd[1024];
    char hostname[1024];
    do
    {
        // print current user in yellow
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;33m");
        gethostname(hostname, sizeof(hostname));
        printf("%s@%s:", getlogin(), hostname);
        printf("\033[0m");
        printf("\033[0;31m");
        printf("\x1b[1m"); 
        printf("%s> ", cwd);
        printf("\x1b[0m");
        printf("\033[0m");

        // read command from user
        gets(command);
        printf("\n");
        if (strcmp(command, "list") == 0)
        {
            listCommand();
        }
        else if (strcmp(command, "list -priv") == 0)
        {
            listPrivCommand(".", 0);
        }
        else if (strcmp(command, "help") == 0 || strcmp(command, "info") == 0)
        {
            helpCommand();
        }
        else if (strcmp(command, "cliquit") == 0)
        {
            // Do nothing, while loop will automatically exit(only for correctness of command)
        }
        else if(strcmp(command, "cls") == 0)
        {
            system("clear");
        }
        else if(strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
        {
            exit(0);
        }
        // implement change directory command
        else if (strncmp(command, "cd", 2) == 0 && command[2] == ' ')
        {
            // split command into tokens
            char *token = strtok(command, " ");
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                printf("No directory specified\n");
            }
            else
            {
                // check if directory exists
                struct stat st;
                if (stat(token, &st) == 0)
                {
                    if (S_ISDIR(st.st_mode))
                    {
                        chdir(token);
                    }
                    else
                    {
                        printf("%s is not a directory\n", token);
                    }
                }
                else
                {
                    printf("%s does not exist\n", token);
                }
            }
        }
        else
        {
            printf("\033[1;31m");
            printf("Invalid command, enter \"help\" for all available commands \n");
            printf("\033[0m");
        }

    } while (strcmp(command, "cliquit") != 0 || strcmp(command, "quit") != 0 || strcmp(command, "exit") != 0);
}

void listCommand()
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if (!(dp = opendir(".")))
    {
        printf("Can't open directory \n");
        exit(1);
    }
    chdir(".");
    while (entry = readdir(dp))
    {
        lstat(entry->d_name, &statbuf);
        printf("%s\t\t\t", entry->d_name);
        printf("\n"); 
    }
    printf("\n");
}

// function to print the documentation for the project by reading CLIHelp.txt
void helpCommand()
{
    FILE *fp;
    char line[128];
    fp = fopen("CLIHelp.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }
    fclose(fp);
}

void listPrivCommand(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "Cannot Open Directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            printf("%*s%s\n", depth, " ", entry->d_name);
            listPrivCommand(entry->d_name, depth + 4);
        }
        else
        {
            printf("%*s", depth, " ");
            printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
            printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
            printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
            printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
            printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
            printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
            printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
            printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
            printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
            printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
            printf("\t%s", entry->d_name);
            printf("\n");
        }
    }
    closedir(dp);
}
