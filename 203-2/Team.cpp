#include "Team.h"

Team::Team()
    : teamID(-1),
      maxLoadCapacity(0),
      currentWorkload(0),
      missionStack(4) {
}

Team::Team(int id, int maxLoad)
    : teamID(id),
      maxLoadCapacity(maxLoad),
      currentWorkload(0),
      missionStack(4) {
}

int Team::getId() const {
    return teamID;
}

int Team::getMaxLoadCapacity() const {
    return maxLoadCapacity;
}

int Team::getCurrentWorkload() const {
    return currentWorkload;
}

void Team::setId(int id) {
    teamID = id;
}

void Team::setMaxLoadCapacity(int maxLoad) {
    maxLoadCapacity = maxLoad;
}

bool Team::hasActiveMission() const {
    return !missionStack.isEmpty();
}

bool Team::tryAssignRequest(const Request& req) {
    //Implement tryAssignRequest function as explained in the PDF.
    if(currentWorkload+req.computeWorkloadContribution()>maxLoadCapacity){ //checking if current workload exceeds max capacity
        return false;
    }
    currentWorkload+=req.computeWorkloadContribution();  //adding the workload
    missionStack.push(req);  //assigning the request by pushing to mission satck
    return true;
}

void Team::rollbackMission(RequestQueue& supplyQueue, RequestQueue& rescueQueue) {
    //Implement rollbackMission function as explained in the PDF.
    if(missionStack.isEmpty()){
        return;
    }
    // Temp stack to restore original order
    MissionStack tempStack(missionStack.getCapacity());
    // Reverse missionStack
    while (!missionStack.isEmpty()) {
        Request data;
        missionStack.pop(data);
        tempStack.push(data);
    }
    // Return requests to their correct queues
    while (!tempStack.isEmpty()) {
        Request data;
        tempStack.pop(data);
        if (data.getType() == "SUPPLY") {
            supplyQueue.enqueue(data) ; 
        }else {
            rescueQueue.enqueue(data) ;  
        }
    }
    // Clear team's mission after rollback
    clearMission();
}

void Team::clearMission() {
    missionStack.clear();
    currentWorkload = 0;
}

const MissionStack& Team::getMissionStack() const {
    return missionStack;
}
