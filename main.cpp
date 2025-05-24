#include <iostream>
#include <string>
using namespace std;

struct Account {
    int accountNumber;
    string customerName;
    double balance;
};

int nextAccountNumber = 1001;

void createAccount() {
    Account acc;
    acc.accountNumber = nextAccountNumber++;
    cout << "Enter customer name: ";
    cin.ignore(); // clear input buffer
    getline(cin, acc.customerName);
    cout << "Enter initial deposit amount: ";
    cin >> acc.balance;

    cout << "\nAccount created successfully!\n";
    cout << "Account Number: " << acc.accountNumber << endl;
    cout << "Customer Name: " << acc.customerName << endl;
    cout << "Balance: $" << acc.balance << endl;
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
    cout << "7. Exit" << endl;

    cout << "\nEnter your choice: ";
    cin >> choice;

    if (choice == 2) {
        createAccount();
    } else {
        cout << "This service is not implemented yet.\n";
    }

    return 0;
}

