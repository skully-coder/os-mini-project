#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char command[1024];

void invalidCommand();
void printerror(char *errmsg);
void cdCommand();
void listCommand();
void helpCommand();
void makelink();
void removelink();
void main()
{
    system("clear");
    printf("\033[1m");
    printf("Command line interface started! Enter \"help\" for all available commands");
    printf("\033[0m");
    printf("\n\n");
    // get current working directory
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
        else if (strncmp(command, "link", 4) == 0 || strncmp(command, "link -", 6) == 0)
        {
            makelink();
        }
        else if (strncmp(command, "unlink", 6) == 0)
        {
            removelink();
        }
        else if (strcmp(command, "cliquit") == 0)
        {
            system("clear");
        }
        else if (strcmp(command, "clr") == 0)
        {
            system("clear");
        }
        // implement change directory command
        else if (strncmp(command, "chdir", 5) == 0 && command[5] == ' ')
        {
            cdCommand();
        }
        else if (strcmp(command, "") == 0)
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
    printerror("Invalid command, enter \"help\" for all available commands \n");
}

void printerror(char *errmsg)
{
    printf("\033[1;31m");
    printf("%s\a", errmsg);
    printf("\033[0m");
}

void cdCommand()
{
    // split command into tokens
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");
    if (token == NULL)
    {
        printerror("No directory specified\n");
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
                printf("%s", token);
                printf("\033[0m");
                printerror(" is not a directory\n");
            }
        }
        else
        {
            printf("\033[1;31m");
            printf("%s", token);
            printf("\033[0m");
            printerror(" does not exist\n");
        }
    }
}

void listCommand()
{
    int priv = 0, inode = 0, time = 0;
    if (strlen(command) > 6 || strlen(command) == 4)
    {
        for (int i = 6; i < strlen(command); i++)
        {
            if (command[i] == 'p')
            {
                priv = 1;
            }
            else if (command[i] == 'n')
            {
                inode = 1;
            }
            else if (command[i] == 't')
            {
                time = 1;
            }
            else if (command[i] == 'a')
            {
                priv = 1;
                time = 1;
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
        printerror("Can't open directory \n");
        return;
    }
    chdir(".");
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        lstat(entry->d_name, &statbuf);
        printf("%-12s\t", entry->d_name);
        if (priv)
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
        if (inode)
        {
            printf("%ld\t\t", entry->d_ino);
        }
        if (time)
        {
            struct stat attr;
            stat(entry->d_name, &attr);
            printf("%s", ctime(&(attr.st_mtime)));
        }
        if (!time)
            printf("\n");
    }
    closedir(dp);
}

void makelink()
{
    // makelink might have some bugs so dont combine it with main.c
    int i = 0, k = 1, t = 1;
    struct stat st;
    char file1[100], file2[100], n_command[1024];
    strcpy(n_command, command);
    int sym = 0;
    if (command[5] == '-')
    {
        for (int i = 6; i < 7; i++)
        {
            if (command[i] == 's')
            {
                sym = 1;
            }
        }
    }
    char *checker = strtok(n_command, " ");
    while (checker != NULL)
    {
        t++;
        checker = strtok(NULL, " ");
    }

    if (t < 3)
    {
        invalidCommand();
    }

    char *tokens = strtok(command, " ");
    if (sym == 1)
    {
        while (tokens != NULL)
        {
            if (tokens == "link" || tokens == "-s")
            {
                continue;
            }
            if (k == 3)
            {
                strcpy(file1, tokens);
            }
            else if (k == 4)
            {
                strcpy(file2, tokens);
            }
            k++;
            tokens = strtok(NULL, " ");
        }
    }
    else if (sym == 0)
    {
        while (tokens != NULL)
        {
            if (tokens == "link" || tokens == "-s")
            {
                continue;
            }
            if (k == 2)
            {
                strcpy(file1, tokens);
            }
            else if (k == 3)
            {
                strcpy(file2, tokens);
            }
            k++;
            tokens = strtok(NULL, " ");
        }
    }

    if (access(file1, F_OK))
    {
        perror("ERROR: filename not found\n");
        return;
    }
    if (!access(file2, F_OK))
    {
        perror("ERROR: filename already exists\n");
        return;
    }
    if (stat(file1, &st) < 0)
    {
        perror("ERROR: unable to get stat info\n");
        return;
    }
    if (!S_ISREG(st.st_mode))
    {
        perror("ERROR: not a Regular File");
        return;
    }

    if (sym == 0)
    {
        if (link(file1, file2) < 0)
        {
            perror("ERROR: unable to create the Link");
            return;
        }
        else
        {
            printf("Link created between %s and %s\n", file1, file2);
            return;
        }
    }

    else if (sym == 1)
    {
        if (symlink(file1, file2) < 0)
        {
            perror("symlink");
            return;
        }
        else
        {
            printf("Symbolic link created between %s and %s\n", file1, file2);
            return;
        }
    }
}

void removelink()
{
    int i = 0, k = 1, t = 1;
    struct stat st;
    char file1[100], file2[100], n_command[1024];
    strcpy(n_command, command);
    int sym = 0;
    char *checker = strtok(n_command, " ");
    while (checker != NULL)
    {
        t++;
        checker = strtok(NULL, " ");
    }

    if (t < 3)
    {
        invalidCommand();
    }

    char *tokens = strtok(command, " ");
    if (sym == 0)
    {
        while (tokens != NULL)
        {
            if (tokens == "unlink")
            {
                continue;
            }
            if (k == 2)
            {
                strcpy(file1, tokens);
            }
            else if (k == 3)
            {
                strcpy(file2, tokens);
            }
            k++;
            tokens = strtok(NULL, " ");
        }
    }

    if (access(file1, F_OK))
    {
        perror("ERROR: linkname not found\n");
        return;
    }
    if (stat(file1, &st) < 0)
    {
        perror("ERROR: unable to get stat info\n");
        return;
    }
    if (!S_ISREG(st.st_mode))
    {
        perror("ERROR: not a Regular File");
        return;
    }

    if (sym == 0)
    {
        if (unlink(file1) < 0)
        {
            perror("ERROR: unable to create the Link");
            return;
        }
        else
        {
            printf("Unlinked %s\n", file1);
            return;
        }
    }
}

// function to print the documentation for the project by reading CLIHelp.txt
void helpCommand()
{
    FILE *fp;
    char line[128];
    fp = fopen("CLIHelp.txt", "r");
    if (fp == NULL)
    {
        printerror("Error opening file\n");
        return;
    }
    system("clear");
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }
    fclose(fp);
}
