#include <iostream>
#include <string>
#include <ctime>     // for time/ctime
using namespace std;

// ======================= Log nodes (singly linked) =======================
struct LogNode {
    string text;
    LogNode* next;
    LogNode(const string& t) : text(t), next(NULL) {}
};

// Keep logs of deleted accounts
struct DeletedLogEntry {
    int accNo;
    LogNode* logs;           // head of the logs list
    DeletedLogEntry* next;
    DeletedLogEntry(int a, LogNode* h) : accNo(a), logs(h), next(NULL) {}
};

// ======================= Account (OOP) =======================
class Account {
public:
    int accNo;
    string name;
    string cnic;      // like xxxxx-xxxxxxx-x
    char gender;      // 'M' or 'F'
    string typeCS;    // "C" or "S"
    int pin;
    long long balance;
    LogNode* logHead; // singly linked list of logs

    Account(int a, const string& nm, const string& c, char g, const string& t, int p, long long b)
    : accNo(a), name(nm), cnic(c), gender(g), typeCS(t), pin(p), balance(b), logHead(NULL) {}

    void addLog(const string& msg) {
        // prepend for simplicity
        LogNode* n = new LogNode(msg);
        n->next = logHead;
        logHead = n;
    }

    void printBrief() const {
        cout << "Account No: " << accNo
             << "; Name: " << name
             << "; Balance: " << balance << endl;
    }

    void printFull() const {
        cout << "Account No: " << accNo
             << "; Name: " << name
             << "; CNIC: " << cnic
             << "; Gender: " << gender
             << "; Type: " << typeCS
             << "; PIN: " << pin
             << "; Balance: " << balance << endl;
    }
};

// ======================= List Node for accounts =======================
struct Node {
    Account* data; // store pointer so we can move logs easily when deleting
    Node* next;
    Node(Account* acc) : data(acc), next(NULL) {}
};

// ======================= Bank (singly linked list + deleted logs) =======================
class Bank {
private:
    Node* head;                 // active accounts
    DeletedLogEntry* delHead;   // deleted accounts' logs

public:
    Bank() : head(NULL), delHead(NULL) {}

