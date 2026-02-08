#include "QuakeAssistController.h"
#include <iostream>
#include <sstream>

QuakeAssistController::QuakeAssistController()
    : teams(nullptr),
      teamCount(0),
      supplyQueue(4),
      rescueQueue(4) {
}

QuakeAssistController::~QuakeAssistController() {
    delete[] teams;
}

bool QuakeAssistController::parseAndExecute(const std::string& line) {
    //Read the input file line by line and execute realtime.
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    if(command=="INIT_TEAMS"){
        int numTeams;
        ss>>numTeams;
        return initializeTeams(numTeams);
    }else if(command=="SET_TEAM_CAPACITY"){
        int teamID;
        ss>>teamID;
        int capacity;
        ss>>capacity;
        return handleSetTeamCapacity(teamID,capacity);
    }else if(command== "ADD_SUPPLY"){
        std::string id;
        ss >> id;              
        std::string city;
        ss >> city;             
        std::string supplyType;
        ss >> supplyType;       
        int amount;
        ss >> amount;           
        int emergencyLevel;
        ss >> emergencyLevel;
        return handleAddSupply(id,city,supplyType,amount,emergencyLevel);
    }else if(command=="ADD_RESCUE"){
        std::string id;
        ss >> id;              
        std::string city;
        ss >> city;             
        int numPeople;
        ss >> numPeople;       
        std::string risk;
        ss >> risk;           
        int emergencyLevel;
        ss >> emergencyLevel;
        return handleAddRescue(id,city,numPeople,risk,emergencyLevel);
    }else if(command=="REMOVE_REQUEST"){
        std::string id;
        ss >> id; 
        return handleRemoveRequest(id);
    }else if(command=="HANDLE_EMERGENCY"){
        int teamID;
        ss>>teamID;
        int k;
        ss>>k;
        return handleHandleEmergency(teamID,k);
    }else if(command=="DISPATCH_TEAM"){
        int teamID;
        ss>>teamID;
        return handleDispatchTeam(teamID);
    }else if(command=="PRINT_QUEUES"){
        printQueues();
        return true;
    }else if(command=="PRINT_TEAM"){
        int teamID;
        ss>>teamID;
        printTeam(teamID);
        return true;
    }else if(command=="CLEAR"){
        clear();
        return false;
    }else{
        std::cout<<"Error: Unknown command '"<<command<<"'."<<std::endl;
        return true;
    }
}

bool QuakeAssistController::initializeTeams(int numTeams) {
    //Create a team array and initialize it with teams.
    
    // If an old array exists, delete it first
     if (teams != nullptr) {
        delete[] teams;
    }
    // Allocate new team array
    teams = new Team[numTeams];
    teamCount=numTeams;
    // Assign IDs to teams
    for(int i=0; i<numTeams; i++){
        teams[i].setId(i);
    }
    supplyQueue.clear();
    rescueQueue.clear();
    std::cout<<"Initialized "<<numTeams<<" teams."<<std::endl;
    return true;
}

int QuakeAssistController::findTeamIndexById(int teamId) const {
    //Find the index of the team using teamId.

    for(int i =0; i<teamCount; i++){
        if(teams[i].getId()==teamId){
            return i;  //found index
        }
    }
    return -1;  //not found
}

bool QuakeAssistController::handleSetTeamCapacity(int teamId, int capacity) {
    //Find the index of team in the array, update the capacity value of the team.
    int index = findTeamIndexById(teamId);
    // If found, update capacity
    if(index != -1){
        teams[index].setMaxLoadCapacity(capacity);
        std::cout<<"Team "<<teamId<<" capacity set to "<<capacity<<"."<<std::endl;
        return true;
    }
    // if not found
    std::cout<<"Error: Team "<<teamId<<" not found."<<std::endl;
    return true;
}

bool QuakeAssistController::handleAddSupply(const std::string& id,
                                            const std::string& cityStr,
                                            const std::string& supplyTypeStr,
                                            int amount,
                                            int emergencyLevel) {
    //Create new supply request, and add it to the SUPPLY queue.
    Request req(id,cityStr,supplyTypeStr,amount,emergencyLevel);
    supplyQueue.enqueue(req);
    std::cout<<"Request "<<id<<" added to SUPPLY queue."<<std::endl;
    return true;
}

