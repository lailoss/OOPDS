#include <iostream>
#include <string>
using namespace std;

class Stack {
private:
    static const int MAX = 100;  
    string items[MAX];
    int top;
public:
    Stack() { top = -1; }


    void pushItem(string item) {
        if (top >= MAX - 1) {
            cout << "Error: Inventory full!\n";
            return;
        }
        items[++top] = item;
        cout << "Item \"" << item << "\" added to inventory.\n";
    }

 
    string popItem() {
        if (isEmpty()) {
            cout << "Error: No items in inventory to process.\n";
            return "";
        }
        return items[top--];
    }

    
    string peekLastItem() {
        if (isEmpty()) return "No incoming items.";
        return items[top];
    }

    
    bool isEmpty() { return (top < 0); }
};


class Queue {
private:
    static const int MAX = 100;   
    string items[MAX];
    int front, rear, count;
public:
    Queue() {
        front = 0;
        rear = -1;
        count = 0;
    }

   
    void enqueue(string item) {
        if (count >= MAX) {
            cout << "Error: Shipping queue full!\n";
            return;
        }
        rear = (rear + 1) % MAX;
        items[rear] = item;
        count++;
        cout << "Processed \"" << item << "\" and added to shipping queue.\n";
    }

    string dequeue() {
        if (isEmpty()) {
            cout << "No items to ship.\n";
            return "";
        }
        string item = items[front];
        front = (front + 1) % MAX;
        count--;
        cout << "Shipping item: " << item << endl;
        return item;
    }


    string peekNextItem() {
        if (isEmpty()) return "No items in shipping queue.";
        return items[front];
    }

    bool isEmpty() { return (count == 0); }
};


int main() {
    Stack inventory;
    Queue shipping;
    int choice;
    string item;

    cout << "=== Warehouse Inventory and Shipping System ===\n";

    do {
        
        cout << "\nMenu:\n";
        cout << "1. Add Incoming Item\n";
        cout << "2. Process Incoming Item\n";
        cout << "3. Ship Item\n";
        cout << "4. View Last Incoming Item\n";
        cout << "5. View Next Shipment\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  

        switch (choice) {
            case 1:
                cout << "Enter item name: ";
                getline(cin, item);
                inventory.pushItem(item);
                break;
            case 2:
                if (!inventory.isEmpty()) {
                    string processed = inventory.popItem();
                    if (processed != "") shipping.enqueue(processed);
                } else {
                    cout << "No items in inventory to process.\n";
                }
                break;
            case 3:
                shipping.dequeue();
                break;
            case 4:
                cout << "Last incoming item: " 
                     << inventory.peekLastItem() << endl;
                break;
            case 5:
                cout << "Next item to ship: " 
                     << shipping.peekNextItem() << endl;
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
