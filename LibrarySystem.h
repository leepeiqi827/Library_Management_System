//LibrarySystem.h - Shared Data Structures & Constants

#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <regex>
using namespace std;

// =========================== Named Constans ===========================
const int MAX_BORROW_LIMIT = 5;
const double FINE_PER_DAY = 1.50;
const int MAX_RENEWALS = 2;
const int BORROW_DURATION_DAYS = 14;
const int MAX_DAYS_LATE = 30;

// =========================== Enumeration Types ===========================
enum MembershipType { STUDENT, STAFF, PUBLIC };
enum BookStatus { AVAILABLE, BORROWED, RESERVED, LOST };
enum ReservationStatus { PENDING, FULFILLED, CANCELLED };

// =========================== Structure ===========================
struct Member {
	string memberID;
	string name;
	string contact;
	string password;
	MembershipType membershipType;
	int borrowedCount;
	bool isActive;
};

struct Book {
	string bookID;
	string title;
	string author;
	string category;
	int totalCopies;
	int availableCopies;
	BookStatus status;
};

struct BorrowRecord {
	string recordID;
	string memberID;
	string bookID;
	string borrowDate;
	string dueDate;
	string returnDate;
	bool isReturned;
	double fineAmount;
	int renewalCount;
};

struct Reservation {
	string reservationID;
	string bookID;
	string memberID;
	string reservationDate;
	ReservationStatus status;
};

struct MonthlyStatistics {
	int month;
	int booksBorrowed;
	int booksReturned;
	double finesCollected;
	int reservationsMade;
};

// =========================== Golbal Variables ===========================
extern vector<Member> members;
extern vector<Book> books;
extern vector<BorrowRecord> borrowRecords;
extern vector<Reservation> reservations;
extern vector<MonthlyStatistics> monthlyStats;

//Shared helper functions (used by multiple modules)
void pause();
string generateID(const string& prefix, int number);
string getCurrentDate();
string getDateFromDays(int daysOffset);
int calculateDaysDifference(const string& date1, const string& date2);
string membershipTypeToString(MembershipType type);
MembershipType stringToMembershipType(const string& str);
string bookStatusToString(BookStatus status);
string reservationStatusToString(ReservationStatus status);

//Find Function
bool findMemberByID(const string& id, int& index);
bool findBookByID(const string& id, int& index);
bool findBorrowRecordByID(const string& id, int& index);
bool findReservationByBookAndMember(const string& bookID, const string& memberID, int& index);

//File operation function Prototypes
void loadAllData();
void saveAllData();
void loadMembers();
void saveMembers();
void loadBooks();
void saveBooks();
void loadBorrowRecords();
void saveBorrowRecords();
void loadReservations();
void saveReservations();

//Module Menu Function
void memberManagementMenu();
void bookManagementMenu();
void reservationManagementMenu();
void paymentReportingMenu();

#endif
