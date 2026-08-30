//BookManagement.cpp - Book Management & Borrow/Return Module
//Team Member: NEOH E CHYN (25WMD04015)

#include "LibrarySystem.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

//Function prototypes
void bookManagementMenu();
void addBook();
void updateBook();
void searchBook();
void borrowBook();
void returnBook();
void recommendBooks();
void rewardPointsMenu();

//Helper functions for safely reading input
static string getLineInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    //Trim spaces
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}
// Display book management menu
void bookManagementMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n================================================\n";
        cout << "\t\tBook Management";
        cout << "\n================================================\n\n";
        cout << "   1. Add New Book Title\n";
        cout << "   2. Update Book Details\n";
        cout << "   3. Search Book (Title / Author / Category)\n";
        cout << "   4. Borrow Book\n";
        cout << "   5. Return Book\n";
        cout << "   6. Book Recommendation\n";
        cout << "   7. Reward Points & Redeem Limit\n";
        cout << "   8. Back to Main Menu\n";
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
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            updateBook();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            borrowBook();
            break;
        case 5:
            returnBook();
            break;
        case 6:
            recommendBooks();
            break;
        case 7:
            rewardPointsMenu();
            break;
        case 8:
            cout << "Returning to main menu...\n";
            pause();
            break;
        default:
            cout << "Invalid choice! Please enter 1-8.\n";
            pause();
        }
    } while (choice != 8);
}

//Add a new book to the catalog
void addBook() {
    cout << "\n================================================\n";
    cout << "\t\tAdd New Book";
    cout << "\n================================================\n";

    Book newBook;
    newBook.bookID = getNextBookId();

    newBook.title = getLineInput("Enter Book Title: ");
    if (newBook.title.empty()) {
        cout << "Title cannot be empty!\n";
        pause();
        return;
    }

    newBook.author = getLineInput("Enter Author Name: ");
    newBook.category = getLineInput("Enter Category [e.g., Fiction, Science, History, Fantasy, Romance, Mystery]: ");

    cout << "Enter Total Copies: ";
    cin >> newBook.totalCopies;
    while (cin.fail() || newBook.totalCopies <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid number! Enter total copies (> 0): ";
        cin >> newBook.totalCopies;
    }

    newBook.availableCopies = newBook.totalCopies;
    newBook.status = AVAILABLE;

    books.push_back(newBook);
    saveBooks();

    cout << "\nBook added successfully! Generated Book ID: " << newBook.bookID << endl;
    cin.ignore(1000, '\n');
    pause();
}

//Update existing book details
void updateBook() {
    cout << "\n================================================\n";
    cout << "\t\tUpdate Book Details";
    cout << "\n================================================\n";

    string bookID = getLineInput("Enter Book ID to update: ");
    for (auto& c : bookID)
        c = toupper(c);
    int targetIndex = -1;

    //Sequential search
    for (size_t i = 0; i < books.size(); i++) {
        if (books[i].bookID == bookID) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex != -1) {
        cout << "\nCurrent Details:\n";
        cout << "Title: " << books[targetIndex].title << "\nAuthor: " << books[targetIndex].author
            << "\nCategory: " << books[targetIndex].category
            << "\nTotal Copies: " << books[targetIndex].totalCopies << endl;

        cout << "\nEnter new details (leave blank to keep current):\n";
        string input = getLineInput("New Title: ");
        if (!input.empty()) books[targetIndex].title = input;

        input = getLineInput("New Author: ");
        if (!input.empty()) books[targetIndex].author = input;

        input = getLineInput("New Category: ");
        if (!input.empty()) books[targetIndex].category = input;

        bool validInput = false;
        while (!validInput) {
            string copiesInput = getLineInput("New Total Copies (leave blank to keep current): ");

            if (copiesInput.empty()) {
                validInput = true;
                break;
            }

            // Validate input is numeric
            bool isDigit = true;
            for (char c : copiesInput) {
                if (!isdigit(c)) {
                    isDigit = false;
                    break;
                }
            }

            if (isDigit) {
                int newCopies = stoi(copiesInput);
                if (newCopies >= 0) {
                    int copyDiff = newCopies - books[targetIndex].totalCopies;
                    books[targetIndex].totalCopies = newCopies;
                    books[targetIndex].availableCopies += copyDiff;
                    if (books[targetIndex].availableCopies < 0) {
                        books[targetIndex].availableCopies = 0;
                    }
                    cout << "Total copies updated successfully!\n";
                    validInput = true;  
                }
                else {
                    cout << "Invalid copy count! Please enter a positive number.\n";
                }
            }
            else {
                cout << "Invalid input! Please enter numbers only.\n";
            }
        }

        saveBooks();
        cout << "Book updated successfully!\n";
    }
    else {
        cout << "Error: Book ID not found!\n";
    }
    pause();
    
}

