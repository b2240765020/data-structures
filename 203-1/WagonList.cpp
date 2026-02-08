#include "WagonList.h"

#include <iostream>

WagonList::~WagonList() { clear(); }

void WagonList::clear()
{
    // TODO: Delete all Wagon objects in this list and reset pointers.
    Wagon *curr = front;
    while(curr != nullptr){
        Wagon *temp = curr->next;
        delete(curr);
        curr= temp;
    }
    front= nullptr;  //resetting the pointers
    rear = nullptr;
    this->totalWeight=0;
}

WagonList::WagonList(WagonList &&other) noexcept
{
    // TODO: Implement move constructor.
    // Transfer ownership of 'other' list’s nodes into this list
    // and leave 'other' in an empty but valid state.
    this->front=other.front;
    this->rear=other.rear;
    this->totalWeight=other.totalWeight;

    other.front=nullptr;   //emptying the other list
    other.rear=nullptr;    
    other.totalWeight=0;

}

WagonList &WagonList::operator=(WagonList &&other) noexcept
{
    // Operation version of the move constructor.
    // TODO: Implement it.
    if(this != &other){
        this->clear();

        this->front=other.front;
        this->rear=other.rear;
        this->totalWeight=other.totalWeight;

        other.front=nullptr;
        other.rear=nullptr;
        other.totalWeight=0;
    }
    return *this;
}

Wagon *WagonList::findById(int id)
{
    // TODO: Find and return the Wagon with given ID.
    // Return nullptr if not found.
    Wagon *temp = front;
    while(temp != nullptr){
        if(temp->getID() == id){
            return temp;
        }else{
            temp=temp->next;
        }
    }
    return nullptr;
}

void WagonList::addWagonToRear(Wagon *w)
{
    // TODO: Add a Wagon to the rear (end) of the list.
    // This function does not respect the weight order
    // it inserts to end regardless of the weight

    if(front==nullptr && rear==nullptr){  // means the list is empty
        front=w;
        rear=w;
        w->prev=nullptr;
    }else{
        rear->next=w;
        w->prev=rear;
        rear=w;
    }
    w->next=nullptr;
    this->totalWeight += w->getWeight();
}

int WagonList::getTotalWeight() const { return totalWeight; }

bool WagonList::isEmpty() const
{
    // TODO: Return true if the list has no wagons.

    if(front==nullptr){  
        return true;
    }
    return false;
}

void WagonList::insertSorted(Wagon *wagon)
{
    // TODO: Insert wagon into this list in descending order of weight.

    this->totalWeight += wagon->getWeight();

    if(front==nullptr && rear==nullptr){  // means the list is empty
        front=wagon;
        rear=wagon;
        return;
    }
    Wagon *temp=front;
    while(temp != nullptr ){
        if(wagon->getWeight() > temp->getWeight()){//checking the heavier wagon
            if(temp==front){   //inserting to front
                wagon->next=temp;
                wagon->prev=nullptr;
                temp->prev=wagon;
                front=wagon;
                return;
            }
            temp->prev->next=wagon;
            wagon->prev=temp->prev;
            wagon->next=temp;
            temp->prev=wagon;
            return;
        }
    temp = temp->next;
    }
    rear->next=wagon;
    wagon->prev=rear;
    rear=wagon;
}

void WagonList::appendList(WagonList &&other)
{
   // TODO: Append another WagonList to this one (merge them).
   // Use move semantics to avoid deep copies. (Double && at the parameter already makes it so)
   // 'other' should end up empty after this operation
   // At merge lists (blocks) will be protected 
   // But the one with heavier wagon at the front will be before the other list
   if(other.front == nullptr){//means the list is empty
    return;
   }
   if(this->front == nullptr){
    *this = std::move(other);
    return;

   }if(other.front->getWeight() > this->front->getWeight()){
    other.rear->next=this->front;
    this->front->prev=other.rear;
    this->front=other.front;
   }else{
    this->rear->next=other.front;
    other.front->prev=this->rear;
    this->rear=other.rear;
   }
   this->totalWeight += other.totalWeight;
   other.front = nullptr;
   other.rear = nullptr;
   other.totalWeight = 0;
}

Wagon *WagonList::detachById(int id)
{
    // TODO: Remove a specific wagon (by ID) from this list and return it.
    // Use: std::cout << "Wagon " << toRemove->id << " detached from Wagon List. " << std::endl;
    // Return nullptr if wagon not found.

    Wagon *curr = front;
    while(curr != nullptr){
        if(curr->getID() == id){
            Wagon *temp=curr;
            this->totalWeight -= temp->getWeight();
            if(curr==front && curr==rear){ //means it is single-node list
                front=nullptr;
                rear=nullptr;
            }
            else if(curr==front){ 
                front=temp->next;
                if(front != nullptr){
                    front->prev=nullptr;
                }
            }else if(curr==rear){
                rear=temp->prev;
                if(rear != nullptr){
                    rear->next=nullptr;
                }
            }else{
                temp->prev->next=temp->next;
                temp->next->prev=temp->prev;
            }
            //detaching the found node
            temp->next=nullptr;
            temp->prev=nullptr;

            std::cout << "Wagon " << temp->id << " detached from Wagon List. " << std::endl;
            return temp;
        }
        curr=curr->next;
    }
    return nullptr;
}


WagonList WagonList::splitAtById(int id)
{
    WagonList newList; // return-by-value (will be moved automatically)

    // TODO: Split this list into two lists at the wagon with given ID.
    // The wagon with 'id' becomes the start of the new list.
    // Return the new WagonList (move return).
    // If 'id' not found, return an empty list.

    Wagon *curr = front;
    while(curr != nullptr){
        if(curr->getID()==id){
            newList.front=curr;
            newList.rear=this->rear;
            if(curr==this->front){
                this->front=nullptr;
                this->rear=nullptr;
            }else {
                Wagon *newRear = curr->prev;
                newRear->next=nullptr;
                this->rear=newRear;
                
            }
            curr->prev=nullptr;
            this->totalWeight=0;
            for(Wagon *w = this->front; w!=nullptr; w=w->next)  //calculating the totalweight of the list
                this->totalWeight += w->getWeight();

            newList.totalWeight = 0;
            for (Wagon* w = newList.front; w != nullptr; w = w->next)   //calculating the totalweight of the newlist
                newList.totalWeight += w->getWeight();
            
            return newList;
        }
        curr=curr->next;
    }

    return newList; // moved, not copied
}

// Print is already implemented
void WagonList::print() const
{

    std::cout << *this << std::endl;
    return;
}

// << operator is already implemented
std::ostream &operator<<(std::ostream &os, const WagonList &list)
{
    if (list.isEmpty())
        return os;

    Wagon *current = list.front;

    while (current)
    {
        os << "W" << current->getID() << "(" << current->getWeight() << "ton)";
        if (current->getNext())
            os << " - ";
        current = current->getNext();
    }
    return os;
}
