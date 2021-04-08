#ifndef _MYFUNCTION_H_
#define _MYFUNCTION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

//
//  FUNCTION PROTOTYPE
//



// DATA STRUCTURE
 
// in "vector".h"
template <typename T>
struct vector {
    // support function
    // access by index
    bool pushBack(T sth); // return false if sth is wrong when pushBack
    bool erase(int index); // return false if sth is wrong when erase
};
 
// STRUCT
struct staff {
    int id;
    string name; // optional
    string password;
};
 
struct date {
    int day, month, year;
};
 
struct lesson {
    string day; // MON/TUE/...
    string time; // '7:30', '9:30', ...
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
