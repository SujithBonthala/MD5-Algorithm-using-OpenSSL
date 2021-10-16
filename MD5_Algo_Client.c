#include<stdio.h>
#include "MD5_Algo_Header.h"
int main(int argc, char *argv[])
{
    printf("\nFile Authentication using MD5\n");
    int choice;
    printf("\n1. Create a MD5 hash to be stored in our database\n");
    printf("2. Validate your downloaded file to the pre-existing hashes in our database\n");
    printf("3. Exit\n");
    printf("Enter your choice : ");
    scanf("%d",&choice);
    fflush(stdin);
    switch(choice)
    {
        case 1:
            storeHash();
            break;
        case 2:
            validateFile();
            break;
        case 3:
        break;
    }
    return 0;
}
