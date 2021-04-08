#include "Function.h"
#include<fstream>

using namespace std;

// FUNCTION PROTOTYPE

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