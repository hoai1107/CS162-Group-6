#include "Function.h"

int main() {
	SetConsoleOutputCP(65001); // Be able to output Vietnamese Words
	Vector<staff> _staff;
	Vector<student> _student;
	Vector<semester> _semester;
	semester nSemester;
	// After load data.

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	date today;
	today.day = newtime.tm_mday;
	today.month = newtime.tm_mon + 1;
	today.year = newtime.tm_year + 1900;

	for (int i = 0; i < _semester.size(); i++)
		if (_semester[i].startDate <= today && today <= _semester[i].endDate) {
			nSemester = _semester[i];
			break;
		}


	int res = 0;
	int indexOfUser = 0;
	while (res != 1 && res != 2) {
		cout << "LOGIN";
		if (res == 0) {
			cout << "Wrong ID or password!";
			system("pause");
		}
		system("CLS");
	}

	if (res == 1) {
		// STAFF
	}

	else {

		if (nSemester.regOpen <= today && today <= nSemester.regClose) {
			Vector<string> allActions; 
			allActions.push_back("Enroll in a course.");
			allActions.push_back("View list of enrolled courses.");
			allActions.push_back("Remove a course from the enrolled list.");
			allActions.push_back("Log out.");
			int choice;
			bool exit = false;
			do {
				cout << "ENTER to go into tasks. UP ARROW, DOWN ARROW to move. BACKSPACE to exit.";
				choice = actionList(allActions, {0, 1});
				switch (choice) {
					system("cls");
					case 0: {
						enrollCourses(_student[indexOfUser], nSemester);
						break;
					}
					case 1: {
						viewEnrolledCourses(_student[indexOfUser], nSemester);
						system("pause");
						break;
					}
					case 2: {
						removeCourseFromList(_student[indexOfUser], nSemester);
						break;
					}
					case 3:
					case 4: {
						cout << "Do you really want to exit?";
						Vector<string> confirm;
						confirm.push_back("YES");
						confirm.push_back("NO");
						int t = actionList(confirm, {0, 1});
						if (t == 0) exit = true;
						else exit = false;
						break;
					}
				}
			} while (!exit);
		}
		else {
			Vector<string> allActions; 
			allActions.push_back("View list of your courses.");
			allActions.push_back("View your scoreboard.");
			allActions.push_back("Log out.");
			int choice;
			bool exit = false;
			do {
				cout << "ENTER to go into tasks. UP ARROW, DOWN ARROW to move. BACKSPACE to exit.";
				choice = actionList(allActions, {0, 1});
				switch (choice) {
					system("cls");
					case 0: {
						viewEnrolledCourses(_student[indexOfUser], nSemester);
						system("pause");
						break;
					}
					case 1: {
						viewStudentScoreboard(_student[indexOfUser]);
						system("pause");
						break;
					}
					case 2:
					case 3: {
						cout << "Do you really want to exit?";
						Vector<string> confirm;
						confirm.push_back("YES");
						confirm.push_back("NO");
						int t = actionList(confirm, {0, 1});
						if (t == 0) exit = true;
						else exit = false;
						break;
					}
				}
			} while (!exit);
		}
	}
	return 0;
}
