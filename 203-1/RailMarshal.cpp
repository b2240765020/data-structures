#include "RailMarshal.h"
#include <iostream>
#include <sstream>
#include <algorithm>

RailMarshal::RailMarshal()
{
    // TODO: Initialize each track in the departure yard.
    // Each TrainTrack corresponds to one Destination.
    for (int i=0; i<NUM_DESTINATIONS_INT; i++){
        Destination dest = static_cast<Destination>(i);
        this->departureYard[i]=TrainTrack(dest);
    }
}

RailMarshal::~RailMarshal()
{
    // TODO: Cleanup remaining trains, prevent memory leaks
    
    this->classificationYard.clear();
}

// Getter (ready)
ClassificationYard &RailMarshal::getClassificationYard()
{
    return classificationYard;
}

// Getter (ready)
TrainTrack &RailMarshal::getDepartureYard(Destination dest)
{
    int idx = static_cast<int>(dest);
    return departureYard[idx];
}

void RailMarshal::processCommand(const std::string &line)
{
    // TODO: Parse user commands from input lines.

    std::stringstream ss(line);
    std::string command;
    ss >> command;
    // if ADD_WAGON
    // Use: std::cout << "Error: Invalid ADD_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << *w << " added to yard." << std::endl;

    if(command=="ADD_WAGON"){
        std::string idStr, cargoTypeStr, destStr, weightStr, maxCouplerLoadstr; 
        if(ss >> idStr >> cargoTypeStr >> destStr >> weightStr >> maxCouplerLoadstr ){
            CargoType cargoType = parseCargo(cargoTypeStr);
            Destination dest = parseDestination(destStr);
            int id = stoi(idStr);
            int weight = stoi(weightStr);
            int maxCouplerLoad = stoi(maxCouplerLoadstr);
            Wagon *w = new Wagon(id,cargoType,dest,weight,maxCouplerLoad);   //creating the new wagon
            this->classificationYard.insertWagon(w);    //inserting to the wagonlist by sorting according to weight
            std::cout << "Wagon " << *w << " added to yard." << std::endl;
            return;
        }else{
            std::cout << "Error: Invalid ADD_WAGON parameters.\n";
        }
        
    }

    // if REMOVE_WAGON
    // Use: std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
    // Use: std::cout << "Wagon " << id << " removed." << std::endl;
    // Use: std::cout << "Error: Wagon " << id << " not found." << std::endl;

    else if (command == "REMOVE_WAGON"){
        int id;
        if(!(ss >> id)){
            std::cout << "Error: Invalid REMOVE_WAGON parameters.\n";
            return;
            }
             // Search through all destinations and cargo type blocks in the classification yard
            for (int i = 0; i < NUM_DESTINATIONS_INT; ++i){
                for (int j = 0; j < NUM_CARGOTYPES_INT; ++j){
                    WagonList& curr = this->classificationYard.getBlockTrain(i,j);
                    Wagon* wagonToRemove = curr.detachById(id); // Try to detach the wagon with the given ID
                    if(wagonToRemove != nullptr){
                        delete wagonToRemove;  // Free the memory of the removed wagon
                        std::cout << "Wagon " << id << " removed." << std::endl;
                        return;
                    }
                } 
            }
            std::cout << "Error: Wagon " << id << " not found." << std::endl;
    }

    // if ASSEMBLE_TRAIN
    //  Use: std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
    //  Use: std::cout << "No wagons to assemble for " << destStr << std::endl;
    //  verify couplers and possibly split (deterministic)
    //  Keep splitting the *front* train until no more overloaded couplers found
    //  create new train with same destination and name suffix
    //  use std::cout << "Train " << newTrain->getName() << " assembled after split with "
    //  << newTrain->getWagons()<< " wagons." << std::endl;
    // use std::cout << "Train " << t->getName() << " assembled with " << t->getWagons() << " wagons." << std::endl;
   
    else if(command == "ASSEMBLE_TRAIN"){
        std::string destStr;
        if(ss >> destStr){
            Destination dest = parseDestination(destStr);
            TrainTrack &track  = this->getDepartureYard(dest);
            Train *t = this->classificationYard.assembleTrain(dest,track.generateTrainName());
            if(t==nullptr){
                std::cout << "No wagons to assemble for " << destStr << std::endl;
                return;
            }
            
            Train *currentSegment = t;
            Train *newTrain = nullptr;
            int i=1;   // Counter for naming split trains
            // Continuously verify coupler limits and split if needed
            while(true){
                newTrain=currentSegment->verifyCouplersAndSplit(i);
            
                if(newTrain != nullptr){
                    track.addTrain(newTrain);
                    std::cout << "Train " << newTrain->getName() << " assembled after split with " << newTrain->getWagons()<< " wagons." << std::endl;
                    i++;  // Increment split counter for naming next segment
                }else{
                    track.addTrain(currentSegment);
                    std::cout << "Train " << currentSegment->getName() << " assembled with " 
                          << currentSegment->getWagons() << " wagons." << std::endl;
                    break;
                }
            }
            
        }else{
            std::cout << "Error: Invalid ASSEMBLE_TRAIN parameters.\n";
        } 
    } 

    // if DISPATCH_TRAIN
    //  use: std::cout << "Error: Invalid DISPATCH parameters.\n";
    //  use: std::cout << "Error: No trains to dispatch from track " << destStr << ".\n";
    //  use:  std::cout << "Dispatching " << train->getName() << " (" << t->getTotalWeight() << " tons)." << std::endl;
    else if (command == "DISPATCH_TRAIN") {
        std::string destStr;
        if (ss >> destStr) {
            Destination dest = parseDestination(destStr);
            this->dispatchFromTrack(dest);
        }else{
            std::cout << "Error: Invalid DISPATCH parameters.\n";
        }
    }
    // if PRINT_YARD
    //  use std::cout << "--- classification Yard ---\n";
    else if(command=="PRINT_YARD"){
        std::cout << "--- classification Yard ---\n";
        this->classificationYard.print();
    }

    // if PRINT_TRACK
    //  use std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
    else if(command=="PRINT_TRACK"){
        std::string destStr;
        if(ss >> destStr){
            Destination dest = parseDestination(destStr);
            departureYard[static_cast<int>(dest)].printTrack();
        }else{
            std::cout << "Error: Invalid PRINT_TRACK parameters.\n";
        }
    }

    // if AUTO_DISPATCH <ON/OFF>
    // Enable or disable automatic dispatch when weight exceeds limits.
    // std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
    // print "Auto dispatch "enabled" / "disabled"
    else if(command=="AUTO_DISPATCH"){
        std::string dispatch;
        if(ss >> dispatch){
            if (dispatch == "ON") {
                TrainTrack::autoDispatch = true;
                std::cout << "Auto dispatch enabled" << std::endl;
            } else if (dispatch == "OFF") {
                TrainTrack::autoDispatch = false;
                std::cout << "Auto dispatch disabled" << std::endl;
            }else{
                std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
            }
        }else {
            std::cout << "Error: Invalid AUTO_DISPATCH parameters.\n";
        }
    }

    // if CLEAR
    // Completely reset the system (yard + departure tracks).
    // std::cout << "System cleared." << std::endl;
    else if(command=="CLEAR"){
        for(int i=0; i<NUM_DESTINATIONS_INT; i++){
            while(true){
                Train *t = this->departureYard[i].departTrain();
                if(t==nullptr)break;
                delete t;
            }
        }
        this->classificationYard.clear();
        std::cout << "System cleared." << std::endl;
    }

    // else std::cout << "Error: Unknown command '" << cmd << "'" << std::endl;
    else {
        std::cout << "Error: Unknown command '" << command << "'" << std::endl;
    }
}

