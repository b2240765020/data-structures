#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    
    if(isFull()){  // doubling the capacity if it is full
        resize(2*capacity);
    }
    data[rear]=req;  //uploading the rear
    rear=nextIndex(rear);
    count++;  
    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    if(isEmpty()){ // we can't dequeue if the queue is empty
        return false;
    }
    outReq = data[front];  //updating the front
    front=nextIndex(front);
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if(isEmpty()){ //checking if the queue is empty
        return false;
    }
    outReq=data[front];  //peeking the front
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    int i = 0;
    int temp = front;
    int foundIndex=-1;
    while(i<count){
        if(data[temp].getId()==id){
            foundIndex=temp;  // Found the index to remove
            break;
        }
        i++;
        temp=nextIndex(temp);
    }
    if(foundIndex!=-1){
        int curr = foundIndex; 
        int next = nextIndex(curr);
        while (curr != rear){
            data[curr]=data[next]; // Shift elements to fill the gap
            curr=next;
            next=nextIndex(next);
        }
        rear=(rear-1+capacity)%capacity; // Move rear backwards
        count--;
        return true;
    }
    return false;  // If not found, nothing to remove
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    if(capacity>=newCapacity){  // Cannot resize to smaller or equal capacity
        return false;
    }
    Request* newData = new (std::nothrow) Request[newCapacity];
    if(newData==nullptr){
        return false; // Allocation failed
    }
    // Copy elements into new array
    int temp = front;
    for(int i=0; i<count; i++ ){
        newData[i]=data[temp];
        temp=nextIndex(temp);
    }
    //resetting
    delete[] data;
    data=newData;
    capacity=newCapacity;
    front=0;
    rear=count;
    return true;
}
