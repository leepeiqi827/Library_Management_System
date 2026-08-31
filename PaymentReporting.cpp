// PaymentReporting.cpp - Fine Payment & Reporting Module
//Team Member: LEE PEI QI (25WMD03889)

#include"LibrarySystem.h"
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

// =========================== Function Prototypes ===========================
void paymentReportingMenu();
void processFinePayment();
void generateBorrowingReport();
void generateOverdueReport();
void displayBookCatalog();
void generateMonthlyReport();

// =========================== Main Menu ===========================
void paymentReportingMenu() {

	int choice;
	do {
		clearScreen();
		cout << "\n================================================\n";
		cout << "\t\tPayment & Reporting";
		cout << "\n================================================\n";
		cout << endl;
		cout << "	1. Process Fine Payment\n";
		cout << "	2. Generate Borrowing Report\n";
		cout << "	3. Generate Overdue Members Report\n";
		cout << "	4. Display Full Book Catalog\n";
		cout << "	5. Generate Monthly Statistics Report\n";
		cout << "	6. Back to Main Menu\n";
		cout << "\n================================================\n";
		cout << "Enter choice: ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input: Please enter a number.\n";
			pause();
			continue;
		}

		switch (choice) {
		case 1:
			processFinePayment();
			break;
		case 2:
			generateBorrowingReport();
			break;
		case 3:
			generateOverdueReport();
			break;
		case 4:
			displayBookCatalog();
			break;
		case 5:
			generateMonthlyReport();
			break;
		case 6:
			cout << "Returning to main menu...\n";
			cin.ignore(1000, '\n');
			pause();
			break;
		default:
			cout << "Invalid choice\n";
			cin.ignore(1000, '\n');
			pause();
		}
	} while (choice != 6);

}

//Process fine payment for a member
void processFinePayment() {
	cout << "\n================================================\n";
	cout << "\t\tProcess Fine Payment";
	cout << "\n================================================\n";
	string memberID;
	cout << "Enter Member ID: ";
	cin >> memberID;
	for (auto& c : memberID)
		c = toupper(c);

	int memberIndex = 0;
	if (!findMemberByID(memberID, memberIndex)) {
		cout << "member not found!" << endl;
		cin.ignore(1000, '\n');
		pause();
		return;
	}

	//Find all unpaid fines for this member;
	double totalFine = 0.0;
	vector<int> fineIndices;

	for (int i = 0; i < borrowRecords.size(); i++) {
		if (borrowRecords[i].memberID == memberID &&
			borrowRecords[i].isReturned &&
			borrowRecords[i].fineAmount > 0) {
			totalFine += borrowRecords[i].fineAmount;
			fineIndices.push_back(i);
		}
	}

	if (totalFine == 0.0) {
		cout << "No outstanding fines for this member.\n";
		cin.ignore(1000, '\n');
		pause();
		return;
	}

	//Display fine details
	cout << "\n=============================================================================\n";
	cout << "\t\tOutstanding Fines";
	cout << "\n=============================================================================\n";
	cout << left << setw(10) << "Record ID"
		<< setw(12) << "Book ID"
		<< setw(15) << "Due Date"
		<< setw(15) << "Return Date"
		<< setw(12) << "Fine (RM)\n";
	cout << "\n=============================================================================\n";
	for (int records : fineIndices) {
		const BorrowRecord& br = borrowRecords[records];
		cout << left << setw(10) << br.recordID
			<< setw(12) << br.bookID
			<< setw(15) << br.dueDate
			<< setw(15) << br.returnDate
			<< setw(12) << fixed << setprecision(2) << br.fineAmount << endl;

	}
	cout << "\n=============================================================================\n";
	cout << "Total Fine: RM " << fixed << setprecision(2) << totalFine << endl;
	cout << "\n=============================================================================\n";

	//Get payment amount
	double paymentAmount;
	do {
		cout << "Enter payment amount: RM ";
		cin >> paymentAmount;

		if (cin.fail() || paymentAmount <= 0) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Inavlid payment amount! Please enter a positive number.";
			continue;
		}
		break;
	} while (true);

	//Calculate payment details
	double remaining = 0, outstandingFine = 0, paid;
	if (paymentAmount > totalFine) {
		paid = totalFine;
		remaining = paymentAmount - totalFine;
	}
	else {
		paid = paymentAmount;
		outstandingFine = totalFine - paymentAmount;
	}

	// Update borrow records (mark fines as paid)
	double remainingToPay = paid;
	for (int idx : fineIndices) {
		if (remainingToPay <= 0)
			break;

		BorrowRecord& br = borrowRecords[idx];
		if (remainingToPay >= br.fineAmount) {
			remainingToPay -= br.fineAmount;
			br.fineAmount = 0;
		}
		else {
			br.fineAmount -= remainingToPay;
			remainingToPay = 0;
		}
	}

	cout << "\n=============================================================================\n";
	cout << "                                   Payment Receipt";
	cout << "\n=============================================================================\n";
	cout << "Member ID: " << memberID << endl;
	cout << "Member Name: " << members[memberIndex].name << endl;
	cout << "Payment Date: " << getCurrentDate() << endl;
	cout << "Total Fine: RM" << fixed << setprecision(2) << totalFine << endl;
	cout << "Amount Paid: " << fixed << setprecision(2) << paymentAmount << endl;
	if (remaining > 0) {
		cout << "Change: " << fixed << setprecision(2) << remaining << endl;
	}
	if (outstandingFine > 0) {
		cout << "  Outstanding Fine: RM " << fixed << setprecision(2) << outstandingFine << endl;
	}
	cout << "\n=============================================================================\n";
	cout << " Thank you for your payment!" << endl;
	cout << "\n=============================================================================\n";

	// update montly statistics
	int month = stoi(getCurrentDate().substr(3, 2)) - 1;
	monthlyStats[month].finesCollected += paid;

	saveBorrowRecords();
	cin.ignore(1000, '\n');
	pause();
}