    ~Bank() {
        // free active accounts + logs
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            freeLogs(cur->data->logHead);
            delete cur->data;
            delete cur;
            cur = nxt;
        }
        // free deleted logs list
        DeletedLogEntry* d = delHead;
        while (d) {
            DeletedLogEntry* dn = d->next;
            freeLogs(d->logs);
            delete d;
            d = dn;
        }
    }

    void freeLogs(LogNode* h) {
        while (h) {
            LogNode* t = h;
            h = h->next;
            delete t;
        }
    }

    int findMaxAccNo() const {
        int mx = 0;
        Node* cur = head;
        while (cur) {
            if (cur->data->accNo > mx) mx = cur->data->accNo;
            cur = cur->next;
        }
        return mx;
    }

    Node* findNode(int accNo) const {
        Node* cur = head;
        while (cur) {
            if (cur->data->accNo == accNo) return cur;
            cur = cur->next;
        }
        return NULL;
    }

    // 1) Create account (prevent duplicates)
    bool addAccount(const string& name, const string& cnic, char gender,
                    const string& typeCS, int pin, long long balance, int& outAccNo) {
        if (balance < 500) return false;
        int accNo = findMaxAccNo() + 1;
        if (findNode(accNo) != NULL) return false; // very unlikely, but check
        Account* a = new Account(accNo, name, cnic, gender, typeCS, pin, balance);
        // Log create
        a->addLog(timestamp("Account created"));
        // insert at head
        Node* n = new Node(a);
        n->next = head;
        head = n;
        outAccNo = accNo;
        return true;
    }

    // 2) Display all
    void displayAll() const {
        if (!head) {
            cout << "No accounts found." << endl;
            return;
        }
        Node* cur = head;
        while (cur) {
            cur->data->printBrief();
            cur = cur->next;
        }
    }

    // 3) Search account -> print (full)
    bool printAccount(int accNo) const {
        Node* n = findNode(accNo);
        if (!n) return false;
        n->data->printFull();
        return true;
    }

    // PIN check: 1 ok, -1 bad pin, 0 not found
    int checkAccPin(int accNo, int pin) const {
        Node* n = findNode(accNo);
        if (!n) return 0;
        if (n->data->pin != pin) return -1;
        return 1;
    }

    // 4) Deposit
    // returns: 1 ok, 0 not found, -1 bad amount
    int deposit(int accNo, int pin, long long amount) {
        Node* n = findNode(accNo);
        if (!n) return 0;
        if (n->data->pin != pin) return -2; // PIN wrong (extra check)
        if (amount <= 0) return -1;
        long long before = n->data->balance;
        n->data->balance += amount;
        n->data->addLog(timestamp("Deposit +" + to_string(amount) +
                                  ", before=" + to_string(before) +
                                  ", after=" + to_string(n->data->balance)));
        return 1;
    }

    // 5) Withdraw
    // returns: 1 ok, 0 not found, -1 insufficient, -2 bad pin, -3 bad amount
    int withdraw(int accNo, int pin, long long amount) {
        Node* n = findNode(accNo);
        if (!n) return 0;
        if (n->data->pin != pin) return -2;
        if (amount <= 0) return -3;
        if (n->data->balance < amount) return -1;
        long long before = n->data->balance;
        n->data->balance -= amount;
        n->data->addLog(timestamp("Withdraw -" + to_string(amount) +
                                  ", before=" + to_string(before) +
                                  ", after=" + to_string(n->data->balance)));
        return 1;
    }

    // 6) Delete account (move its logs to deleted list so we can show later)
    bool deleteAccount(int accNo) {
        if (!head) return false;
        if (head->data->accNo == accNo) {
            Node* t = head; head = head->next;
            moveLogsToDeleted(t->data);
            delete t->data;
            delete t;
            return true;
        }
        Node* prev = head;
        Node* cur  = head->next;
        while (cur) {
            if (cur->data->accNo == accNo) {
                prev->next = cur->next;
                moveLogsToDeleted(cur->data);
                delete cur->data;
                delete cur;
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    // Edit user info
    // returns: 1 ok, 0 not found
    int changeInfo(int accNo, const string& newName, const string& newCnic,
                   char newGender, const string& newTypeCS, int newPIN) {
        Node* n = findNode(accNo);
        if (!n) return 0;
        n->data->name = newName;
        n->data->cnic = newCnic;
        n->data->gender = newGender;
        n->data->typeCS = newTypeCS;
        n->data->pin = newPIN;
        n->data->addLog(timestamp("Info changed"));
        return 1;
    }

    // For Admin "Show list" (simple)
    void printForAdmin() const {
        if (!head) {
            cout << "No accounts found." << endl;
            return;
        }
        cout << "ACC_Number | NAME | CNIC | GENDER | TYPE | PIN | BALANCE" << endl;
        Node* cur = head;
        while (cur) {
            Account* a = cur->data;
            cout << a->accNo << " | "
                 << a->name << " | "
                 << a->cnic << " | "
                 << a->gender << " | "
                 << a->typeCS << " | "
                 << a->pin << " | "
                 << a->balance << endl;
            cur = cur->next;
        }
    }

    // Logging helpers
    void insert_log(int accNo, const string& msg) {
        Node* n = findNode(accNo);
        if (!n) return;
        n->data->addLog(timestamp(msg));
    }

    // display1: return 1 if logs in deleted section exist (like your prototype)
    int display1(int accNo) const {
        const DeletedLogEntry* d = findDeleted(accNo);
        return d ? 1 : 0;
    }

    // display: print logs either from active account or from deleted-logs
    void display(int accNo) const {
        Node* n = findNode(accNo);
        if (n) {
            printLogs(n->data->logHead);
            return;
        }
        const DeletedLogEntry* d = findDeleted(accNo);
        if (d) {
            printLogs(d->logs);
            return;
        }
        cout << "Logs Not Found....!!!" << endl;
    }

private:
    string timestamp(const string& msg) const {
        time_t now = time(0);
        char* dt = ctime(&now); // has newline at end
        string t = msg + " at " + string(dt);
        // remove trailing newline for cleaner print (optional)
        if (!t.empty() && t.back() == '\n') t.pop_back();
        return t;
    }

    void printLogs(LogNode* h) const {
        if (!h) { cout << "[No logs]\n"; return; }
        LogNode* cur = h;
        while (cur) {
            cout << cur->text << endl;
            cur = cur->next;
        }
    }

    void moveLogsToDeleted(Account* a) {
        // prepend to deleted list (keep logs)
        DeletedLogEntry* e = new DeletedLogEntry(a->accNo, a->logHead);
        e->next = delHead;
        delHead = e;
        // detach logs from account so destructor won't free twice
        a->logHead = NULL;
    }

    const DeletedLogEntry* findDeleted(int accNo) const {
        const DeletedLogEntry* d = delHead;
        while (d) {
            if (d->accNo == accNo) return d;
            d = d->next;
        }
        return NULL;
    }
};

// ======================= Panels (simple loops, no goto) =======================
int admin_pswd = 1122;
int staff_pswd = 2233;

void admin_panel(Bank& bank);
void staff_panel(Bank& bank);
void atm_panel(Bank& bank);

int main() {
    Bank bank;

    // Seed 1 sample record (like your template)
    {
        int outAcc;
        bank.addAccount("M.ROHAN Farooqui","12345-6789101-1",'M',"C",1234,500, outAcc);
        bank.insert_log(outAcc, "Account seeded");
    }

    while (true) {
        int a;
        cout << "\n********** LOGIN PANEL **********\n";
        cout << "1. ADMIN Login\n";
        cout << "2. STAFF Login\n";
        cout << "3. ATM/CDM Service\n";
        cout << "4. Exit\n";
        cout << "Enter Your Choice: ";
        if (!(cin >> a)) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        if (a == 1) {
            int pin;
            cout << "Enter Admin Pin: "; cin >> pin;
            if (pin == admin_pswd) admin_panel(bank);
            else cout << "Wrong PIN.\n";
        } else if (a == 2) {
            int pin;
            cout << "Enter Staff Pin: "; cin >> pin;
            if (pin == staff_pswd) staff_panel(bank);
            else cout << "Wrong PIN.\n";
        } else if (a == 3) {
            atm_panel(bank);
        } else if (a == 4) {
            cout << "Bye!\n";
            break;
        }
    }
    return 0;
}

// ---------------- Admin ----------------
void admin_panel(Bank& bank) {
    while (true) {
        int b;
        cout << "\n********** ADMIN PANEL **********\n";
        cout << "1. Create Account\n";
        cout << "2. Delete Account\n";
        cout << "3. Search Account\n";
        cout << "4. Show All Accounts\n";
        cout << "5. Edit Information\n";
        cout << "6. Show Logs of Deleted Account\n";
        cout << "7. Back to Main Menu\n";
        cout << "Enter an Option: ";
        if (!(cin >> b)) { cin.clear(); cin.ignore(10000,'\n'); continue; }
        cin.ignore(10000,'\n'); // for getline after numbers

        if (b == 1) {
            string full_name, cnic, acc_type;
            char g;
            int pin;
            long long bal;
            cout << "Enter Customer's Full Name: "; getline(cin, full_name);
            cout << "Enter CNIC (xxxxx-xxxxxxx-x): "; getline(cin, cnic);
            cout << "Enter Gender (M/F): "; cin >> g; cin.ignore(10000,'\n');
            cout << "Enter Account Type (C/S): "; getline(cin, acc_type);
            cout << "Enter PIN: "; cin >> pin;
            cout << "Enter Balance (Min:500): "; cin >> bal;

            int accNoOut=0;
            if (bal < 500) {
                cout << "Minimum Balance is 500.\n";
            } else if (bank.addAccount(full_name, cnic, g, acc_type, pin, bal, accNoOut)) {
                cout << "Account created. Account No: " << accNoOut << endl;
            } else {
                cout << "Create failed (duplicate or memory).\n";
            }
        }
        else if (b == 2) {
            int acc;
            cout << "Enter Account Number to Delete: ";
            cin >> acc;
            if (bank.deleteAccount(acc)) cout << "Account deleted.\n";
            else cout << "Account not found.\n";
        }
        else if (b == 3) {
            int acc;
            cout << "Enter Account Number to Search: ";
            cin >> acc;
            if (!bank.printAccount(acc)) cout << "Account not found.\n";
        }
        else if (b == 4) {
            bank.displayAll();
        }
        else if (b == 5) {
            int acc, newPIN;
            string newName, newCnic, newType;
            char newGender;
            cout << "Enter Account Number: "; cin >> acc; cin.ignore(10000,'\n');
            cout << "Enter New Name: "; getline(cin, newName);
            cout << "Enter New CNIC: "; getline(cin, newCnic);
            cout << "Enter Gender (M/F): "; cin >> newGender; cin.ignore(10000,'\n');
            cout << "Enter Account Type (C/S): "; getline(cin, newType);
            cout << "Enter New PIN: "; cin >> newPIN;

            int r = bank.changeInfo(acc, newName, newCnic, newGender, newType, newPIN);
            if (r==1) cout << "Information changed.\n"; else cout << "Account not found.\n";
        }
        else if (b == 6) {
            int acc;
            cout << "Enter Account Number: ";
            cin >> acc;
            int hasDel = bank.display1(acc);
            bank.display(acc); // will print either active or deleted logs
            if (!hasDel) {
                // if not in deleted, we already printed active logs (or "[No logs]" or "Logs Not Found")
            }
        }
        else if (b == 7) {
            break;
        }
    }
}

// ---------------- Staff ----------------
void staff_panel(Bank& bank) {
    while (true) {
        int c;
        cout << "\n********** STAFF PANEL **********\n";
        cout << "1. Check Account Info\n";
        cout << "2. Deposit Cash\n";
        cout << "3. Withdraw Cash\n";
        cout << "4. Check Logs of User\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter an Option: ";
        if (!(cin >> c)) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        if (c == 1) {
            int acc; cout << "Enter Account Number: "; cin >> acc;
            if (!bank.printAccount(acc)) cout << "User not found.\n";
        }
        else if (c == 2) {
            int acc, pin; long long amt;
            cout << "Enter Account: "; cin >> acc;
            cout << "Enter Account PIN: "; cin >> pin;
            cout << "Enter Amount to Deposit: "; cin >> amt;
            // BEFORE
            cout << "\nStatus BEFORE Deposit:\n";
            bank.printAccount(acc);
            int res = bank.deposit(acc, pin, amt);
            if (res == 1) {
                cout << "\nStatus AFTER Deposit:\n";
                bank.printAccount(acc);
                cout << "Deposit successful.\n";
            } else if (res == 0) cout << "Account not found.\n";
            else if (res == -1) cout << "Invalid amount.\n";
            else if (res == -2) cout << "PIN incorrect.\n";
        }
        else if (c == 3) {
            int acc, pin; long long amt;
            cout << "Enter Account: "; cin >> acc;
            cout << "Enter Account PIN: "; cin >> pin;
            cout << "Enter Amount to Withdraw: "; cin >> amt;
            // BEFORE
            cout << "\nStatus BEFORE Withdraw:\n";
            bank.printAccount(acc);
            int res = bank.withdraw(acc, pin, amt);
            if (res == 1) {
                cout << "\nStatus AFTER Withdraw:\n";
                bank.printAccount(acc);
                cout << "Withdraw successful.\n";
            } else if (res == 0) cout << "Account not found.\n";
            else if (res == -1) cout << "Insufficient funds.\n";
            else if (res == -2) cout << "PIN incorrect.\n";
            else if (res == -3) cout << "Invalid amount.\n";
        }
        else if (c == 4) {
            int acc; cout << "Enter Account Number: "; cin >> acc;
            int inDeleted = bank.display1(acc);
            bank.display(acc);
            if (!inDeleted) {
                // active logs printed already (or message)
            }
        }
        else if (c == 5) {
            break;
        }
    }
}

// ---------------- ATM / CDM ----------------
void atm_panel(Bank& bank) {
    while (true) {
        int d;
        cout << "\n********** ATM / CDM **********\n";
        cout << "1. Deposit Cash (CDM)\n";
        cout << "2. Withdraw Cash (ATM)\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter an Option: ";
        if (!(cin >> d)) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        if (d == 1) {
            // CDM: Deposit Cash
            int acc, pin; long long amt;
            cout << "Enter Account Number: "; cin >> acc;
            cout << "Enter Account PIN: "; cin >> pin;
            cout << "Enter Amount to Deposit: "; cin >> amt;

            cout << "\nStatus BEFORE Deposit:\n";
            if (!bank.printAccount(acc)) {
                cout << "Account not found.\n";
                continue;
            }

            int res = bank.deposit(acc, pin, amt);
            if (res == 1) {
                cout << "\nStatus AFTER Deposit:\n";
                bank.printAccount(acc);
                cout << "Deposit successful.\n";
            } else if (res == 0) {
                cout << "Account not found.\n";
            } else if (res == -1) {
                cout << "Invalid amount.\n";
            } else if (res == -2) {
                cout << "PIN incorrect.\n";
            }
        }
        else if (d == 2) {
            // ATM: Withdraw Cash
            int acc, pin; long long amt;
            cout << "Enter Account Number: "; cin >> acc;
            cout << "Enter Account PIN: "; cin >> pin;
            cout << "Enter Amount to Withdraw: "; cin >> amt;

            cout << "\nStatus BEFORE Withdraw:\n";
            if (!bank.printAccount(acc)) {
                cout << "Account not found.\n";
                continue;
            }

            int res = bank.withdraw(acc, pin, amt);
            if (res == 1) {
                cout << "\nStatus AFTER Withdraw:\n";
                bank.printAccount(acc);
                cout << "Withdraw successful.\n";
            } else if (res == 0) {
                cout << "Account not found.\n";
            } else if (res == -1) {
                cout << "Insufficient funds.\n";
            } else if (res == -2) {
                cout << "PIN incorrect.\n";
            } else if (res == -3) {
                cout << "Invalid amount.\n";
            }
        }
        else if (d == 3) {
            break; // back to main
        }
        else {
            cout << "Invalid option.\n";
        }
    }
}

