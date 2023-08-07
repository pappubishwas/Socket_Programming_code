#include<stdio.h>
#include<stdlib.h>
#include <string.h>
struct dob{
    int day,month,year;
};

struct student_info{
    int roll;
    char name[50];
    float cgpa;
    struct dob age;
};


void printStudentInfoByValue(struct student_info student) {
    printf("Roll Number: %d\n", student.roll);
    printf("Name: %s\n", student.name);
    printf("CGPA: %.2f\n", student.cgpa);
    printf("Date of Birth: %d-%d-%d\n", student.age.day, student.age.month, student.age.year);
}

void printStudentInfoByAddress(struct student_info *student){
      printf("Roll Number: %d\n", student->roll);
    printf("Name: %s\n", student->name);
    printf("CGPA: %.2f\n", student->cgpa);
    printf("Date of Birth: %d-%d-%d\n", student->age.day, student->age.month, student->age.year);
}
int main(int argc, char *argv[]){
struct dob dob1 = {14, 2, 2002}; // Date of birth: 10th May 2000

struct student_info student1;
student1.roll = 101;
strcpy(student1.name, "John Doe");
student1.cgpa = 8.5;
student1.age = dob1;
 printStudentInfoByValue(student1);
 printStudentInfoByAddress(&student1);
 
}
