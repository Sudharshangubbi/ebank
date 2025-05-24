#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Account {
    int accountNumber;
    string customerName;
    double balance;
    vector<string> transactions;
    bool hasLoan = false;
    double loanAmount = 0.0;
};

vector<Account> accounts;
int nextAccountNumber = 1001;

void createAccount() {
    Account acc;
    acc.accountNumber = nextAccountNumber++;
    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, acc.customerName);
    cout << "Enter initial deposit amount: ";
    cin >> acc.balance;

    acc.transactions.push_back("Account created with initial deposit: $" + to_string(acc.balance));
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

    for (const auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            cout << "\nAccount Found!\n";
            cout << "Account Number: " << acc.accountNumber << endl;
            cout << "Customer Name: " << acc.customerName << endl;
            cout << "Balance: $" << acc.balance << endl;
            cout << "Loan Status: " << (acc.hasLoan ? "Active ($" + to_string(acc.loanAmount) + ")" : "None") << endl;
            return;
        }
    }
    cout << "Account not found.\n";
}

void depositAmount() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;

    for (auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            cout << "Enter amount to deposit: ";
            cin >> amount;
            acc.balance += amount;
            acc.transactions.push_back("Deposited: $" + to_string(amount));
            cout << "Deposit successful. New balance: $" << acc.balance << endl;
            return;
        }
    }
    cout << "Account not found.\n";
}

void withdrawAmount() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;

    for (auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (acc.balance >= amount) {
                acc.balance -= amount;
                acc.transactions.push_back("Withdrew: $" + to_string(amount));
                cout << "Withdrawal successful. Remaining balance: $" << acc.balance << endl;
            } else {
                cout << "Insufficient balance.\n";
            }
            return;
        }
    }
    cout << "Account not found.\n";
}

void printMiniStatement() {
    int accNo;
    cout << "Enter account number for mini statement: ";
    cin >> accNo;

    for (const auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            cout << "\nMini Statement for Account #" << acc.accountNumber << ":\n";
            for (const string& t : acc.transactions) {
                cout << "- " << t << endl;
            }
            return;
        }
    }
    cout << "Account not found.\n";
}

void applyForLoan() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;

    for (auto& acc : accounts) {
        if (acc.accountNumber == accNo) {
            if (acc.hasLoan) {
                cout << "Loan already active: $" << acc.loanAmount << "\n";
                return;
            }

            cout << "Enter loan amount to apply: ";
            cin >> amount;
            if (amount <= 0) {
                cout << "Invalid loan amount.\n";
                return;
            }

            acc.hasLoan = true;
            acc.loanAmount = amount;
            acc.transactions.push_back("Loan Applied: $" + to_string(amount));
            cout << "Loan application successful for $" << amount << "\n";
            return;
        }
    }

    cout << "Account not found.\n";
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
    cout << "8. Mini Statement" << endl;
    cout << "9. Apply for Loan" << endl;
    cout << "10. Exit" << endl;

    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
        case 2:
            createAccount();
            break;
        case 3:
            depositAmount();
            break;
        case 4:
            withdrawAmount();
            break;
        case 7:
            searchAccount();
            break;
        case 8:
            printMiniStatement();
            break;
        case 9:
            applyForLoan();
            break;
        default:
            cout << "This service is not implemented yet.\n";
    }

    return 0;
}

