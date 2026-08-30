/*
Main.cpp - Library Management System
AMCS2123 System And Programming Concepts																																																																																																	
Team: DCSY2S1G3

Team Members:
1. LEE PEI QI (25WMD03889) - Fines / Payment & Reporting
2. LIM ZHI YUAN (25WMD04162) - Reservation & Renewal
3. NEOH E CHYN (25WMD04015) - Book Management & Borrow/Return
4. FRENGKY WONG HAI XIANG (25WMD03962) - Member Management
*/

#include "LibrarySystem.h"
#include<iostream>
using namespace std;

//Function Prototypes
void displayWelcomeScreen();
void displayMainMenu();
void exitSystem();



// =========================== Main Function ===========================
int main() {

	displayWelcomeScreen();

	// Load data from files
	loadAllData();
	initializeMonthlyStats();


	int choice;
	do {
		displayMainMenu();
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input! Please enter a number.\n";
			pause();
			continue;
		}

		switch (choice) {
		case 1:
			memberManagementMenu();
			break;
		case 2:
			bookManagementMenu();
			break;
		case 3:
			reservationManagementMenu();
			break;
		case 4:
			paymentReportingMenu();
			break;
		case 5:
			exitSystem();
			break;
		default:
			cout << "Invalid choice! Please enter 1-5.\n";
			cin.ignore(1000, '\n');
			pause();
		}
	} while (choice != 5);

	return 0;
}

// Display welcome screen on program startup
void displayWelcomeScreen() {
	clearScreen();
	cout << "\n";
	cout << "==================================================================\n";
	cout << "                   LIBRARY MANAGEMENT SYSTEM\n";
	cout << "==================================================================\n";
	cout << "             AMCS2123 Systems And Programming Concepts\n";
	cout << "                      TAR UMT - June 2026\n";
	cout << "==================================================================\n";
	cout << "  Team: DCSY2S1 G3\n";
	cout << "  Tutor: CHIA SHEN KHOO\n";
	cout << "==================================================================\n";
	cout << "  Team Members:\n";
	cout << "  1. LEE PEI QI (25WMD03889) - Payment & Reporting\n";
	cout << "  2. LIM ZHI YUAN (25WMD04162) - Reservation & Renewal\n";
	cout << "  3. NEOH E CHYN (25WMD04015) - Book Management & Borrow/Return\n";
	cout << "  4. FRENGKY WONG HAI XIANG (25WMD03962) - Member Management\n";
	cout << "==================================================================\n";
	cout << "\n";
	pause();
}

// Display main menu
void displayMainMenu() {
	clearScreen();
	cout << "\n================================================\n";
	cout << "            Library Management System";
	cout << "\n================================================\n";
	cout << endl;
	cout << "\t1. Member Management\n";
	cout << "\t2. Book Management\n";
	cout << "\t3. Reservation & renewal Management\n";
	cout << "\t4. Payment & Reporting\n";
	cout << "\t5. Exit System\n";
	cout << "\n================================================\n";
	cout << "\tEnter choice (1-5): ";

}

// Exit system and save all data
void exitSystem() {
	saveAllData();
	cout << "Exiting Library Management System......\n";
	cout << "Thank you for using the Library Management System!\n";
	cout << "Goodbye!\n";
}
