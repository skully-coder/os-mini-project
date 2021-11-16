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
    printf("Command line interface started, enter \"help\" for all available commands \n");
    do
    {
        printf("PC>\t");
        gets(command);
        printf("\n");
        if(strcmp(command,"list")==0)
        {
            listCommand();
        }
        else if(strcmp(command,"list -priv")==0)
        {
            listPrivCommand();
        }
        else if(strcmp(command,"help")==0)
        {
            helpCommand();
        }
        else if(strcmp(command,"cliquit")==0)
        {
            //Do nothing, while loop will automatically exit(only for correctness of command)
        }
        else
        {
            printf("Wrong Command entered!\n");
        }

    } while(strcmp(command,"cliquit")!=0);
}

void listCommand()
{
    DIR * dp;    
    struct dirent * entry;    
    struct stat statbuf;    
    if(!(dp = opendir(".")))    
    {        
        printf("Can't open directory \n");        
        exit(1);
    }    
    chdir(".");  
    while(entry = readdir(dp))    
    {        
        lstat(entry->d_name, &statbuf);        
        printf("%s\t\t\t",entry->d_name);        
        printf("\n");
    }
    printf("\n");
}

void listPrivCommand()
{
    DIR * dp;    
    struct dirent * entry;    
    struct stat statbuf;    
    if(!(dp = opendir(".")))    
    {        
        printf("Can't open directory \n");        
        exit(1);
    }    
    chdir(".");  
    while(entry = readdir(dp))    
    {        
        lstat(entry->d_name, &statbuf);        
        printf("%s\t\t\t",entry->d_name);        
        if(!S_ISDIR(statbuf.st_mode))        
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
            printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");            
            printf("\n");        
        }    
    }
    printf("\n");
}

void helpCommand()
{
    FILE *fptr;
    char* c;
    fptr = fopen("CLIHelp.txt", "r"); //Open CLIHelp.txt for reading
    if (fptr == NULL)
    {
        printf("CLIHelp.txt missing!\n");
        exit(0);
    }

    c = getc(fptr);
    while(c!=EOF)
    {
        printf("%c",c);
        c= getc(fptr);
    }
    printf("\n"); 
    fclose(fptr);
}