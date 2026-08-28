//LibrarySystem.cpp - Implementation of Shared Functions

#include "LibrarySystem.h"
#include<vector>
#include <sstream>
#include <cstring>

// =========================== Golbal Variables ===========================
vector<Member> members;
vector<Book> books;
vector<BorrowRecord> borrowRecords;
vector<Reservation> reservations;
vector<MonthlyStatistics> monthlyStats(12);

// =========================== Helper Functions ===========================
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
	for (int m = 0; m < m1 -1; m++)
		total1 += daysInMonth[m];
	total1 += d1;

	int total2 = 0;
	for (int y = 0; y < y2; y++)
		total2 += 365;
	for (int m = 0; m < m2 - 1; m++)
		total2 += daysInMonth[m];
	total2 += d2;

	return total1 - total2;
}

// Convert MembershipType to string
string membershipTypeToString(MembershipType type) {
	switch (type) {
	case STUDENT: 
		return "STUDENT";
	case STAFF:   
		return "STAFF";
	case PUBLIC:  
		return "PUBLIC";
	default:      
		return "UNKNOWN";
	}
}

//Convert MembershipType to string
MembershipType stringToMembershipType(const string& str) {
	string s = str;
	for (auto& c : s) 
		c = toupper(c);
	if (s == "STUDENT") 
		return STUDENT;
	if (s == "STAFF") 
		return STAFF;
	return PUBLIC;
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

// =========================== Find Functions ===========================
bool findMemberByID(const string& id, int& index) {
	for (size_t i = 0; i < members.size(); i++) {
		if (members[i].memberID == id) {
			index = i;
			return true;
		}
	}
	return false;
}

bool findBookByID(const string& id, int& index) {
	for (size_t i = 0; i < books.size(); i++) {
		if (books[i].bookID == id) {
			index = i;
			return true;
		}
	}
	return false;
}

bool findBorrowRecordByID(const string& id, int& index) {
	for (size_t i = 0; i < borrowRecords.size(); i++) {
		if (borrowRecords[i].recordID == id) {
			index = i;
			return true;
		}
	}
	return false;
}

bool findReservationByBookAndMember(const string& bookID, const string& memberID, int& index) {
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].bookID == bookID &&
			reservations[i].memberID == memberID &&
			reservations[i].status == PENDING) {
			index = i;
			return true;
		}
	}
	return false;
}

// =========================== File Operations ===========================
void loadMembers() {
	members.clear();
	ifstream file("data/members.txt");
	if (!file.is_open())
		return;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string id, name, contact, pwd, typeStr;
		int borrowCount;
		bool isActive;

		getline(ss, id, '|');
		getline(ss, name, '|');
		getline(ss, contact, '|');
		getline(ss, pwd, '|');
		getline(ss, typeStr, '|');
		ss >> borrowCount;
		ss.ignore();
		ss >> isActive;

		MembershipType type = stringToMembershipType(typeStr);
		Member m;                    
		m.memberID = id;
		m.name = name;
		m.contact = contact;
		m.password = pwd;
		m.membershipType = type;
		m.borrowedCount = borrowCount;
		m.isActive = isActive;
		members.push_back(m);
	}
	file.close();
}
void saveMembers() {
	ofstream file("data/members.txt");
	if (!file.is_open()) 
		return;

	for (const auto& m : members) {
		file << m.memberID << "|"
			<< m.name << "|"
			<< m.contact << "|"
			<< m.password << "|"
			<< membershipTypeToString(m.membershipType) << "|"
			<< m.borrowedCount << "|"
			<< m.isActive << "\n";
	}
	file.close();
}
void loadBooks() {
	books.clear();
	ifstream file("data/books.txt");
	if (!file.is_open()) 
		return;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string id, title, author, category;
		int total, available;
		int statusInt;

		getline(ss, id, '|');
		getline(ss, title, '|');
		getline(ss, author, '|');
		getline(ss, category, '|');
		ss >> total;
		ss.ignore();
		ss >> available;
		ss.ignore();
		ss >> statusInt;

		Book b;
		b.bookID = id;
		b.title = title;
		b.author = author;
		b.category = category;
		b.totalCopies = total;
		b.availableCopies = available;
		b.status = static_cast<BookStatus>(statusInt);
		books.push_back(b);
	}
	file.close();
}
void saveBooks() {
	ofstream file("data/books.txt");
	if (!file.is_open()) 
		return;

	for (const auto& b : books) {
		file << b.bookID << "|"
			<< b.title << "|"
			<< b.author << "|"
			<< b.category << "|"
			<< b.totalCopies << "|"
			<< b.availableCopies << "|"
			<< static_cast<int>(b.status) << "\n";
	}
	file.close();
}
void loadBorrowRecords() {
	borrowRecords.clear();
	ifstream file("data/borrowRecords.txt");
	if (!file.is_open()) 
		return;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string id, memberID, bookID, borrowDate, dueDate, returnDate;
		bool isReturned;
		double fine;
		int renewalCount;

		getline(ss, id, '|');
		getline(ss, memberID, '|');
		getline(ss, bookID, '|');
		getline(ss, borrowDate, '|');
		getline(ss, dueDate, '|');
		getline(ss, returnDate, '|');
		ss >> isReturned;
		ss.ignore();
		ss >> fine;
		ss.ignore();
		ss >> renewalCount;

		BorrowRecord br;
		br.recordID = id;
		br.memberID = memberID;
		br.bookID = bookID;
		br.borrowDate = borrowDate;
		br.dueDate = dueDate;
		br.returnDate = returnDate;
		br.isReturned = isReturned;
		br.fineAmount = fine;
		br.renewalCount = renewalCount;
		borrowRecords.push_back(br);
	}
	file.close();
}
void saveBorrowRecords() {
	ofstream file("data/borrowRecords.txt");
	if (!file.is_open()) 
		return;

	for (const auto& br : borrowRecords) {
		file << br.recordID << "|"
			<< br.memberID << "|"
			<< br.bookID << "|"
			<< br.borrowDate << "|"
			<< br.dueDate << "|"
			<< br.returnDate << "|"
			<< br.isReturned << "|"
			<< br.fineAmount << "|"
			<< br.renewalCount << "\n";
	}
	file.close();
}
void loadReservations() {
	reservations.clear();
	ifstream file("data/reservations.txt");
	if (!file.is_open()) 
		return;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string id, bookID, memberID, date;
		int statusInt;

		getline(ss, id, '|');
		getline(ss, bookID, '|');
		getline(ss, memberID, '|');
		getline(ss, date, '|');
		ss >> statusInt;

		Reservation r;
		r.reservationID = id;
		r.bookID = bookID;
		r.memberID = memberID;
		r.reservationDate = date;
		r.status = static_cast<ReservationStatus>(statusInt);
		reservations.push_back(r);
	}
	file.close();
}
void saveReservations() {
	ofstream file("data/reservations.txt");
	if (!file.is_open()) 
		return;

	for (const auto& r : reservations) {
		file << r.reservationID << "|"
			<< r.bookID << "|"
			<< r.memberID << "|"
			<< r.reservationDate << "|"
			<< static_cast<int>(r.status) << "\n";
	}
	file.close();
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
void initializeMonthlyStats() {
	for (int i = 0; i < 12; i++) {
		monthlyStats[i].month = i + 1;
		monthlyStats[i].booksBorrowed = 0;
		monthlyStats[i].booksReturned = 0;
		monthlyStats[i].finesCollected = 0.0;
		monthlyStats[i].reservationsMade = 0;
	}
}