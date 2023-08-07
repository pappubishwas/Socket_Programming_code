#include<stdio.h>

int main(){
int n;
printf("Enter a number : ");
scanf("%d",&n);
unsigned char byte1=(n>>24) & 0xFF;
unsigned char byte2=(n>>16) & 0xFF;
unsigned char byte3=(n>>8) & 0xFF;
unsigned char byte4=n & 0xFF;

printf("Byte1 : 0x%02X\n",byte1);
printf("Byte2 : 0x%02X\n",byte2);
printf("Byte3 : 0x%02X\n",byte3);
printf("Byte4 : 0x%02X\n",byte4);
}