//Search books by title, author, or category
void searchBook() {
    cout << "\n================================================\n";
    cout << "\t\tSearch Book Catalog";
    cout << "\n================================================\n";

    string query = getLineInput("Enter keyword (Title / Author / Category): ");
    if (query.empty()) return;
    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    bool found = false;

    cout << "\n" << left << setw(8) << "ID"
        << setw(30) << "Title"
        << setw(20) << "Author"
        << setw(15) << "Category"
        << setw(10) << "Available"
        << setw(10) << "Status" << endl;
    cout << "==============================================================================================\n";

    //Sequential search 
    for (const auto& b : books) {
        string t = b.title, a = b.author, c = b.category;
        transform(t.begin(), t.end(), t.begin(), ::tolower);
        transform(a.begin(), a.end(), a.begin(), ::tolower);
        transform(c.begin(), c.end(), c.begin(), ::tolower);

        if (t.find(lowerQuery) != string::npos ||
            a.find(lowerQuery) != string::npos ||
            c.find(lowerQuery) != string::npos) {

            cout << left << setw(8) << b.bookID
                << setw(30) << b.title
                << setw(20) << b.author
                << setw(15) << b.category
                << setw(10) << b.availableCopies
                << setw(10) << bookStatusToString(b.status) << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No matching books found.\n";
    }
    pause();
}

//Borrow a book for a member
void borrowBook() {
    cout << "\n============================================================\n";
    cout << "\t\tBorrow Book";
    cout << "\n============================================================\n";

    if (books.empty()) {
        cout << "No books available.\n";
        pause();
        return;
    }

    cout << "\n--- Available Books ---\n";
    cout << left << setw(8) << "ID"
        << setw(35) << "Title"
        << setw(25) << "Author"
        << setw(10) << "Available" << endl;
    cout << string(78, '-') << endl;

    for (const auto& b : books) {
        if (b.availableCopies > 0) {
            cout << left << setw(8) << b.bookID
                << setw(35) << b.title
                << setw(25) << b.author
                << setw(10) << b.availableCopies << endl;
        }
    }

    string memberID = getLineInput("Enter Member ID: ");
    for (auto& c : memberID)
        c = toupper(c);
    int memberIndex = -1;

    //validation
    if (findMemberByID(memberID, memberIndex)) {
        cout << "\nMember: " << members[memberIndex].name << endl;
        cout << "Status: " << (members[memberIndex].isActive ? "ACTIVE" : "INACTIVE") << endl;
        cout << "Borrowed: " << members[memberIndex].borrowedCount << "/"
            << (MAX_BORROW_LIMIT + members[memberIndex].bonusBorrowLimit) << endl;

        if (!members[memberIndex].isActive) {
            cout << "\nThis member account is INACTIVE!\n";
            cout << "Please activate the account first (Member Management -> Option 7).\n";
            pause();
            return;
        }
        //Check borrowing limit
        int allowedLimit = MAX_BORROW_LIMIT + members[memberIndex].bonusBorrowLimit;
        if (members[memberIndex].borrowedCount < allowedLimit) {

            string bookID = getLineInput("Enter Book ID to borrow: ");
            for (auto& c : bookID)
                c = toupper(c);
            int bookIndex = -1;
            for (size_t i = 0; i < books.size(); i++) {
                if (books[i].bookID == bookID) {
                    bookIndex = i;
                    break;
                }
            }

            // Check if member has already borrowed this book
            bool alreadyBorrowed = false;
            for (const auto& br : borrowRecords) {
                if (br.memberID == memberID && br.bookID == bookID && !br.isReturned) {
                    alreadyBorrowed = true;
                    break;
                }
            }

            if (alreadyBorrowed) {
                cout << "Transaction Failed: You have already borrowed this book!\n";
                pause();
                return;
            }

            if (bookIndex != -1) {
                if (books[bookIndex].availableCopies > 0) {
                    //Create borrow record
                    BorrowRecord br;
                    br.recordID = getNextBorrowRecordId();
                    br.memberID = memberID;
                    br.bookID = bookID;
                    br.borrowDate = getCurrentDate();
                    br.dueDate = getDateFromDays(14);
                    br.returnDate = "-";
                    br.isReturned = false;
                    br.fineAmount = 0.0;

                    //Update states
                    books[bookIndex].availableCopies--;
                    if (books[bookIndex].availableCopies == 0) {
                        books[bookIndex].status = BORROWED;
                    }
                    members[memberIndex].borrowedCount++;

                    string currentDate = getCurrentDate();
                    int currentMonth = stoi(currentDate.substr(3, 2)) - 1;
                    if (currentMonth >= 0 && currentMonth < 12) {
                        monthlyStats[currentMonth].booksBorrowed++;
                    }

                    borrowRecords.push_back(br);
                    saveAllData();

                    cout << "\nBook borrowed successfully!";
                    cout << "\nRecord ID: " << br.recordID << " | Due Date: " << br.dueDate << endl;
                }
                else {
                    cout << "Transaction Failed: Book is currently out of stock.\n";
                }
            }
            else {
                cout << "Transaction Failed: Target Book ID does not exist.\n";
            }
        }
        else {
            cout << "Transaction Failed: Member has reached maximum borrowing limit ("
                << allowedLimit << " books).\n";
        }
    }
    else {
        cout << "Transaction Failed: Target Member ID does not exist.\n";
    }
    pause();
}

//Return a borrowed book
void returnBook() {
    cout << "\n================================================\n";
    cout << "\t\tReturn Book";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    for (auto& c : memberID) 
        c = toupper(c);

    int memberIndex = -1;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member not found!\n";
        pause();
        return;
    }

    cout << "\n--- Books Currently Borrowed by " << members[memberIndex].name << " ---\n";
    cout << left << setw(12) << "Record ID"
        << setw(10) << "Book ID"
        << setw(35) << "Title"
        << setw(15) << "Due Date" << endl;
    cout << string("--------------------------------------------------------------------") << endl;

    vector<int> borrowedIndices;
    for (size_t i = 0; i < borrowRecords.size(); i++) {
        if (borrowRecords[i].memberID == memberID && !borrowRecords[i].isReturned) {
            borrowedIndices.push_back(i);
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

    if (borrowedIndices.empty()) {
        cout << "No books currently borrowed by this member.\n";
        pause();
        return;
    }

    string recordID = getLineInput("Enter Borrow Record ID: ");
    for (auto& c : recordID)
        c = toupper(c);
    int recIndex = -1;
    for (int idx : borrowedIndices) {
        if (borrowRecords[idx].recordID == recordID) {
            recIndex = idx;
            break;
        }
    }

    if (recIndex == -1) {
        cout << "Error: Invalid Record ID or book already returned!\n";
        pause();
        return;
    }

    BorrowRecord& br = borrowRecords[recIndex];
    br.returnDate = getCurrentDate();
    br.isReturned = true;
    br.fineAmount = calculateFine(br.dueDate, br.returnDate);

    //Award points
    if (br.fineAmount == 0.0) {
        int mIdx = -1;
        if (findMemberByID(br.memberID, mIdx)) {
            members[mIdx].rewardPoints += 5; //add 5 points 
            cout << "On-Time Return Bonus: Earned +5 Reward Points!\n";
        }
    }

    //Update book inventory
    for (auto& b : books) {
        if (b.bookID == br.bookID) {
            b.availableCopies++;
            b.status = AVAILABLE;
            break;
        }
    }

    //Update member record
    if (findMemberByID(br.memberID, memberIndex)) {
        if (members[memberIndex].borrowedCount > 0) {
            members[memberIndex].borrowedCount--;
        }
    }

    // Monthly statistics update
    string currentDate = getCurrentDate();
    int currentMonth = stoi(currentDate.substr(3, 2)) - 1;
    if (currentMonth >= 0 && currentMonth < 12) {
        monthlyStats[currentMonth].booksReturned++;
    }

    saveAllData();

    cout << "\nBook returned successfully!\n";
    if (br.fineAmount > 0) {
        cout << "Late Return Warning: Fine incurred = RM " << fixed << setprecision(2) << br.fineAmount << endl;
    }

    // After returning book, process reservation queue
    processReservationQueue(br.bookID);
    pause();
}

//Recommend books based on member's borrowing history
void recommendBooks() {
    cout << "\n================================================\n";
    cout << "\t\tBook Recommendation";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    for (auto& c : memberID)
        c = toupper(c);
    int memberIndex = -1;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member ID not found!\n";
        pause();
        return;
    }

    //Find the most borrowed category for this member
    string favoriteCategory = "";
    int maxCount = 0;

    for (const auto& br : borrowRecords) {
        if (br.memberID == memberID) {
            string currentCategory = "";
            for (const auto& b : books) {
                if (b.bookID == br.bookID) {
                    currentCategory = b.category;
                    break;
                }
            }

            //Count total borrows in this category by this member
            int count = 0;
            for (const auto& innerBr : borrowRecords) {
                if (innerBr.memberID == memberID) {
                    for (const auto& b : books) {
                        if (b.bookID == innerBr.bookID && b.category == currentCategory) {
                            count++;
                        }
                    }
                }
            }

            if (count > maxCount) {
                maxCount = count;
                favoriteCategory = currentCategory;
            }
        }
    }

    if (favoriteCategory.empty()) {
        cout << "No borrowing history found to make recommendations.\n";
        pause();
        return;
    }

    cout << "\nFavorite Category based on history: " << favoriteCategory << "\n\n";
    cout << left << setw(8) << "ID"
        << setw(30) << "Title"
        << setw(20) << "Author"
        << setw(15) << "Category" << endl;
    cout << "=================================================================\n";

    //Display books in that category
    bool found = false;
    for (const auto& b : books) {
        if (b.category == favoriteCategory) {
            cout << left << setw(8) << b.bookID
                << setw(30) << b.title
                << setw(20) << b.author
                << setw(15) << b.category << endl;
            found = true;
        }
    }

    if (!found) cout << "No recommendations found in this category right now.\n";
    pause();
}

// Display reward points and allow redemption
void rewardPointsMenu() {
    cout << "\n================================================\n";
    cout << "\tMembership Reward Points";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    for (auto& c : memberID)
        c = toupper(c);
    int memberIndex = -1;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member ID not found!\n";
        pause();
        return;
    }

    Member& m = members[memberIndex];
    int totalLimit = MAX_BORROW_LIMIT + m.bonusBorrowLimit;

    cout << "\nMember Name: " << m.name << endl;
    cout << "Current Reward Points: " << m.rewardPoints << " pts\n";
    cout << "Current Borrow Limit: " << totalLimit << " books (Base: " << MAX_BORROW_LIMIT << " + Bonus: " << m.bonusBorrowLimit << ")\n";

    cout << "\nRedemption Rule: Redeem 10 Points to increase your limit by +1 book slot.\n";
    string choice = getLineInput("Would you like to redeem 10 points? (y/n): ");

    if (choice == "y" || choice == "Y") {
        if (m.rewardPoints >= 10) {
            m.rewardPoints -= 10;
            m.bonusBorrowLimit += 1;
            saveMembers();
            cout << "Success! Your new borrowing limit is " << (MAX_BORROW_LIMIT + m.bonusBorrowLimit) << " books.\n";
        }
        else {
            cout << "Failed: Not enough points (Requires 10 pts).\n";
        }
    }
    pause();
}