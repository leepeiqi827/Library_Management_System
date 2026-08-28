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
void displayMainMenu();
void exitSystem();



// =========================== Main Function ===========================
int main() {

	// Load data from files
	loadAllData();
	initializeMonthlyStats();

	// Initialize monthly statistics if empty
	initializeMonthlyStats();

	int choice;
	do {
		displayMainMenu();
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cout << "Invalid input! Please enter a number.\n";
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
		}
	} while (choice != 5);

	return 0;
}

void displayMainMenu() {
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

void exitSystem() {
	cout << "\n================================================\n";
	cout << "		Exiting Library Management System......";
	cout << "\n================================================\n";
	cout << "  Thank you for using the Library Management System!\n";
	cout << "  Goodbye!\n";
}

void initializeMonthlyStats() {

}