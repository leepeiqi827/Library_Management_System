//MemberManagement.cpp - Member Management Module
//Team Member: FRENGKY WONG HAI XIANG (25WMD03962)

#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <regex>
#include <cctype>

using namespace std;

//Validate name (only letters, spaces, and optionally allowed chars)
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// ---------------------- Helper Functions for Member Management ----------------------
//Validate email format using regex
bool isValidEmail(const string& email) {
    // Simple regex for email validation
    regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

//Validate phone number (at least 7 digits)
bool isValidPhone(const string& phone) {
    // Accept digits, spaces, parentheses, hyphens; at least 7 digits
    string clean;
    for (char c : phone) {
        if (isdigit(c)) clean += c;
    }
    return clean.length() >= 7 && clean.length() <= 11;
}

//Validate password (at least 6 characters)
bool isValidPassword(const string& pass) {
    // At least 6 characters
    return pass.length() >= 6;
}

//yes/no prompt
bool getYesNo(const string& prompt) {
    string input;
    while (true) {
        cout << prompt << " (y/n): ";
        getline(cin, input);
        // Trim spaces
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);
        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }
}

//Display a single member's information
void displayMember(const Member& m) {
    cout << "ID: " << m.memberID
        << " | Name: " << m.name
        << " | Contact: " << m.contact
        << " | Type: " << membershipTypeToString(m.membershipType)
        << " | Borrowed: " << m.borrowedCount
        << " | Active: " << (m.isActive ? "Yes" : "No") 
        << " | Points: " << m.rewardPoints
        << " | Bonus Limit: +" << m.bonusBorrowLimit << endl;
}

//Prompt user for string input with trimming
string inputString(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    // Trim leading/trailing spaces
    val.erase(0, val.find_first_not_of(" \t\n\r"));
    val.erase(val.find_last_not_of(" \t\n\r") + 1);
    return val;
}

//Prompt user for integer input with validation
int inputInt(const string& prompt) {
    int val;
    cout << prompt;
    cin >> val;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number: ";
        cin >> val;
    }
    cin.ignore(1000, '\n');
    return val;
}

