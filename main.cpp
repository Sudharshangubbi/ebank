#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Account {
public:
    int accountNumber;
    string holderName;
    double balance;

    void display() const {
        cout << "Account No: " << accountNumber << ", Holder: " << holderName
             << ", Balance: " << balance << endl;
    }
};

vector<Account> accounts;

// Add new account
void addAccount() {
    Account a;
    cout << "Enter account number: ";
    cin >> a.accountNumber;
    cin.ignore();

    cout << "Enter account holder name: ";
    getline(cin, a.holderName);

    cout << "Enter initial balance: ";
    cin >> a.balance;
    cin.ignore();

    accounts.push_back(a);
    cout << "Account added successfully.\n";
}

// Display all accounts
void displayAccounts() {
    cout << "\nAll Accounts:\n";
    for (const auto& a : accounts) {
        a.display();
    }
}

// Search account by account number
void searchAccount() {
    int accNum;
    cout << "Enter account number to search: ";
    cin >> accNum;
    cin.ignore();

    for (const auto& a : accounts) {
        if (a.accountNumber == accNum) {
            a.display();
            return;
        }
    }
    cout << "Account not found.\n";
}

// Deposit amount
void deposit() {
    int accNum;
    double amount;
    cout << "Enter account number to deposit: ";
    cin >> accNum;
    cin.ignore();

    for (auto& a : accounts) {
        if (a.accountNumber == accNum) {
            cout << "Enter amount to deposit: ";
            cin >> amount;
            cin.ignore();

            if (amount > 0) {
                a.balance += amount;
                cout << "Deposit successful. New balance: " << a.balance << endl;
            } else {
                cout << "Invalid amount.\n";
            }
            return;
        }
    }
    cout << "Account not found.\n";
}

// Withdraw amount
void withdraw() {
    int accNum;
    double amount;
    cout << "Enter account number to withdraw: ";
    cin >> accNum;
    cin.ignore();

    for (auto& a : accounts) {
        if (a.accountNumber == accNum) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            cin.ignore();

            if (amount > 0 && amount <= a.balance) {
                a.balance -= amount;
                cout << "Withdrawal successful. New balance: " << a.balance << endl;
            } else {
                cout << "Invalid amount or insufficient balance.\n";
            }
            return;
        }
    }
    cout << "Account not found.\n";
}

// Update account holder name
void updateAccountHolderName() {
    int accNum;
    cout << "Enter account number to update holder name: ";
    cin >> accNum;
    cin.ignore();

    for (auto& a : accounts) {
        if (a.accountNumber == accNum) {
            cout << "Enter new holder name: ";
            getline(cin, a.holderName);
            cout << "Holder name updated successfully.\n";
            return;
        }
    }
    cout << "Account not found.\n";
}

// Delete account by account number
void deleteAccount() {
    int accNum;
    cout << "Enter account number to delete: ";
    cin >> accNum;
    cin.ignore();

    auto it = remove_if(accounts.begin(), accounts.end(),
                        [accNum](const Account& a) { return a.accountNumber == accNum; });
    if (it != accounts.end()) {
        accounts.erase(it, accounts.end());
        cout << "Account deleted successfully.\n";
    } else {
        cout << "Account not found.\n";
    }
}

// Shreyas's contribution: Display top 3 accounts by balance
void displayTop3AccountsByBalance() {
    if (accounts.empty()) {
        cout << "No accounts available.\n";
        return;
    }

    vector<Account> sortedAccounts = accounts;
    sort(sortedAccounts.begin(), sortedAccounts.end(),
         [](const Account& a, const Account& b) { return a.balance > b.balance; });

    cout << "\nTop 3 Accounts by Balance:\n";
    for (size_t i = 0; i < min(sortedAccounts.size(), size_t(3)); ++i) {
        sortedAccounts[i].display();
    }
}

// Chinami's contribution: Display accounts with balance below threshold
void displayLowBalanceAccounts() {
    double threshold;
    cout << "Enter balance threshold: ";
    cin >> threshold;
    cin.ignore();

    bool found = false;
    cout << "\nAccounts with balance below " << threshold << ":\n";
    for (const auto& a : accounts) {
        if (a.balance < threshold) {
            a.display();
            found = true;
        }
    }
    if (!found) {
        cout << "No accounts found below the threshold.\n";
    }
}

void mainMenu() {
    int choice;
    while (true) {
        cout << "\nBank Management System Menu:\n";
        cout << "1. Add Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account by Account Number\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "6. Update Account Holder Name\n";
        cout << "7. Delete Account\n";
        cout << "8. Display Top 3 Accounts by Balance\n";  // Shreyas's contribution
        cout << "9. Display Accounts with Low Balance\n";   // Chinami's contribution
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addAccount(); break;
            case 2: displayAccounts(); break;
            case 3: searchAccount(); break;
            case 4: deposit(); break;
            case 5: withdraw(); break;
            case 6: updateAccountHolderName(); break;
            case 7: deleteAccount(); break;
            case 8: displayTop3AccountsByBalance(); break;
            case 9: displayLowBalanceAccounts(); break;
            case 10: cout << "Exiting...\n"; return;
            default: cout << "Invalid choice! Try again.\n"; break;
        }
    }
}

int main() {
    cout << "Welcome to Bank Management System\n";
    mainMenu();
    return 0;
}
