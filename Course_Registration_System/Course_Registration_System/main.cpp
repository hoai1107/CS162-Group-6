#include "Function.h"

int main() {
	SetConsoleOutputCP(65001); // Be able to output Vietnamese Words
	vector<staff> _staff;
	vector<student> _student;

	// After load data.
	int res = 0;
	int indexOfUser = 0;
	while (res != 1 && res != 2) {
		gotoxy(55, 1);
		cout << "LOGIN";
		// APPEARANCE OF LOGIN SYSTEM
		res = login(_staff, _student, indexOfUser);

		gotoxy(47, 5);
		if (res == 0) {
			cout << "Wrong ID or password!";
			Sleep(1000);
		}
		system("CLS");
	}

	if (res == 1) {
		// STAFF
	}

	else {
		vector<string> allActions; // 16 can be changed.
		int choice;
		choice = actionList(allActions, allActions.size(), "Enter to go into task!", {0, 0});
		while (choice != allActions.size() - 1) {
			switch (choice) {
				case 1: {
					break;
				}
				case 2: {
					break;
				}
				case 3: {
					break;
				}
			}
			choice = actionList(allActions, allActions.size(), "Enter to go into task!", {0, 0});
		}
		system("CLS");
		cout << "Ending!" << endl;
	}
	return 0;
}