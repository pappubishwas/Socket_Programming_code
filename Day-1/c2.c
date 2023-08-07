#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dob {
    int day, month, year;
};

struct student_info {
    int roll;
    char name[50];
    float cgpa;
    struct dob age;
};

// Function to print student information using call by value
void printStudentInfoByValue(struct student_info student) {
    printf("Printing student information using call by value:\n");
    printf("Roll Number: %d\n", student.roll);
    printf("Name: %s\n", student.name);
    printf("CGPA: %.2f\n", student.cgpa);
    printf("Date of Birth: %d-%d-%d\n", student.age.day, student.age.month, student.age.year);
}

// Function to print student information using call by address
void printStudentInfoByAddress(struct student_info *student) {
    printf("Printing student information using call by address:\n");
    printf("Roll Number: %d\n", student->roll);
    printf("Name: %s\n", student->name);
    printf("CGPA: %.2f\n", student->cgpa);
    printf("Date of Birth: %d-%d-%d\n", student->age.day, student->age.month, student->age.year);
}

int main() {
    struct dob dob1 = {10, 5, 2000}; // Date of birth: 10th May 2000

    struct student_info student1;
    student1.roll = 101;
    strcpy(student1.name, "John Doe");
    student1.cgpa = 8.5;
    student1.age = dob1;

    // Call the printStudentInfoByValue function to print student information using call by value
    printStudentInfoByValue(student1);

    // Call the printStudentInfoByAddress function to print student information using call by address
    printStudentInfoByAddress(&student1);

    return 0;
}
