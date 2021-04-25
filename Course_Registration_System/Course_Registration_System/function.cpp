#include "Function.h"
#include<fstream>
#include<ctime>
#include<filesystem>
#include<sys/stat.h>

using namespace std;
namespace fs= std::filesystem;

fs::path root= fs::current_path()/"data";
course fakeCourse;

void addStudentToClass(classUni& className) {
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
		fin.ignore(1, ',');

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

void viewCourses(Vector<course> &courseList) {
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
	system("pause");
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
	system("pause");
}

bool deleteCourseInSemester(semester& _semester){
	course& removeCourse = getCourse(_semester);
	for(int i=0;i< _semester.listCourse.size();++i){
		if(removeCourse.ID==_semester.listCourse[i].ID){
			_semester.listCourse.erase(i);
			if (fs::exists(root / "Semester"/ _semester.name / removeCourse.ID)) {
				fs::remove_all(root / "Semester"/ _semester.name / removeCourse.ID);
			}
			saveSemester(_semester);
			cout << "Course is removed successfully.\n";
			system("pause");
			return true;
		}
	}
	return false;
}

void createNewClasses(schoolYear& _schoolYear){
	system ("cls");
	classUni nClass;
	cout << "Enter name of the class: ";
	getline(cin,nClass.name);
	cout << "Enter or drag path of your .csv file: ";
	string link; getline(cin, link);
	fs::path path = root/"Class"/nClass.name;
	system(("mkdir " + path.string()).c_str());
	system(("cd " + path.string() + "&& type nul > " + nClass.name + ".csv").c_str());
	path = path/(nClass.name + ".csv");
	system(("copy " + link + " " + path.string() + " > nul").c_str());
	addStudentToClass(nClass);
	_schoolYear.newClass.push_back(nClass);
	cout << "Class has been added successfully.\n";
	system("pause");
}

void addCourseToSemester(semester& sem){ // chi add info, chua add student
	course crs;
	cout << "Course ID: ";
	cin >> crs.ID; cin.get();
	cout << "Course name: ";
	getline(cin, crs.name);
	cout << "Teacher name: ";
	getline(cin, crs.teacher);
	cout << "Number of credits: ";
	cin >> crs.numCredits;
	for(int i=0; i<2; i++){
		cout << "Session " << (i+1) << " of the course:" << endl; 
		lesson sess;
		cout << "	Day for the session (MON / TUE / WED / THU / FRI / SAT): ";
		cin >> sess.day;
		cout << sess.day << '\n';
		cout << "	Time for the session (7:30 / 9:30 / 13:30 / 15:30): ";
		cin >> sess.time;
		cout << sess.time << '\n';
		crs.listLesson.push_back(sess);
	}
	cout << crs.listLesson.size() << '\n';
	sem.listCourse.push_back(crs);
	saveSemester(sem);
	// saveCourseInfo(sem, crs);
	cout << "Course is added successfully!\n";
	system("pause");
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

void chooseSemester(schoolYear& _schoolYear) {
	system("cls");
	if (_schoolYear.listSemester.size() == 0) {
		cout << "None semester in this year!\n";
		return;
	}
	Vector <string> Sem;
	// cout << "Oh no\n";
	for (int i = 0; i < _schoolYear.listSemester.size(); i++) {
		Sem.push_back(_schoolYear.listSemester[i].name);
		// cout << Sem[Sem.size()-1]  << '\n';
	}
	// exit (0);

	while (true) {
		system("cls");
		cout << "Choose the semester. BACKSPACE to stop\n";
		int t = actionList(Sem, {0, 1});

		if (t == Sem.size()) return;

		editSemester(_schoolYear.listSemester[t]);
	}
}

void editSemester(semester& _semester) {
	system("cls");
	Vector <string> listAction;
	listAction.push_back("Create course registration session");
	listAction.push_back("Add a course");
	listAction.push_back("View list of courses");
	listAction.push_back("Update a course infomation");
	listAction.push_back("Delete a course");
	listAction.push_back("View course scoreboard");
	while(true) {
		system("cls");
		cout << "Choose your function. BACKSPACE to stop\n";
		int t = actionList(listAction, {0, 1});

		if (t == listAction.size()) return;

		switch (t) {
			case 0:
				createCourseRegistration(_semester);
				break;

			case 1:
				addCourseToSemester(_semester);
				break;

			case 2:
				viewCourses(_semester.listCourse);
				break;
			
			case 3:
				updateCourseInfo(_semester);
				break;

			case 4:
				deleteCourseInSemester(_semester);
				break;

			case 5:
				viewCourseScoreboard(_semester);
				break;

			default:
				break;
		}
	}
}

void createCourseRegistration(semester& sem) {
	system("cls");
	cout << "Please set the start date and end date for registration:" << endl;
	cout << "Enter start date (DD/MM/YYYY): ";
	cin >> sem.regOpen.day; cin.get();
	cin >> sem.regOpen.month; cin.get();
	cin >> sem.regOpen.year;
	cout << "Enter end date (DD/MM/YYYY): ";
	cin >> sem.regClose.day; cin.get();
	cin >> sem.regClose.month; cin.get();
	cin >> sem.regClose.year;

	if (!(sem.regOpen <= sem.regClose)) {
		cout << "Error: Start date must be before End date!\n";
		system("pause");
	}
	else {
		saveSemester(sem);
		cout << "Registration session is created successfully.\n";
		system("pause");
		return;
	}
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
	COORD c = { SHORT(x), SHORT(y) };
	SetConsoleCursorPosition(h, c);
}

/// STILL BUGGGGGGGGGGGGGGGGGGGGGGGGGGG
void updateCourseInfo(semester& _semester){
	course& _course=getCourse(_semester);
	if (_course.name == "-1") return;

	Vector <string> update;
	update.push_back("Course ID");
	update.push_back("Course Name");
	update.push_back("Teacher Name");
	update.push_back("Number of Credits");
	update.push_back("Sessions");

	fs::path link = root / "Semester" / _semester.name;
	// cout << link << '\n';
	// exit (0);
	cin.ignore();
	while(true){
		system ("cls");
		cout<<"Please choose the section you want to update for the course. BACKSPACE to stop"<<endl;
		int t = actionList(update, {0, 1});
		if (t == update.size()) break;
		switch(t){
			case 0: 
			{
				cout<<"Enter the new ID: ";
				// cin.ignore();
				// cout << int(cin.get()) << '\n';
				string tmp = _course.ID;
				getline(cin, _course.ID);
				fs::rename(link / tmp, link / _course.ID);
				cout << "Updated successfully!\n";
				system("pause");
				// system ("pause");
				break;
			}
			case 1:	
			{
				cout<<"Enter the new course name: ";
				// cin.ignore();
				getline(cin,_course.name);
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 2: 
			{
				cout<<"Enter the new teacher name: ";
				// cin.ignore();
				getline(cin,_course.teacher);
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 3: 
			{
				cout<<"Enter the new number of credits: ";
				cin>>_course.numCredits; cin.ignore();
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 4: 
			{	
				cout<<"Choose the session you want to update:\n";
				cout<<"1: "<<_course.listLesson[0].day<<' '<<_course.listLesson[0].time<<endl;
				cout<<"2: "<<_course.listLesson[1].day<<' '<<_course.listLesson[1].time<<endl;
				int temp;
				cin>>temp;

				cin.ignore();
				cout<<"Enter the new day: ";
				getline(cin,_course.listLesson[temp-1].day);
				cout<<"Enter the new time: ";
				getline(cin,_course.listLesson[temp-1].time);
				
				cout<<"Do you want to change the other session?(y/n)";
				char check;
				cin>>check; cin.ignore();

				if(tolower(check)=='y'){
					temp = 3 - temp;
					// cin.ignore();
					cout<<"Enter the new day: ";
					getline(cin,_course.listLesson[temp-1].day);
					cout<<"Enter the new time: ";
					getline(cin,_course.listLesson[temp-1].time);
				}
				cout << "Updated successfully!\n";
				system("pause");
			}
		}
	}
	saveSemester(_semester);
	saveCourseInfo(_semester, _course);
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

void viewCourseScoreboard(semester& _semester){     
	course crs = getCourse(_semester);
	if (crs.name == "-1") return;
	system("cls");
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
	system("pause");
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

course& getCourse(semester& _semester){
	Vector <string> lCourse;
	for(int i=0;i<_semester.listCourse.size();++i){
		lCourse.push_back(_semester.listCourse[i].name);
	}
	system("cls");
	if (lCourse.size() == 0) {
		cout << "None course in this semester!\n";
		system("pause");
		return fakeCourse;
	}
	while (true) {
		system("cls");
		cout << "Choose your course. BACKSPACE to stop\n";
		int t = actionList(lCourse, {0, 1});
		if (t == lCourse.size()) return fakeCourse;
		return _semester.listCourse[t];
	}
}

lesson getLesson(semester& _semester,string ID,int index){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(ID == _semester.listCourse[i].ID){
			return _semester.listCourse[i].listLesson[index];
		}
	}
	cout << "Something Wrong Here!!! -> getLesson";
	return _semester.listCourse[0].listLesson[0];
}

bool checkFullSlot(semester _semester,string ID){
	for(int i=0;i<_semester.listCourse.size();++i){
		if(ID == _semester.listCourse[i].ID){
			return _semester.listCourse[i].listStudent.size() >= _semester.listCourse[i].maxStudents;
		}
	}
	return false;
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

void loadSemesterInfo(Vector<semester>& _semester) {
	
	ifstream fin;
	
	for (int i = 0; i < 3; i++) {
		
		string folderName;
		semester sTemp;

		if (i == 0) folderName = "Fall";
		if (i == 1) folderName = "Summer";
		if (i == 2) folderName = "Autumn";

		fin.open(root / "Semester"/ folderName / "semester.txt");

		if (fin.is_open()) {
			getline(fin, sTemp.name);
			fin >> sTemp.academicYear;
			

			fin >> sTemp.startDate.day;
			fin.ignore(1,'/');
			fin >> sTemp.startDate.month;
			fin.ignore(1,'/');
			fin >> sTemp.startDate.year;
			fin.ignore();

			fin >> sTemp.endDate.day;
			fin.ignore(1,'/');
			fin >> sTemp.endDate.month;
			fin.ignore(1,'/');
			fin >> sTemp.endDate.year;
			fin.ignore();

			fin >> sTemp.regOpen.day;
			fin.ignore(1,'/');
			fin >> sTemp.regOpen.month;
			fin.ignore(1,'/');
			fin >> sTemp.regOpen.year;
			fin.ignore();

			fin >> sTemp.regClose.day;
			fin.ignore(1,'/');
			fin >> sTemp.regClose.month;
			fin.ignore(1,'/');
			fin >> sTemp.regClose.year;
			fin.ignore();

			course _course;
			while (getline(fin, _course.ID)) {
				if (_course.ID.size() == 0) continue;
				loadCourseInfo(sTemp ,_course);
				sTemp.listCourse.push_back(_course);
			}

			fin.close();
			_semester.push_back(sTemp);
		}
	}
}

void loadCourseInfo(semester _semester, course& _course) {
	ifstream fin;

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";

	fin.open(root / "Semester" / folderName / _course.ID / ("course_info.txt"));

	if (fin.is_open()) {
		getline(fin, _course.name);
		getline(fin, _course.teacher);
		fin >> _course.numCredits; fin.ignore();

		lesson _lesson;
		getline(fin, _lesson.day, ',');
		getline(fin, _lesson.time);
		_course.listLesson.push_back(_lesson);

		getline(fin, _lesson.day, ',');
		getline(fin, _lesson.time);
		_course.listLesson.push_back(_lesson);
	}

	fin.close();
}

void saveCourseInfo(semester _semester, course& _course) {
	ofstream fout;
	
	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";

	if (!fs::exists(root / "Semester"/ folderName / _course.ID)) {
		fs::create_directory(root / "Semester"/ folderName / _course.ID);
	}

	fout.open(root / "Semester"/ folderName / _course.ID / ("course_info.txt"));

	if (fout.is_open()) {
		fout << _course.name << endl;
		fout << _course.teacher << endl;
		fout << _course.numCredits << endl;

		for (int i = 0; i < 2; ++i) {
			fout << _course.listLesson[i].day << ',' << _course.listLesson[i].time << '\n';
		}
	}

	fout.close();
}

void saveSemesterInfo(Vector<semester>& _semester) {
	ofstream fout;
	
	for (int i = 0; i < 3; i++) {
		
		string folderName;
		if (_semester[i].name == "1" || _semester[i].name == "fall" || _semester[i].name == "Fall") folderName = "Fall";
		if (_semester[i].name == "2" || _semester[i].name == "summer" || _semester[i].name == "Summer") folderName = "Summer";
		if (_semester[i].name == "3" || _semester[i].name == "autumn" || _semester[i].name == "Autumn") folderName = "Autumn";

		if (!fs::exists(root / "Semester" / folderName / "semester.txt")) {
			fs::create_directory(root / "Semester" / folderName / "semester.txt");
		}

		fout.open(root / "Semester" / _semester[i].name / "semester.txt");
		
		// if (fout.is_open()) {
			fout << _semester[i].name << endl;
			fout << _semester[i].academicYear << endl;

			fout << _semester[i].startDate.day << '/' << _semester[i].startDate.month << '/' << _semester[i].startDate.year << endl;
			fout << _semester[i].endDate.day << '/' << _semester[i].endDate.month << '/' << _semester[i].endDate.year;
			fout << _semester[i].regOpen.day << '/' << _semester[i].regOpen.month << '/' << _semester[i].regOpen.year << endl;
			fout << _semester[i].regClose.day << '/' << _semester[i].regClose.month << '/' << _semester[i].regClose.year << endl;

			for (int i = 0; i < _semester[i].listCourse.size(); ++i) {
				fout << endl << _semester[i].listCourse[i].ID;
				saveCourseInfo(_semester[i] ,_semester[i].listCourse[i]);
			}
		// }
		
		fout.close();
	}
}

bool IsPathExist(const string &s) {
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void addClassToYear(schoolYear &year) {
	fs::path link = root/year.name;

	// CLASS
	if (!fs::exists(link/"Class"));
	else {
		for (const auto& entry : fs::directory_iterator(link/"Class")) {
			string folder = entry.path().filename().string();
			if (entry.is_directory()) {
				classUni newClass;
				newClass.name = folder;
				addStudentToClass(newClass);
				year.newClass.push_back(newClass);
			}
		}
	}
}

void addSemesterToYear(schoolYear &year) {
	if (!fs::exists(root/"Semester"));
	else {
		Vector <semester> _semester;
		loadSemesterInfo(_semester);
		year.listSemester = _semester;
	}
}

//https://www.bfilipek.com/2019/04/dir-iterate.html
void loadLastSave(Vector <schoolYear> &listYear) {
	for (const auto& entry : fs::directory_iterator(root)) {
		string folder = entry.path().filename().string();
		if (entry.is_directory()) {
			if (folder.size() != 9) continue;
			bool wrong = false;
			for (int i = 0; i < 9; i++) {
				if (i == 4) {
					if (folder[i] != '-') wrong = true;
				}
				else if (!isdigit(folder[i])) wrong = true;
			}
			if (wrong) continue;

			fs::path tmpRoot = root;
			root = root/folder;

			schoolYear tmp;
			tmp.name = folder;
			addClassToYear(tmp);
			addSemesterToYear(tmp);
			listYear.push_back(tmp);

			root = tmpRoot;
		}
	}
}

void createNewYear(Vector<schoolYear> &allYear) {
	system("cls");
	cout << "------New Academic Year------\n\n";
	cout << "Start Year: ";
	string startYear; cin >> startYear;
	cout << "End Year: ";
	string endYear; cin >> endYear;
	string academicYear = startYear + '-' + endYear;
	if (IsPathExist(academicYear))
		cout << "Year " << academicYear << " has been created before!\n";
	else {
        system(("mkdir " + academicYear).c_str());
		schoolYear tmp;
		tmp.name = academicYear;
		allYear.push_back(tmp);
		cout << "Year " << academicYear << " has been created.\n";
	}
    system("pause");
}

void editSchoolYear(schoolYear &year) {
	Vector <string> listOperation;
	listOperation.push_back("Add new classes");
	listOperation.push_back("Create new semester");
	listOperation.push_back("Edit semester");

	while (true) {
		system ("cls");
		cout << "-----" << year.name << "-----\n\n";
		cout << "Choose the function you want to perform. BACKSPACE to stop\n";

		int t = actionList(listOperation, {0, 4});

		if (t == listOperation.size()) return;

		switch (t) {
		case 0:
			createNewClasses(year);
			break;

		case 1:
			createSemester(year);
			break;
		
		case 2:
			chooseSemester(year);
			break;

		default:
			break;
		}

		system ("cls");
	}
	system("pause");
}

void chooseAcademicYear(Vector<schoolYear> &allYear) {
	system("cls");
	// cout << "------List Academic Year------\n\n";
	Vector <string> listYear;
	for (int i = 0; i < allYear.size(); i++)
		listYear.push_back(allYear[i].name);

	while (true) {
        cout << "Choose the Academic Year you want to edit. BACKSPACE to stop\n";

        int t = actionList(listYear, {0, 1});
        
		if (t == listYear.size()) return;

		fs::path tmp = root;
		root = root / listYear[t].c_str();

		editSchoolYear(allYear[t]);

		root = tmp;

        system("CLS");
    }	
}

void allStaffFunction() {
	fakeCourse.name = "-1";
	// exit (0);
	Vector <schoolYear> allYear;
	loadLastSave(allYear);

	// cout << allYear[1].listSemester[1].listCourse.size() << '\n';
	// exit (0);
	// for (int i = 0; i < allYear[1].listSemester.size(); i++) {
	// 	cout << allYear[1].listSemester[i].name << '\n';
	// }
	// exit (0);

	system("cls");

    Vector < string > listOperation;
    listOperation.push_back("Create a new academic year");
	listOperation.push_back("Edit an academic year");

    while (true) {
        cout << "Choose the function you want to perform. BACKSPACE to stop\n";

        int t = actionList(listOperation, {0, 1});
        switch (t) {
            case 0:
                createNewYear(allYear);
                break;

			case 1:
				chooseAcademicYear(allYear);
				break;

            default:
                return;
        }


        system("CLS");
    }
}

void createSemester(schoolYear& _schoolYear) {
	system ("cls");
	semester _semester;
	cout << "Enter semester name: ";
	getline(cin, _semester.name);

	for (int i = 0; i < _schoolYear.listSemester.size(); i++)
		if (_schoolYear.listSemester[i].name == _semester.name) {
			cout << "This semester has been created before!\n";
			system("pause");
			return;
		}

	// cout << "Enter the academic year of the semester: ";
	// getline(cin, _semester.academicYear);
	_semester.academicYear = _schoolYear.name;
	cout << "Enter start date (DD/MM/YYYY): ";
	cin >> _semester.startDate.day; cin.get();
	cin >> _semester.startDate.month; cin.get();
	cin >> _semester.startDate.year;
	cout << "Enter end date (DD/MM/YYYY): ";
	cin >> _semester.endDate.day; cin.get();
	cin >> _semester.endDate.month; cin.get();
	cin >> _semester.endDate.year;
	if (!(_semester.startDate <= _semester.endDate)) {
		cout << "Start date must be before End date!\n";
		system("pause");
		return;
	}

	saveSemester(_semester);
	
	_schoolYear.listSemester.push_back(_semester);

	cout << "Semester is created successfully.\n";
	system("pause");
}

void saveSemester(semester& _semester) {
	fs :: path link = root/"Semester"/_semester.name;
	// cout << "bug!\n";
	if (!fs::exists(link))
		system(("mkdir " + link.string()).c_str());
	// cout << "bug!\n";

	ofstream fout;
	fout.open(root / "Semester" / _semester.name / "semester.txt");
		
	if (fout.is_open()) {
		fout << _semester.name << endl;
		fout << _semester.academicYear << endl;

		fout << _semester.startDate.day << '/' << _semester.startDate.month << '/' << _semester.startDate.year << endl;
		fout << _semester.endDate.day << '/' << _semester.endDate.month << '/' << _semester.endDate.year << endl;
		fout << _semester.regOpen.day << '/' << _semester.regOpen.month << '/' << _semester.regOpen.year << endl;
		fout << _semester.regClose.day << '/' << _semester.regClose.month << '/' << _semester.regClose.year << endl;

		for (int i = 0; i < _semester.listCourse.size(); ++i) {
			// cout << _semester.listCourse[i].ID << '\n';
			fout << _semester.listCourse[i].ID << '\n';
			saveCourseInfo(_semester ,_semester.listCourse[i]);
		}
	}
	
	fout.close();
}