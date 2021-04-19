#ifndef _MYFUNCTION_H_
#define _MYFUNCTION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "vector.h"
#include <conio.h>

using namespace std;

enum ACTION {UP, DOWN, RIGHT, LEFT, ENTER, BACK};

// STRUCT
struct staff {
    int ID;
    string name; // optional
    string password;
};

struct date {
    int day, month, year;

    const bool operator <= (const date& other) const {
        if (year < other.year) {
            return true;
        }
        if (year == other.year) {
            if (month < other.month) {
                return true;
            }

            if (month == other.month) {
                if (day <= other.day) {
                    return true;
                }
            }
        }
        return false;
    }
};

struct lesson {
    string day; // MON/TUE/...
    string time; // '7:30', '9:30', ...

    const bool operator == (const lesson& other) const {
        return day == other.day && time == other.time;
    }
};

struct mark {
    double midterm, final, other, total;
};

struct module {
    string ID; // ID of course
    mark grade;
};

struct student {
    int no, ID; // id to login also
    string password;
    string firstName, lastName;
    string className;
    int gender; // 0: male, 1: female
    date DOB;
    string socialID;
    vector <module> enrolled;
};

struct classUni {
    string name;
    vector <student> listStudent;
};

struct course {
    string ID, name;
    string teacher;
    int numCredits;
    int maxStudents = 50;
    vector <lesson> listLesson;
    vector <student> listStudent;
};

struct semester {
    string name;
    int academicYear;
    date regOpen, regClose;
    date startDate, endDate;
    vector <course> listCourse;
};


struct schoolYear {
    vector <classUni> newClass;
    vector <semester> listSemester;
};

//
//  FUNCTION PROTOTYPE
//

//Staff function
void createNewYear();
void viewCourses(vector<course> courseList); //(task 9/19)
void viewStudentsInCourse(course crs);//(task 20)
void viewListOfClass(schoolYear _schoolYear);//(task 17)
bool deleteCourseInSemester(semester& _semester, string removeCourseID);//(task 11) return false in case there's no such course with that ID in list
void createNewClasses(schoolYear& _schoolYear);//(task 2)
void addCourseToSemester(semester& sem); //(task 8)
void createCourseRegistration(semester& sem);//(task 7)
void updateCourseInfo(semester& _semester);//(task 10)
void viewCourseScoreboard(course crs);//(task 23)
void exportStudentInCourseToCSV(course& _course);//(task 21)
void updateStudentResult(student& stu);
void viewClassScoreboard(schoolYear& _schoolYear, string className);

//Class function
void addStudentToClass(classUni className);//(task 4)
void displayStudent(const student& _student);//(task 18)
void displayClass(classUni _class);//(task 18)

//Student function
void viewEnrolledCourses(student _student, semester _semester);//(task 14)
void removeCourseFromList(student& _student, semester _semester); //(task 15) return false in case there's no such course with that ID in list
void enrollCourses(student& _student, semester& _semester);
void viewStudentScoreboard(student stu);

//Additional function
course& getCourse(semester& _semester,string courseID);
lesson getLesson(semester& _semester,string ID,int index);//get the time of the course with ID
bool checkFullSlot(semester _semester,string ID);//check if the course is full yet
void addStudentToCourse(student _student, string _courseID, semester& _semester);
bool createScoreboardFile(const course& _course);
int login(vector<staff> _staff, vector<student> _student, int& index); // Return 0 if it doesn't match any IDs and pass
                                                                       // 1 if it is staff, 2 if it is student
void gotoxy(int x, int y);
int changePassword_Staff(vector<staff>& _staff, int index); // return 0 if change successfully, 1 if old password doesn't match password, 2 if new password doesn't match each other
int changePassword_Student(vector<student>& _student, int index); // return 0 if change successfully, 1 if old password doesn't match password, 2 if new password doesn't match each other
void textColor(int color);
ACTION key(int z);
int actionList(vector<string> str, int n, COORD position);
vector<course> getUnenrolledCourseList(semester _semester, student _student);

// DATA STRUCTURE
 


#endif // !_MYFUNCTION_H_
