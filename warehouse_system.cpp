#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

/* --------- helpers --------- */
static string trim(const string &s)
{
    size_t b = 0, e = s.size();
    while (b < e && isspace(static_cast<unsigned char>(s[b])))
        ++b;
    while (e > b && isspace(static_cast<unsigned char>(s[e - 1])))
        --e;
    return s.substr(b, e - b);
}

static int readMenuChoice(int lo, int hi)
{
    int choice;
    while (true)
    {
        cout << "\nMenu:\n"
                "1. Add Incoming Item\n"
                "2. Process Incoming Item\n"
                "3. Ship Item\n"
                "4. View Last Incoming Item\n"
                "5. View Next Shipment\n"
                "6. Exit\n"
                "Enter your choice: ";
        if (cin >> choice)
        {
            // drop leftover chars (e.g., spaces) up to newline
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice >= lo && choice <= hi)
                return choice;
            cout << "Please enter a number " << lo << '-' << hi << ".\n";
        }
        else
        {
            // user typed letters or junk -> clear and flush entire bad line
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
        }
    }
}

/* --------- Stack (incoming, LIFO) --------- */
class Stack
{
private:
    static const int MAX = 100;
    string items[MAX];
    int top;

public:
    Stack() : top(-1) {}

    void pushItem(const string &item)
    {
        // Prevent overflow by checking capacity first
        if (isFull())
        {
            cout << "Error: Inventory full!\n";
            return;
        }
        items[++top] = item;
        cout << "Item \"" << item << "\" added to inventory.\n";
    }

    string popItem()
    {
        if (isEmpty())
        {
            cout << "Error: No items in inventory to process.\n";
            return "";
        }
        return items[top--];
    }

    string peekLastItem()
    {
        if (isEmpty())
            return "No incoming items.";
        return items[top];
    }

    void displayStack()
    {
        if (isEmpty())
        {
            cout << "Inventory is empty.\n";
            return;
        }
        cout << "Current Inventory (Top to Bottom):\n";
        for (int i = top; i >= 0; --i)
        {
            cout << " - " << items[i] << "\n";
        }
    }

    bool isEmpty() { return (top < 0); }
    bool isFull() const { return top >= MAX - 1; } // check for stack overflow
};

/* --------- Queue (shipping, FIFO) --------- */
class Queue
{
private:
    static const int MAX = 100;
    string items[MAX];
    int front, rear, count; // circular buffer
public:
    Queue() : front(0), rear(-1), count(0) {}

    void enqueue(const string &item)
    {
        // Prevent overflow by ensuring capacity
        if (isFull())
        {
            cout << "Error: Shipping queue full!\n";
            return;
        }
        rear = (rear + 1) % MAX;
        items[rear] = item;
        ++count;
        cout << "Processed \"" << item << "\" and added to shipping queue.\n";
    }

    string dequeue()
    {
        if (isEmpty())
        {
            cout << "No items to ship.\n";
            return "";
        }
        string item = items[front];
        front = (front + 1) % MAX;
        --count;
        cout << "Shipping item: " << item << "\n";
        return item;
    }

    string peekNextItem()
    {
        if (isEmpty())
            return "No items in shipping queue.";
        return items[front];
    }

    void displayQueue()
    {
        if (isEmpty())
        {
            cout << "Shipping queue is empty.\n";
            return;
        }
        cout << "Current Shipping Queue (Front to Rear):\n";
        for (int i = 0; i < count; ++i)
        {
            int idx = (front + i) % MAX;
            cout << " - " << items[idx] << "\n";
        }
    }

    bool isEmpty() { return count == 0; }
    bool isFull() const { return count >= MAX; } // check for queue overflow
};

/* --------- main --------- */
int main()
{
    Stack inventory;
    Queue shipping;

    cout << "=== Warehouse Inventory and Shipping System ===\n";

    while (true)
    {
        int choice = readMenuChoice(1, 6);

        switch (choice)
        {
        case 1:
        { // Add Incoming Item
            cout << "Enter item name: ";
            string name;
            getline(cin, name);
            name = trim(name);
            if (!name.empty())
                inventory.pushItem(name);
            else
                cout << "Item name cannot be empty.\n";
            break;
        }

        case 2:
        { // Process Incoming (Stack -> Queue)
            if (!inventory.isEmpty())
            {
                string processed = inventory.popItem();
                if (!processed.empty())
                    shipping.enqueue(processed);
            }
            else
            {
                cout << "No items in inventory to process.\n";
            }
            break;
        }

        case 3:
        { // Ship Item
            shipping.dequeue();
            break;
        }

        case 4:
        { // View Last Incoming
            cout << "Last incoming item: " << inventory.peekLastItem() << "\n";
            break;
        }

        case 5:
        { // View Next Shipment
            cout << "Next item to ship: " << shipping.peekNextItem() << "\n";
            break;
        }

        case 6:
            cout << "Exiting...\n";
            return 0;
        }
    }
}

