#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Customer {
    int id;
    string name;
    string email;
};

struct Account {
    int accountNumber;
    int customerId;
    string customerName;
    double balance;
    vector<string> transactions;
    bool hasLoan = false;
    double loanPrincipal = 0.0;
    double loanInterestRate = 0.0;
    int loanDurationMonths = 0;
    double totalLoanDue = 0.0;
};

vector<Customer> customers;
vector<Account> accounts;
int nextCustomerId = 1;
int nextAccountNumber = 1001;

// Add a customer
void addCustomer() {
    Customer c;
    c.id = nextCustomerId++;
    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, c.name);
    cout << "Enter email: ";
    getline(cin, c.email);
    customers.push_back(c);
    cout << "Customer added successfully! ID: " << c.id << "\n";
}

// Display all customers
void displayCustomers() {
    if (customers.empty()) {
        cout << "No customers found.\n";
        return;
    }
    cout << "\n--- Customers List ---\n";
    for (const auto& c : customers) {
        cout << "ID: " << c.id << ", Name: " << c.name << ", Email: " << c.email << "\n";
    }
}

// Display all accounts
void displayAccounts() {
    if (accounts.empty()) {
        cout << "No accounts found.\n";
        return;
    }
    cout << "\n--- Accounts List ---\n";
    for (const auto& a : accounts) {
        cout << "Account No: " << a.accountNumber << ", Customer ID: " << a.customerId 
             << ", Name: " << a.customerName << ", Balance: $" << a.balance << "\n";
        cout << "Loan Status: " << (a.hasLoan ? "Active (Due: $" + to_string(a.totalLoanDue) + ")" : "None") << "\n";
    }
}

