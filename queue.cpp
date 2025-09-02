#include <iostream>
using namespace std;

//outgoing shipping - delilah -

class Queue{
    private:
        static const int MAX = 100; // max size
        string arr[MAX];
        int front, rear;

    public:
        Queue(){
            front = -1;
            rear = -1;
        }

        bool isEmpty(){
            return (front == -1 && rear == -1);
        }

        bool isFull(){
            return (rear == MAX -1);
        }

        void enqueue(string item){
            if (isFull()){
                cout << "Queue overflow..Cannot add more items.\n";
                return;
            }

            if (isEmpty()){
                front = 0;
                rear = 0;
            } else {
                rear++;
            }
            arr[rear] = item;
            cout << "Item \"" << item << "\" added to shipping queue.\n";
        }

        void dequeue(){
            if (isEmpty()){
                cout << "Queue underflow..No items to ship.\n";
                return;
            }
            cout << "Shipping item: " << arr[front] << endl;

            if (front == rear){
                // to reset queue after last item
                front = -1;
                rear = -1;
            } else {
                front++;
            }
        }

        void peekNextItem() {
            if (isEmpty()) {
                cout << "No items in shipping queue.\n";
            } else {
                cout << " Next item to ship: " << arr[front] << endl;
            }
            
        }

        void displayQueue() {
            if (isEmpty()) {
                cout << " Queue is empty \n";
                return;
            }
            cout << " Current shipping queue: ";
            for (int i = front; i <= rear; i++) {
                cout << arr[i] << " ";
            }
            cout << endl;
        }
};

//my own testing

int main() {
    Queue q;

    cout << "--cuba try test--\n";
    q.enqueue("Dell");
    q.enqueue("HP");
    q.enqueue("Apple");

    q.displayQueue();
    q.peekNextItem();

    q.dequeue();
    q.displayQueue();

    q.dequeue();
    q.dequeue();
    q.dequeue(); // trying to dequeue from empty queue

    return 0;
}