//Generate borrowing report (all borrow records)
void generateBorrowingReport() {
	cout << "\n=====================================================================================================\n";
	cout << "                                           Borrowing Report";
	cout << "\n=====================================================================================================\n";

	if (borrowRecords.empty()) {
		cout << "No Borrowing records found.";
		cin.ignore(1000, '\n');
		pause();
		return;
	}

	int totalBorrowed = 0;
	int totalReturned = 0;
	double totalFines = 0.0;

	cout << left << setw(10) << "Record ID"
		<< setw(10) << "Member ID"
		<< setw(10) << "Book ID"
		<< setw(14) << "Borrow Date"
		<< setw(14) << "Due Date"
		<< setw(14) << "Return Date"
		<< setw(12) << "Status"
		<< setw(14) << "Fine (RM)" << endl;
	cout << "\n=====================================================================================================\n";
	for (const auto& br : borrowRecords) {
		string status = br.isReturned ? "Returned" : "Borrowed";

		cout << left << setw(10) << br.recordID
			<< setw(10) << br.memberID
			<< setw(10) << br.bookID
			<< setw(14) << br.borrowDate
			<< setw(14) << br.dueDate
			<< setw(14) << (br.isReturned ? br.returnDate : "-")
			<< setw(12) << status
			<< setw(14) << fixed << setprecision(2) << br.fineAmount << endl;

		totalBorrowed++;
		if (br.isReturned)
			totalReturned++;
		totalFines += br.fineAmount;
	}
	cout << "\n=====================================================================================================\n";
	cout << " Summary: ";
	cout << " Total Borrowed: " << totalBorrowed << endl;
	cout << " Total Returned: " << totalReturned << endl;
	cout << " Total Fines Collected: RM " << fixed << setprecision(2) << totalFines << endl;
	cout << "\n=====================================================================================================\n";
	cin.ignore(1000, '\n');
	pause();

}

