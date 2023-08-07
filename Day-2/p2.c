#include<stdio.h>

int main(){
int num=0x12345678;

unsigned char lsb,msb;

lsb=num & 0xFF;
msb=(num>>24) & 0xFF;

printf("Number: 0x%08X\n",num);
printf("LSB: 0x%02X\n",lsb);
printf("MSB: 0x%02X\n",msb);

return 0;
}
