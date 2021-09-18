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
void md5Algo(unsigned char *msg, int len_org, int *len)
{
    step1(msg,len);
    step2(msg,len_org,len);
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
