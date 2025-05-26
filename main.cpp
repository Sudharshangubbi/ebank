#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <algorithm>
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
    Account() {
        accNumber = 0;
        name = "";
        type = 'S';
        balance = 0.0;
        loanAmount = 0.0;
        statement = "";
    }

    void createAccount() {
        cout << "\nEnter The account No. : ";
        cin >> accNumber;
        cout << "\nEnter The Name of The account Holder : ";
        cin.ignore();
        getline(cin, name);
        cout << "\nEnter Type of The account (C/S) : ";
        cin >> type;
        type = toupper(type);
        cout << "\nEnter The Initial amount (>=500 for Saving and >=1000 for Current): ";
        cin >> balance;
        while ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000)) {
            cout << "Insufficient balance. Please re-enter: ";
            cin >> balance;
        }
        cout << "\nAccount Created Successfully.\n";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accNumber;
        cout << "\nHolder Name : " << name;
        cout << "\nType : " << type;
        cout << "\nBalance : " << balance;
        cout << "\nLoan Taken : " << loanAmount;
    }

    void modify() {
        cout << "\nModify Account Holder Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Modify Type (C/S): ";
        cin >> type;
        type = toupper(type);
        cout << "Modify Balance: ";
        cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
        addStatement("Deposited", amt);
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << "Insufficient balance!";
            return;
        }
        balance -= amt;
        addStatement("Withdrawn", amt);
    }

    void addLoan(double amt) {
        loanAmount += amt;
        addStatement("Loan taken", amt);
    }

    void addStatement(string action, double amt) {
        time_t now = time(0);
        char* dt = ctime(&now);
        statement += action + " of " + to_string(amt) + " on " + dt;
    }

    void displayStatement() const {
        cout << "\nMini Statement:\n" << statement;
    }

    int getAccountNumber() const { return accNumber; }
    double getBalance() const { return balance; }
    char getType() const { return type; }
    string getName() const { return name; }
};

// File Operations
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
        }
    }
    inFile.close();
    if (!found) cout << "\nAccount Not Found!\n";
}

void modifyAccount(int n) {
    Account ac;
    fstream File("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!File.eof()) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            cout << "\nEnter New Details:\n";
            ac.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            found = true;
            cout << "\nAccount Updated.\n";
            break;
        }
    }
    File.close();
    if (!found) cout << "\nRecord Not Found\n";
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
    cout << "\nAccount Deleted.\n";
}

void displayAll() {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    cout << "\nACCOUNT HOLDER LIST:\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.showAccount();
        cout << "\n-------------------------------\n";
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    Account ac;
    double amt;
    fstream File("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!File.eof()) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            if (option == 1) {
                cout << "\nEnter amount to deposit: "; cin >> amt;
                ac.deposit(amt);
            } else {
                cout << "\nEnter amount to withdraw: "; cin >> amt;
                ac.withdraw(amt);
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nTransaction successful.\n";
            found = true;
            break;
        }
    }
    File.close();
    if (!found) cout << "\nAccount Not Found.\n";
}

void loanSection(int n) {
    Account ac;
    double amount;
    fstream File("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!File.eof()) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            cout << "\nEnter loan amount: ";
            cin >> amount;
            ac.addLoan(amount);
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nLoan Granted.\n";
            found = true;
            break;
        }
    }
    File.close();
    if (!found) cout << "\nAccount Not Found.\n";
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
    cout << "\nAccount Not Found.\n";
}

// Extra Features
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
        cout << "\n-----------------------------\n";
    }
}

void displayLowBalanceAccounts() {
    double threshold;
    cout << "\nEnter balance threshold: ";
    cin >> threshold;

    Account ac;
    ifstream inFile("account.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getBalance() < threshold) {
            ac.showAccount();
            cout << "\n---------------------------\n";
            found = true;
        }
    }
    inFile.close();
    if (!found) cout << "\nNo accounts below threshold.\n";
}

// Main Menu
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
        cout << "\nB. Accounts below Threshold";
        cout << "\n0. Exit";
        cout << "\nSelect Your Option: ";
        cin >> choice;

        switch (toupper(choice)) {
            case '1': writeAccount(); break;
            case '2': cout << "Enter account no: "; cin >> num; depositWithdraw(num, 1); break;
            case '3': cout << "Enter account no: "; cin >> num; depositWithdraw(num, 2); break;
            case '4': cout << "Enter account no: "; cin >> num; displayAccount(num); break;
            case '5': displayAll(); break;
            case '6': cout << "Enter account no: "; cin >> num; deleteAccount(num); break;
            case '7': cout << "Enter account no: "; cin >> num; modifyAccount(num); break;
            case '8': cout << "Enter account no: "; cin >> num; loanSection(num); break;
            case '9': cout << "Enter account no: "; cin >> num; miniStatement(num); break;
            case 'A': displayTop3AccountsByBalance(); break;
            case 'B': displayLowBalanceAccounts(); break;
            case '0': cout << "\nThank you for using the system!\n"; break;
            default: cout << "\nInvalid Option!\n";
        }
    } while (choice != '0');

    return 0;
}
