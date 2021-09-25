#include<openssl/ssl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void step1_2(unsigned char *msg_str, char *msg, long long int len)
{
    //Append Padding Bits
    int j=0;
    for(int i=0;i<strlen(msg_str);i++)
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
    char a[64];
    int i;
    len=len%(long long int)(pow(2,64));
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

void step3(int *buffer)
{
    //initialise word buffers
    buffer[0] = 0x67452301;//A
    buffer[1] = 0xefcdab89;//B
    buffer[2] = 0x98badcfe;//C
    buffer[3] = 0x10325476;//D
}

void step4(int *s)
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

int leftrotate(unsigned int x, unsigned int c)
{
    return (x << c)|(x >> (32-c));
}

void md5Algo(char *msg, int len)
{
    unsigned int F,g,K[64];
    unsigned int hash[4] = {0,0,0,0};
    unsigned int buffer[4];
    int s[64];
    step4(s);
    for (int i = 0; i < 64; i++)
    {
        K[i] = (unsigned int)floor(abs(sin(i+1))*pow(2,32));
    }
    for(int j=0;j<len;j+=512)
    {
        step3(buffer);
        char temp[512];
        for(int i=0;i<512;i++)
        {
            temp[i]=msg[i+j];
        }
        int M[16];
        for(int i=0;i<16;i++)
        {
            M[i] = (int)temp[32*i];
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
                g = (5*i + 1)%16;
            }
            else if(32 <= i <= 47)
            {
                F = buffer[1]^buffer[2]^buffer[3];
                g = (3*i + 5)%16;
            }
            else if(48 <= i <= 63)
                F = buffer[2]^(buffer[1]|(~buffer[3]));
                g = (7*i)%16;
            unsigned int dTemp = buffer[3];
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
    //printf("%d %d %d %d\n",hash[0],hash[1],hash[2],hash[3]);
    char hash1[12],hash2[12],hash3[12],hash4[12];
    itoa(hash[0],hash1,16);
    itoa(hash[1],hash2,16);
    itoa(hash[2],hash3,16);
    itoa(hash[3],hash4,16);
    char hash_value[33];
    for(int i=0;i<8;i++)
    {
        hash_value[i]=hash1[i];
        hash_value[i+8]=hash2[i];
        hash_value[i+16]=hash3[i];
        hash_value[i+24]=hash4[i];
    }
    hash_value[32]='\0';
    printf("Hash Value : %s\n",hash_value);
}
int main(int argc, char *argv[])
{
    unsigned char *message_str;
    printf("MD5 Algorithm Implementation\n");
    int n;
    printf("\nEnter the length of the message string : ");
    scanf("%d",&n);
    fflush(stdin);
    message_str=malloc(n*sizeof(unsigned char)+1);
    printf("Enter the message string : ");
    scanf("%[^\n]s",message_str);
    fflush(stdin);
    char *message;
    int m = strlen(message_str);
    n = strlen(message_str)*8+513;
    message = malloc(n*sizeof(char));
    step1_2(message_str,message,m);
    md5Algo(message,strlen(message));
    return 0;
}