bool QuakeAssistController::handleAddRescue(const std::string& id,
                                            const std::string& cityStr,
                                            int numPeople,
                                            const std::string& riskStr,
                                            int emergencyLevel) {
    //Create new rescue request, and add it to the RESCUE queue.
    Request req(id,cityStr,numPeople,riskStr,emergencyLevel);
    rescueQueue.enqueue(req);
    std::cout<<"Request "<<id<<" added to RESCUE queue."<<std::endl;
    return true;
}

bool QuakeAssistController::handleRemoveRequest(const std::string& id) {
    //Remove request using request ID from request(SUPPLY, RESCUE) queue.
    bool supplyFound = supplyQueue.removeById(id);
    bool rescueFound = rescueQueue.removeById(id);
    bool found = supplyFound || rescueFound;  //checking if in one of two queues the index found
    if(found){
        std::cout<<"Request "<<id<<" removed from queues."<<std::endl;
        return true;
    }else{
        std::cout<<"Error: Request "<<id<<" not found."<<std::endl;
        return true;
    }
}

bool QuakeAssistController::handleHandleEmergency(int teamId, int k) {
    // TODO: Implement:
    // 1) Find team by id.
    // 2) For up to k steps:
    //    - Look at front of supplyQueue and rescueQueue using peek().
    //    - Use Request::computeEmergencyScore() to decide which to take.
    //    - If both empty -> break.
    //    - Try teams[teamIdx].tryAssignRequest(chosenRequest).
    //       * If this returns false, print overload message and
    //         call teams[teamIdx].rollbackMission(supplyQueue, rescueQueue),
    //         then break.
    //       * Else, dequeue chosen request from its queue and continue.
    int teamIdx = findTeamIndexById(teamId);
    if(teamIdx==-1){    // Team does not exist
        std::cout<<"Error: Team "<<teamId<<" not found."<<std::endl;
        return true;
    }
    int supplyCount = 0;   // Number of supply requests assigned
    int rescueCount = 0;   // Number of rescue requests assigned
    for(int i = 0; i<k; i++){
        Request frontSupply;
        Request frontRescue;
        //  Both queues have a request and choose by emergency score
        if(supplyQueue.peek(frontSupply) && rescueQueue.peek(frontRescue)){
            int scoreSupply=frontSupply.computeEmergencyScore();
            int scoreRescue=frontRescue.computeEmergencyScore();
            // Pick the one with higher score
            if(scoreSupply>scoreRescue){
                // Try assigning SUPPLY
                if(teams[teamIdx].tryAssignRequest(frontSupply)){
                    supplyQueue.dequeue(frontSupply);
                    supplyCount++;
                    continue;
                }else{
                    // Team overloaded, rollback all assigned missions
                    supplyQueue.dequeue(frontSupply);
                    std::cout<<"Overload on Team "<<teamId<<": rolling back mission."<<std::endl;
                    teams[teamIdx].rollbackMission(supplyQueue,rescueQueue);
                    supplyQueue.enqueue(frontSupply);  //adding the overloaded request to the end 
                    return true;
                    
                }
            }else{
                // Try assigning RESCUE
                 if(teams[teamIdx].tryAssignRequest(frontRescue)){
                    rescueQueue.dequeue(frontRescue);
                    rescueCount++;
                    continue;
                }else{
                    // Team overloaded, rollback all assigned missions
                    rescueQueue.dequeue(frontRescue);
                    std::cout<<"Overload on Team "<<teamId<<": rolling back mission."<<std::endl;
                    teams[teamIdx].rollbackMission(supplyQueue,rescueQueue);
                    rescueQueue.enqueue(frontRescue); //adding the overloaded request to the end 
                    return true;
                    
                }
            }
        // Only SUPPLY queue has items
        }else if(supplyQueue.peek(frontSupply)){
            if(teams[teamIdx].tryAssignRequest(frontSupply)){
                supplyQueue.dequeue(frontSupply);
                supplyCount++;
                continue;
            }else{
                supplyQueue.dequeue(frontSupply);
                std::cout<<"Overload on Team "<<teamId<<": rolling back mission."<<std::endl;
                teams[teamIdx].rollbackMission(supplyQueue,rescueQueue);
                supplyQueue.enqueue(frontSupply); 
                return true;
                
            }
        //  Only RESCUE queue has items
        }else if(rescueQueue.peek(frontRescue)){
            if(teams[teamIdx].tryAssignRequest(frontRescue)){
                rescueQueue.dequeue(frontRescue);
                rescueCount++;
                continue;
            }else{
                rescueQueue.dequeue(frontRescue);
                std::cout<<"Overload on Team "<<teamId<<": rolling back mission."<<std::endl;
                teams[teamIdx].rollbackMission(supplyQueue,rescueQueue);
                rescueQueue.enqueue(frontRescue);
                return true;
                
            }
        //Both queues empty
        }else{
            break;
        }
    }
    int workload = teams[teamIdx].getCurrentWorkload();
    // Print team assignment summary if at least one request was taken
    if(supplyCount+rescueCount>0){
        std::cout << "Team " << teamId << " assigned " << supplyCount+rescueCount 
                  << " requests (" << supplyCount << " SUPPLY, " 
                  << rescueCount << " RESCUE), total workload " 
                  << workload << ".\n";
    }
    return true;

}

