#include "Function.h"

int main() {
	SetConsoleOutputCP(65001); // Be able to output Vietnamese Words
	vector<staff> _staff;
	vector<student> _student;

	// After load data.
	int res = 0;
	int indexOfUser;
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
		// STUDENT
	}
	return 0;
}