#include <iostream>
#include <stack>
#include <queue>
#include <string>
using namespace std;

class Stack {
    private:
        static const int MAX = 100;   // Maximum stack size
        string items[MAX];
        int top;
    public:
        Stack() { top = -1; }

    // Add item to stack (Push)
    void pushItem(string item) {
        if (top >= MAX - 1) {
            cout << "Error: Inventory full!\n";
            return;
        }
        items[++top] = item;
        cout << "Item \"" << item << "\" added to inventory.\n";
    }

    // Remove item from stack (Pop)
    string popItem() {
        if (isEmpty()) {
            cout << "Error: No items in inventory to process.\n";
            return "";
        }
        return items[top--];
    }

    // View last incoming item (Peek)
    string peekLastItem() {
        if (isEmpty()) return "No incoming items.";
        return items[top];
    }

    // Check if stack empty
    bool isEmpty() { return (top < 0); }
};

void addIncoming(stack<string>& inventory) {
    string item;
    cout << "Enter item name: ";
    getline(cin, item);
    inventory.push(item);
    cout << "Item \"" << item << "\" added to inventory.\n";
}

void processIncoming(stack<string>& inventory, queue<string>& shipping) {
    if (inventory.empty()) {
        cout << "No items in inventory to process.\n";
    } else {
        string item = inventory.top();
        inventory.pop();
        shipping.push(item);
        cout << "Processed \"" << item << "\" and added to shipping queue.\n";
    }
}

void viewLastIncoming(stack<string>& inventory) {
    if (inventory.empty()) {
        cout << "No incoming items.\n";
    } else {
        cout << "Last incoming item: " << inventory.top() << endl;
    }
}


int main() {
    Stack inventory;
    queue<string> shipping;

    int choice;
    do {
        cout << "\n=== Warehouse Inventory & Shipping System ===\n";
        cout << "1. Add Incoming Item\n";
        cout << "2. Process Incoming Item\n";
        cout << "3. View Last Incoming Item\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: {
                string item;
                cout << "Enter item name: ";
                getline(cin, item);
                inventory.pushItem(item);
                break;
            }
            case 2: {
                string item = inventory.popItem();
                if (!item.empty()) {
                    shipping.push(item);
                    cout << "Processed \"" << item << "\" and added to shipping queue.\n";
                }
                break;
            }
            case 3:
                cout << "Last incoming item: " << inventory.peekLastItem() << endl;
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 4);

    return 0;
}
