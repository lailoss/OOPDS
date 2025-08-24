#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

// Structure to hold user account information
struct Account {
    string name;
    string birthdate;
    string mobileNumber;
    string city;
    long long int accountNumber;
    long long int balance;

    Account(string name, string birthdate, string mobileNumber, string city) {
        this->name = name;
        this->birthdate = birthdate;
        this->mobileNumber = mobileNumber;
        this->city = city;
        // Generate a random 11-digit account number
        srand(time(NULL));
        this->accountNumber = rand() % 9000000000 + 1000000000;
        this->balance = 0;
    }
};

// Function to create a new account
void createNewAccount(vector<Account>& accounts) {
    string name, birthdate, mobileNumber, city;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your birthdate (DD/MM/YYYY): ";
    getline(cin, birthdate);
    cout << "Enter your mobile number: ";
    getline(cin, mobileNumber);
    cout << "Enter your city: ";
    getline(cin, city);

    Account newAccount(name, birthdate, mobileNumber, city);
    accounts.push_back(newAccount);

    cout << "Account created successfully!" << endl;
    cout << "Your account number is: " << newAccount.accountNumber << endl;
}

// Function to delete an account
void deleteAccount(vector<Account>& accounts) {
    string birthdate;
    long long int accountNumber;

    cout << "Enter your birthdate (DD/MM/YYYY): ";
    cin.ignore();
    getline(cin, birthdate);
    cout << "Enter your account number: ";
    cin >> accountNumber;

    bool found = false;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].birthdate == birthdate && accounts[i].accountNumber == accountNumber) {
            accounts.erase(accounts.begin() + i);
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Your account has been deleted successfully." << endl;
    } else {
        cout << "No account found in this bank." << endl;
    }
}

// Function to deposit amount
void depositAmount(vector<Account>& accounts) {
    long long int accountNumber;
    long long int amount;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    bool found = false;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNumber == accountNumber) {
            cout << "Enter the amount to deposit: ";
            cin >> amount;
            accounts[i].balance += amount;
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Amount deposited successfully." << endl;
    } else {
        cout << "Not your account in this bank." << endl;
    }
}

// Function to check balance
void checkBalance(vector<Account>& accounts) {
    long long int accountNumber;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    bool found = false;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNumber == accountNumber) {
            cout << "Your account balance is: " << accounts[i].balance << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No account found in this bank." << endl;
    }
}

// Function to withdraw amount
void withdrawAmount(vector<Account>& accounts) {
    long long int accountNumber;
    long long int amount;

    cout << "Enter your account number: ";
    cin >> accountNumber;

    bool found = false;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNumber == accountNumber) {
            cout << "Enter the amount to withdraw: ";
            cin >> amount;

            if (accounts[i].balance >= amount) {
                accounts[i].balance -= amount;
                cout << "Amount withdrawn successfully." << endl;
            } else {
                cout << "Not enough amount in your account." << endl;
            }

            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No account found in this bank." << endl;
    }
}

int main() {
    vector<Account> accounts;

    int choice;
    while (true) {
        cout << "Banking System" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Delete Account" << endl;
        cout << "3. Deposit Amount" << endl;
        cout << "4. Check Balance" << endl;
        cout << "5. Withdraw Amount" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createNewAccount(accounts);
                break;
            case 2:
                deleteAccount(accounts);
                break;
            case 3:
                depositAmount(accounts);
                break;
            case 4:
                checkBalance(accounts);
                break;
            case 5:
                withdrawAmount(accounts);
                break;
            case 0:
                cout << "Thank you for using the banking system. Goodbye!" << endl;
                // Free memory by clearing the vector
                accounts.clear();
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << "----------------------" << endl;
    }

    return 0;
}
