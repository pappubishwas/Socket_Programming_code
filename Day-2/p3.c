#include<stdio.h>

struct pkt{
unsigned char ch1;
unsigned char ch2[2];
unsigned char ch3;
};

int main(){
int n;
printf("Enter a number:");
scanf("%d",&n);
struct pkt s1;
s1.ch1=n>>24 & 0xFF;
s1.ch2[0]=n>>16 & 0xFF;
s1.ch2[1]=n>>8 & 0xFF;
s1.ch3=n & 0xFF;

printf("1st member of the structure : 0x%02X\n",s1.ch1);
printf("2nd member of the structure : 0x%02X ,  0x%02X\n",s1.ch2[0],s1.ch2[1]);

printf("3rd member of the structure : 0x%02X\n",s1.ch3);

int orginalnumber=(s1.ch1<<24) | (s1.ch2[0]<<16) | (s1.ch2[1] << 8) | s1.ch3;

printf("Orginal number regnerate: %d\n",orginalnumber);
}

