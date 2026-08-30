//ReservationManagement.cpp - Reservation & Renewal Management Module
//Team Member: LIM ZHI YUAN (25WMD04162)

#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

// =========================== Function Prototypes ===========================
void reserveBook();
void renewBorrowing();
void viewOverdueList();
void cancelReservation();
void viewMemberReservations();
bool canMemberReserve(const string& memberID);
bool isBookReservable(const string& bookID, string& errorMsg);
bool canRenew(const string& recordID, string& errorMsg);

// =========================== Main Menu ===========================
void reservationManagementMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n================================================\n";
        cout << "        RESERVATION & RENEWAL MANAGEMENT";
        cout << "\n================================================\n";
        cout << endl;
        cout << "   1. Reserve a Book\n";
        cout << "   2. Renew a Borrowed Book\n";
        cout << "   3. View Overdue List\n";
        cout << "   4. Cancel a Reservation\n";
        cout << "   5. View My Reservations\n";
        cout << "   6. Back to Main Menu\n";
        cout << "\n================================================\n";
        cout << "Enter choice (1-6): ";

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
            reserveBook();
            break;
        case 2:
            renewBorrowing();
            break;
        case 3:
            viewOverdueList();
            break;
        case 4:
            cancelReservation();
            break;
        case 5:
            viewMemberReservations();
            break;
        case 6:
            cout << "Returning to main menu...\n";
            pause();
            break;
        default:
            cout << "Invalid choice! Please enter 1-6.\n";
            cin.ignore(1000, '\n');
            pause();
        }
    } while (choice != 6);
}