//Generate overdue members report
void generateOverdueReport() {
	cout << "\n==================================================================================================\n";
	cout << "                                      Overdue Members Report";
	cout << "\n==================================================================================================\n";

	string currentDate = getCurrentDate();
	bool found = false;

	cout << left << setw(10) << "Member ID"
		<< setw(30) << "member Name"
		<< setw(10) << "Book ID"
		<< setw(12) << "Borrow Date"
		<< setw(12) << "Due Date"
		<< setw(10) << "Days Late"
		<< setw(12) << "Fine (RM)" << endl;
	cout << "\n==================================================================================================\n";

	for (const auto& br : borrowRecords) {
		if (br.isReturned)
			continue;

		int daysLate = calculateDaysDifference(br.dueDate, currentDate);
		if (daysLate > 0) {
			found = true;
			int memberIndex = 0;
			string memberName = "Unknown";
			if (findMemberByID(br.memberID, memberIndex)) {
				memberName = members[memberIndex].name;
			}
			cout << left << setw(10) << br.memberID
				<< setw(30) << memberName
				<< setw(10) << br.bookID
				<< setw(12) << br.borrowDate
				<< setw(12) << br.dueDate
				<< setw(10) << daysLate
				<< setw(12) << fixed << setprecision(2)
				<< (daysLate * FINE_PER_DAY) << endl;

		}

	}
	if (!found) {
		cout << "No overdue members found." << endl;
	}
	cout << "\n==================================================================================================\n";
	cin.ignore(1000, '\n');
	pause();

}

//Display full book catalog
void displayBookCatalog() {
	cout << "\n=======================================================================================================================\n";
	cout << "                                                   Full Book Catalog";
	cout << "\n=======================================================================================================================\n";

	if (books.empty()) {
		cout << "No books in catalog";
		pause();
		return;
	}

	cout << left << setw(8) << "ID"
		<< setw(37) << "Title"
		<< setw(20) << "Author"
		<< setw(18) << "Category"
		<< setw(8) << "Total"
		<< setw(10) << "Available"
		<< setw(12) << "Status" << endl;
	cout << "\n=======================================================================================================================\n";

	for (const auto& b : books) {
		cout << left << setw(8) << b.bookID
			<< setw(37) << b.title
			<< setw(20) << b.author
			<< setw(18) << b.category
			<< setw(8) << b.totalCopies
			<< setw(10) << b.availableCopies
			<< setw(12) << bookStatusToString(b.status) << endl;
	}
	cin.ignore(1000, '\n');
	pause();
}

//Generate monthly statistics report
void generateMonthlyReport() {
	cout << "\n=============================================================================\n";
	cout << "                            Monthly Statistics Report";
	cout << "\n=============================================================================\n";

	cout << left << setw(8) << "Month"
		<< setw(18) << "Books Borrowed"
		<< setw(18) << "Books Returned"
		<< setw(18) << "Reservations"
		<< setw(18) << "Renewals"
		<< setw(18) << "Fines (RM)" << endl;
	cout << "\n=============================================================================\n";

	string months[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };

	for (int i = 0; i < 12; i++) {
		cout << left << setw(8) << months[i]
			<< setw(18) << monthlyStats[i].booksBorrowed
			<< setw(18) << monthlyStats[i].booksReturned
			<< setw(18) << monthlyStats[i].reservationsMade
			<< setw(18) << monthlyStats[i].renewalsMade
			<< setw(18) << fixed << setprecision(2)
			<< monthlyStats[i].finesCollected << endl;
	}
	cout << "\n=============================================================================\n";

	int totalBorrowed = 0, totalReturned = 0, totalReservations = 0;
	double totalFines = 0.0;
	for (int i = 0; i < 12; i++) {
		totalBorrowed += monthlyStats[i].booksBorrowed;
		totalReturned += monthlyStats[i].booksReturned;
		totalReservations += monthlyStats[i].reservationsMade;
		totalFines += monthlyStats[i].finesCollected;
	}

	cout << left << setw(8) << "Total"
		<< setw(18) << totalBorrowed
		<< setw(18) << totalReturned
		<< setw(18) << totalReservations
		<< setw(18) << fixed << setprecision(2) << totalFines << endl;
	cout << "\n=============================================================================\n";
	cin.ignore(1000, '\n');
	pause();
}