bool QuakeAssistController::handleDispatchTeam(int teamId) {
    int idx = findTeamIndexById(teamId);
    if (idx == -1) {
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return true;
    }
    Team& t = teams[idx];
    if (!t.hasActiveMission()) {
        std::cout << "Error: Team " << teamId << " has no active mission." << std::endl;
        return true;
    }
    int workload = t.getCurrentWorkload();
    std::cout << "Team " << teamId << " dispatched with workload " << workload << "." << std::endl;
    t.clearMission();
    return true;
}

void QuakeAssistController::printQueues() const {
    //Print queues.
    Request *supplyreq = supplyQueue.getData();
    std::cout<<"SUPPLY QUEUE:\n";
    if(supplyreq){
        int temp = supplyQueue.getFrontIndex();
        //traversing circular array
        for(int i=0; i<supplyQueue.getCount(); i++ ){
            std::cout << supplyreq[temp].getId() << " " 
              << supplyreq[temp].getCity() << " " 
              << supplyreq[temp].getSupplyType() << " " 
              << supplyreq[temp].getAmount() << " " 
              << supplyreq[temp].getEmergencyLevel() << "\n";
            temp = (temp + 1) % supplyQueue.getCapacity();
        }
    }
    std::cout<<"RESCUE QUEUE:\n";
    Request *rescuereq = rescueQueue.getData();
    if(rescuereq){
        int temp = rescueQueue.getFrontIndex();
        //traversing circular array
        for(int i=0; i<rescueQueue.getCount(); i++ ){
            std::cout << rescuereq[temp].getId() << " " 
              << rescuereq[temp].getCity() << " " 
              << rescuereq[temp].getNumPeople() << " " 
              << rescuereq[temp].getRisk() << " " 
              << rescuereq[temp].getEmergencyLevel()<< "\n";
            temp = (temp + 1) % rescueQueue.getCapacity(); 
        }
    }
}

void QuakeAssistController::printTeam(int teamId) const {
    //Print team data using teamId.
    int i = findTeamIndexById(teamId);
    if(i == -1){   // check if team exists
        std::cout << "Error: Team " << teamId << " not found." << std::endl;
        return;
    }
    const MissionStack& missionStack = teams[i].getMissionStack();
    Request* req = missionStack.getData();
    std::cout << "TEAM " << teamId << " STACK:\n";
    // Print only if there is an active mission
    if(teams[i].hasActiveMission()){
        //printed from top to bottom
        for(int i = missionStack.getTopIndex(); i >= 0; i--){
        if(req[i].getType()=="SUPPLY"){
            std::cout << req[i].getId() << " " 
              << req[i].getCity() << " " 
              << req[i].getSupplyType() << " " 
              << req[i].getAmount() << " " 
              << req[i].getEmergencyLevel();
        }else if(req[i].getType()=="RESCUE"){
            std::cout << req[i].getId() << " " 
              << req[i].getCity() << " " 
              << req[i].getNumPeople() << " " 
              << req[i].getRisk() << " " 
              << req[i].getEmergencyLevel();
        }
        std::cout << "\n";
    }
    }
    
}

void QuakeAssistController::clear() {
    //Clear data.
    supplyQueue.clear();
    rescueQueue.clear();
    for (int i = 0; i < teamCount; ++i) {
        teams[i].clearMission();   //clearing all the missions
    }
    std::cout << "System cleared.\n";
}
