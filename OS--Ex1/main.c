/** מגישים
* דביר גב 209530583
* דניאל זקן 207296989
*/
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "TCP.h"
#include <stdio.h>
#include <stdlib.h>

#define lenght 100

void printDir()
{
    DIR *dir;
    struct dirent *sdir;
    dir = opendir(".");
    if (dir == NULL)
    {
        printf("Erro Somthing Bad Happened :(\n");
        return;
    }
    while ((sdir = readdir(dir)) != NULL)
    {
        if (sdir->d_name[0] != '.')
        {
            printf("%s\t", sdir->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}
void updateLocation(char *location)
{
    if (getcwd(location, lenght) == NULL)
    {
        printf("Erro Somthing Bad Happened :(\n");
        return 1;
    }
}
int main(int argc, char const *argv[])
{
    char command[lenght];
    char location[lenght];

    while (strcmp(command, "EXIT"))
    {
        updateLocation(location);
        // printf("yes master>$ ");
        printf("%s>", location);
        gets(command);
        if (strncmp(command, "ECHO ", 5) == 0)
        {
            for (int i = 5; i < lenght && command[i] != '\0'; i++)
            {
                printf("%c", command[i]);
            }
            printf("\n");
        }
        else if(strncmp(command, "TCP PORT", 8) == 0)
        {
            clinet();
            dup2(1,410);
            dup2(sock, 1);
        }
        else if(strncmp(command, "LOCAL", 5) == 0)
        {
            close(sock);
            dup2(410,1);
        }
        else if (strcmp(command, "DIR") == 0)
        {
            printDir();
        }
        else if (strncmp(command, "CD ", 3) == 0)
        {
            if (chdir(&(command[3])) != 0)
            {
                printf("Erro Somthing Bad Happened :(\n");
                return 1;
            }
            // chdir היא פונקצית קריאה למערכת
        }
        else if (strncmp(command, "COPY ", 5) == 0)
        {  
            char srcFile[lenght],desFile[lenght];
            int i=5,j1=0,j2=0;
            while(command[i] != ' '){

                    srcFile[j1++]=command[i++];
            }

            srcFile[j1]='\0';

            while(command[++i] != '\0'){
                    desFile[j2++]=command[i];
            }
            desFile[j2]='\0';
            FILE* fileSrc=NULL;
            FILE* fileDes=NULL;
            fileSrc=fopen(srcFile,"r");
            fileDes=fopen(desFile,"w+");
            char temp;
            if(fileSrc==NULL||fileDes==NULL){
                printf("Erro Somthing Bad Happened,problem with the files :(\n");
                fclose(fileSrc);
                fclose(fileDes);
                return 1;
            }
           
            while (fscanf(fileSrc,"%c",&temp)!=EOF)
            {  
                fprintf(fileDes,"%c",temp);
            }
            fclose(fileSrc);
            fclose(fileDes);
            
            
        
        }
        else if (strncmp(command, "DELETE ", 7) == 0)
        {
            unlink(&(command[7]));
        }
        else
        {
            // system(command);
            // system היא פונקצית קריאה למערכת

            int check = fork();
            if(check<0){
                printf("Erro Somthing Bad Happened :(\n");
                return 1;
            }
            if (check==0){
                char str1[lenght]="/bin/";
                strcat(str1,command);
                execlp(str1,command,NULL);
            }
            wait();
        }
            
        
    }
    return 0;
}
