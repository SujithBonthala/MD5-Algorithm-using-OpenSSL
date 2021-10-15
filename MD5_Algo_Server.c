#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "MD5_Algo_Header.h"
void step1_2(char *msg_str, char *msg, int len)
{
    //Append Padding Bits
    int j=0;
    for(int i=0;i<len;i++)
    {
        char ch = msg_str[i];
        int b = (int)ch;
        char str[8];
        itoa(b,str,2);
        for(int k=0;k<(8-strlen(str));k++)
        {
            msg[j++]='0';
        }
        for(int k=0;k<strlen(str);k++)
        {
            msg[j++]=str[k];
        }
    }
    msg[j++]='1';
    while((j+64)%512!=0)
    {
        msg[j++]='0';
    }
    //Append Length
    char a[64];
    // unsigned int i = 0;
    // for(int k=0;k<strlen(msg);k++)
    // {
    //     i++;
    // }
    // itoa(strlen,a,2);
    int i;
    for(i=0;len>0;i++)
    {
        a[i]=(char)(len%2 + 48);
        len/=2;
    }
    for(;i<64;i++)
    {
        a[i]='0';
    }
    for(int k=0;k<64;k++)
    {
        msg[j++]=a[k];
    }
    msg[j]='\0';
}

void step3(unsigned int *buffer)
{
    //initialise word buffers
    buffer[0] = 0x67452301;//A
    buffer[1] = 0xefcdab89;//B
    buffer[2] = 0x98badcfe;//C
    buffer[3] = 0x10325476;//D
}

void step4(unsigned int *s)
{
    for(int i=0;i<64;i++)
    {
        if(i<16)
        {
            if(i%4 == 0)
            {
                s[i] = 7;
            } 
            else if(i%4 == 1)
            {
                s[i] = 12;
            }
            else if(i%4 == 2)
            {
                s[i] = 17;
            }
            else if(i%4 == 3)
            {
                s[i] = 22;
            }
        }
        if(16<=i<32)
        {
            if(i%4 == 0)
            {
                s[i] = 5;
            } 
            else if(i%4 == 1)
            {
                s[i] = 9;
            }
            else if(i%4 == 2)
            {
                s[i] = 14;
            }
            else if(i%4 == 3)
            {
                s[i] = 20;
            }
        }
        if(32<=i<48)
        {
            if(i%4 == 0)
            {
                s[i] = 4;
            } 
            else if(i%4 == 1)
            {
                s[i] = 11;
            }
            else if(i%4 == 2)
            {
                s[i] = 16;
            }
            else if(i%4 == 3)
            {
                s[i] = 23;
            }
        }
        if(48<=i)
        {
            if(i%4 == 0)
            {
                s[i] = 6;
            } 
            else if(i%4 == 1)
            {
                s[i] = 10;
            }
            else if(i%4 == 2)
            {
                s[i] = 15;
            }
            else if(i%4 == 3)
            {
                s[i] = 21;
            }
        }
    }
}

int leftrotate(int x, int c)
{
    return (x << c)|(x >> (32-c));
}

int power_fn(int x, int n)
{
    if(n==0)
    {
        return 1;
    }
    else
    {
        return x*power_fn(x, n-1);
    }
}

