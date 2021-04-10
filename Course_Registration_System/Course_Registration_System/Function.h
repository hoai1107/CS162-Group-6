#ifndef _MYFUNCTION_H_
#define _MYFUNCTION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "vector.h"

using namespace std;

//
//  FUNCTION PROTOTYPE
//

//Staff function
void viewCourses(const semester& sem); //(task 9/19)
void viewStudentsInCourse(course crs);//(task 20)
void viewListOfClass(const schoolYear& _schoolYear);//(task 17)
bool deleteCourseInSemester(semester& _semester, string removeCourseID);//(task 11) return false in case there's no such course with that ID in list
void createNewClasses(schoolYear& _schoolYear);//(task 2)
void addCourseToSemester(semester& sem); //(task 8)
void createCourseRegistration(semester& sem);//(task 7)
void updateCourseInfo(semester& _semester);


//Class function
void addStudentToClass(const classUni& className);//(task 4)
void displayStudent(const student& _student);//(task 18)
void displayClass(const	classUni& _class);//(task 18)

//Student function
void viewEnrolledCourses(const student& _student, const semester& _semester);//(task 14)
bool removeCourseFromList(student& _student, string removeCourseID); //(task 15) return false in case there's no such course with that ID in list
void enrollCourses(student& _student, const semester& _semester);
void viewStudentScoreboard(student stu);
//Additional function
course& getCourse(semester _semester,string courseID);
lesson getLesson(semester& _semester,string ID);//get the time of the course with ID
bool checkFullSlot(string ID);//check if the course is full yet
void addStudentToCourse(student _student, string _courseID, semester& _semester);
int login(vector<staff> _staff, vector<student> _student); // Return 0 if it doesn't match any IDs and pass
                                                           // 1 if it is staff, 2 if it is student
void gotoxy(int x, int y);

// DATA STRUCTURE
 
// STRUCT
struct staff {
    int ID;
    string name; // optional
    string password;
};
 
struct date {
    int day, month, year;
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

#endif // !_MYFUNCTION_H_