// Display member management menu
void memberManagementMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n================================================\n";
        cout << "            MEMBER MANAGEMENT\n";
        cout << "================================================\n";
        cout << "  1. Add Member\n";
        cout << "  2. View All Members\n";
        cout << "  3. Search Member by ID\n";
        cout << "  4. Search Member by Name (partial)\n";
        cout << "  5. Update Member Information\n";
        cout << "  6. Delete Member (permanent)\n";
        cout << "  7. Toggle Member Active Status\n";
        cout << "  8. Return to Main Menu\n";
        cout << "================================================\n";
        cout << "  Enter choice (1-8): ";

        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            pause();
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1: {
            // Add Member
            Member newMember;
            newMember.memberID = getNextMemberId();

            newMember.name = inputString("Enter full name: ");
            if (newMember.name.empty()) {
                cout << "Name cannot be empty.\n";
                pause();
                break;
            }
            else if  (!isValidName(newMember.name)) {
                cout << "Invalid name! Only letters and spaces are allowed (no numbers or symbols).\n";
                pause();
                break;
            }

            bool contactValid = false;
            do {
                newMember.contact = inputString("Enter contact (email or phone) [e.g., user@mail.com or 0123456789]: ");
                if (isValidEmail(newMember.contact) || isValidPhone(newMember.contact)) {
                    contactValid = true;
                }
                else {
                    cout << "Invalid contact format! Please enter a valid email or phone number.\n";
                }
            } while (!contactValid);

            newMember.password = inputString("Enter password (min 6 chars): ");
            if (!isValidPassword(newMember.password)) {
                cout << "Password must be at least 6 characters.\n";
                pause();
                break;
            }

            cout << "Membership Type (STUDENT, STAFF, PUBLIC): ";
            string typeStr;
            getline(cin, typeStr);
            for (auto& c : typeStr) 
                c = toupper(c);
            while (typeStr != "STUDENT" && typeStr != "STAFF" && typeStr != "PUBLIC") {
                cout << "Invalid membership type! Please enter STUDENT, STAFF, or PUBLIC: ";
                getline(cin, typeStr);
                for (auto& c : typeStr)
                 c = toupper(c);
            }
            newMember.membershipType = stringToMembershipType(typeStr);

            newMember.borrowedCount = 0;
            newMember.isActive = true;

            members.push_back(newMember);
            saveMembers();
            cout << "Member added successfully! ID: " << newMember.memberID << endl;
            pause();
            break;
        }

        case 2: {
            // View All Members
            if (members.empty()) {
                cout << "No members found.\n";
                pause();
                break;
            }
            cout << "\n--- All Members ---\n";
            for (const auto& m : members) {
                displayMember(m);
            }
            pause();
            break;
        }

        case 3: {
            // Search by ID
            string id = inputString("Enter Member ID: ");
            for (auto& c : id) 
                c = toupper(c);
            bool found = false;
            for (const auto& m : members) {
                if (m.memberID == id) {
                    displayMember(m);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Member with ID " << id << " not found.\n";
            pause();
            break;
        }

        case 4: {
            // Search by Name (case-insensitive partial match)
            string name = inputString("Enter name (or part): ");
            if (name.empty()) break;
            string lowerName = name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            bool found = false;
            cout << "\n--- Search Results ---\n";
            for (const auto& m : members) {
                string memberName = m.name;
                transform(memberName.begin(), memberName.end(), memberName.begin(), ::tolower);
                if (memberName.find(lowerName) != string::npos) {
                    displayMember(m);
                    found = true;
                }
            }
            if (!found) cout << "No members with name containing \"" << name << "\".\n";
            pause();
            break;
        }

        case 5: {
            // Update Member
            string id = inputString("Enter Member ID to update: ");
            for (auto& c : id)
                c = toupper(c);
            auto it = find_if(members.begin(), members.end(),
                [&](const Member& m) { return m.memberID == id; });
            if (it == members.end()) {
                cout << "Member not found.\n";
                pause();
                break;
            }

            cout << "Current details:\n";
            displayMember(*it);
            cout << "Enter new values (leave blank to keep current):\n";

            string newName = inputString("Name [" + it->name + "]: ");
            if (!newName.empty()) it->name = newName;

            string newContact = inputString("Contact [" + it->contact + "]: ");
            if (!newContact.empty()) {
                if (isValidEmail(newContact) || isValidPhone(newContact)) {
                    it->contact = newContact;
                }
                else {
                    cout << "Invalid contact format. Contact not updated.\n";
                    pause();
                }
            }

            string newPass = inputString("Password (min 6 chars) [leave blank to keep]: ");
            if (!newPass.empty()) {
                if (isValidPassword(newPass)) {
                    it->password = newPass;
                }
                else {
                    cout << "Password too short. Not updated.\n";
                }
            }

            cout << "Membership Type (STUDENT, STAFF, PUBLIC) ["
                << membershipTypeToString(it->membershipType) << "]: ";
            string typeStr;
            getline(cin, typeStr);
            if (!typeStr.empty()) {
                for (auto& c : typeStr) c = toupper(c);
                while (typeStr != "STUDENT" && typeStr != "STAFF" && typeStr != "PUBLIC") {
                    cout << "Invalid membership type! Please enter STUDENT, STAFF, or PUBLIC: ";
                    getline(cin, typeStr);
                    for (auto& c : typeStr) c = toupper(c);
                }
                MembershipType newType = stringToMembershipType(typeStr);
                if (newType != it->membershipType) {
                    it->membershipType = newType;
                }
            }

            saveMembers();
            cout << "Member updated successfully.\n";
            pause();
            break;
        }

        case 6: {
            // Delete Member (permanent)
            if (members.empty()) {
                cout << "No members found.\n";
                pause();
                break;
            }

            cout << "\n--- Current Members ---\n";
            for (const auto& m : members) {
                displayMember(m);
            }
            string id = inputString("Enter Member ID to delete: ");
            for (auto& c : id) 
                c = toupper(c);
            auto it = find_if(members.begin(), members.end(),
                [&](const Member& m) { return m.memberID == id; });
            if (it == members.end()) {
                cout << "Member not found.\n";
                pause();
                break;
            }
            if (it->borrowedCount > 0) {
                cout << "Cannot delete member with active borrowings. Return all books first.\n";
                pause();
                break;
            }
            if (getYesNo("Are you sure you want to delete member " + it->name + "?")) {
                members.erase(it);
                saveMembers();
                cout << "Member deleted.\n";
            } else {
                cout << "Deletion cancelled.\n";
            }
            pause();
            break;
        }

        case 7: {
            if (members.empty()) {
                cout << "No members found.\n";
                pause();
                break;
            }

            cout << "\n--- Current Members ---\n";
            cout << left << setw(8) << "ID"
                << setw(25) << "Name"
                << setw(12) << "Status" << endl;
            cout << string(45, '-') << endl;
            for (const auto& m : members) {
                cout << left << setw(8) << m.memberID
                    << setw(25) << m.name
                    << setw(12) << (m.isActive ? "ACTIVE" : "INACTIVE") << endl;
            }

            cout << "\n";
            string id = inputString("Enter Member ID to toggle status: ");
            for (auto& c : id) c = toupper(c);

            auto it = find_if(members.begin(), members.end(),
                [&](const Member& m) { return m.memberID == id; });
            if (it == members.end()) {
                cout << "Member not found.\n";
                pause();
                break;
            }

            // Check if member has unreturned books before deactivating
            if (it->isActive) {
                bool hasBorrowedBooks = false;
                for (const auto& br : borrowRecords) {
                    if (br.memberID == it->memberID && !br.isReturned) {
                        hasBorrowedBooks = true;
                        break;
                    }
                }

                if (hasBorrowedBooks) {
                    cout << "\nCannot deactivate member!\n";
                    cout << "Member " << it->name << " has unreturned books.\n";
                    cout << "Please return all books first.\n";
                    pause();
                    break;
                }
            }

            cout << "\nMember: " << it->name << endl;
            cout << "Current Status: " << (it->isActive ? "ACTIVE" : "INACTIVE") << endl;
            cout << "New Status: " << (it->isActive ? "INACTIVE" : "ACTIVE") << endl;
            if (getYesNo("Confirm status change")) {
                it->isActive = !it->isActive;
                saveMembers();
                cout << "Member " << it->name << " is now "
                    << (it->isActive ? "ACTIVE" : "INACTIVE") << ".\n";
            } else {
                cout << "Operation cancelled.\n";
                pause();
                break;
            }

            it->isActive = !it->isActive;
            saveMembers();
            cout << "Member " << it->name << " is now "
                << (it->isActive ? "ACTIVE" : "INACTIVE") << ".\n";
            pause();
            break;
        }

        case 8:
            cout << "Returning to main menu...\n";
            pause();
            break;

        default:
            cout << "Invalid choice. Please enter 1-8.\n";
            pause();
        }
    } while (choice != 8);
}