void md5Algo(char *msg, int len, char *hash_value)
{
    unsigned int F,g,K[64];
    unsigned int hash[4] = {0,0,0,0};
    unsigned int buffer[4];
    unsigned int s[64];
    step4(s);
    for (long long int i = 0; i < 64; i++)
    {
        K[i] = (unsigned int)floor(abs(sin(i+1))*pow(2,32));
    }
    for(long long int j=0;j<len;j+=512)
    {
        step3(buffer);
        char temp[512];
        for(int i=0;i<512;i++)
        {
            temp[i]=msg[i+j];
        }
        int M[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        char temp2[32];
        int count = 0;
        while(count < 16)
        {
            for(int i=0;i<32;i++)
            {
                temp2[i] = temp[i+count*32];
            }
            int power = 31;
            for(int i=0;i<32;i++)
            {
                int power1 = (int)(temp2[i])-48;
                M[count] = M[count] + power_fn(2,power*power1);
                power--;
            }
            count++;
        }
        for(int i=0;i<64;i++)
        {
            if(0 <= i <= 15)
            { 
                F = (buffer[1]&buffer[2])|((~buffer[1])&buffer[3]);
                g = i;
            }
            else if(16 <= i <= 31)
            {
                F = (buffer[3]&buffer[1])|((~buffer[3])&buffer[2]);
                g = (5*i+1)%16;
            }
            else if(32 <= i <= 47)
            {
                F = buffer[1]^buffer[2]^buffer[3];
                g = (3*i+5)%16;
            }
            else if(48 <= i <= 63)
            {
                F = buffer[2]^(buffer[1]|(~buffer[3]));
                g = (7*i)%16;
            }
            long long int dTemp = buffer[3];
            buffer[3] = buffer[2];
            buffer[2] = buffer[1];
            buffer[1] = buffer[1] + leftrotate((buffer[0] + F + K[i] + M[g]), s[i]);
            buffer[0] = dTemp;
        } 
        hash[0] = hash[0] + buffer[0];
        hash[1] = hash[1] + buffer[1];
        hash[2] = hash[2] + buffer[2];
        hash[3] = hash[3] + buffer[3];
    }
    char hash1[12],hash2[12],hash3[12],hash4[12];
    itoa(hash[0],hash1,16);
    itoa(hash[1],hash2,16);
    itoa(hash[2],hash3,16);
    itoa(hash[3],hash4,16);
    for(int i=0;i<8;i++)
    {
        hash_value[i]=hash1[i];
        hash_value[i+8]=hash2[i];
        hash_value[i+16]=hash3[i];
        hash_value[i+24]=hash4[i];
    }
    hash_value[32]='\0';
}
void storeHash()
{
    char *message_str;
    char file_path[200];
    FILE *fp;
    printf("\nEnter the path of the file whose hash needs to be stored : ");
    scanf("%[^\n]s",file_path);
    fflush(stdin);
    fp=fopen(file_path,"r");
    if(fp==NULL)
    {
        printf("\nFile not found.\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    int fsize = ftell(fp);
    rewind(fp);
    message_str = malloc(fsize + 1);
    fread(message_str, 1, fsize, fp);
    fclose(fp);
    message_str[fsize] = '\0';
    char *message;
    int m = fsize;
    fsize = strlen(message_str)*8+513;
    message = malloc(fsize*sizeof(char));
    char *hash;
    hash = malloc(33);
    step1_2(message_str,message,m);
    md5Algo(message,strlen(message),hash);
    HASH_DATA h;
    h.file_path=malloc(strlen(file_path)+1);
    h.hash_value=malloc(33);
    strcpy(h.file_path,file_path);
    strcpy(h.hash_value,hash);
    FILE *fp1;
    fp1 = fopen("Hash Values Database.txt","a");
    fwrite(&h,sizeof(HASH_DATA),1,fp1);
    fclose(fp1);
    printf("\nHash Value of the file is stored.\n");
}
void validateFile()
{
    char *message_str;
    char file_path[200];
    FILE *fp;
    printf("\nEnter the path of the file that needs to get validated : ");
    scanf("%[^\n]s",file_path);
    fflush(stdin);
    fp=fopen(file_path,"r");
    if(fp==NULL)
    {
        printf("\nFile not found.\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    int fsize = ftell(fp);
    rewind(fp);
    message_str = malloc(fsize + 1);
    fread(message_str, 1, fsize, fp);
    fclose(fp);
    fp=NULL;
    message_str[fsize] = '\0';
    char *message;
    int m = fsize;
    fsize = strlen(message_str)*8+513;
    message = malloc(fsize*sizeof(char));
    char *hash;
    hash = malloc(33);
    step1_2(message_str,message,m);
    md5Algo(message,strlen(message),hash);
    FILE *fp1;
    fp1 = fopen("Hash Values Database.txt","r");
    if(fp1==NULL)
    {
        printf("\nDatabase is empty.\n");
        return;
    }
    // fseek(fp1,0,SEEK_END);
    // int n = ftell(fp1)/sizeof(HASH_DATA);
    // rewind(fp1);
    HASH_DATA h;
    printf("\nThe various paths of files stored in the database are as follows\n");
    while(fread(&h,sizeof(HASH_DATA),1,fp1))
    {
        printf("%s\n",h.file_path);
    }
    char file_path1[200];
    printf("\nEnter the path of the file that was stored in the database : ");
    scanf("%[^\n]s",file_path1);
    fflush(stdin);
    rewind(fp1);
    while(fread(&h,sizeof(HASH_DATA),1,fp1))
    {
        if(strcmp(h.file_path,file_path1)==0)
        {
            if(strcmp(hash,h.hash_value)==0)
            {
                printf("\nFile validated successfully. File is not tampered with in the cloud!\n");
                fclose(fp1);
                return;
            }
            else
            {
                fclose(fp1);
                printf("\nAlert! File is tampered with in the cloud.\n");
                return;
            }
        }
    }
    fclose(fp1);
    printf("\nThe given file path is not present in the database.\n");
}