// Find customer by ID, returns nullptr if not found
Customer* findCustomer(int id) {
    for (auto& c : customers) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

// Find account by account number, returns nullptr if not found
Account* findAccount(int accNo) {
    for (auto& a : accounts) {
        if (a.accountNumber == accNo) return &a;
    }
    return nullptr;
}

// Create account for a customer
void createAccount() {
    int custId;
    cout << "Enter customer ID: ";
    cin >> custId;
    Customer* c = findCustomer(custId);
    if (!c) {
        cout << "Customer not found. Please add customer first.\n";
        return;
    }

    Account acc;
    acc.accountNumber = nextAccountNumber++;
    acc.customerId = c->id;
    acc.customerName = c->name;
    cout << "Enter initial deposit amount: ";
    cin >> acc.balance;

    acc.transactions.push_back("Account created with initial deposit: $" + to_string(acc.balance));
    accounts.push_back(acc);

    cout << "Account created successfully!\nAccount Number: " << acc.accountNumber << "\n";
}

// Deposit amount into account
void depositAmount() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;

    Account* acc = findAccount(accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> amount;
    if(amount <= 0){
        cout << "Invalid amount.\n";
        return;
    }

    acc->balance += amount;
    acc->transactions.push_back("Deposited: $" + to_string(amount));
    cout << "Deposit successful. New balance: $" << acc->balance << endl;
}

// Withdraw amount from account
void withdrawAmount() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;

    Account* acc = findAccount(accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> amount;
    if(amount <= 0){
        cout << "Invalid amount.\n";
        return;
    }

    if (acc->balance >= amount) {
        acc->balance -= amount;
        acc->transactions.push_back("Withdrew: $" + to_string(amount));
        cout << "Withdrawal successful. Remaining balance: $" << acc->balance << endl;
    } else {
        cout << "Insufficient balance.\n";
    }
}

// Print mini statement for account
void printMiniStatement() {
    int accNo;
    cout << "Enter account number for mini statement: ";
    cin >> accNo;

    Account* acc = findAccount(accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    cout << "\nMini Statement for Account #" << acc->accountNumber << ":\n";
    for (const string& t : acc->transactions) {
        cout << "- " << t << endl;
    }
}

// Apply for loan with simple interest calculation
void applyForLoan() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;

    Account* acc = findAccount(accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    if (acc->hasLoan) {
        cout << "Loan already active. Please repay the existing loan first.\n";
        return;
    }

    double principal;
    cout << "Enter loan principal amount: ";
    cin >> principal;
    if(principal <= 0){
        cout << "Invalid loan amount.\n";
        return;
    }

    double interestRate;
    int durationMonths;
    cout << "Enter annual interest rate (in %): ";
    cin >> interestRate;
    if(interestRate < 0){
        cout << "Invalid interest rate.\n";
        return;
    }
    cout << "Enter loan duration (in months): ";
    cin >> durationMonths;
    if(durationMonths <= 0){
        cout << "Invalid loan duration.\n";
        return;
    }

    double totalInterest = (principal * interestRate * durationMonths) / (100.0 * 12.0);
    acc->loanPrincipal = principal;
    acc->loanInterestRate = interestRate;
    acc->loanDurationMonths = durationMonths;
    acc->totalLoanDue = principal + totalInterest;
    acc->hasLoan = true;

    acc->transactions.push_back("Loan Approved: Principal $" + to_string(principal) + 
                                ", Interest $" + to_string(totalInterest) + 
                                ", Total Due $" + to_string(acc->totalLoanDue));
    
    cout << "\nLoan approved!\n";
    cout << "Principal: $" << principal << "\n";
    cout << "Total Payable (with interest): $" << acc->totalLoanDue << "\n";
    cout << "Monthly EMI: $" << acc->totalLoanDue / durationMonths << "\n";
}

// Repay loan amount
void repayLoan() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;

    Account* acc = findAccount(accNo);
    if (!acc) {
        cout << "Account not found.\n";
        return;
    }

    if (!acc->hasLoan) {
        cout << "No active loan for this account.\n";
        return;
    }

    cout << "Remaining loan due: $" << acc->totalLoanDue << "\n";
    double amount;
    cout << "Enter repayment amount: ";
    cin >> amount;
    if(amount <= 0){
        cout << "Invalid amount.\n";
        return;
    }

    if (amount >= acc->totalLoanDue) {
        acc->transactions.push_back("Loan repaid in full: $" + to_string(acc->totalLoanDue));
        acc->totalLoanDue = 0;
        acc->hasLoan = false;
        acc->loanPrincipal = 0;
        acc->loanInterestRate = 0;
        acc->loanDurationMonths = 0;
        cout << "Loan fully repaid.\n";
    } else {
        acc->totalLoanDue -= amount;
        acc->transactions.push_back("Partial loan repayment: $" + to_string(amount));
        cout << "Partial repayment done. Remaining due: $" << acc->totalLoanDue << "\n";
    }
}

int main() {
    int choice;

    cout << "----------------------------------" << endl;
    cout << "   Welcome to ATM Service" << endl;
    cout << "----------------------------------" << endl;

    while (true) {
        cout << "\n--- ATM Services ---" << endl;
        cout << "1. Add Customer" << endl;
        cout << "2. Create Account" << endl;
        cout << "3. Deposit" << endl;
        cout << "4. Withdraw" << endl;
        cout << "5. Display Customers" << endl;
        cout << "6. Display Accounts" << endl;
        cout << "7. Search Account (by Account No)" << endl;
        cout << "8. Mini Statement" << endl;
        cout << "9. Apply for Loan" << endl;
        cout << "10. Repay Loan" << endl;
        cout << "11. Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                depositAmount();
                break;
            case 4:
                withdrawAmount();
                break;
            case 5:
                displayCustomers();
                break;
            case 6:
                displayAccounts();
                break;
            case 7: {
                int accNo;
                cout << "Enter account number: ";
                cin >> accNo;
                Account* acc = findAccount(accNo);
                if (acc) {
                    cout << "\nAccount Details:\n";
                    cout << "Account No: " << acc->accountNumber << "\nCustomer ID: " << acc->customerId << "\nName: " << acc->customerName << "\nBalance: $" << acc->balance << "\n";
                    cout << "Loan Status: " << (acc->hasLoan ? "Active (Due: $" + to_string(acc->totalLoanDue) + ")" : "None") << "\n";
                } else {
                    cout << "Account not found.\n";
                }
                break;
            }
            case 8:
                printMiniStatement();
                break;
            case 9:
                applyForLoan();
                break;
            case 10:
                repayLoan();
                break;
            case 11:
                cout << "Thank you for using ATM Service. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

