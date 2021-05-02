#include "Function.h"
#include<fstream>
#include<ctime>
#include<filesystem>
#include<sys/stat.h>
#include<iostream>
#include <sstream>

using namespace std;
namespace fs= std::filesystem;

fs::path root= fs::current_path()/"data";
course fakeCourse;
classUni fakeClass;

void addStudentToClass(classUni& className) {
	ifstream fin;

	fs::path classPath = root / "Class" / className.name / (className.name + ".csv");
	if(!fs::exists(classPath)) {
		cout<<className.name + ".csv"<<" does not exist!";
		return;
	}

	fin.open(classPath);
	
	string tmp;
	getline(fin, tmp);
	
	int i = 1;
	while (!fin.eof()) {
		string line;
		student st;
		while (getline(fin, line)) {
			vector<string> data;
			stringstream s(line);
			string field;
			while (getline(s, field, ',')) {
				data.push_back(field);
			}

			st.no = i;
			for (int i = 0; i < data.size(); ++i) {
				switch (i) {
				case 0: st.ID = data[i]; break;
				case 1:st.firstName = data[i]; break;
				case 2:st.lastName = data[i]; break;
				case 3:st.gender = data[i]; break;
				case 4:
				{
					int first = -1, second = -1;
					for (int j = 0; j < data[i].length(); ++j) {
						if (data[i][j] == '/') {
							if (first == -1) {
								first = j;
							}
							else {
								second = j;
							}
						}
					}

					st.DOB.day = stoi(data[i].substr(0, first));
					st.DOB.month = stoi(data[i].substr(first + 1, second - first - 1));
					st.DOB.year = stoi(data[i].substr(second + 1, data[i].length() - second - 1));
					break;
				}
				case 5:st.socialID = data[i]; break;
				}
			}

			className.listStudent.push_back(st);
			++i;
		}
	}
	fin.close();
	

	fs::path passPath = root / "Class" / className.name / "password.csv";
	if (fs::exists(passPath)) {
		fin.open(passPath);

		int i = 0;
		while (fin.good() && i < className.listStudent.size()) {
			getline(fin, className.listStudent[i].password, '\n');
			i++;
		}
	}
	else {
		for (int i = 0; i < className.listStudent.size(); i++) {
			string password = "";
			for (int j = className.listStudent[i].ID.size() - 4; j < className.listStudent[i].ID.size(); j++)
				password += className.listStudent[i].ID[j];
			if (className.listStudent[i].DOB.day < 10) password += "0" + to_string(className.listStudent[i].DOB.day);
			else password += to_string(className.listStudent[i].DOB.day);
			if (className.listStudent[i].DOB.month < 10) password += "0" + to_string(className.listStudent[i].DOB.month);
			else password += to_string(className.listStudent[i].DOB.month);
			className.listStudent[i].password = password;
	
		}
	}
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
	cout << "You have enrolled:\n";
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
        int t = actionList(actions, { 105, 3 });

        if (t == actions.size()) return;

		for (int i = 0; i < _semester.listCourse.size(); i++) {
			if (_semester.listCourse[i].ID == _student.enrolled[t].ID) {
				for (int j = 0; j < _semester.listCourse[i].listStudent.size(); j++) 
					if (_semester.listCourse[i].listStudent[j].ID == _student.ID) {
						_semester.listCourse[i].listStudent.erase(j);
						break;
					}
				exportScoreboard(_semester, _semester.listCourse[i], false);
				break;
			}
		}

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
		
		cout << left << setw(5) << crs.listStudent[i].no 
			 	 	 << setw(10) << crs.listStudent[i].ID 
					 << setw(20) << crs.listStudent[i].lastName
					 << setw(15) << crs.listStudent[i].firstName 
					 << setw(15) << crs.listStudent[i].className 
					 << setw(10) << crs.listStudent[i].gender
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
	string link;
	getline(cin, link);

	/*fs::path path = root/"Class"/nClass.name;
	system(("mkdir " + path.string()).c_str());
	system(("cd " + path.string() + "&& type nul > " + nClass.name + ".csv").c_str());
	path = path/(nClass.name + ".csv");
	system(("copy " + link + " " + path.string() + " > nul").c_str());*/

	const string filename = nClass.name + ".csv";
	fs::path source = link;
	fs::path dest = root / "Class" / nClass.name;

	fs::create_directories(dest);
	dest /= filename;

	fs::rename(source, dest);


	addStudentToClass(nClass);
	_schoolYear.newClass.push_back(nClass);
	cout << "Class has been added successfully.\n";
	system("pause");
}

