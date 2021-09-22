#include<openssl/ssl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
void step1(unsigned char *msg, int *len)
{
    //Append Padding Bits
    msg[*len]=(char)(128);
    (*len)++;
    int ref1=(*len)*8+8;
    while((ref1+64)%512!=0)
    {
        msg[*len]=(char)0;
        (*len)++;
        ref1+=8;
    }
}
void step2(unsigned char *msg, int len_org, int *len)
{
    //Append Length
    int a[64];
    int i;
    for(i=0;len_org>0;i++)
    {
        a[i]=len_org%2;
        len_org/=2;
    }
    for(;i<64;i++)
    {
        a[i]=0;
    }
    int b=0;
    for(int j=63;j>=56;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=55;j>=48;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=47;j>=40;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=39;j>=32;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=31;j>=24;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=23;j>=16;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=15;j>=8;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
    b=0;
    for(int j=7;j>=0;j--)
    {
        b+=(a[j]*(int)(pow(2,j%8)));
    }
    msg[*len]=(char)b;
    (*len)++;
}

int *step3(int *buffer)
{
    //initialise word buffers
    buffer[0] = 0x67452301;//A
    buffer[1] = 0xefcdab89;//B
    buffer[2] = 0x98badcfe;//C
    buffer[3] = 0x10325476;//D
    return buffer;

}

int *step4(int *s)
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
    return s;
}

int leftrotate(int x,int c)
{
    return (x << c)|(x >> (32-c));
}

void md5Algo(unsigned char *msg, int len_org, int *len)
{
    step1(msg,len);
    step2(msg,len_org,len);
    int k = (strlen(msg)+1)/64;
    int l = 0;
    int F,g,K[64];
    int hash[4] = {0,0,0,0};
    int buffer[4];
    step3(buffer);
    int s[64];
    step4(s);
    for (int i = 0; i < 64; i++)
    {
        K[i] = sin(i+1)*pow(2,32);
    }   
    for(int j = 64*l;j<64*k;l++)
    {
        char *temp = &msg[64*j];
        int M[16];
        M =  (int *)temp;
        for(int i = 0;i<64;i++)
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
            int dTemp = buffer[3];
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
    char hash_value[16] = (char)hash[0]+(char)hash[1]+(char)hash[2]+(char)hash[3];
}
int main(int argc, char *argv[])
{
    unsigned char *message_str;
    printf("MD5 Algorithm Implementation\n");
    int n;
    printf("\nEnter the length of the message string : ");
    scanf("%d",&n);
    fflush(stdin);
    message_str=malloc((n+16)*sizeof(unsigned char));
    printf("Enter the message string : ");
    scanf("%[^\n]s",message_str);
    int m=n;
    md5Algo(message_str,n,&m);
    //printf(NULL);
    //printf("%s\n",message_str);
    return 0;
}
