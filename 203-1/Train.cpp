#include "Train.h"
#include <iostream>

Train::Train() : name(""), destination(Destination::OTHERS), totalWeight(0), nextLocomotive(nullptr) {}
Train::Train(const std::string &_name, Destination _dest) : name(_name), destination(_dest), totalWeight(0), nextLocomotive(nullptr) {}
Train::~Train() { clear(); }

// This function is given to you ready
void Train::appendWagonList(WagonList &wl)
{
    // Makes appendList use move semantics
    wagons.appendList(std::move(wl));
    totalWeight = wagons.getTotalWeight();
}

// This function is given to you ready
void Train::addWagonToRear(Wagon *w)
{
    wagons.addWagonToRear(w);
    totalWeight = wagons.getTotalWeight();
}

void Train::clear()
{ 
    //TODO: Do the cleaning as necesssary
    this->wagons.clear();
    this->totalWeight=0;
    this->nextLocomotive=nullptr;
    this->name="";
    this->destination = Destination::OTHERS;

}

// This function is given to you ready
void Train::print() const
{
    std::cout << "Train " << name << " (" << totalWeight << " tons): ";
    std::cout << wagons << std::endl;
}

Train *Train::verifyCouplersAndSplit(int splitCounter)
{

    // TODO: Verify whether any wagon’s coupler is overloaded.
    // You must traverse from the rear (backmost wagon) toward the front.
    //
    // Splitting rule:
    // Split the train AFTER the overloaded wagon (the overloaded one stays).
    // Use WagonList::splitAtById to detach the overloaded segment into a new WagonList.
    //
    // If no overload occurs, return nullptr (no split needed).
    //
    // If a valid split occurs:
    // new Train should be named "<oldName>_split_<splitCounter>".

    // print message
    // std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
    // std::cout << newTrain->wagons << std::endl;

    WagonList& wagons = this->getWagons();
    Wagon *curr = wagons.getRear();   // Start checking from the rear wagon
    int cumulativeWeightBehind = 0;     // Keep track of total weight behind the current wagon
    while(curr != nullptr){
        if(cumulativeWeightBehind > curr->getMaxCouplerLoad()){
            if(curr->getNext()!=nullptr){
                int splitId = curr->getNext()->getID() ;    // ID of the wagon where the split will occur
            WagonList newList = wagons.splitAtById(splitId);
            int detachedWeight = newList.getTotalWeight();
            Train *newTrain = new Train();     // Create a new train for the detached wagons
            newTrain->wagons=std::move(newList);
            newTrain->name = this->name + "_split_" + std::to_string(splitCounter);
            newTrain->destination=destination;
            newTrain->totalWeight = detachedWeight;
            std::cout << "Train " << name << " split due to coupler overload before Wagon " << splitId << std::endl;
            std::cout << newTrain->wagons << std::endl;
            this->setTotalWeight(this->wagons.getTotalWeight());
            return newTrain;
            }
            
        }
        cumulativeWeightBehind += curr->getWeight();
        curr=curr->getPrev();
    }
    return nullptr;
}