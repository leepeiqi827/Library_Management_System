//LibrarySystem.cpp - Implementation of Shared Functions

#include "LibrarySystem.h"
#include<vector>
#include <sstream>
#include <cstring>

// =========================== Golbal Variables ===========================
vector<Member> member;
vector<Book> books;
vector<BorrowRecord> borrowRecords;
vector<Reservation> reservations;
vector<MonthlyStatistics> monthlyStats(12);


void pause() {
	cout << "Press enter to continue...";
	cin.get();
}

//Generate unique ID
string generateID(const string& prefix, int number) {
	stringstream ss;
	ss << prefix << setw(3) << setfill('0') << number;
	return ss.str();
}

//Get current date as string
string getCurrentDate() {
	return "q";
}

//Get date offset by days
string getDateFromDays(int daysOffset) {
	return "q";
}

//Calculate days differnt between two dates
int calculateDaysDifference(const string& date1, const string& date2) {

	int d1, m1, y1, d2, m2, y2;
	char slash;

	stringstream ss1(date1);
	ss1 >> d1 >> m1 >> y1;

	stringstream ss2(date2);
	ss2 >> d2 >> m2 >> y2;

	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	int total1 = 0;
	for (int y = 0; y < y1; y++)
		total1 += 365;
	for (int m = 0; m < m1; m++)
		total1 += daysInMonth[m];
	total1 += d1;

	int total2 = 0;
	for (int y = 0; y < y2; y++)
		total2 += 365;
	for (int m = 0; m < m2; m++)
		total2 += daysInMonth[m];
	total2 += d2;

	return total1 - total2;
}

//Convert BookStatus to string
string bookStatusToString(BookStatus status) {
	if (status == AVAILABLE) {
		return "Available";
	}
	else if (status == BORROWED) {
		return "Borrowed";
	}
	else if (status == RESERVED) {
		return "Reserved";
	}
	else {
		return "Lost";
	}
}

//Convert reservationStatus to string
string reservationStatusToString(ReservationStatus status) {
	if (status == PENDING) {
		return "Pending";
	}
	else if (status == FULFILLED) {
		return "Fulfilled";
	}
	else {
		return "Cancelled";
	}
}

bool findMemberByID(const string& id, int& index) {
	for (size_t i = 0; i < members.size(); i++) {
		if (members[i].memberID == id) {
			index = i;
			return true;
		}
	}
	return false;
}

// =========================== File Operations ===========================
void loadMembers() {

}
void saveMembers() {

}
void loadBooks() {

}
void saveBooks() {

}
void loadBorrowRecords() {

}
void saveBorrowRecords() {

}
void loadReservations() {

}
void saveReservations() {

}
void loadAllData() {
	loadMembers();
	loadBooks();
	loadBorrowRecords();
	loadReservations();
	cout << "Data loaded successfully.\n";
}
void saveAllData() {
	saveMembers();
	saveBooks();
	saveBorrowRecords();
	saveReservations();
	cout << "Data saved successfully.\n";
}
void initialMonthlyStats() {
	for (int i = 0; i < 12; i++) {
		monthlyStats[i].month = i + 1;
		monthlyStats[i].booksBorrowed = 0;
		monthlyStats[i].booksReturned = 0;
		monthlyStats[i].finesCollected = 0.0;
		monthlyStats[i].reservationsMade = 0;
	}
}