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
    string ID;
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
    double midterm = 0, final = 0, other = 0, total = 0;
};

struct module {
    string nameSem;
    string ID; // ID of course
    mark grade;
};

struct student {
    int no;
    string ID; // id to login also
    string password;
    string firstName, lastName, fullName;
    string className;
    string gender;
    date DOB;
    string socialID;
    Vector <module> enrolled;
};

struct classUni {
    string name;
    Vector <student> listStudent;
};

struct course {
    string ID, name;
    string teacher;
    int numCredits;
    int maxStudents = 50;
    Vector <lesson> listLesson;
    Vector <student> listStudent;
};

struct semester {
    string name;
    string academicYear;
    date regOpen, regClose;
    date startDate, endDate;
    Vector <course> listCourse;
};


struct schoolYear {
    string name;
    Vector <classUni> newClass;
    Vector <semester> listSemester;
};

//
//  FUNCTION PROTOTYPE
//

//Staff function
void createNewYear(Vector<schoolYear> &allYear);
void viewCourses(Vector<course> &courseList); //(task 9/19)
void viewStudentsInCourse(course crs);//(task 20)
void viewListOfClass(schoolYear _schoolYear);//(task 17)
bool deleteCourseInSemester(semester& _semester);//(task 11) return false in case there's no such course with that ID in list
void createNewClasses(schoolYear& _schoolYear);//(task 2)
void createSemester(schoolYear& _schoolYear);//(task 6)
void addCourseToSemester(semester& sem); //(task 8)
void createCourseRegistration(semester& _semester);//(task 7)
void updateCourseInfo(semester& _semester);//(task 10)
void viewCourseScoreboard(semester& _semester);//(task 23)
void exportStudentInCourseToCSV(semester& _semester);//(task 21)
void importScoreboard(schoolYear& _schoolYear, semester& _semester);
void updateStudentResult(schoolYear& _schoolYear, semester& _semester);
void viewClassScoreboard(schoolYear& _schoolYear, semester& _semester);

//Class function
void addStudentToClass(classUni& className);//(task 4)
void displayStudent(const student& _student);//(task 18)
void displayClass(classUni _class);//(task 18)

//Student function
void viewEnrolledCourses(student _student, semester _semester);//(task 14)
void removeCourseFromList(student& _student, semester _semester); //(task 15) return false in case there's no such course with that ID in list
void enrollCourses(student& _student, semester& _semester);
void viewStudentScoreboard(schoolYear year, student stu);

//Additional function
course& getCourse(semester& _semester);
lesson getLesson(semester& _semester,string ID,int index);//get the time of the course with ID
bool checkFullSlot(semester _semester,string ID);//check if the course is full yet
void addStudentToCourse(student _student, string _courseID, semester& _semester);
void createScoreboardFile(course& _course, semester _semester);
int login(Vector<staff> _staff, Vector<schoolYear> _year, int& userIndex, int& classIndex, int& yearIndex); // Return 0 if it doesn't match any IDs and pass
                                                                       // 1 if it is staff, 2 if it is student
void gotoxy(int x, int y);
int changePassword_Staff(Vector<staff>& _staff, int index); // return 0 if change successfully, 1 if old password doesn't match password, 2 if new password doesn't match each other
int changePassword_Student(Vector<student>& _student, int index); // return 0 if change successfully, 1 if old password doesn't match password, 2 if new password doesn't match each other
void textColor(int color);
ACTION key(int z);
int actionList(Vector<string> str, COORD position);
Vector<course> getUnenrolledCourseList(semester _semester, student _student);

void loadSemesterInfo(schoolYear& _schoolYear, Vector<semester>& _semester);
void loadCourseInfo(schoolYear& _schoolYear, semester& _semester, course& _course);
void saveCourseInfo(semester& _semester, course& _course);
void saveSemesterInfo(Vector<semester>& _semester);
void loadLastSave(Vector <schoolYear> &listYear, Vector<staff> &_staff);
void loadStaff(Vector<staff> &_staff);
void saveAccountInfo(Vector<schoolYear> _year, Vector<staff> _staff);
void viewUserInfo(int studentOrStaff, staff _staff, student _student);

bool IsPathExist(const string &s);
void allStaffFunction(Vector <staff>& _staff, Vector <schoolYear>& allYear);
void chooseAcademicYear(Vector<schoolYear> &allYear);
void editSchoolYear(schoolYear &year);
void addClassToYear(schoolYear &year);
void addSemesterToYear(schoolYear &year);
void saveSemester(semester& _semester);
void editSemester(schoolYear& _schoolYear, semester& _semester);
int chooseSemester(schoolYear& _schoolYear);
void exportScoreboard(semester& _semester, course& _course, bool empty = true);
bool readScoreboard(schoolYear& _schoolYear, semester& _semester, course& _course);
void initiateCourseList(schoolYear& _schoolYear, semester& _semester, course& _course);
classUni& getClass(schoolYear& _schoolYear);

void saveStudentInCourse(Vector<schoolYear>& _year);
// void fakeFunction();

#endif // !_MYFUNCTION_H_
