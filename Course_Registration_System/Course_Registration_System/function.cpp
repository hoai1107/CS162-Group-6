#include "Function.h"
#include<fstream>

using namespace std;

void addStudentToClass(classUni className) {
	ifstream fin;
	const string name = className.name + ".csv";
	fin.open(name);
	int i = 1;
	while(fin.good()) {
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
		className.listStudent.push_back(st);
	}
	fin.close();
}

void displayStudent(const student& _student) {
	cout <<setw(5)<<left<< _student.no;
	cout <<setw(15)<<left<< _student.ID;
	cout <<setw(20)<<left<< (_student.lastName + ' ' + _student.firstName);
	cout <<setw(20)<<left<< (to_string(_student.DOB.day) + '/' +to_string( _student.DOB.month) + to_string('/' + _student.DOB.year));
	cout << _student.socialID;

	cout << endl;
}

void displayClass(classUni _class) {
	cout << "Class " << _class.name << endl;
	cout << setw(5) << left << "No";
	cout << setw(15) << left << "Student ID";
	cout << setw(20) << left << "Full Name";
	cout << setw(20) << left << "Date of Birth";
	cout << "Social ID"<<endl;

	for (int i = 0; i < _class.listStudent.size(); ++i) {
		displayStudent(_class.listStudent[i]);
	}
}

void viewEnrolledCourses(student _student, semester _semester) {
	for (int i = 0; i < _student.enrolled.size(); ++i) {
		string courseID = _student.enrolled[i].ID;
		for (int j = 0; j < _semester.listCourse.size(); ++j) {
			if (courseID == _semester.listCourse[i].ID) {
				cout << _semester.listCourse[i].name;
				break;
			}
		}
	}
}

bool removeCourseFromList(student& _student, string removeCourseID) {
	for (int i = 0; i <	_student.enrolled.size(); ++i) {
		if (removeCourseID == _student.enrolled[i].ID) {
			_student.enrolled.erase(i);
			return true;
		}
	}
	return false;
}

void viewListOfClass(schoolYear _schoolYear) {
	for (int i = 0; i < _schoolYear.newClass.size(); ++i) {
		cout << _schoolYear.newClass[i].name << endl;
	}
}

void viewCourses(semester sem) {
	cout << left << setw(10) << "ID" << setw(50) << "Name" << setw(30) << "Teacher" << setw(10) << "Credits" << endl;
	for (int i = 0; i < sem.listCourse.size(); i++) {
		cout << left << setw(10) << sem.listCourse[i].ID << setw(50) << sem.listCourse[i].name << setw(30) << sem.listCourse[i].teacher
			 << setw(10) << sem.listCourse[i].numCredits << endl;
	}
}	

void viewStudentsInCourse(course crs) {
	cout << left << setw(5) << "No" << setw(10) << "ID" << setw(20) << "Last name"
		<< setw(15) << "First name" << setw(15) << "Class name" << setw(10) << "Gender"
		<< setw(20) << "Date of birth" << setw(15) << "Social ID" << endl;
	for (int i = 0; i < crs.listStudent.size(); i++) {
		string gnd;
		if (crs.listStudent[i].gender == 0)
			gnd = "male";
		else gnd = "female";
		cout << left << setw(5) << crs.listStudent[i].no << setw(10) << crs.listStudent[i].ID << setw(20) << crs.listStudent[i].lastName
			<< setw(15) << crs.listStudent[i].firstName << setw(15) << crs.listStudent[i].className << setw(10) << gnd
			<< setw(20) << crs.listStudent[i].DOB.day << '/' << crs.listStudent[i].DOB.month << '/' << crs.listStudent[i].DOB.year 
			<< setw(15) << "Social ID" << endl;
	}
}


bool deleteCourseInSemester(semester& _semester, string removeCourseID){
	for(int i=0;i< _semester.listCourse.size();++i){
		if(removeCourseID==_semester.listCourse[i].name){
			_semester.listCourse.erase(i);
			return true;
		}
	}
	return false;
}

void createNewClasses(schoolYear& _schoolYear){
	cout<<"Enter 1 to add a new class or 0 to stop.";
	
	int choice;
	cin>>choice;

	while(choice){
		classUni newClass;
		cout<<"Enter name of the class: ";
		cin>>newClass.name;
		addStudentToClass(newClass);
		_schoolYear.newClass.push_back(newClass);

		cout<<"Enter 1 to add another class or 0 to stop.";
		cin>>choice;
	}
}