// =========================== 1. Reserve a Book ===========================
void reserveBook() {
    cout << "\n=====================================================================\n";
    cout << "                           RESERVE A BOOK";
    cout << "\n=====================================================================\n";

    string memberID;
    cout << "Enter Member ID: ";
    cin >> memberID;
    for (auto& c : memberID)
        c = toupper(c);

    int memberIndex;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member not found!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    if (!members[memberIndex].isActive) {
        cout << "Error: This member account is INACTIVE!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    if (!canMemberReserve(memberID)) {
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string bookID;
    cout << "Enter Book ID: ";
    cin >> bookID;
    for (auto& c : bookID)
        c = toupper(c);
    int bookIndex;
    if (!findBookByID(bookID, bookIndex)) {
        cout << "Error: Book not found!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string errorMsg;
    if (!isBookReservable(bookID, errorMsg)) {
        cout << "Error: " << errorMsg << endl;
        cin.ignore(1000, '\n');
        pause();
        return;
    }
    for (const auto& br : borrowRecords) {
        if (br.memberID == memberID && br.bookID == bookID && !br.isReturned) {
            cout << "Error: You are currently borrowing this book!\n";
            cout << "You cannot reserve a book you already have.\n";
            cin.ignore(1000, '\n');
            pause();
            return;
        }
    }

    int existingIndex;
    if (findReservationByBookAndMember(bookID, memberID, existingIndex)) {
        cout << "Error: You already have a pending reservation for this book!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    Reservation newRes;
    newRes.reservationID = getNextReservationId();
    newRes.bookID = bookID;
    newRes.memberID = memberID;
    newRes.reservationDate = getCurrentDate();
    newRes.status = PENDING;

    reservations.push_back(newRes);

    // Update book status if all copies are borrowed
    if (books[bookIndex].availableCopies == 0) {
        books[bookIndex].status = RESERVED;
    }

    // Update monthly statistics
    string currentDate = getCurrentDate();
    int month = stoi(currentDate.substr(3, 2)) - 1;
    if (month >= 0 && month < 12) {
        monthlyStats[month].reservationsMade++;
    }

    saveReservations();
    saveBooks();

    cout << "\n=====================================================================\n";
    cout << "                        RESERVATION SUCCESSFUL!";
    cout << "\n=====================================================================\n";
    cout << "Reservation ID: " << newRes.reservationID << endl;
    cout << "Book Title: " << books[bookIndex].title << endl;
    cout << "Reservation Date: " << newRes.reservationDate << endl;
    cout << "Status: PENDING\n";
    cout << "You will be notified when the book becomes available.\n";
    cout << "\n=====================================================================\n";
    cin.ignore(1000, '\n');
    pause();
}

// =========================== 2. Renew a Borrowed Book ===========================
void renewBorrowing() {
    cout << "\n=============================================================================\n";
    cout << "                            RENEW A BORROWED BOOK";
    cout << "\n=============================================================================\n";

    string memberID;
    cout << "Enter Member ID: ";
    cin >> memberID;
    for (auto& c : memberID) c = toupper(c);

    int memberIndex;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member not found!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    cout << "\n--- Books Currently Borrowed by " << members[memberIndex].name << " ---\n";
    cout << left << setw(12) << "Record ID"
        << setw(10) << "Book ID"
        << setw(35) << "Title"
        << setw(15) << "Due Date" << endl;
    cout << string(72, '-') << endl;

    vector<int> borrowIndices;
    for (size_t i = 0; i < borrowRecords.size(); i++) {
        if (borrowRecords[i].memberID == memberID && !borrowRecords[i].isReturned) {
            borrowIndices.push_back(i);
            int bookIndex;
            string bookTitle = "Unknown";
            if (findBookByID(borrowRecords[i].bookID, bookIndex)) {
                bookTitle = books[bookIndex].title;
            }
            cout << left << setw(12) << borrowRecords[i].recordID
                << setw(10) << borrowRecords[i].bookID
                << setw(35) << bookTitle
                << setw(15) << borrowRecords[i].dueDate << endl;
        }
    }

    if (borrowIndices.empty()) {
        cout << "No books currently borrowed by this member.\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string recordID;
    cout << "Enter Borrow Record ID: ";
    cin >> recordID;
    for (auto& c : recordID)
        c = toupper(c);
    int recordIndex = -1;
    for (int idx : borrowIndices) {
        if (borrowRecords[idx].recordID == recordID) {
            recordIndex = idx;
            break;
        }
    }

    if (recordIndex == -1) {
        cout << "Error: Invalid Record ID!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    BorrowRecord& br = borrowRecords[recordIndex];

    if (br.isReturned) {
        cout << "Error: This book has already been returned.\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string errorMsg;
    if (!canRenew(recordID, errorMsg)) {
        cout << "Error: " << errorMsg << endl;
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    // Perform renewal
    br.renewalCount++;
    br.dueDate = getDateFromDays(BORROW_DURATION_DAYS);

    // Update monthly statistics
    string currentDate = getCurrentDate();
    int month = stoi(currentDate.substr(3, 2)) - 1;
    if (month >= 0 && month < 12) {
        monthlyStats[month].booksBorrowed++;
    }

    saveBorrowRecords();

    cout << "\n=============================================================================\n";
    cout << "                               RENEWAL SUCCESSFUL!";
    cout << "\n=============================================================================\n";
    cout << "Record ID: " << br.recordID << endl;
    cout << "New Due Date: " << br.dueDate << endl;
    cout << "Renewal Count: " << br.renewalCount << " / " << MAX_RENEWALS << endl;
    cout << "\n=============================================================================\n";
    cin.ignore(1000, '\n');
    pause();
}

// =========================== 3. View Overdue List ===========================
void viewOverdueList() {
    cout << "\n=====================================================================================================\n";
    cout << "                                      OVERDUE BOOKS LIST";
    cout << "\n=====================================================================================================\n";

    string currentDate = getCurrentDate();
    bool found = false;

    cout << left << setw(12) << "Member ID"
        << setw(25) << "Member Name"
        << setw(10) << "Book ID"
        << setw(20) << "Book Title"
        << setw(12) << "Due Date"
        << setw(10) << "Days Late"
        << setw(12) << "Fine (RM)" << endl;
    cout << string(101, '-') << endl;

    for (const auto& br : borrowRecords) {
        if (br.isReturned) continue;

        int daysLate = calculateDaysDifference(br.dueDate,currentDate);

        if (daysLate > 0) {
            found = true;

            int memberIndex;
            string memberName = "Unknown";
            if (findMemberByID(br.memberID, memberIndex)) {
                memberName = members[memberIndex].name;
            }

            int bookIndex;
            string bookTitle = "Unknown";
            if (findBookByID(br.bookID, bookIndex)) {
                bookTitle = books[bookIndex].title;
            }

            double fine = daysLate * FINE_PER_DAY;

            cout << left << setw(12) << br.memberID
                << setw(25) << memberName
                << setw(10) << br.bookID
                << setw(20) << bookTitle
                << setw(12) << br.dueDate
                << setw(10) << daysLate
                << setw(12) << fixed << setprecision(2) << fine << endl;
        }
    }

    if (!found) {
        cout << "\nNo overdue books found. All books are on time!\n";
    }

    cout << "\n=====================================================================================================\n";
    cin.ignore(1000, '\n');
    pause();
}

// =========================== 4. Cancel a Reservation ===========================
void cancelReservation() {
    cout << "\n=========================================================================================\n";
    cout << "                                   CANCEL RESERVATION";
    cout << "\n=========================================================================================\n";

    string memberID;
    cout << "Enter Member ID: ";
    cin >> memberID;
    for (auto& c : memberID)
        c = toupper(c);

    int memberIndex;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member not found!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    cout << "\n--- Reservations by " << members[memberIndex].name << " ---\n";
    cout << left << setw(14) << "Reservation ID"
        << setw(10) << "Book ID"
        << setw(35) << "Book Title"
        << setw(14) << "Date"
        << setw(12) << "Status" << endl;
    cout << string(85, '-') << endl;

    vector<int> reservationIndices;
    for (size_t i = 0; i < reservations.size(); i++) {
        if (reservations[i].memberID == memberID) {
            reservationIndices.push_back(i);
            string bookTitle = "Unknown";
            int bookIndex;
            if (findBookByID(reservations[i].bookID, bookIndex)) {
                bookTitle = books[bookIndex].title;
            }
            cout << left << setw(14) << reservations[i].reservationID
                << setw(10) << reservations[i].bookID
                << setw(35) << bookTitle
                << setw(14) << reservations[i].reservationDate
                << setw(12) << reservationStatusToString(reservations[i].status) << endl;
        }
    }

    if (reservationIndices.empty()) {
        cout << "No reservations found for this member.\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string reservationID;
    cout << "\nEnter Reservation ID to cancel: ";
    cin >> reservationID;
    for (auto& c : reservationID) 
        c = toupper(c);

    int reservationIndex = -1;
    for (int idx : reservationIndices) {
        if (reservations[idx].reservationID == reservationID && reservations[idx].status == PENDING) {
            reservationIndex = idx;
            break;
        }
    }

    if (reservationIndex == -1) {
        cout << "Error: No pending reservation found for this book.\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    char confirm;
    cout << "Are you sure you want to cancel this reservation? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) != 'y') {
        cout << "Cancellation aborted.\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    string bookID = reservations[reservationIndex].bookID;
    reservations[reservationIndex].status = CANCELLED;

    // Check if there are any other pending reservations for this book
    bool hasOtherReservations = false;
    for (const auto& r : reservations) {
        if (r.bookID == bookID && r.status == PENDING) {
            hasOtherReservations = true;
            break;
        }
    }

    if (!hasOtherReservations) {
        int bookIndex;
        if (findBookByID(bookID, bookIndex)) {
            if (books[bookIndex].availableCopies == 0) {
                books[bookIndex].status = BORROWED;
            }
        }
    }

    saveReservations();
    saveBooks();

    cout << "\n=========================================================================================\n";
    cout << "                             RESERVATION CANCELLED SUCCESSFULLY";
    cout << "\n=========================================================================================\n";
    cout << "Reservation for book " << bookID << " has been cancelled.\n";
    cout << "\n=========================================================================================\n";
    cin.ignore(1000, '\n');
    pause();
}

// =========================== 5. View Member Reservations ===========================
void viewMemberReservations() {
    cout << "\n===================================================================================\n";
    cout << "                                  MY RESERVATIONS";
    cout << "\n===================================================================================\n";

    string memberID;
    cout << "Enter Member ID: ";
    cin >> memberID;
    for (auto& c : memberID)
        c = toupper(c);

    int memberIndex;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member not found!\n";
        cin.ignore(1000, '\n');
        pause();
        return;
    }

    bool found = false;
    cout << left << setw(14) << "Reservation ID"
        << setw(10) << "Book ID"
        << setw(35) << "Book Title"
        << setw(14) << "Date"
        << setw(12) << "Status" << endl;
    cout << string(85, '-') << endl;

    for (const auto& r : reservations) {
        if (r.memberID != memberID) continue;

        found = true;
        string bookTitle = "Unknown";
        int bookIndex;
        if (findBookByID(r.bookID, bookIndex)) {
            bookTitle = books[bookIndex].title;
        }

        cout << left << setw(14) << r.reservationID
            << setw(10) << r.bookID
            << setw(35) << bookTitle
            << setw(14) << r.reservationDate
            << setw(12) << reservationStatusToString(r.status) << endl;
    }

    if (!found) {
        cout << "\nNo reservations found for this member.\n";
    }

    cout << "\n===================================================================================\n";
    cin.ignore(1000, '\n');
    pause();
}

// =========================== Helper Functions ===========================

bool canMemberReserve(const string& memberID) {
    string currentDate = getCurrentDate();
    int overdueCount = 0;

    for (const auto& br : borrowRecords) {
        if (br.memberID == memberID && !br.isReturned) {
            int daysLate = calculateDaysDifference(br.dueDate,currentDate);
            if (daysLate > 0) {
                overdueCount++;
            }
        }
    }

    if (overdueCount > 0) {
        cout << "Error: Member has " << overdueCount << " overdue book(s). "
            << "Please return all overdue books before making a reservation.\n";
        return false;
    }

    int activeBorrows = 0;
    for (const auto& br : borrowRecords) {
        if (br.memberID == memberID && !br.isReturned) {
            activeBorrows++;
        }
    }

    // Check if member has reached borrowing limit (including bonus limit from Member struct)
    int memberIndex;
    int allowedLimit = MAX_BORROW_LIMIT;
    if (findMemberByID(memberID, memberIndex)) {
        allowedLimit = MAX_BORROW_LIMIT + members[memberIndex].bonusBorrowLimit;
    }

    if (activeBorrows >= allowedLimit) {
        cout << "Error: Member has reached maximum borrowing limit ("
            << allowedLimit << " books).\n";
        return false;
    }

    return true;
}

bool isBookReservable(const string& bookID, string& errorMsg) {
    int bookIndex;
    if (!findBookByID(bookID, bookIndex)) {
        errorMsg = "Book not found.";
        return false;
    }

    const Book& b = books[bookIndex];

    if (b.status == LOST) {
        errorMsg = "This book is marked as LOST and cannot be reserved.";
        return false;
    }

    if (b.availableCopies > 0) {
        errorMsg = "This book is currently available. Please borrow it directly.";
        return false;
    }

    if (b.totalCopies == 0) {
        errorMsg = "This book has no copies in the library.";
        return false;
    }

    return true;
}

bool canRenew(const string& recordID, string& errorMsg) {
    int recordIndex;
    if (!findBorrowRecordByID(recordID, recordIndex)) {
        errorMsg = "Borrow record not found.";
        return false;
    }

    const BorrowRecord& br = borrowRecords[recordIndex];

    if (br.isReturned) {
        errorMsg = "This book has already been returned.";
        return false;
    }

    if (br.renewalCount >= MAX_RENEWALS) {
        errorMsg = "Maximum renewal limit (" + to_string(MAX_RENEWALS) + ") reached.";
        return false;
    }

    string currentDate = getCurrentDate();
    int daysLate = calculateDaysDifference(br.dueDate,currentDate);
    if (daysLate > 0) {
        errorMsg = "This book is overdue by " + to_string(daysLate) +
            " days. Please return it first.";
        return false;
    }

    // Check if there are pending reservations for this book
    for (const auto& r : reservations) {
        if (r.bookID == br.bookID && r.status == PENDING) {
            errorMsg = "This book has pending reservations. Cannot renew.";
            return false;
        }
    }

    return true;
}

// =========================== Process Reservation Queue ===========================
// This function is called by returnBook() in Book Management module
void processReservationQueue(const string& bookID) {
    // FIFO: Get first pending reservation for this book
    int firstReservationIndex = -1;
    for (size_t i = 0; i < reservations.size(); i++) {
        if (reservations[i].bookID == bookID &&
            reservations[i].status == PENDING) {
            firstReservationIndex = i;
            break;
        }
    }

    if (firstReservationIndex == -1) {
        return;
    }

    int bookIndex;
    if (!findBookByID(bookID, bookIndex)) {
        return;
    }

    // Mark reservation as fulfilled
    Reservation& r = reservations[firstReservationIndex];
    r.status = FULFILLED;

    // Create a borrow record for the first person in queue
    BorrowRecord newBr;
    newBr.recordID = getNextBorrowRecordId();
    newBr.memberID = r.memberID;
    newBr.bookID = bookID;
    newBr.borrowDate = getCurrentDate();
    newBr.dueDate = getDateFromDays(BORROW_DURATION_DAYS);
    newBr.returnDate = "-";
    newBr.isReturned = false;
    newBr.fineAmount = 0.0;
    newBr.renewalCount = 0;

    borrowRecords.push_back(newBr);

    // Update member's borrowed count
    int memberIndex;
    if (findMemberByID(r.memberID, memberIndex)) {
        members[memberIndex].borrowedCount++;
    }

    // Decrease available copies (since this copy is now borrowed)
    books[bookIndex].availableCopies--;
    if (books[bookIndex].availableCopies == 0) {
        books[bookIndex].status = BORROWED;
    }

    // Update monthly statistics
    string currentDate = getCurrentDate();
    int month = stoi(currentDate.substr(3, 2)) - 1;
    if (month >= 0 && month < 12) {
        monthlyStats[month].booksBorrowed++;
        monthlyStats[month].reservationsMade++;
    }

    saveReservations();
    saveBorrowRecords();
    saveBooks();

    cout << "\n================================================\n";
    cout << "     RESERVATION QUEUE PROCESSED";
    cout << "\n================================================\n";
    cout << "Book has been automatically issued to: " << r.memberID << endl;
    cout << "New Borrow Record ID: " << newBr.recordID << endl;
    cout << "Due Date: " << newBr.dueDate << endl;
    cout << "================================================\n";
}