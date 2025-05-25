// changes by shreyas
// Structured account handling.
//Modular code with class-based architecture.
//Loan and statement tracking.
//Update, search, and display capabilities.
//File-based persistence using binary I/O.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
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
        cout << "\n\nEnter The Name of The account Holder : ";
        cin.ignore();
        getline(cin, name);
        cout << "\nEnter Type of The account (C/S) : ";
        cin >> type;
        type = toupper(type);
        cout << "\nEnter The Initial amount (>=500 for Saving and >=1000 for Current ): ";
        cin >> balance;
        while ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000)) {
            cout << "Insufficient balance. Please re-enter: ";
            cin >> balance;
        }
        cout << "\n\nAccount Created..";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accNumber;
        cout << "\nAccount Holder Name : " << name;
        cout << "\nType of Account : " << type;
        cout << "\nBalance amount : " << balance;
        cout << "\nLoan Taken : " << loanAmount;
    }

    void modify() {
        cout << "\nAccount No. : " << accNumber;
        cout << "\nModify Account Holder Name : ";
        cin.ignore();
        getline(cin, name);
        cout << "\nModify Type of Account : ";
        cin >> type;
        type = toupper(type);
        cout << "\nModify Balance amount : ";
        cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
        addStatement("Deposited", amt);
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << "\nInsufficient balance!";
            return;
        }
        balance -= amt;
        addStatement("Withdrawn", amt);
    }

    void addLoan(double amt) {
        loanAmount += amt;
        addStatement("Loan taken", amt);
    }

    void displayStatement() const {
        cout << "\nMini Statement:\n";
        cout << statement;
    }

    void addStatement(string action, double amt) {
        time_t now = time(0);
        char* dt = ctime(&now);
        statement += action + " of " + to_string(amt) + " on " + dt;
    }

    int getAccountNumber() const {
        return accNumber;
    }

    double getBalance() const {
        return balance;
    }

    char getType() const {
        return type;
    }
};

void writeAccount() {
    Account ac;
    ofstream outFile("account.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account ac;
    bool found = false;
    ifstream inFile("account.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n\nAccount number does not exist";
}

void modifyAccount(int n) {
    Account ac;
    fstream File;
    bool found = false;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    while (!File.eof() && found == false) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            cout << "\n\nEnter The New Details of account" << endl;
            ac.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() != n) {
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("temp.dat", "account.dat");
    cout << "\n\n\tRecord Deleted ..";
}

void displayAll() {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type  Balance  Loan\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        cout << setw(10) << ac.getAccountNumber() << " ";
        cout << setw(15) << ac.getType() << " ";
        cout << setw(5) << ac.getType() << " ";
        cout << setw(10) << fixed << setprecision(2) << ac.getBalance() << " ";
        cout << endl;
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    Account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    while (!File.eof() && found == false) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            if (option == 1) {
                cout << "\n\n\tTO DEPOSIT AMOUNT ";
                cout << "\n\nEnter The amount to be deposited: ";
                cin >> amt;
                ac.deposit(amt);
            } else if (option == 2) {
                cout << "\n\n\tTO WITHDRAW AMOUNT ";
                cout << "\n\nEnter The amount to be withdrawn: ";
                cin >> amt;
                ac.withdraw(amt);
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void loanSection(int n) {
    Account ac;
    fstream File;
    double amount;
    bool found = false;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    while (!File.eof() && found == false) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.getAccountNumber() == n) {
            ac.showAccount();
            cout << "\nEnter loan amount to apply: ";
            cin >> amount;
            ac.addLoan(amount);
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nLoan processed successfully!";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\nAccount not found!";
}

void miniStatement(int n) {
    Account ac;
    ifstream inFile("account.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.getAccountNumber() == n) {
            ac.displayStatement();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n\nAccount not found!";
}

int main() {
    char ch;
    int num;
    do {
        system("cls");
        cout << "\n\n\tBANK MANAGEMENT SYSTEM";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. APPLY FOR LOAN";
        cout << "\n\n\t09. MINI STATEMENT";
        cout << "\n\n\t10. EXIT";
        cout << "\n\n\tSelect Your Option (1-10): ";
        cin >> ch;
        system("cls");
        switch (ch) {
            case '1':
                writeAccount();
                break;
            case '2':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                depositWithdraw(num, 1);
                break;
            case '3':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                depositWithdraw(num, 2);
                break;
            case '4':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                displayAccount(num);
                break;
            case '5':
                displayAll();
                break;
            case '6':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                deleteAccount(num);
                break;
            case '7':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                modifyAccount(num);
                break;
            case '8':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                loanSection(num);
                break;
            case '9':
                cout << "\n\n\tEnter The account No. : "; cin >> num;
                miniStatement(num);
                break;
            case '10':
                cout << "\n\n\tThanks for using bank management system!";
                break;
            default:
                cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '10');
    return 0;
}

