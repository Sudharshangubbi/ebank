#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Account {
    int accountNumber;
    string customerName;
    double balance;
};

vector<Account> accounts;
int nextAccountNumber = 1001;

void createAccount() {
    Account acc;
    acc.accountNumber = nextAccountNumber++;
    cout << "Enter customer name: ";
    cin.ignore(); // clear input buffer
    getline(cin, acc.customerName);
    cout << "Enter initial deposit amount: ";
    cin >> acc.balance;

    accounts.push_back(acc);

    cout << "\nAccount created successfully!\n";
    cout << "Account Number: " << acc.accountNumber << endl;
    cout << "Customer Name: " << acc.customerName << endl;
    cout << "Balance: $" << acc.balance << endl;
}

void searchAccount() {
    int accNo;
    cout << "Enter account number to search: ";
    cin >> accNo;

    bool found = false;
    for (const auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            cout << "\nAccount Found!\n";
            cout << "Account Number: " << acc.accountNumber << endl;
            cout << "Customer Name: " << acc.customerName << endl;
            cout << "Balance: $" << acc.balance << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Account not found.\n";
    }
}

int main() {
    int choice;

    cout << "----------------------------------" << endl;
    cout << "   Welcome to ATM Service" << endl;
    cout << "----------------------------------" << endl;

    cout << "\n--- ATM Services ---" << endl;
    cout << "1. Add Customer" << endl;
    cout << "2. Create Account" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Withdraw" << endl;
    cout << "5. Display Customers" << endl;
    cout << "6. Display Accounts" << endl;
    cout << "7. Search Account" << endl;
    cout << "8. Exit" << endl;

    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
        case 2:
            createAccount();
            break;
        case 7:
            searchAccount();
            break;
        default:
            cout << "This service is not implemented yet.\n";
    }

    return 0;
}

