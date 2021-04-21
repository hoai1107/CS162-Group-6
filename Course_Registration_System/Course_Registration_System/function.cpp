#include "Function.h"
#include<fstream>
#include<ctime>
#include<filesystem>

namespace fs= std::filesystem;
using namespace std;

const fs::path root= fs::current_path()/"2020-2021";

void addStudentToClass(classUni className) {
	ifstream fin;

	fs::path classPath =root/"Class"/className.name/(className.name + ".csv");
	if(!fs::exists(classPath)) {
		cout<<className.name + ".csv"<<" does not exist!";
		return;
	}


	fin.open(classPath);
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
	cout << setw(10) << left << _student.no;
	cout << setw(20) << left << _student.ID;
	cout << setw(40) << left << (_student.lastName + ' ' + _student.firstName);
	cout << setw(20) << left << (to_string(_student.DOB.day) + '/' +to_string( _student.DOB.month) + to_string('/' + _student.DOB.year));
	cout << setw(20) << left << _student.socialID;
	cout << endl;
}

void displayClass(classUni _class) {
	cout << "Class " << _class.name << endl;
	cout << setw(10) << left << "No";
	cout << setw(20) << left << "Student ID";
	cout << setw(40) << left << "Full Name";
	cout << setw(20) << left << "Date of Birth";
	cout << setw(20) << left << "Social ID" << endl;

	for (int i = 0; i < _class.listStudent.size(); ++i) {
		displayStudent(_class.listStudent[i]);
	}
}

void viewEnrolledCourses(student _student, semester _semester) {
    Vector <course> res;
    for (int i = 0; i < _student.enrolled.size(); ++i) {
        for (int j = 0; j < _semester.listCourse.size(); ++j)
            if (_student.enrolled[i].ID == _semester.listCourse[j].ID) {
                res.push_back(_semester.listCourse[i]);
                break;
            }
    }
    viewCourses(res);
}

void removeCourseFromList(student& _student, semester _semester) {
    if (_student.enrolled.size() == 0) {
        cout << "You haven't enrolled in a subject yet." << endl;
        system("pause");
        return;
    }
    while (true) {
        cout << "Choose the course you want to remove. BACKSPACE to stop." << endl;
        viewEnrolledCourses(_student, _semester);

        Vector<string> actions;
        for (int i = 0; i < _student.enrolled.size(); i++) actions.push_back("Remove.");
        int t = actionList(actions, { 105, 2 });

        if (t == actions.size()) return;

        _student.enrolled.erase(t);

        system("CLS");

        cout << "Remove successfully!" << endl;
        system("pause");
        if (_student.enrolled.size() == 0) {
            system("CLS");
            cout << "You have removed all courses." << endl;
            system("pause");
            return;
        }

        system("CLS");
    }
}

void viewListOfClass(schoolYear _schoolYear) {
	for (int i = 0; i < _schoolYear.newClass.size(); ++i) {
		cout << _schoolYear.newClass[i].name << endl;
	}
}

void viewCourses(Vector<course> courseList) {
    cout << left << setw(10) << "ID"
        << setw(40) << "Name"
        << setw(40) << "Teacher"
        << setw(10) << "Credits" << endl;
    for (int i = 0; i < courseList.size(); i++) {
        cout << left << setw(10) << courseList[i].ID
            << setw(40) << courseList[i].name
            << setw(40) << courseList[i].teacher
            << setw(10) << courseList[i].numCredits << endl;
    }
}	