void addCourseToSemester(semester& sem){ // chi add info, chua add student
	cout<<"Enter 1 to add a new course or 0 to stop.";
	
	int choice;
	cin>>choice;

	while(choice){
		course crs;
		cout << "Enter ID of the course: ";
		cin >> crs.ID;
		cout << "Enter name of the course: ";
		cin >> crs.name;
		cout << "Enter name of the teacher: ";
		cin >> crs.teacher;
		cout << "Enter the number of credits: ";
		cin >> crs.numCredits;
		for(int i=0; i<2; i++){
			cout << "Enter session " << (i+1) << " of the course:" << endl; 
			lesson sess;
			cout << "	Enter the day for the session: ";
			cin >> sess.day;
			cout << "	At what time ? : ";
			cin >> sess.time;
			crs.listLesson.push_back(sess);
		}
		cout << "Done!" << endl << "Enter 1 to add another course or 0 to stop.";
		cin>>choice;
	}
}

void enrollCourses(student& _student, semester& _semester){
	cout<<"This is the detail of all the courses available in this semester. Choose the course(s) you want to enroll."<<endl;
	viewCourses(_semester);

	cout<<"Please enter 1 to start enroll.";
	int choice;
	cin>>choice;

	while(choice && _student.enrolled.size()<=5){
		module temp;
		string ID;
		bool canEnroll=true;

		cout<<"Enter the ID of the course you want to enroll: ";
		cin>>ID;
		temp.ID=ID;

		if(_student.enrolled.size()==0){
			_student.enrolled.push_back(temp);
		}else{
			for(int i=0;i<_student.enrolled.size();++i){
				for(int j=0;j<=1;++j){
					//0 for first session and 1 for the other session
					lesson time1=getLesson(_semester, ID, j);
					lesson time2=getLesson(_semester, _student.enrolled[i].ID, j);
					if(time1==time2){
						cout<<"This course is conflicted with existing courses. Please choose another course.";
						canEnroll=false;
					}
				}
			}

			if(canEnroll && checkFullSlot(_semester,ID)){
				cout<<"This course if full. Please choose another course";
				canEnroll=false;
			}

			if(canEnroll){
				cout<<"Enroll successfully!";
				_student.enrolled.push_back(temp);
				addStudentToCourse(_student, ID, _semester);
			}
		}

		if(_student.enrolled.size()==5){
			cout<<"You have reach the maximum numbers of courses that you can enroll.";
			break;
		}

		cout<<"If you want to enroll in another course, enter 1. If you want to stop, enter 0.";
		cin>>choice;
	}
}

void createCourseRegistration(semester& sem){
	cout << "Please set the start date and end date for registration:" << endl;
	cout << "	Start date: ";
	cin >> sem.regOpen.day >> sem.regOpen.month >> sem.regOpen.year;
	cout << "	End date: ";
	cin >> sem.regClose.day >> sem.regClose.month >> sem.regClose.year;
}

