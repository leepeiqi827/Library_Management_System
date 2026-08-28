#include "LibrarySystem.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

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

void bookManagementMenu() {
    int choice;
    do {
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
			break;
        default:
            cout << "Invalid choice! Please enter 1-6.\n";
        }
    } while (choice != 6);
}

//Add new book title
void addBook() {
    cout << "\n================================================\n";
    cout << "\t\tAdd New Book";
    cout << "\n================================================\n";

    Book newBook;
    newBook.bookID = generateID("B", books.size() + 1);

    newBook.title = getLineInput("Enter Book Title: ");
    if (newBook.title.empty()) {
        cout << "Title cannot be empty!\n";
        return;
    }

    newBook.author = getLineInput("Enter Author Name: ");
    newBook.category = getLineInput("Enter Category: ");

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
}

//Update book details
void updateBook() {
    cout << "\n================================================\n";
    cout << "\t\tUpdate Book Details";
    cout << "\n================================================\n";

    string bookID = getLineInput("Enter Book ID to update: ");
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

        cout << "New Total Copies (-1 to keep current): ";
        int newCopies;
        cin >> newCopies;
        if (!cin.fail() && newCopies >= 0) {
            int copyDiff = newCopies - books[targetIndex].totalCopies;
            books[targetIndex].totalCopies = newCopies;
            books[targetIndex].availableCopies += copyDiff;
            if (books[targetIndex].availableCopies < 0) books[targetIndex].availableCopies = 0;
        }
        cin.clear();
        cin.ignore(1000, '\n');

        saveBooks();
        cout << "Book updated successfully!\n";
    }
    else {
        cout << "Error: Book ID not found!\n";
    }
}

//Search book by title, author or category
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
    cout << "=================================================================================\n";

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
}

//Borrow book
void borrowBook() {
    cout << "\n================================================\n";
    cout << "\t\tBorrow Book";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    int memberIndex = -1;

    //validation
    if (findMemberByID(memberID, memberIndex)) {
        if (members[memberIndex].isActive) {
            //Check borrowing limit
            int allowedLimit = MAX_BORROW_LIMIT + members[memberIndex].bonusBorrowLimit;
            if (members[memberIndex].borrowedCount < allowedLimit) {

                string bookID = getLineInput("Enter Book ID to borrow: ");
                int bookIndex = -1;
                for (size_t i = 0; i < books.size(); i++) {
                    if (books[i].bookID == bookID) {
                        bookIndex = i;
                        break;
                    }
                }

                if (bookIndex != -1) {
                    if (books[bookIndex].availableCopies > 0) {
                        //Create borrow record
                        BorrowRecord br;
                        br.recordID = generateID("R", borrowRecords.size() + 1);
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

                        int currentMonth = 0; 
                        monthlyStats[currentMonth].booksBorrowed++;

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
            cout << "Transaction Failed: Member account is inactive.\n";
        }
    }
    else {
        cout << "Transaction Failed: Target Member ID does not exist.\n";
    }
}

//Return book
void returnBook() {
    cout << "\n================================================\n";
    cout << "\t\tReturn Book";
    cout << "\n================================================\n";

    string recordID = getLineInput("Enter Borrow Record ID: ");
    int recIndex = -1;

    for (size_t i = 0; i < borrowRecords.size(); i++) {
        if (borrowRecords[i].recordID == recordID && !borrowRecords[i].isReturned) {
            recIndex = i;
            break;
        }
    }

    if (recIndex == -1) {
        cout << "Error: Active borrow record not found!\n";
        return;
    }

    BorrowRecord& br = borrowRecords[recIndex];
    br.returnDate = getCurrentDate();
    br.isReturned = true;
    br.fineAmount = calculateFine(br.dueDate, br.returnDate);

    //Update book inventory
    for (auto& b : books) {
        if (b.bookID == br.bookID) {
            b.availableCopies++;
            b.status = AVAILABLE;
            break;
        }
    }

    //Update member record
    int memberIndex = -1;
    if (findMemberByID(br.memberID, memberIndex)) {
        if (members[memberIndex].borrowedCount > 0) {
            members[memberIndex].borrowedCount--;
        }
    }

    // Monthly statistics update
    int currentMonth = 0;
    monthlyStats[currentMonth].booksReturned++;

    saveAllData();

    cout << "\nBook returned successfully!\n";
    if (br.fineAmount > 0) {
        cout << "Late Return Warning: Fine incurred = RM " << fixed << setprecision(2) << br.fineAmount << endl;
    }
}

void recommendBooks() {
    cout << "\n================================================\n";
    cout << "\t\tBook Recommendation";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    int memberIndex = -1;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member ID not found!\n";
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
}


void rewardPointsMenu() {
    cout << "\n================================================\n";
    cout << "\t\tMembership Reward Points";
    cout << "\n================================================\n";

    string memberID = getLineInput("Enter Member ID: ");
    int memberIndex = -1;
    if (!findMemberByID(memberID, memberIndex)) {
        cout << "Error: Member ID not found!\n";
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
}