void RailMarshal::dispatchFromTrack(Destination track)
{
    // TODO: Dispatch the next train (frontmost) from the specified track.
    // std::cout << "Error: No trains to dispatch from Track " << destIndex << ".\n";
    /*std::cout << "Train " << t->getName()
              << " departed from Track " << destIndex
              << " (" << destinationToString(static_cast<Destination>(destIndex)) << ").\n";
     */
    TrainTrack& TrainTrack = this->getDepartureYard(track);
    Train* dispatchedTrain = TrainTrack.departTrain(); 
    if(dispatchedTrain != nullptr){
        std::cout << "Dispatching " << dispatchedTrain->getName() << " (" << dispatchedTrain->getTotalWeight() << " tons)." << std::endl;
        delete dispatchedTrain;
    }else{
        std::cout << "Error: No trains to dispatch from track " << destinationToString(track) << ".\n";
    }
}

void RailMarshal::printDepartureYard() const
{
    for (int i = 0; i < NUM_DESTINATIONS_INT; ++i)
    {
        std::cout << "Track " << i << " ("
                  << destinationToString(static_cast<Destination>(i)) << "):\n";
        departureYard[i].printTrack();
    }
}

// Debug helper functions
void RailMarshal::printStatus() const
{
    std::cout << "--- classification Yard ---\n";
    classificationYard.print();

    std::cout << "--- Departure Yard ---\n";
    for (int i = 0; i < static_cast<int>(Destination::NUM_DESTINATIONS); ++i)
    {
        departureYard[i].printTrack();
    }
}
