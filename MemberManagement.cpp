// member : Frengky Wong Hai Xiang

#include "LibrarySystem.h"
#include <iostream>
#include <iomanip>
#include <regex>
#include <cctype>

using namespace std;

// ---------------------- Helper Functions for Member Management ----------------------

bool isValidEmail(const string& email) {
    // Simple regex for email validation
    regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

bool isValidPhone(const string& phone) {
    // Accept digits, spaces, parentheses, hyphens; at least 7 digits
    string clean;
    for (char c : phone) {
        if (isdigit(c)) clean += c;
    }
    return clean.length() >= 7 && clean.length() <= 15;
}

bool isValidPassword(const string& pass) {
    // At least 6 characters
    return pass.length() >= 6;
}

void displayMember(const Member& m) {
    cout << "ID: " << m.memberID
         << " | Name: " << m.name
         << " | Contact: " << m.contact
         << " | Type: " << membershipTypeToString(m.membershipType)
         << " | Borrowed: " << m.borrowedCount
         << " | Active: " << (m.isActive ? "Yes" : "No") << endl;
}

string inputString(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    // Trim leading/trailing spaces
    val.erase(0, val.find_first_not_of(" \t\n\r"));
    val.erase(val.find_last_not_of(" \t\n\r") + 1);
    return val;
}

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

// ---------------------- Menu Implementation ----------------------

void memberManagementMenu() {
    int choice;
    do {
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
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {
            case 1: {
                // Add Member
                Member newMember;
                newMember.memberID = generateID("M", members.size() + 1);

                newMember.name = inputString("Enter full name: ");
                if (newMember.name.empty()) {
                    cout << "Name cannot be empty.\n";
                    break;
                }

                newMember.contact = inputString("Enter contact (email or phone): ");
                // Simple validation: try to validate as email or phone
                if (!isValidEmail(newMember.contact) && !isValidPhone(newMember.contact)) {
                    cout << "Warning: Contact format may be invalid. Proceed anyway? (y/n): ";
                    char ans;
                    cin >> ans;
                    cin.ignore(1000, '\n');
                    if (tolower(ans) != 'y') break;
                }

                newMember.password = inputString("Enter password (min 6 chars): ");
                if (!isValidPassword(newMember.password)) {
                    cout << "Password must be at least 6 characters.\n";
                    break;
                }

                cout << "Membership Type (STUDENT, STAFF, PUBLIC): ";
                string typeStr;
                getline(cin, typeStr);
                // Convert to uppercase
                for (auto& c : typeStr) c = toupper(c);
                newMember.membershipType = stringToMembershipType(typeStr);

                newMember.borrowedCount = 0;
                newMember.isActive = true;

                members.push_back(newMember);
                saveMembers();
                cout << "Member added successfully! ID: " << newMember.memberID << endl;
                break;
            }

            case 2: {
                // View All Members
                if (members.empty()) {
                    cout << "No members found.\n";
                    break;
                }
                cout << "\n--- All Members ---\n";
                for (const auto& m : members) {
                    displayMember(m);
                }
                break;
            }

            case 3: {
                // Search by ID
                string id = inputString("Enter Member ID: ");
                bool found = false;
                for (const auto& m : members) {
                    if (m.memberID == id) {
                        displayMember(m);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Member with ID " << id << " not found.\n";
                break;
            }

            case 4: {
                // Search by Name (case-insensitive partial match)
                string name = inputString("Enter name (or part): ");
                if (name.empty()) break;
                bool found = false;
                for (const auto& m : members) {
                    if (m.name.find(name) != string::npos) {
                        displayMember(m);
                        found = true;
                    }
                }
                if (!found) cout << "No members with name containing \"" << name << "\".\n";
                break;
            }

            case 5: {
                // Update Member
                string id = inputString("Enter Member ID to update: ");
                auto it = find_if(members.begin(), members.end(),
                                  [&](const Member& m) { return m.memberID == id; });
                if (it == members.end()) {
                    cout << "Member not found.\n";
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
                    } else {
                        cout << "Invalid contact format. Contact not updated.\n";
                    }
                }

                string newPass = inputString("Password (min 6 chars) [leave blank to keep]: ");
                if (!newPass.empty()) {
                    if (isValidPassword(newPass)) {
                        it->password = newPass;
                    } else {
                        cout << "Password too short. Not updated.\n";
                    }
                }

                cout << "Membership Type (STUDENT, STAFF, PUBLIC) [" 
                     << membershipTypeToString(it->membershipType) << "]: ";
                string typeStr;
                getline(cin, typeStr);
                if (!typeStr.empty()) {
                    for (auto& c : typeStr) c = toupper(c);
                    MembershipType newType = stringToMembershipType(typeStr);
                    if (newType != it->membershipType) {
                        it->membershipType = newType;
                    }
                }

                saveMembers();
                cout << "Member updated successfully.\n";
                break;
            }

            case 6: {
                // Delete Member (permanent)
                string id = inputString("Enter Member ID to delete: ");
                auto it = find_if(members.begin(), members.end(),
                                  [&](const Member& m) { return m.memberID == id; });
                if (it == members.end()) {
                    cout << "Member not found.\n";
                    break;
                }
                if (it->borrowedCount > 0) {
                    cout << "Cannot delete member with active borrowings. Return all books first.\n";
                    break;
                }
                cout << "Are you sure you want to delete member " << it->name << "? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore(1000, '\n');
                if (tolower(confirm) == 'y') {
                    members.erase(it);
                    saveMembers();
                    cout << "Member deleted.\n";
                } else {
                    cout << "Deletion cancelled.\n";
                }
                break;
            }

            case 7: {
                // Toggle Active Status
                string id = inputString("Enter Member ID: ");
                auto it = find_if(members.begin(), members.end(),
                                  [&](const Member& m) { return m.memberID == id; });
                if (it == members.end()) {
                    cout << "Member not found.\n";
                    break;
                }
                it->isActive = !it->isActive;
                saveMembers();
                cout << "Member " << it->name << " is now " 
                     << (it->isActive ? "ACTIVE" : "INACTIVE") << ".\n";
                break;
            }

            case 8:
                cout << "Returning to main menu...\n";
                break;

            default:
                cout << "Invalid choice. Please enter 1-8.\n";
        }
    } while (choice != 8);
}
