#include <iostream>
#include <stack>
#include <queue>
#include <string>
using namespace std;

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

// --- Testing Main (Stack only) ---
int main() {
    stack<string> inventory;
    queue<string> shipping;

    int choice;
    do {
        cout << "\n=== Stack Tester (Inventory) ===\n";
        cout << "1. Add Incoming Item\n";
        cout << "2. Process Incoming Item\n";
        cout << "3. View Last Incoming Item\n";
        cout << "4. Exit (stack test)\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addIncoming(inventory); break;
            case 2: processIncoming(inventory, shipping); break;
            case 3: viewLastIncoming(inventory); break;
            case 4: cout << "Exiting stack tester...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
