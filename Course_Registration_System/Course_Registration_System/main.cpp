#include "Function.h"

int main() {
	SetConsoleOutputCP(65001); // Be able to output Vietnamese Words
	Vector<staff> _staff;
	Vector<schoolYear> year;
	semester nSemester;
	// After load data.

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	date today;
	today.day = newtime.tm_mday;
	today.month = newtime.tm_mon + 1;
	today.year = newtime.tm_year + 1900;

	int yearIndex, classIndex, userIndex;

	int res = login(_staff, year, userIndex, classIndex, yearIndex);
	while (res != 1 && res != 2) {
		cout << "-------------------LOGIN------------------";
		if (res == 0) {
			cout << "Wrong ID or password!";
			system("pause");
		}
		system("CLS");
		res = login(_staff, year, userIndex, classIndex, yearIndex);
	}

	if (res == 1) {
		// STAFF
	}

	else {
		for (int i = 0; i < year[yearIndex].listSemester.size(); i++) 
			if (year[yearIndex].listSemester[i].startDate <= today && today <= year[yearIndex].listSemester[i].endDate){
				nSemester = year[yearIndex].listSemester[i];
				break;
			}

		if (nSemester.regOpen <= today && today <= nSemester.regClose) {
		system("CLS");
		Vector<string> allActions;
		allActions.push_back("Enroll in a course.");
		allActions.push_back("View list of enrolled courses.");
		allActions.push_back("Remove a course from the enrolled list.");
		allActions.push_back("User info.");
		allActions.push_back("Change password.");
		allActions.push_back("Log out.");
		int choice;
		bool exit = false;
		do {
			system("cls");
			cout << "ENTER to go into tasks. UP ARROW, DOWN ARROW to move. BACKSPACE to exit.";
			choice = actionList(allActions, { 0, 1 });
			switch (choice) {
				system("CLS");
			case 0: {
				enrollCourses(year[yearIndex].newClass[classIndex].listStudent[userIndex], nSemester);
				break;
			}
			case 1: {
				viewEnrolledCourses(year[yearIndex].newClass[classIndex].listStudent[userIndex], nSemester);
				system("pause");
				break;
			}
			case 2: {
				removeCourseFromList(year[yearIndex].newClass[classIndex].listStudent[userIndex], nSemester);
				break;
			}
			case 3: {
				viewUserInfo(2, _staff[0], year[yearIndex].newClass[classIndex].listStudent[userIndex]);
				system("pause");
				break;
			}
			case 4: {
				int t = changePassword_Student(year[yearIndex].newClass[classIndex].listStudent, userIndex);
				while (t != 0) {
					system("CLS");
					if (t == 1) {
						cout << "Old password is wrong!\n";
					}
					else {
						cout << "Two passwords don't match!\n";
					}
					system("pause");
					system("CLS");
					t = changePassword_Student(year[yearIndex].newClass[classIndex].listStudent, userIndex);
				}
				system("cls");
				cout << "Change successfully!\n";
				saveAccountInfo(year[yearIndex], year[yearIndex].newClass[classIndex], _staff);
				system("pause");

				break;
			}
			case 5: 
			case 6: {
				cout << "Do you really want to exit?";
				Vector<string> confirm;
				confirm.push_back("YES");
				confirm.push_back("NO");
				int t = actionList(confirm, { 0, 1 });
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
		allActions.push_back("User info.");
		allActions.push_back("Change password.");
		allActions.push_back("Log out.");
		int choice;
		bool exit = false;
		do {
			system("cls");
			cout << "ENTER to go into tasks. UP ARROW, DOWN ARROW to move. BACKSPACE to exit.";
			choice = actionList(allActions, { 0, 1 });
			switch (choice) {
				system("CLS");
			case 0: {
				viewEnrolledCourses(year[yearIndex].newClass[classIndex].listStudent[userIndex], nSemester);
				system("pause");
				break;
			}
			case 1: {
				viewStudentScoreboard(year[yearIndex].newClass[classIndex].listStudent[userIndex]);
				system("pause");
				break;
			}
			case 2: {
				viewUserInfo(2, _staff[0], year[yearIndex].newClass[classIndex].listStudent[userIndex]);
				system("pause");
				break;
			}
			case 3: {
				int t = changePassword_Student(year[yearIndex].newClass[classIndex].listStudent, userIndex);
				while (t != 0) {
					system("CLS");
					if (t == 1) {
						cout << "Old password is wrong!\n";
					}
					else {
						cout << "Two passwords don't match!\n";
					}
					system("pause");
					system("CLS");
					t = changePassword_Student(year[yearIndex].newClass[classIndex].listStudent, userIndex);
				}
				system("cls");
				cout << "Change successfully!\n";
				saveAccountInfo(year[yearIndex], year[yearIndex].newClass[classIndex], _staff);
				system("pause");
				break;
			}
			case 4: 
			case 5: {
				cout << "Do you really want to exit?";
				Vector<string> confirm;
				confirm.push_back("YES");
				confirm.push_back("NO");
				int t = actionList(confirm, { 0, 1 });
				if (t == 0) exit = true;
				else exit = false;
				break;
			}
			}
		} while (!exit);
	}
	return 0;
}