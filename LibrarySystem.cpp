//LibrarySystem.cpp - Implementation of Shared Functions
//Shared by all modules

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

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

//Generate unique ID
string generateID(const string& prefix, int number) {
	stringstream ss;
	ss << prefix << setw(3) << setfill('0') << number;
	return ss.str();
}

//Get current date as string (DD/MM/YYYY)
string getCurrentDate() {
	time_t now = time(nullptr);
	tm localTime = {};

	localtime_s(&localTime, &now);

	stringstream ss;
	ss << setw(2) << setfill('0') << localTime.tm_mday << "/"
		<< setw(2) << setfill('0') << (localTime.tm_mon + 1) << "/"
		<< (localTime.tm_year + 1900);

	return ss.str();
}

//Get date offset by days (DD/MM/YYYY)
string getDateFromDays(int daysOffset) {
	time_t now = time(nullptr);
	now += daysOffset * 24 * 60 * 60;
	tm localTime = {};

	localtime_s(&localTime, &now);

	stringstream ss;
	ss << setw(2) << setfill('0') << localTime.tm_mday << "/"
		<< setw(2) << setfill('0') << (localTime.tm_mon + 1) << "/"
		<< (localTime.tm_year + 1900);

	return ss.str();
}

//Calculate days differnt between two dates
int calculateDaysDifference(const string& date1, const string& date2) {

	int d1 = stoi(date1.substr(0, 2));
	int m1 = stoi(date1.substr(3, 2));
	int y1 = stoi(date1.substr(6, 4));

	int d2 = stoi(date2.substr(0, 2));
	int m2 = stoi(date2.substr(3, 2));
	int y2 = stoi(date2.substr(6, 4));

	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int total1 = 0;
	for (int y = 0; y < y1; y++) 
		total1 += 365;
	for (int m = 0; m < m1 - 1; m++) 
		total1 += daysInMonth[m];
	total1 += d1;

	int total2 = 0;

	for (int y = 0; y < y2; y++) total2 += 365;
	for (int m = 0; m < m2 - 1; m++) 
		total2 += daysInMonth[m];
	total2 += d2;

	//Return date2 - date1 (positive = date2 is later)
	return total2 - total1;
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

//Calculate fine amount
double calculateFine(const string& dueDate, const string& returnDate) {

	int daysLate = calculateDaysDifference(dueDate,returnDate);

	if (daysLate <= 0)
		return 0.0;
	return daysLate * FINE_PER_DAY;
}

// =========================== Id Generation Functions ===========================
string getNextBookId() {
	int maxNum = 0;
	for (const auto& b : books) {
		if (b.bookID.length() > 1) {
			string numPart = b.bookID.substr(1);
			bool isDigit = true;
			for (char c : numPart) {
				if (!isdigit(c)) { isDigit = false; break; }
			}
			if (isDigit) {
				int num = stoi(numPart);
				if (num > maxNum) maxNum = num;
			}
		}
	}
	return generateID("B", maxNum + 1);
}

string getNextMemberId() {
	int maxNum = 0;
	for (const auto& m : members) {
		if (m.memberID.length() > 1) {
			string numPart = m.memberID.substr(1);
			bool isDigit = true;
			for (char c : numPart) {
				if (!isdigit(c)) { isDigit = false; break; }
			}
			if (isDigit) {
				int num = stoi(numPart);
				if (num > maxNum) maxNum = num;
			}
		}
	}
	return generateID("M", maxNum + 1);
}

string getNextBorrowRecordId() {
	int maxNum = 0;
	for (const auto& br : borrowRecords) {
		if (br.recordID.length() > 2) { 
			string numPart = br.recordID.substr(2);  
			bool isDigit = true;
			for (char c : numPart) {
				if (!isdigit(c)) {
					isDigit = false;
					break;
				}
			}
			if (isDigit) {
				int num = stoi(numPart);
				if (num > maxNum) maxNum = num;
			}
		}
	}
	return generateID("BR", maxNum + 1);
}

string getNextReservationId() {
	int maxNum = 0;
	for (const auto& r : reservations) {
		if (r.reservationID.length() > 2) {  
			string numPart = r.reservationID.substr(2);  
			bool isDigit = true;
			for (char c : numPart) {
				if (!isdigit(c)) { isDigit = false; break; }
			}
			if (isDigit) {
				int num = stoi(numPart);
				if (num > maxNum) maxNum = num;
			}
		}
	}
	return generateID("RV", maxNum + 1);
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
		int rewardPoints;          
		int bonusBorrowLimit;

		getline(ss, id, '|');
		getline(ss, name, '|');
		getline(ss, contact, '|');
		getline(ss, pwd, '|');
		getline(ss, typeStr, '|');
		ss >> borrowCount;
		ss.ignore();
		ss >> isActive;
		ss.ignore();
		ss >> rewardPoints;      
		ss.ignore();
		ss >> bonusBorrowLimit;

		MembershipType type = stringToMembershipType(typeStr);
		Member m;
		m.memberID = id;
		m.name = name;
		m.contact = contact;
		m.password = pwd;
		m.membershipType = type;
		m.borrowedCount = borrowCount;
		m.isActive = isActive;
		m.rewardPoints = rewardPoints;        
		m.bonusBorrowLimit = bonusBorrowLimit;
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
			<< m.isActive << "|"
			<< m.rewardPoints << "|"         
			<< m.bonusBorrowLimit << "\n";
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

void saveMonthlyStats() {
	ofstream file("data/monthlyStats.txt");
	if (!file.is_open()) {
		cout << "Warning: Could not save monthly statistics.\n";
		return;
	}

	for (int i = 0; i < 12; i++) {
		file << monthlyStats[i].month << "|"
			<< monthlyStats[i].booksBorrowed << "|"
			<< monthlyStats[i].booksReturned << "|"
			<< monthlyStats[i].finesCollected << "|"
			<< monthlyStats[i].reservationsMade << "\n";
	}
	file.close();
}

void loadMonthlyStats() {
	ifstream file("data/monthlyStats.txt");
	if (!file.is_open()) {
		initializeMonthlyStats();
		return;
	}

	string line;
	int i = 0;
	while (getline(file, line) && i < 12) {
		stringstream ss(line);
		string month, borrowed, returned, fines, reservations;

		getline(ss, month, '|');
		getline(ss, borrowed, '|');
		getline(ss, returned, '|');
		getline(ss, fines, '|');
		getline(ss, reservations, '|');

		monthlyStats[i].month = stoi(month);
		monthlyStats[i].booksBorrowed = stoi(borrowed);
		monthlyStats[i].booksReturned = stoi(returned);
		monthlyStats[i].finesCollected = stod(fines);
		monthlyStats[i].reservationsMade = stoi(reservations);
		i++;
	}
	file.close();
}

void loadAllData() {
	loadMembers();
	loadBooks();
	loadBorrowRecords();
	loadReservations();
	loadMonthlyStats();
	cout << "Data loaded successfully.\n";
}
void saveAllData() {
	saveMembers();
	saveBooks();
	saveBorrowRecords();
	saveReservations();
	saveMonthlyStats();
	cout << "Data saved successfully.\n";
}
void initializeMonthlyStats() {
	bool hasData = false;
	for (int i = 0; i < 12; i++) {
		if (monthlyStats[i].booksBorrowed > 0 ||
			monthlyStats[i].booksReturned > 0 ||
			monthlyStats[i].finesCollected > 0 ||
			monthlyStats[i].reservationsMade > 0) {
			hasData = true;
			break;
		}
	}

	if (!hasData) {
		for (int i = 0; i < 12; i++) {
			monthlyStats[i].month = i + 1;
			monthlyStats[i].booksBorrowed = 0;
			monthlyStats[i].booksReturned = 0;
			monthlyStats[i].finesCollected = 0.0;
			monthlyStats[i].reservationsMade = 0;
		}
	}
}