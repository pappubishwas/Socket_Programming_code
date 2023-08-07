#include<stdio.h>

int islitleendian(){
int num=1;
char *ptr=(char *)&num;

return (*ptr==1);
}

int main(){

printf("Host machine is %s Endian\n",islitleendian() ? "Litle" : "Big");
int n;
printf("Enter a number:");
scanf("%d",&n);
printf("Content of each byte location: \n");

char *byte=(char*)&n;

for(int i=0;i<sizeof(n);i++){
printf("Byte %d : %d\n",i+1,byte[i]);
}
int cn=0;
for(int i=0;i<sizeof(n);i++){
cn=(cn<<8)|byte[i];
}

printf("Converted number is : %d\n",cn);

return 0;

}