void addCourseToSemester(semester& sem){ 
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
	cout << "Course is added successfully!\n";
	system("pause");
}

void enrollCourses(student& _student, semester& _semester) {
	while (_student.enrolled.size() <= 5) {
        Vector<course> unenrolledCourse = getUnenrolledCourseList(_semester, _student);
		if (unenrolledCourse.size() == 0) {
			cout << "There isn't any courses available for you.";
			system("pause");
			return;
		}
        cout << "Choose the course you want to enroll. BACKSPACE to stop." << endl;
        viewCourses(unenrolledCourse);

        Vector<string> actions;
        for (int i = 0; i < unenrolledCourse.size(); i++) actions.push_back("Enroll.");
        int t = actionList(actions, { 105, 2 });

        if (t == actions.size()) return;

        module temp;
        temp.ID = unenrolledCourse[t].ID;
		temp.nameSem = _semester.name;
        bool canEnroll = true;

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
					break;
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

int chooseSemester(schoolYear& _schoolYear) {
	system("cls");
	if (_schoolYear.listSemester.size() == 0) {
		cout << "None semester in this year!\n";
		return -1;
	}
	Vector <string> Sem;
	for (int i = 0; i < _schoolYear.listSemester.size(); i++) {
		Sem.push_back(_schoolYear.listSemester[i].name);
	}

		system("cls");
		cout << "Choose the semester. BACKSPACE to stop\n";
		int t = actionList(Sem, {0, 1});

		return t;
}

void editSemester(schoolYear& _schoolYear, semester& _semester) {
	system("cls");
	Vector <string> listAction;
	listAction.push_back("Create course registration session");
	listAction.push_back("Add a course");
	listAction.push_back("View list of courses");
	listAction.push_back("Update a course infomation");
	listAction.push_back("Delete a course");
	listAction.push_back("View course scoreboard");
	listAction.push_back("Export list of students in course (.csv)");
	listAction.push_back("Export empty course scoreboard (.csv)");
	listAction.push_back("Import course scoreboard (.csv)");
	listAction.push_back("Update student result");
	listAction.push_back("View class scoreboard");

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
				system("pause");
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

			case 6:
				exportStudentInCourseToCSV(_semester);
				break;

			case 7: {
				auto& _course = getCourse(_semester);
				if (_course.name == "-1") break;
				exportScoreboard(_semester, _course, true);
				break;
			}
			
			case 8:
				importScoreboard(_schoolYear, _semester);
				break;

			case 9:
				updateStudentResult(_schoolYear, _semester);
				break;

			case 10:
				viewClassScoreboard(_schoolYear, _semester);
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

int login(Vector<staff> _staff, Vector<schoolYear> _year, int& userIndex, int& classIndex, int& yearIndex) {

	string attempID;
	string attempPass;

	cout << "ID      : "; getline(cin, attempID);
	cout << "Password: "; getline(cin, attempPass);
	cout << "\n";

	for (int i = 0; i < _staff.size(); i++)
		if (attempID == _staff[i].ID && attempPass == _staff[i].password) {
			userIndex = i;
			return 1;
		}
	for (int i = 0; i < _year.size(); i++) 
		for (int j = 0; j < _year[i].newClass.size(); j++) 
			for (int k = 0; k < _year[i].newClass[j].listStudent.size(); k++)
				if (_year[i].newClass[j].listStudent[k].ID == attempID && _year[i].newClass[j].listStudent[k].password == attempPass) {
					userIndex = k;
					classIndex = j;
					yearIndex = i;
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
	while(true){
		system ("cls");
		cout<<"Please choose the section you want to update for the course. BACKSPACE to stop"<<endl;
		int t = actionList(update, {0, 1});
		if (t == update.size()) break;
		switch(t){
			case 0: 
			{
				cout<<"Enter the new ID: ";
				string tmp = _course.ID;
				getline(cin, _course.ID);
				fs::rename(link / tmp, link / _course.ID);
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 1:	
			{
				cout<<"Enter the new course name: ";
				getline(cin,_course.name);
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 2: 
			{
				cout<<"Enter the new teacher name: ";
				getline(cin,_course.teacher);
				cout << "Updated successfully!\n";
				system("pause");
				break;
			}
			case 3: 
			{
				cout<<"Enter the new number of credits: ";
				cin>>_course.numCredits;
				 cin.ignore();
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
				cin>>check; 
				cin.ignore();

				if(tolower(check)=='y'){
					temp = 3 - temp;
					
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

void viewStudentScoreboard(schoolYear year, student stu){
	int t = chooseSemester(year);
	if (t == -1) return;
	cout << left << setw(10) << "ID" 
				 << setw(10) << "Midterm" 
				 << setw(10) << "Final" 
				 << setw(10) << "Other" 
				 << setw(10) << "Total" << endl;
	for(int i=0; i<stu.enrolled.size(); i++){
		if (stu.enrolled[i].nameSem == year.listSemester[t].name) {
			cout << left << setw(10) << stu.enrolled[i].ID 
					<< setw(10) << stu.enrolled[i].grade.midterm 
		 			<< setw(10) << stu.enrolled[i].grade.final 
					<< setw(10) << stu.enrolled[i].grade.other 
					<< setw(10) << stu.enrolled[i].grade.total << endl;
			}
	}
}

void viewCourseScoreboard(semester& _semester){     
	course& crs = getCourse(_semester);
	if (crs.name == "-1") return;
	system("cls");
	cout << left << setw(10) << "ID"
				 << setw(20) << "Name" 
				 << setw(15) << "Midterm" 
				 << setw(15) << "Final" 
				 << setw(15) << "Other" 
				 << setw(15) << "Total" << endl;
	for(int i=0; i < crs.listStudent.size(); i++){
		for(int j=0; j < crs.listStudent[i].enrolled.size(); j++){
			if(crs.listStudent[i].enrolled[j].ID == crs.ID){
				cout << left << setw(10) << crs.listStudent[i].ID
							 << setw(20) << crs.listStudent[i].fullName 
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.midterm
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.final
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.other
							 << setw(15) << crs.listStudent[i].enrolled[j].grade.total << endl;
			}
		}
	}
	system("pause");
}

void updateStudentResult(schoolYear& _schoolYear, semester& _semester) {
	course& crs = getCourse(_semester);
	if (crs.name == "-1") return;

	system ("cls");
	cout << "Student ID: ";
	string id;
	cin >> id;
	int pos = -1;
	for (int i = 0; i < crs.listStudent.size(); i++)
		if (crs.listStudent[i].ID == id) {
			pos = i;
			break;
		}

	if (pos == -1) {
		cout << "He/She did not enroll this course!\n";
		system("pause");
		return;
	}

	auto& stu = crs.listStudent[pos];

	system ("cls");
	bool found = false;
	for (int i = 0; i < stu.enrolled.size(); i++) {
		if (stu.enrolled[i].ID == crs.ID) {
			cout << "Midterm score: "; cin >> stu.enrolled[i].grade.midterm;
			cout << "Final score: "; cin >> stu.enrolled[i].grade.final;
			cout << "Other score: "; cin >> stu.enrolled[i].grade.other;
			cout << "Total score: "; cin >> stu.enrolled[i].grade.total;
			found = true;
		}
	}

	if (!found) {
		cout << "bug somewhere, course.student.enrolled doesnt contain this course!!!\n";
		system("pause");
		return;
	}

	exportScoreboard(_semester, crs, false);

	readScoreboard(_schoolYear, _semester, crs);

	
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

classUni& getClass(schoolYear& _schoolYear) {
	Vector<string> lClass;
	for(int i=0;i<_schoolYear.newClass.size();++i){
		lClass.push_back(_schoolYear.newClass[i].name);
	}
	system("cls");
	if (lClass.size() == 0) {
		cout << "None new class in this school year!\n";
		system("pause");
		return fakeClass;
	}
	while (true) {
		system("cls");
		cout << "Choose your course. BACKSPACE to stop\n";
		int t = actionList(lClass, {0, 1});
		if (t == lClass.size()) return fakeClass;
		return _schoolYear.newClass[t];
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
			exportScoreboard(_semester, _semester.listCourse[i], false);
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

void exportStudentInCourseToCSV(semester& _semester) {
	auto _course = getCourse(_semester);
	if (_course.name == "-1") return;
	ofstream fout;

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";

	const string name = _course.ID + "_list" + ".csv";
	fs::path link = root / "Semester" / folderName /_course.ID / name;
	fout.open(root / "Semester" / folderName /_course.ID / name);

	if (fout.is_open()) {
		fout << "No" << ','
			<< "ID" << ','
			<< "Fullname" << ','
			<< "Class";

		for (int i = 0; i < _course.listStudent.size(); ++i) {
			student _student = _course.listStudent[i];
			fout << i + 1 << ','
				<< _student.ID << ','
				<< _student.firstName + _student.className << ','
				<< _student.className;

			fout << endl;
		}
		fout.close();

		string temp = link.string();
		const char* c = temp.c_str();
		wchar_t aaa[MAX_PATH];
		copy(c, c + strlen(c) + 1, aaa);

		ShellExecute(NULL, L"open", aaa, NULL, NULL, SW_SHOW);
		cout << "Export file successfully\n";
		system("pause");
	}
	else {
		cout << "Something wrong with the file!!";
		fout.close();
		system ("pause");
	}
}

void exportScoreboard(semester& _semester, course& _course, bool empty) {
	ofstream fout;

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";

	const string name = "scoreboard.csv";
	fs::path link = root / "Semester" / folderName /_course.ID / name;
	fout.open(root / "Semester" / folderName /_course.ID / name);

	if (fout.is_open()) {
		fout << "No" << ','
			<< "ID" << ','
			<< "Fullname" << ','
			<< "Class" << ','
			<< "Midterm mark" << ','
			<< "Final mark" << ','
			<< "Other mark" << ','
			<< "Total mark" << '\n';

		for (int i = 0; i < _course.listStudent.size(); ++i) {
			student _student = _course.listStudent[i];
			if (empty)
				fout << i + 1 << ','
					<< _student.ID << ','
					<< _student.lastName + " " + _student.firstName << ','
					<< _student.className << ','
					<< ','
					<< ','
					<< ','
					<< '\n';
			else {
				int posditme = -1;
				for (int j = 0; j < _student.enrolled.size(); j++)
					if (_student.enrolled[j].ID == _course.ID) {
						posditme = j;
						break;
					}
				if (posditme == -1) {
					cout << "Bug chetcondime roi, course.student.enrolled ko co course nay!!!!!!!\n";
					system ("pause");
					return;
				}
				fout << i + 1 << ','
					<< _student.ID << ','
					<< _student.firstName + _student.className << ','
					<< _student.className << ','
					<< _student.enrolled[posditme].grade.midterm << ','
					<< _student.enrolled[posditme].grade.final << ','
					<< _student.enrolled[posditme].grade.other << ','
					<< _student.enrolled[posditme].grade.total << '\n';
			}

		}
		fout.close();
		
		if (empty) {
			string temp = link.string();
			const char* c = temp.c_str();
			wchar_t aaa[MAX_PATH];
			copy(c, c + strlen(c) + 1, aaa);

			ShellExecute(NULL, L"open", aaa, NULL, NULL, SW_SHOW);
			cout << "Export file successfully\n";
			system("pause");
		}
	}
	else {
		cout << "Something wrong with the file!!\n";
		fout.close();
		system ("pause");
	}
}

void importScoreboard(schoolYear& _schoolYear, semester& _semester) {
	auto& _course = getCourse(_semester);
	if (_course.name == "-1") return;

	cout << "Enter or drag path of your .csv file: ";
	string link; getline(cin, link);

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";
	fs::path fakepath = root / "Semester" / folderName / _course.ID;
	fs::path coursePath = root / "Semester" / folderName / _course.ID / "scoreboard.csv";

	if (!fs::exists(coursePath)) {
		system(("cd " + fakepath.string() + "&& type nul > scoreboard.csv").c_str());
	}

	system(("copy " + link + " " + coursePath.string() + " > nul").c_str());
	bool isOk = readScoreboard(_schoolYear, _semester, _course);
	if (isOk) {
		cout << "Imported successfully.\n";
		system("pause");
	}
	else {
		cout << "Something wrong with file!\n";
		system ("pause");
	}
	return;
}

bool readScoreboard(schoolYear& _schoolYear, semester& _semester, course& _course) {
	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";
	fs::path coursePath = root / "Semester" / folderName / _course.ID / "scoreboard.csv";

	if (!fs::exists(coursePath)) {
		return false;
	}

	ifstream fin;
	fin.open(coursePath);

	if (fin.is_open()) {
		fin.ignore(1000, '\n');
		string s;
		getline(fin, s);
		int countCommas = 0;
		for (char c : s) countCommas += (c == ',');
		fin.close();
		if (countCommas != 7) return false;
	}
	else {
		fin.close();
		return false;
	}
	

	fin.open(coursePath);
	//cout << _course.name<< '\n';
	if (fin.is_open()) {
		//Ignore first line
		fin.ignore(1000, '\n');

		// Assume that the order of student in the file is the same as the file when it is exported
		for (int i = 0; i < _course.listStudent.size(); ++i) {

			//Just to store the old info to get to the mark
			student temp;
			fin >> temp.no;
			fin.ignore(1, ',');
			getline(fin, temp.ID, ',');
			getline(fin, temp.fullName, ',');
			getline(fin, temp.className, ',');
			//cout << temp.ID << ' ' << temp.fullName << ' ';

			//Get the score
			module _module;
			_module.nameSem = _semester.name;
			_module.ID = _course.ID;
			fin >> _module.grade.midterm;
			fin.ignore(1, ',');
			fin >> _module.grade.final;
			fin.ignore(1, ',');
			fin >> _module.grade.other;
			fin.ignore(1, ',');
			fin >> _module.grade.total;
			fin.ignore();
			//cout << _module.grade.total << '\n';

			//Store to the course
			auto& huhu = _course.listStudent[i].enrolled;
			bool found = false;
			for (int j = 0; j < huhu.size(); j++) if (huhu[j].ID == _module.ID && huhu[j].nameSem == _semester.name) {
				huhu[j] = _module; found = true;
			}
			if (!found) huhu.push_back(_module);
			

			//Store to the class
			for (int j = 0; j < _schoolYear.newClass.size(); j++)
				if (_schoolYear.newClass[j].name == temp.className) {
					for (int k = 0; k < _schoolYear.newClass[j].listStudent.size(); ++k) {
						if (_schoolYear.newClass[j].listStudent[k].ID == temp.ID) {
							auto& huhu = _schoolYear.newClass[j].listStudent[k];
							for (int t = 0; t < huhu.enrolled.size(); t++) {
								if (huhu.enrolled[t].ID == _module.ID && huhu.enrolled[t].nameSem == _semester.name) {
									huhu.enrolled[t] = _module;
								goto nextStudent;
								}
							}
							_schoolYear.newClass[j].listStudent[k].enrolled.push_back(_module);
							goto nextStudent;
						}
					}
				}
		nextStudent: continue;
		}

	}
	else {
		fin.close();
		return false;
	}
	fin.close();
	return true;
	
}

void viewClassScoreboard(schoolYear& _schoolYear, semester& _semester) {
	auto _class = getClass(_schoolYear);
	if (_class.name == "-1") return;

	cout << setw(5) << left << "No.";
	cout << setw(15) << left << "Student ID";
	cout << setw(20) << left << "Full Name";
	cout << setw(15) << left << "GPA semester";
	cout << setw(15) << left << "GPA overall" << '\n';

	for (int i = 0; i < _class.listStudent.size(); ++i) {
		student _student = _class.listStudent[i];

		cout << setw(5) << left << i + 1;
		cout << setw(15) << left << _student.ID;
		cout << setw(20) << left << (_student.lastName + ' ' + _student.firstName);

		float sumSem = 0, sumOverall = 0;
		int cntSem = 0, cntOverall = 0;

		for (int j = 0; j < _student.enrolled.size(); ++j) {
			auto score = _student.enrolled[j].grade.total;
			auto sem = _student.enrolled[j].nameSem;
			if (sem == _semester.name)
				sumSem += score, cntSem++;
			sumOverall += score, cntOverall++;
		}
		if (cntSem == 0) cout << setw(15) << 0 << '\n';
		else cout << setw(15) << sumSem / cntSem;
		if (cntOverall == 0) cout << setw(15) << 0 << '\n';
		else cout << setw(15) << sumOverall / cntOverall << endl;
	}

	for (int x = 0; x < _semester.listCourse.size(); x++) {
		auto crs = _semester.listCourse[x];
		cout << '\n' << crs.name << "\n\n";

		cout << setw(5) << left << "No.";
		cout << setw(15) << left << "Student ID";
		cout << setw(20) << left << "Full Name";
		cout << setw(15) << left << "Finalmark" << '\n';

		for (int i = 0, ttt = 0; i < _class.listStudent.size(); ++i) {
			student _student = _class.listStudent[i];


			for (int j = 0; j < _student.enrolled.size(); ++j) {
				auto score = _student.enrolled[j].grade.total;
				auto IDcrs = _student.enrolled[j].ID;
				auto sem = _student.enrolled[j].nameSem;
				if (sem == _semester.name && IDcrs == crs.ID) {
					cout << setw(5) << left << ++ttt;
					cout << setw(15) << left << _student.ID;
					cout << setw(20) << left << (_student.lastName + ' ' + _student.firstName);
					cout << setw(15) << left << score << '\n';
					break;
				}
			}
		}
	}
	system("pause");
}

void createScoreboardFile(course& _course, semester _semester) {
	ofstream fout;

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";
	fs::path coursePath = root / "Semester" / folderName / _course.ID / "scoreboard.csv";
	fout.open(coursePath);

	if (fout.is_open()) {
		fout << "No" << ','
			<< "ID" << ','
			<< "First Name" << ','
			<< "Last Name" << ','
			<< "Class" << ','
			<< "Gender" << ','
			<< "DOB" << ','
			<< "Social ID" << ','
			<< "Midterm" << ','
			<< "Final" << ','
			<< "Other" << ','
			<< "Total" << endl;

		for (int i = 0; i < _course.listStudent.size(); ++i) {
			student _student = _course.listStudent[i];
			fout << i + 1 << ','
				<< _student.ID << ','
				<< _student.firstName << ','
				<< _student.lastName << ','
				<< _student.className << ','
				<< _student.gender << ','
				<< _student.DOB.day << '/' << _student.DOB.month << '/' << _student.DOB.year << ','
				<< _student.socialID;

			fout << endl;
		}
	}
	else {
		cout << "Something wrong with the file!!";
	}
	fout.close();
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

void loadSemesterInfo(schoolYear& _schoolYear, Vector<semester>& _semester) {
	
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
				loadCourseInfo(_schoolYear, sTemp ,_course);
				initiateCourseList(_schoolYear, sTemp ,_course);
				readScoreboard(_schoolYear, sTemp, _course);
				sTemp.listCourse.push_back(_course);
			}

			fin.close();
			_semester.push_back(sTemp);
		}
	}
}

void loadCourseInfo(schoolYear& _schoolYear, semester& _semester, course& _course) {
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

void initiateCourseList(schoolYear& _schoolYear, semester& _semester, course& _course) {
	ifstream fin;

	string folderName;
	if (_semester.name == "1" || _semester.name == "fall" || _semester.name == "Fall") folderName = "Fall";
	if (_semester.name == "2" || _semester.name == "summer" || _semester.name == "Summer") folderName = "Summer";
	if (_semester.name == "3" || _semester.name == "autumn" || _semester.name == "Autumn") folderName = "Autumn";

	const string name = _course.ID + "_list" + ".csv";
	fin.open(root / "Semester" / folderName / _course.ID / name);
	if (fin.is_open()) {
		//Ignore first line
		string temp;
		getline(fin, temp);
		
		while (!fin.eof()) {
			string line;
			student st;
			while (getline(fin, line)) {
				vector<string> data;
				stringstream s(line);
				string field;
				while (getline(s, field, ',')) {
					data.push_back(field);
				}

				for (int i = 0; i < data.size(); ++i) {
					switch (i) {
					case 0:st.no = stoi(data[i]); break;
					case 1:st.ID = data[i]; break;
					case 2:st.fullName = data[i]; break;
					case 3:st.className = stoi(data[i]); break;
					}
				}

				_course.listStudent.push_back(st);
			}
		}

	}

	fin.close();
}

void saveCourseInfo(semester& _semester, course& _course) {
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
		
		
		fout.close();
	}
}

bool IsPathExist(const string &s) {
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

void addClassToYear(schoolYear &year) {
	fs::path link = root;
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
		loadSemesterInfo(year, _semester);
		year.listSemester = _semester;
	}
}

//https://www.bfilipek.com/2019/04/dir-iterate.html
void loadLastSave(Vector <schoolYear> &listYear, Vector<staff>& _staff) {
	
	loadStaff(_staff);

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
		
		case 2: {
			int t = chooseSemester(year);
			if (t == -1) break;
			if (t != year.listSemester.size()) editSemester(year, year.listSemester[t]); 
			break;
		}

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

void allStaffFunction(Vector <staff>& _staff, Vector <schoolYear>& allYear) {
	fakeCourse.name = "-1";
	fakeClass.name = "-1";

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

	if (!fs::exists(link))
		system(("mkdir " + link.string()).c_str());
	

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
			fout << _semester.listCourse[i].ID << '\n';
			saveCourseInfo(_semester ,_semester.listCourse[i]);
		}
	}
	
	fout.close();
}

void loadStaff(Vector<staff> &_staff) {
	ifstream fin;

	fs::path staffPath = root / ("staff.csv");
	if (!fs::exists(staffPath)) {
		cout << "There isn't staff.csv file.\n";
		system("pause");
		return; 
	}

	fin.open(staffPath);
	int i = 0;

	while (fin.good()) {
		staff tStaff;

		getline(fin, tStaff.ID, ',');

		if (i >= 1 && tStaff.ID == _staff[i - 1].ID) break;

		getline(fin, tStaff.name, ',');
		getline(fin, tStaff.password, '\n');

		_staff.push_back(tStaff);
		i++;
	}
}

void saveAccountInfo(Vector<schoolYear> _year , Vector<staff> _staff) {
	ofstream fout;

	fs::path staffPath = root / "staff.csv";
	fout.open(staffPath);
	for (int i = 0; i < _staff.size(); i++) 
		fout << _staff[i].ID << "," << _staff[i].name << "," << _staff[i].password << endl;
	fout.close();

	for (int k = 0; k < _year.size(); ++k) {
		for (int j = 0; j < _year[k].newClass.size(); ++j) {
			classUni _class = _year[k].newClass[j];
			fs::path studentPath = root / _year[k].name / "Class" / _class.name / "password.csv";
			fout.open(studentPath);
			fout << _class.listStudent[0].password;
			for (int i = 1; i < _class.listStudent.size(); i++)
				fout << endl << _class.listStudent[i].password;
			fout.close();
		}
	}
}

void viewUserInfo(int studentOrStaff, staff _staff, student _student) {
	if (studentOrStaff == 1) {
		cout << "-----------STAFF INFO-----------\n";
		cout << "ID       : " << _staff.ID << "\n";
		cout << "Full name: " << _staff.name << endl;
	}
	else {
		cout << "-----------STUDENT INFO-----------\n";
		cout << "ID           : " << _student.ID << "\n";
		cout << "Last name    : " << _student.lastName << "\n";
		cout << "First name   : " << _student.firstName << "\n";
		cout << "Gender       : " << _student.gender << "\n";
		cout << "Date of birth: " << _student.DOB.day << "/" << _student.DOB.month << "/" << _student.DOB.year << "\n";
		cout << "Social ID    : " << _student.socialID << endl;
	}
}