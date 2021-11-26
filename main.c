#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char command[128];

void invalidCommand();
void cdCommand();
void listCommand();
void helpCommand();
void main()
{
    system("clear");
    printf("\033[1m");
    printf("Command line interface started! Enter \"help\" for all available commands");
    printf("\033[0m");
    printf("\n\n");
    //get current working directory
    char cwd[1024];
    char hostname[1024];
    do
    {
        // print current user in yellow
        getcwd(cwd, sizeof(cwd));
        printf("\033[1m");
        gethostname(hostname, sizeof(hostname));
        printf("%s@%s:", getlogin(), hostname);
        printf("\033[0m");
        printf("\033[0;34;1m");
        printf("\x1b[1m"); 
        printf("%s> ", cwd);
        printf("\x1b[0m");
        printf("\033[0m");
        // read command from user
        gets(command);
        if (strcmp(command, "list") == 0 || strncmp(command, "list -", 6) == 0)
        {
            listCommand();
        }
        else if (strcmp(command, "help") == 0)
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
        // implement change directory command
        else if (strncmp(command, "cd", 2) == 0 && command[2] == ' ')
        {
            cdCommand();
        }
        else if(strcmp(command, "") == 0)
        {

        }
        else
        {
            invalidCommand();
        }

    } while (strcmp(command, "cliquit") != 0);
}

void invalidCommand()
{
    printf("\033[1;31m");
    printf("Invalid command, enter \"help\" for all available commands \n");
    printf("\033[0m");
}

void cdCommand()
{
    // split command into tokens
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");
    if (token == NULL)
    {
        printf("\033[1;31m");
        printf("No directory specified\n");
        printf("\033[0m");
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
                printf("\033[1;31m");
                printf("%s is not a directory\n", token);
                printf("\033[0m");
            }
        }
        else
        {
            printf("\033[1;31m");
            printf("%s does not exist\n", token);
            printf("\033[0m");
        }
    }
}

void listCommand()
{
    int priv = 0, inode = 0;
    if(strlen(command)>6 || strlen(command) ==4)
    {
        for(int i=6;i<strlen(command); i++)
        {
            if(command[i]=='p')
            {
                priv = 1;
            }
            else if(command[i]=='n')
            {
                inode = 1;
            }
            else
            {
                invalidCommand();
                return;
            }
        }
    }
    else
    {
        invalidCommand();
        return;
    }

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if (!(dp = opendir(".")))
    {
        printf("Can't open directory \n");
        exit(1);
    }
    chdir(".");
    while ((entry = readdir(dp)) != NULL)
    {
        if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)
            continue;
        lstat(entry->d_name, &statbuf);
        printf("%-16s\t", entry->d_name);
        if(priv)
        {
            printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
            printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
            printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
            printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
            printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
            printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
            printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
            printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
            printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
            printf((statbuf.st_mode & S_IXOTH) ? "x\t" : "-\t");
        }
        if(inode)
        {
            printf("%ld\t", entry->d_ino);
        }
        printf("\n");  
    }
    closedir(dp);
}

// function to print the documentation for the project by reading CLIHelp.txt
void helpCommand()
{
    system("clear");
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
