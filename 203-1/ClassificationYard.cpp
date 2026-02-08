#include "ClassificationYard.h"
#include <iostream>

ClassificationYard::ClassificationYard() {}
ClassificationYard::~ClassificationYard() { clear(); }

WagonList &ClassificationYard::getBlockTrain(int destination, int cargoType)
{
    return blockTrains[destination][cargoType];
}

WagonList *ClassificationYard::getBlocksFor(Destination dest)
{
    return blockTrains[static_cast<int>(dest)];
}

// Inserts vagon to the corract place at the yard
void ClassificationYard::insertWagon(Wagon *w)
{
    if (!w)
        return;
    int dest = static_cast<int>(w->getDestination());
    int cargo = static_cast<int>(w->getCargoType());
    blockTrains[dest][cargo].insertSorted(w);
}

// Merges multiple blocks into a train while keeping blocks grouped
Train *ClassificationYard::assembleTrain(Destination dest, const std::string &trainName)
{
    // TODO: Collect wagons of the same destination and assemble them into a single Train.

    /**
     * - Blocks of the same cargo type must remain grouped together.
     * - These groups must be appended to the train in descending order
     *   based on their heaviest wagon.
     * - Hazardous cargo (e.g., OIL) must always be placed at the very end of the train,
     *   and only one hazardous block can be included per train.*/

    int destIndex = static_cast<int>(dest);
    WagonList assembledWagons;    // will hold the non-hazardous groups in final order
    WagonList hazardousBlock;     // temporary holder for a hazardous block (only one allowed per train)
    for(int i=0; i<NUM_CARGOTYPES_INT; i++){
        WagonList& block = blockTrains[destIndex][i];
        if(block.getFront()==nullptr)   
            continue;  //skipping empty block
        if (block.getFront()->getCargoType()==CargoType::HAZARDOUS){  // If this block is hazardous, store it separately
            if(!hazardousBlock.isEmpty()){
                continue;
            }
            hazardousBlock.appendList(std::move(block));   // move whole block into hazardousBlock
        }else{
            assembledWagons.appendList(std::move(block));     // Non-hazardous blocks: move them into the assembledWagons list
        }
    }
    if(!hazardousBlock.isEmpty()){
        Wagon *w = hazardousBlock.detachById(hazardousBlock.getFront()->getID());    // detach the front wagon from hazardousBlock
        assembledWagons.addWagonToRear(w); // place that detached wagon at the very end of the assembled wagons
        int hazardousIndex = static_cast<int>(CargoType::HAZARDOUS);
        WagonList& originalBlock = blockTrains[destIndex][hazardousIndex];
        originalBlock.appendList(std::move(hazardousBlock));    // put the remaining hazardous wagons back into the original hazardous block 
        }
    if(assembledWagons.isEmpty()){
        return nullptr;
    }
    Train* assembledTrain = new Train(trainName,dest);
    assembledTrain->getWagons()=std::move(assembledWagons);
    assembledTrain->setTotalWeight(assembledTrain->getWagons().getTotalWeight());
    return assembledTrain;
}

bool ClassificationYard::isEmpty() const
{
    /** TODO: Check if the entire classification yard is empty.
     *
     * The yard is empty if every blockTrain list for all destination-cargo pairs is empty.
     */
    for(int i = 0; i< NUM_DESTINATIONS_INT; i++){
        for(int j = 0; j< NUM_CARGOTYPES_INT; j++){
            if(!blockTrains[i][j].isEmpty())
                return false;
        }
    }
    return true;
}

void ClassificationYard::clear()
{
    /** TODO: Clear all wagons from the classification yard.
     *
     * Used when resetting or ending the simulation.
     */
    for(int i = 0; i< NUM_DESTINATIONS_INT; i++){
        for(int j = 0; j< NUM_CARGOTYPES_INT; j++){
            blockTrains[i][j].clear();   //clearing each wagon
        }
    }
}

// Print function is already implemented to keep output uniform
void ClassificationYard::print() const
{
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        auto dest = destinationToString(static_cast<Destination>(i));
        std::cout << "Destination " << dest << ":\n";
        for (int j = 0; j < static_cast<int>(CargoType::NUM_CARGOTYPES); ++j)
        {
            if (!blockTrains[i][j].isEmpty())
            {
                auto type = cargoTypeToString(static_cast<CargoType>(j));
                std::cout << "  CargoType " << type << ": ";
                blockTrains[i][j].print();
            }
        }
    }
}