void viewStudentsInCourse(course crs) {
	cout << left << setw(5) << "No" 
				 << setw(10) << "ID" 
				 << setw(20) << "Last name"
				 << setw(15) << "First name" 
				 << setw(15) << "Class name" 
				 << setw(10) << "Gender"
				 << setw(20) << "Date of birth" 
				 << setw(15) << "Social ID" << endl;
	for (int i = 0; i < crs.listStudent.size(); i++) {
		string gnd;
		if (crs.listStudent[i].gender == 0)
			gnd = "male";
		else gnd = "female";
		cout << left << setw(5) << crs.listStudent[i].no 
			 	 	 << setw(10) << crs.listStudent[i].ID 
					 << setw(20) << crs.listStudent[i].lastName
					 << setw(15) << crs.listStudent[i].firstName 
					 << setw(15) << crs.listStudent[i].className 
					 << setw(10) << gnd
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

void enrollCourses(student& _student, semester _semester) {
    while (_student.enrolled.size() <= 5) {
        Vector<course> unenrolledCourse = getUnenrolledCourseList(_semester, _student);
        cout << "Choose the course you want to enroll. BACKSPACE to stop." << endl;
        viewCourses(unenrolledCourse);

        Vector<string> actions;
        for (int i = 0; i < unenrolledCourse.size(); i++) actions.push_back("Enroll.");
        int t = actionList(actions, { 105, 2 });

        if (t == actions.size()) return;

        module temp;
        temp.ID = unenrolledCourse[t].ID;
        bool canEnroll = true;

        if (_student.enrolled.size() == 0) _student.enrolled.push_back(temp);

        for (int i = 0; i < _student.enrolled.size(); ++i) {
            for (int j = 0; j <= 1; ++j) {
                //0 for first session and 1 for the other session
                lesson time1 = getLesson(_semester, temp.ID, j);
                lesson time2 = getLesson(_semester, _student.enrolled[i].ID, j);
                if (time1 == time2) {
					system("CLS");
                    cout << "This course is conflicted with existing courses. Please choose another course." << endl;
                    canEnroll = false;
					system("pause");
                }
			}
        }

        if (canEnroll && checkFullSlot(_semester, temp.ID)) {
			system("CLS");
            cout << "This course if full. Please choose another course" << endl;
			system("pause");
            canEnroll = false;
        }

        if (canEnroll) {
			system("CLS");
            cout << "Enroll successfully!" << endl;
			system("pause");
            _student.enrolled.push_back(temp);
            addStudentToCourse(_student, temp.ID, _semester);
        }

        if (_student.enrolled.size() == 5) {
			system("CLS");
            cout << "You have reach the maximum numbers of courses that you can enroll." << endl;
            system("pause");
			break;
        }

		system("CLS");
    }
}

void createCourseRegistration(semester& sem){
	cout << "Please set the start date and end date for registration:" << endl;
	cout << "	Start date: ";
	cin >> sem.regOpen.day >> sem.regOpen.month >> sem.regOpen.year;
	cout << "	End date: ";
	cin >> sem.regClose.day >> sem.regClose.month >> sem.regClose.year;
}

int login(Vector<staff> _staff, Vector<student> _student, int& index) {

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
	cout << left << setw(10) << "ID" 
				 << setw(10) << "Midterm" 
				 << setw(10) << "Final" 
				 << setw(10) << "Other" 
				 << setw(10) << "Total" << endl;
	for(int i=0; i<stu.enrolled.size(); i++){
		cout << left << setw(10) << stu.enrolled[i].ID 
					 << setw(10) << stu.enrolled[i].grade.midterm 
		 			 << setw(10) << stu.enrolled[i].grade.final 
					 << setw(10) << stu.enrolled[i].grade.other 
					 << setw(10) << stu.enrolled[i].grade.total << endl;
	}
}

void viewCourseScoreboard(course crs){                              
	cout << left << setw(40) << "Name" 
				 << setw(15) << "Midterm" 
				 << setw(15) << "Final" 
				 << setw(15) << "Other" 
				 << setw(15) << "Total" << endl;
	for(int i=0; i < crs.listStudent.size(); i++){
		for(int j=0; i < crs.listStudent[i].enrolled.size(); j++){
			if(crs.listStudent[i].enrolled[j].ID == crs.ID){
				cout << left << setw(40) << crs.listStudent[i].lastName << ' ' << crs.listStudent[i].firstName 
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.midterm
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.final
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.other
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.total << endl;
			}
		}
	}
}

void updateStudentResult(student& stu) {
	cout << "Please enter the ID of the course you want to update result: ";
	int id;
	cin >> id;
	for (int i = 0; i < stu.enrolled.size(); i++) {
		if (stu.enrolled[i].ID == to_string(id)) {
			cout << "Enter the mark you want to update (1:midterm 2:final 3:other) : ";
			int choice;
			cin >> choice;
			cout << "Enter the new score: ";
			int score;
			cin >> score;
			switch (choice) {
			case 1:
				stu.enrolled[i].grade.midterm = score;
				return;
			case 2:
				stu.enrolled[i].grade.final = score;
				return;
			case 3:
				stu.enrolled[i].grade.other = score;
				return;
			}
		}
	}
	cout << "The course you entered doesn't exist";
	return;
}

int changePassword_Staff(Vector<staff>& _staff, int index) {
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

int changePassword_Student(Vector<student>& _student, int index) {
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

void textColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

ACTION key(int z) {
	if (z == 224) {
		char c;
		c = _getch();
		if (c == 72) return UP;
		if (c == 80) return DOWN;
		if (c == 77) return RIGHT;
		if (c == 75) return LEFT;
	}
	else if (z == 13) return ENTER;
	else if (z == 8) return BACK;
	return RIGHT;
}

int actionList(Vector<string> str, COORD position) {
	int n = str.size();
    int status = 0;
    int* color = new int[n];
    for (int i = 0; i < n; i++) color[i] = 15;
    color[status] = 176;
    for (int i = 0; i < n; i++) {
        gotoxy(position.X, position.Y + i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color[i]);
        cout << i + 1 << ". " << str[i];
    }
    while (true) {
        // if (status == n - 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        int z = _getch(); int action = key(z);
        switch (action) {
        case UP: {
            gotoxy(position.X, position.Y + status);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout << status + 1 << ". " << str[status];
            if (!status) status = n - 1;
            else status--;
            break;
        }
        case DOWN: {
            gotoxy(position.X, position.Y + status);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout << status + 1 << ". " << str[status];
            if (status == n - 1) status = 0;
            else status++;
            break;
        }
        case ENTER: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            system("CLS");
            return status;
        }
        case BACK: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            return str.size();
        }
        };
        gotoxy(position.X, position.Y + status);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176);
        cout << status + 1 << ". " << str[status];
    }
	delete[] color;
}

void exportStudentInCourseToCSV(course& _course) {
	ofstream fout;
	const string name = _course.name + "_list" + ".csv";
	fout.open(name);

	if (fout.is_open()) {
		for (int i = 0; i < _course.listStudent.size(); ++i) {
			student _student = _course.listStudent[i];
			fout << i + 1 << ','
				<< _student.ID<< ','
				<< _student.firstName << ','
				<< _student.lastName << ','
				<< _student.className << ','
				<< _student.gender << ','
				<< _student.DOB.day << '/' << _student.DOB.month << '/' << _student.DOB.year << ','
				<< _student.socialID;

			fout<<endl;	
		}
	}
	else {
		cout << "Something wrong with the file!!";
	}
	fout.close();
}

void importScoreboard(course& _course, schoolYear& _schoolYear) {
	ifstream fin;

	fs::path coursePath=root/"Semester"/_course.ID/"scoreboard.csv";
	
	if(!fs::exists(coursePath)){
		cout<<"Can't import the scoreboard!!!";
		return;
	}

	fin.open(coursePath);

	if(fin.is_open()){
		//Assume that the order of student in the file is the same as the file when it is exported
		for (int i = 0; i < _course.listStudent.size(); ++i) {

			//Just to store the old info to get to the mark
			student temp;
			fin >> temp.no;
			fin.ignore(1, ',');
			getline(fin, temp.firstName, ',');
			getline(fin, temp.lastName, ',');
			getline(fin, temp.className, ',');
			fin >> temp.gender;
			fin.ignore(1,',');
			fin >> temp.DOB.day;
			fin.ignore(1, '/');
			fin >> temp.DOB.month;
			fin.ignore(1, '/');
			fin >> temp.DOB.year;
			fin.ignore(1, '/');
			fin >> temp.socialID;
			fin.ignore(1,',');


			//Get the score
			module _module;
			_module.ID = _course.ID;
			fin >> _module.grade.final;
			fin.ignore(1, ',');
			fin >> _module.grade.midterm;
			fin.ignore(1, ',');
			fin >> _module.grade.other;
			fin.ignore(1, ',');
			fin >> _module.grade.total;
			fin.ignore();

			//Store to the course
			_course.listStudent[i].enrolled.push_back(_module);

			//Store to the class
			for (int i = 0; i < _schoolYear.newClass.size(); ++i) {
				if (_schoolYear.newClass[i].name == temp.className) {
					for (int j = 0; j < _schoolYear.newClass[i].listStudent.size(); ++i) {
						if (_schoolYear.newClass[i].listStudent[j].ID == temp.ID) {
							_schoolYear.newClass[i].listStudent[j].enrolled.push_back(_module);
							goto nextStudent;
						}
					}
				}
			}

		nextStudent: continue;
		}
	}else{
		cout<<"Something wrong!!";
	}
}

void viewClassScoreboard(schoolYear& _schoolYear, string className) {
	classUni _class;
	for (int i = 0; i < _schoolYear.newClass.size(); ++i) {
		if (_schoolYear.newClass[i].name == className) {
			_class = _schoolYear.newClass[i];
			break;
		}
	}

	cout << setw(5) << left << "No.";
	cout << setw(15) << left << "Student ID";
	cout << setw(20) << left << "Full Name";

	for (int i = 0; i < _class.listStudent.size(); ++i) {
		student _student = _class.listStudent[i];

		float sum = 0;
		cout << setw(5) << left << i + 1;
		cout << setw(15) << left << _student.ID;
		cout << setw(20) << left << (_student.lastName + ' ' + _student.firstName);

		for (int j = 0; j < _student.enrolled.size(); ++j) {
			cout << setw(10) << left << _student.enrolled[j].grade.total << '(' << _student.enrolled[j].ID << ')';
			sum += _student.enrolled[j].grade.total;
		}

		cout << "Avg GPA: " << fixed << setprecision(2);
		cout << sum / _student.enrolled.size() << endl;
	}
}

bool CheckExistence(string filename) {
    struct stat fileinfo;
    return !stat(filename.c_str(), &fileinfo);
}

void createNewYear() {
	cout << "New Academic Year\n";
	cout << "Start Year: ";
	string startYear; cin >> startYear;
	cout << "End Year: ";
	string endYear; cin >> endYear;
	string academicYear = startYear + ' ' + endYear;
	if (CheckExistence(academicYear.c_str()))
		cout << "Year " << academicYear << " has been created before!\n";
	else
		cout << "Year " << academicYear << " has been created.\n";
}

bool createScoreboardFile(const course& _course){
	if(fs::exists(root/"Semester"/_course.ID/"scoreboard.csv")){
		return true;
	}
	if(!fs::exists(root/"Semester"/_course.ID/(_course.ID+"_list.csv"))){
		return false;
	}
	fs::copy_file(root/"Semester"/_course.ID/(_course.ID+"_list.csv"),root/"Semester"/_course.ID/"scoreboard.csv");
	return true;
}

Vector<course> getUnenrolledCourseList(semester _semester, student _student) {
    Vector<course> res;
    bool check = true;
    for (int i = 0; i < _semester.listCourse.size(); i++) {
        check = true;
        for (int j = 0; j < _student.enrolled.size(); j++)
            if (_student.enrolled[j].ID == _semester.listCourse[i].ID) {
                check = false;
                break;
            }
        if (check) res.push_back(_semester.listCourse[i]);
    }
    return res;
}

void loadSemesterInfo(semester& _semester) {
	ifstream fin;
	fin.open(root / "Semester" / "semester.txt");

	if (fin.is_open()) {
		getline(fin, _semester.name);
		fin >> _semester.academicYear;
		

		fin >> _semester.regOpen.day;
		fin.ignore(1,'/');
		fin >> _semester.regOpen.month;
		fin.ignore(1,'/');
		fin >> _semester.regOpen.year;
		fin.ignore();

		fin >> _semester.regClose.day;
		fin.ignore(1,'/');
		fin >> _semester.regClose.month;
		fin.ignore(1,'/');
		fin >> _semester.regClose.year;
		fin.ignore();

		fin >> _semester.startDate.day;
		fin.ignore(1,'/');
		fin >> _semester.startDate.month;
		fin.ignore(1,'/');
		fin >> _semester.startDate.year;
		fin.ignore();

		fin >> _semester.endDate.day;
		fin.ignore(1,'/');
		fin >> _semester.endDate.month;
		fin.ignore(1,'/');
		fin >> _semester.endDate.year;
		fin.ignore();

		while (!fin.eof()) {
			course _course;
			getline(fin, _course.ID);
			loadCourseInfo(_course);
			_semester.listCourse.push_back(_course);
		}

	}
}

void loadCourseInfo(course& _course) {
	ifstream fin;
	fin.open(root / "Semester" / _course.ID / ("course_info.txt"));

	if (fin.is_open()) {
		getline(fin, _course.name);
		getline(fin, _course.teacher);
		fin >> _course.numCredits;

		lesson _lesson;
		getline(fin, _lesson.day, ',');
		getline(fin, _lesson.time);
		_course.listLesson.push_back(_lesson);

		getline(fin, _lesson.day, ',');
		getline(fin, _lesson.time);
		_course.listLesson.push_back(_lesson);
	}
}

void saveCourseInfo(course& _course) {
	ofstream fout;

	if (!fs::exists(root / "Semester" / _course.ID / ("course_info.txt"))) {
		fs::create_directory(root / "Semester" / _course.ID / ("course_info.txt"));
	}

	fout.open(root / "Semester" / _course.ID / ("course_info.txt"));

	if (fout.is_open()) {
		fout << _course.name << endl;
		fout << _course.teacher << endl;
		fout << _course.numCredits;

		for (int i = 0; i < 2; ++i) {
			fout << endl << _course.listLesson[i].day << ',' << _course.listLesson[i].time;
		}
	}
}

void saveSemesterInfo(semester& _semester) {
	ofstream fout;

	if (!fs::exists(root / "Semester" / "semester.txt")) {
		fs::create_directory(root / "Semester" / "semester.txt");
	}

	fout.open(root / "Semester" / "semester.txt");
	
	if (fout.is_open()) {
		fout << _semester.name << endl;
		fout << _semester.academicYear << endl;

		fout << _semester.regOpen.day << '/' << _semester.regOpen.month << '/' << _semester.regOpen.year << endl;
		fout << _semester.regClose.day << '/' << _semester.regClose.month << '/' << _semester.regClose.year << endl;
		fout << _semester.startDate.day << '/' << _semester.startDate.month << '/' << _semester.startDate.year << endl;
		fout << _semester.endDate.day << '/' << _semester.endDate.month << '/' << _semester.endDate.year;

		for (int i = 0; i < _semester.listCourse.size(); ++i) {
			fout << endl << _semester.listCourse[i].ID;
			saveCourseInfo(_semester.listCourse[i]);
		}
	}
}