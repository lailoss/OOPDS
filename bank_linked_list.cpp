/*
 * Bank Account Management System
 * --------------------------------
 * Implements a simple banking system using a singly linked list.
 * Each node represents a bank account and the Bank class manages
 * account operations such as adding, searching, and transactions.
 */
#include <iostream>
#include <string>
#include <limits>
using namespace std;

// -------------------- Singly Linked List Node --------------------
// Represents a single bank account in the list.
class Node
{
public:
    int accNo;       // Account number (unique)
    string name;     // Account holder's name
    long long balance; // Current balance
    Node *next;      // Pointer to next account in the list

    // Construct a node with given account data
    Node(int a, const string &n, long long b)
        : accNo(a), name(n), balance(b), next(nullptr) {}
};

// -------------------- Bank (manages the linked list) --------------------
class Bank
{
private:
    Node *head; // Head of the singly linked list

public:
    // Construct an empty bank
    Bank() : head(nullptr) {}

    // Destructor releases all dynamically allocated nodes
    ~Bank()
    {
        Node *cur = head;
        while (cur)
        {
            Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // Check if an account number already exists
    bool exists(int accNo) const
    {
        Node *cur = head;
        while (cur)
        {
            if (cur->accNo == accNo)
                return true;
            cur = cur->next;
        }
        return false;
    }

    // Add a new account to the list (front insertion)
    bool addAccount(int accNo, const string &name, long long initial)
    {
        if (accNo <= 0 || initial < 0)
            return false;
        if (exists(accNo))
            return false; // duplicate

        Node *n = new (std::nothrow) Node(accNo, name, initial);
        if (!n)
            return false;

        // insert at front for O(1)
        n->next = head;
        head = n;
        return true;
    }

    // Deposit funds into an account
    bool deposit(int accNo, long long amount)
    {
        if (amount <= 0)
            return false;
        Node *cur = head;
        while (cur)
        {
            if (cur->accNo == accNo)
            {
                cur->balance += amount;
                return true;
            }
            cur = cur->next;
        }
        return false; // not found
    }

    // Withdraw funds from an account
    // returns:
    //   1  success
    //   0  account not found
    //  -1  insufficient funds or invalid amount
    int withdraw(int accNo, long long amount)
    {
        if (amount <= 0)
            return -1;
        Node *cur = head;
        while (cur)
        {
            if (cur->accNo == accNo)
            {
                if (cur->balance < amount)
                    return -1;
                cur->balance -= amount;
                return 1;
            }
            cur = cur->next;
        }
        return 0; // not found
    }

    // Locate an account by number
    const Node *search(int accNo) const
    {
        Node *cur = head;
        while (cur)
        {
            if (cur->accNo == accNo)
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    // Remove account with the given number
    bool deleteAccount(int accNo)
    {
        if (!head)
            return false;

        // If the head is the target
        if (head->accNo == accNo)
        {
            Node *t = head;
            head = head->next; // Move head to next node
            delete t;          // Delete the old head
            return true;
        }

        // Otherwise, iterate through the list and find the target
        Node *cur = head;
        while (cur && cur->next)
        {
            if (cur->next->accNo == accNo)
            {
                Node *t = cur->next;         // Node to be deleted
                cur->next = cur->next->next; // Skip over the target node
                delete t;                    // Delete the target node
                return true;
            }
            cur = cur->next; // Move to the next node
        }
        return false; // Account not found
    }

    // Print all accounts in the bank
    void displayAll() const
    {
        if (!head)
        {
            cout << "No accounts found.\n";
            return;
        }
        Node *cur = head;
        while (cur)
        {
            cout << "Account No: " << cur->accNo
                 << "; Name: " << cur->name
                 << "; Balance: " << cur->balance << "\n";
            cur = cur->next;
        }
    }
};

// -------------------- Input helpers --------------------
// Clear leftover input from the stream
void clearStdin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Prompt user to retry an operation
bool askRetry()
{
    cout << "Do you want to retry? y/n: ";
    char ch;
    cin >> ch;
    clearStdin();
    return (ch == 'y' || ch == 'Y');
}

// Read an integer from user with validation
int readInt(const string &prompt)
{
    while (true)
    {
        cout << prompt;
        int x;
        if (cin >> x)
        {
            clearStdin();
            return x;
        }
        cout << "Invalid input. ";
        clearStdin();
        if (!askRetry())
            return -1;
    }
}

// Read a long long from user with validation
long long readLL(const string &prompt)
{
    while (true)
    {
        cout << prompt;
        long long x;

        if (cin >> x)
        {
            clearStdin();
            return x;
        }
        else
        {

            cout << "Invalid input. Please enter a valid number.\n";
            clearStdin();

            if (!askRetry())
                return -1;
        }
    }
}

// Read a non-empty line of text from the user
string readLine(const string &prompt)
{
    while (true)
    {
        cout << prompt;
        string s;
        getline(cin, s);

        // Simple validation: input should not be empty
        if (!s.empty())
            return s;

        cout << "Invalid input. Please enter something." << endl;
        if (!askRetry())
            return ""; // Exit if user doesn't want to retry
    }
}

// -------------------- Display Menu --------------------
// Menu-driven interface for user interaction
void displayMenu(Bank &bank)
{
    while (true)
    {
        cout << "\n--- MENU ---\n"
             << "1. Add Account\n"
             << "2. Display Accounts\n"
             << "3. Deposit\n"
             << "4. Withdraw\n"
             << "5. Search Account\n"
             << "6. Delete Account\n"
             << "7. Exit\n"
             << "Enter choice: ";

        int choice;
        if (!(cin >> choice))
        {
            cout << "Invalid choice.\n";
            clearStdin();
            if (!askRetry())
                break;
            else
                continue;
        }
        clearStdin();

        if (choice == 1)
        {
            // Add Account
            do
            {
                int acc = readInt("Enter unique account number: ");
                if (acc == -1)
                    break;
                string name = readLine("Enter full name: ");
                if (name.empty())
                    break;
                long long bal = readLL("Enter initial balance (>= 0): ");
                if (bal < 0)
                {
                    cout << "Invalid amount.\n";
                    if (!askRetry())
                        break;
                    else
                        continue;
                }

                if (bank.addAccount(acc, name, bal))
                {
                    cout << "Account added.\n";
                    break;
                }
                else
                {
                    cout << "Add failed (duplicate number or invalid data).\n";
                }
            } while (askRetry());
        }
        else if (choice == 2)
        {
            bank.displayAll();
        }
        else if (choice == 3)
        {
            // Deposit
            do
            {
                int acc = readInt("Enter account number: ");
                if (acc == -1)
                    break;
                long long amt = readLL("Enter deposit amount (> 0): ");
                if (amt <= 0)
                {
                    cout << "Invalid amount.\n";
                    if (!askRetry())
                        break;
                    else
                        continue;
                }

                if (bank.deposit(acc, amt))
                {
                    cout << "Deposit successful.\n";
                    break;
                }
                else
                {
                    cout << "Deposit failed (account not found or invalid amount).\n";
                }
            } while (askRetry());
        }
        else if (choice == 4)
        {
            // Withdraw
            do
            {
                int acc = readInt("Enter account number: ");
                if (acc == -1)
                    break;
                long long amt = readLL("Enter withdrawal amount (> 0): ");
                if (amt <= 0)
                {
                    cout << "Invalid amount.\n";
                    if (!askRetry())
                        break;
                    else
                        continue;
                }

                int r = bank.withdraw(acc, amt);
                if (r == 1)
                {
                    cout << "Withdraw successful.\n";
                    break;
                }
                else if (r == 0)
                {
                    cout << "Account not found.\n";
                }
                else
                {
                    cout << "Insufficient funds or invalid amount.\n";
                }
            } while (askRetry());
        }
        else if (choice == 5)
        {
            // Search
            do
            {
                int acc = readInt("Enter account number to search: ");
                if (acc == -1)
                    break;
                auto node = bank.search(acc);
                if (node)
                {
                    cout << "Account No: " << node->accNo
                         << "; Name: " << node->name
                         << "; Balance: " << node->balance << "\n";
                    break;
                }
                else
                {
                    cout << "Account not found.\n";
                }
            } while (askRetry());
        }
        else if (choice == 6)
        {
            // Delete
            do
            {
                int acc = readInt("Enter account number to delete: ");
                if (acc == -1)
                    break;
                if (bank.deleteAccount(acc))
                {
                    cout << "Account deleted.\n";
                    break;
                }
                else
                {
                    cout << "Delete failed (account not found).\n";
                }
            } while (askRetry());
        }
        else if (choice == 7)
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid choice.\n";
            if (!askRetry())
                break;
        }
    }
}

// -------------------- Main menu --------------------
// Entry point of the program
int main()
{
    Bank bank;
    displayMenu(bank);

    return 0;
}