int login(vector<staff> _staff, vector<student> _student, int& index) {

	int attempID;
	string attempPass;

	gotoxy(45, 3);
	cout << "ID      : "; cin >> attempID;

	gotoxy(45, 4);
	cout << "Password: "; getline(cin, attempPass);

	for (int i = 0; i < _staff.size(); i++)
		if (attempID == _staff[i].ID && attempPass == _staff[i].password) {
			index = i;
			return 1;
		}
	for (int i = 0; i < _staff.size(); i++)
		if (attempID == _student[i].ID && attempPass == _student[i].password) {
			index = i;
			return 2;
		}
	return 0;
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void updateCourseInfo(semester& _semester){
	cout<<"Enter the ID of the course you want to update information: ";
	string courseID;
	getline(cin,courseID);

	course& _course=getCourse(_semester,courseID);

	cout<<"Please choose the section you want to update for the course:"<<endl
		<<"1: Course ID"<<endl
		<<"2: Course Name"<<endl
		<<"3: Teacher Name"<<endl
		<<"4: Number of Credits"<<endl
		<<"5: Sessions"<<endl
		<<"0: Stop update"<<endl
		<<"Enter the number of the part you want to change: ";

	int choice;
	cin>>choice;

	while(choice){
		switch(choice){
			case 1: 
			{
				cout<<"Enter the new ID: ";
				cin.ignore();
				cin>>_course.ID;
				break;
			}
			case 2:	
			{
				cout<<"Enter the new course name: ";
				cin.ignore();
				getline(cin,_course.name);
				break;
			}
			case 3: 
			{
				cout<<"Enter the new teacher name: ";
				cin.ignore();
				getline(cin,_course.teacher);
				break;
			}
			case 4: 
			{
				cout<<"Enter the new number of credits: ";
				cin>>_course.numCredits;
				break;
			}
			case 5: 
			{	
				cout<<"Choose the session you want to update: ";
				cout<<"1: "<<_course.listLesson[0].day<<' '<<_course.listLesson[0].time<<endl;
				cout<<"2: "<<_course.listLesson[1].day<<' '<<_course.listLesson[1].time<<endl;
				int temp;
				cin>>temp;

				cin.ignore();
				cout<<"Enter the new day: ";
				getline(cin,_course.listLesson[temp].day);
				cout<<"Enter the new time: ";
				getline(cin,_course.listLesson[temp].time);
				
				cout<<"Do you want to change the other session?(y/n)";
				char check;
				cin>>check;

				if(tolower(check)=='y'){
					temp^=1;
					cin.ignore();
					cout<<"Enter the new day: ";
					getline(cin,_course.listLesson[temp].day);
					cout<<"Enter the new time: ";
					getline(cin,_course.listLesson[temp].time);
				}
			}
		}
		cout<<"Do you want to update anything else?";
		cin>>choice;
	}
}

void viewStudentScoreboard(student stu){
	cout << left << setw(10) << "ID" << setw(10) << "Midterm" << setw(10) << "Final" << setw(10) << "Other" << setw(10) << "Total";
	for(int i=0; i<stu.enrolled.size(); i++){
		cout << left << setw(10) << stu.enrolled[i].ID << setw(10) << stu.enrolled[i].grade.midterm 
		<< setw(10) << stu.enrolled[i].grade.final << setw(10) << stu.enrolled[i].grade.other << setw(10) << stu.enrolled[i].grade.total;
	}
}

void viewCourseScoreboard(course crs){
	cout << left << setw(30) << "Name" << setw(10) << "Midterm" << setw(10) << "Final" << setw(10) << "Other" << setw(10) << "Total";
	for(int i=0; i < crs.listStudent.size(); i++){
		for(int j=0; i < crs.listStudent[i].enrolled.size(); j++){
			if(crs.listStudent[i].enrolled[j].ID == crs.ID){
				cout << left << setw(30) << crs.listStudent[i].lastName << ' ' << crs.listStudent[i].firstName 
				<< setw(10) << crs.listStudent[i].enrolled[j].grade.midterm
				<< setw(10) << crs.listStudent[i].enrolled[j].grade.final
				<< setw(10) << crs.listStudent[i].enrolled[j].grade.other
				<< setw(10) << crs.listStudent[i].enrolled[j].grade.total;
			}
		}
	}
}

int changePassword_Staff(vector<staff>& _staff, int index) {
	string oPass, nPass, cNPass;
	gotoxy(5, 5); // Change later
	cout << "Old password: "; getline(cin, oPass);
	if (oPass != _staff[index].password) return 1;
	gotoxy(5, 6); // Change later
	cout << "New password: "; getline(cin, nPass);
	gotoxy(5, 7); // Change later
	cout << "Confirm new password: "; getline(cin, cNPass);
	if (nPass != cNPass) return 2;
	_staff[index].password = cNPass;
	return 0;
}

int changePassword_Student(vector<student>& _student, int index) {
	string oPass, nPass, cNPass;
	gotoxy(5, 5); // Change later
	cout << "Old password: "; getline(cin, oPass);
	if (oPass != _student[index].password) return 1;
	gotoxy(5, 6); // Change later
	cout << "New password: "; getline(cin, nPass);
	gotoxy(5, 7); // Change later
	cout << "Confirm new password: "; getline(cin, cNPass);
	if (nPass != cNPass) return 2;
	_student[index].password = cNPass;
	return 0;
}

course& getCourse(semester& _semester,string courseID){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(courseID == _semester.listCourse[i].ID){
			return _semester.listCourse[i];
		}
	}
}

lesson getLesson(semester& _semester,string ID,int index){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(ID == _semester.listCourse[i].ID){
			return _semester.listCourse[i].listLesson[index];
		}
	}
}

bool checkFullSlot(semester _semester,string ID){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(ID == _semester.listCourse[i].ID){
			return _semester.listCourse[i].listStudent.size() >= _semester.listCourse[i].maxStudents;
		}
	}
}

void addStudentToCourse(student _student, string _courseID, semester& _semester){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(_courseID == _semester.listCourse[i].ID){
			_semester.listCourse[i].listStudent.push_back(_student);
			return;
		}
	}
}