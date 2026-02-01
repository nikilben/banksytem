#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
public:
    int accNo;
    char name[50];
    double balance;

    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accNo;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << "Account Number: " << accNo << endl;
        cout << "Account Holder: " << name << endl;
        cout << "Balance: " << balance << endl;
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        balance -= amt;
    }
};

void writeAccount() {
    Account ac;
    ofstream outFile("accounts.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.accNo == n) {
            ac.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "Account not found\n";
}

void depositWithdraw(int n, int option) {
    Account ac;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    while (!file.eof() && !found) {
        file.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.accNo == n) {
            double amt;
            cout << "Enter amount: ";
            cin >> amt;
            if (option == 1)
                ac.deposit(amt);
            else
                ac.withdraw(amt);
            int pos = (-1) * static_cast<int>(sizeof(Account));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "Account not found\n";
}

void deleteAccount(int n) {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.accNo != n)
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    }
    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
}

void displayAll() {
    Account ac;
    ifstream inFile("accounts.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.showAccount();
        cout << "----------------------\n";
    }
    inFile.close();
}

int main() {
    int choice;
    do {
        cout << "\nBANKING SYSTEM\n";
        cout << "1. Create Account\n";
        cout << "2. Display Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Delete Account\n";
        cout << "6. Display All Accounts\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int num;
        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2:
            cout << "Enter Account Number: ";
            cin >> num;
            displayAccount(num);
            break;
        case 3:
            cout << "Enter Account Number: ";
            cin >> num;
            depositWithdraw(num, 1);
            break;
        case 4:
            cout << "Enter Account Number: ";
            cin >> num;
            depositWithdraw(num, 2);
            break;
        case 5:
            cout << "Enter Account Number: ";
            cin >> num;
            deleteAccount(num);
            break;
        case 6:
            displayAll();
            break;
        case 7:
            break;
        default:
            cout << "Invalid choice\n";
        }
    } while (choice != 7);

    return 0;
}
