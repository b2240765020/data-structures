#include "MissionStack.h"
#include <new>     // for std::nothrow

MissionStack::MissionStack()
    : data(nullptr),
      capacity(0),
      top(-1) {
    resize(4);
}

MissionStack::MissionStack(int initialCapacity)
    : data(nullptr),
      capacity(0),
      top(-1) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

MissionStack::~MissionStack() {
    delete[] data;
}

bool MissionStack::isEmpty() const {
    return top == -1;
}

int MissionStack::size() const {
    return top + 1;
}

bool MissionStack::push(const Request& req) {
    //Implement push function as explained in the PDF.

    if(top==capacity-1){ //means the stack is full
        resize(capacity*2);  // double the capacity
    }
    top++;  // Inserting new request on top
    data[top]=req;
    return true;
}

bool MissionStack::pop(Request& outReq) {
    //Implement pop function as explained in the PDF.

    if(isEmpty()){    // Cannot pop from an empty stack
        return false;
    }
    outReq=data[top]; // Return top element
    top--;
    return true;
}

bool MissionStack::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.

    if(isEmpty()){    // Cannot peek if stack is empty
        return false;
    }
    outReq=data[top];
    return true;
}

void MissionStack::clear() {
    top = -1;
}

bool MissionStack::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    Request* newData = new (std::nothrow) Request[newCapacity];
    if(newData==nullptr){
        return false;    // allocation failed
    }
    // Copy old elements to new array
    for(int i=0; i<=top; i++){
        newData[i]=data[i];
    }
    //replacing
    delete[] data;
    data=newData;
    capacity=newCapacity;
    return true;
}