#include "Function.h"
#include<fstream>

using namespace std;

// FUNCTION PROTOTYPE
// staff function
void viewCourses(semester sem); //(task 9)
//Class function
void addStudentToClass(const classUni& className);
void displayStudent(const student& _student);
void displayClass(const	classUni& _class);

//Student function
void viewEnrolledCourses(const student& _student, const semester& _semester);
bool removeCourseFromList(student& _student, string removeCourseID); //return false in case there's no such course with that ID in list
void viewListOfClass(const schoolYear& _schoolYear);



void addStudentToClass(classUni& className) {
	ifstream fin;
	const string name = className.name + ".csv";
	fin.open(name);
	int i = 1;
	while (fin.is_open()) {
		student st;
		st.no = i;
		fin >> st.ID;
		fin.ignore(1, ',');

		getline(fin, st.firstName, ',');
		getline(fin, st.lastName, ',');

		fin >> st.gender;
		fin.ignore(1, ',');

		fin >> st.DOB.day;
		fin.ignore(1, '/');
		fin >> st.DOB.month;
		fin.ignore(1, '/');
		fin >> st.DOB.year;
		fin.ignore(1, '/');

		fin >> st.socialID;
        fin.ignore();
        ++i;
	}
	fin.close();
}

void displayStudent(const student& _student) {
	const string sep = string(2, ' ');
	cout << _student.no << sep;
	cout << _student.ID << sep;
	cout << _student.firstName << ' ' << _student.lastName << sep;
	cout << _student.DOB.day << '/' << _student.DOB.month << '/' << _student.DOB.year << sep;
	cout << _student.socialID;

	cout << endl;
}

void displayClass(const classUni& _class) {
	cout << "Class " << _class.name << endl;
	for (int i = 0; i < size; ++i) {
		displayStudent(_class.listStudent[i]);
	}
}

void viewEnrolledCourses(const student& _student,const semester& _semester) {
	for (int i = 0; i < size1; ++i) {
		string courseID = _student.enrolled[i].ID;
		for (int j = 0; j < size2; ++j) {
			if (courseID == _semester.listCourse[i].ID) {
				cout << _semester.listCourse[i].name;
				break;
			}
		}
	}
}

bool removeCourseFromList(student& _student, string removeCourseID) {
	for (int i = 0; i < size; ++i) {
		if (removeCourseID == _student.enrolled[i].ID) {
			_student.enrolled.erase(i);
			return true;
		}
	}
	return false;
}

void viewListOfClass(const schoolYear& _schoolYear) {
	for (int i = 0; i < size; ++i) {
		cout << _schoolYear.newClass[i] << endl;
	}
}

void viewCourses(semester sem) {
	cout << left << setw(10) << "ID" << setw(50) << "Name" << setw(30) << "Teacher" << setw(10) << "Credits" << endl;
	for (int i = 0; i < sem.listCourse.size; i++) {
		cout << left << setw(10) << sem.listCourse[i].ID << setw(50) << sem.listCourse[i].name << setw(30) << sem.listCourse[i].teacher
			 << setw(10) << sem.listCourse[i].numCredits << endl;
	}
}	

void viewStudentsInCourse(course crs) {
	cout << left << setw(5) << "No" << setw(10) << "ID" << setw(20) << "Last name"
		<< setw(15) << "First name" << setw(15) << "Class name" << setw(10) << "Gender"
		<< setw(20) << "Date of birth" << setw(15) << "Social ID" << endl;
	for (int i = 0; i < crs.listStudent.size; i++) {
		string gnd;
		if (crs.listStudent[i].gender == 0)
			gnd = "male";
		else gnd = "female"
		cout << left << setw(5) << crs.listStudent[i].no << setw(10) << crs.listStudent[i].ID << setw(20) << crs.listStudent[i].lastName
			<< setw(15) << crs.listStudent[i].firstName << setw(15) << crs.listStudent[i].className << setw(10) << gnd
			<< setw(20) << crs.listStudent[i].DOB.day << '/' << crs.listStudent[i].DOB.month << '/' << crs.listStudent[i].DOB.year 
			<< setw(15) << "Social ID" << endl;
	}
}