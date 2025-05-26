#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Account {
protected:
    int accNumber;
    string name;
    char type;
    double balance;
    double loanAmount;
    string statement;

public:
    Account() : accNumber(0), name(""), type('S'), balance(0.0), loanAmount(0.0), statement("") {}

    void createAccount() {
        cout << "\nEnter Account No.: "; cin >> accNumber;
        cout << "Enter Account Holder Name: "; cin.ignore(); getline(cin, name);
        cout << "Enter Type (C/S): "; cin >> type; type = toupper(type);
        cout << "Enter Initial Balance (>=500 for S, >=1000 for C): "; cin >> balance;
        while ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000)) {
            cout << "Insufficient balance. Re-enter: "; cin >> balance;
        }
        cout << "Account Created.\n";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accNumber
             << "\nHolder Name : " << name
             << "\nType        : " << type
             << "\nBalance     : " << balance
             << "\nLoan Taken  : " << loanAmount << "\n";
    }

    void modify() {
        cout << "Modify Name: "; cin.ignore(); getline(cin, name);
        cout << "Modify Type (C/S): "; cin >> type; type = toupper(type);
        cout << "Modify Balance: "; cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
        addStatement("Deposited", amt);
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << "Insufficient balance.\n";
            return;
        }
        balance -= amt;
        addStatement("Withdrawn", amt);
    }

    void addLoan(double amt) {
        loanAmount += amt;
        addStatement("Loan Taken", amt);
    }

    void addStatement(const string& action, double amt) {
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back(); // Remove newline
        statement += action + " " + to_string(amt) + " on " + timeStr + "\n";
    }

    void displayStatement() const {
        cout << "\nMini Statement:\n" << statement;
    }

    int getAccountNumber() const { return accNumber; }
    double getBalance() const { return balance; }
    char getType() const { return type; }
    string getName() const { return name; }
};

// File operations
void writeAccount() {
    Account ac;
    ofstream outFile("account.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) cout << "Account not found.\n";
}

void modifyAccount(int n) {
    Account ac;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof()) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            cout << "\nEnter new details:\n";
            ac.modify();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "Account updated.\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << "Account not found.\n";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() != n)
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("temp.dat", "account.dat");
    cout << "Account deleted.\n";
}

void depositWithdraw(int n, int option) {
    Account ac;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof()) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            double amt;
            cout << "Enter amount: "; cin >> amt;
            if (option == 1) ac.deposit(amt);
            else ac.withdraw(amt);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "Transaction complete.\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << "Account not found.\n";
}

void loanSection(int n) {
    Account ac;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof()) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            double amt;
            cout << "Enter loan amount: "; cin >> amt;
            ac.addLoan(amt);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "Loan processed.\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) cout << "Account not found.\n";
}

void miniStatement(int n) {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == n) {
            ac.displayStatement();
            return;
        }
    }
    cout << "Account not found.\n";
}

void displayAll() {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.showAccount();
        cout << "---------------------------\n";
    }
    inFile.close();
}

void displayTop3AccountsByBalance() {
    vector<Account> all;
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        all.push_back(ac);
    }
    inFile.close();

    sort(all.begin(), all.end(), [](Account a, Account b) {
        return a.getBalance() > b.getBalance();
    });

    cout << "\nTop 3 Accounts by Balance:\n";
    for (size_t i = 0; i < min(all.size(), size_t(3)); ++i) {
        all[i].showAccount();
        cout << "---------------------------\n";
    }
}

void displayLowBalanceAccounts() {
    double threshold;
    cout << "Enter balance threshold: "; cin >> threshold;
    Account ac;
    bool found = false;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getBalance() < threshold) {
            ac.showAccount();
            cout << "---------------------------\n";
            found = true;
        }
    }
    if (!found) cout << "No accounts found below threshold.\n";
}

void displayTotalBalance() {
    double total = 0.0;
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        total += ac.getBalance();
    }
    cout << "Total balance across all accounts: " << total << "\n";
}

int main() {
    char choice;
    int num;
    do {
        cout << "\n\n\tBANK MANAGEMENT SYSTEM";
        cout << "\n1. New Account";
        cout << "\n2. Deposit Amount";
        cout << "\n3. Withdraw Amount";
        cout << "\n4. Balance Enquiry";
        cout << "\n5. All Accounts";
        cout << "\n6. Close Account";
        cout << "\n7. Modify Account";
        cout << "\n8. Apply Loan";
        cout << "\n9. Mini Statement";
        cout << "\nA. Top 3 Accounts by Balance";
        cout << "\nB. Accounts Below Threshold";
        cout << "\nC. Total Balance Across All Accounts";
        cout << "\n0. Exit";
        cout << "\nSelect Your Option: ";
        cin >> choice;

        switch (toupper(choice)) {
            case '1': writeAccount(); break;
            case '2': cout << "Enter Account No: "; cin >> num; depositWithdraw(num, 1); break;
            case '3': cout << "Enter Account No: "; cin >> num; depositWithdraw(num, 2); break;
            case '4': cout << "Enter Account No: "; cin >> num; displayAccount(num); break;
            case '5': displayAll(); break;
            case '6': cout << "Enter Account No: "; cin >> num; deleteAccount(num); break;
            case '7': cout << "Enter Account No: "; cin >> num; modifyAccount(num); break;
            case '8': cout << "Enter Account No: "; cin >> num; loanSection(num); break;
            case '9': cout << "Enter Account No: "; cin >> num; miniStatement(num); break;
            case 'A': displayTop3AccountsByBalance(); break;
            case 'B': displayLowBalanceAccounts(); break;
            case 'C': displayTotalBalance(); break;
            case '0': cout << "\nThank you for using the system!\n"; break;
            default: cout << "\nInvalid Option!\n";
        }
    } while (choice != '0');

    return 0;
}
