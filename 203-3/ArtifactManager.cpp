
#include "ArtifactManager.h"
#include <iostream>
#include <sstream>

ArtifactManager::ArtifactManager()
{
}

ArtifactManager::~ArtifactManager()
{
}

int ArtifactManager::tokenize(const std::string &line, std::string tokens[], int maxTokens) const
{
    std::istringstream iss(line);
    std::string tok;
    int count = 0;
    while (iss >> tok && count < maxTokens)
    {
        tokens[count++] = tok;
    }
    return count;
}

void ArtifactManager::parseAndExecute(const std::string &line)
{
    // TODO: read lines and execuıte each command
    // Print "Error: Unknown command" if command is not known

    const int MAX_TOKENS = 10; 
    std::string tokens[MAX_TOKENS];
    int count = tokenize(line, tokens, MAX_TOKENS);  // number of tokens

    if (count == 0) return; // Empty line

    std::string command = tokens[0];

    if(command=="ADD_ARTIFACT"){
        handleAddArtifact(tokens,count);

    }else if(command=="REMOVE_ARTIFACT"){
        handleRemoveArtifact(tokens, count);
        
    }else if (command == "HIRE_RESEARCHER") {
        handleHireResearcher(tokens, count);

    }else if (command == "FIRE_RESEARCHER") {
         handleFireResearcher(tokens, count);

    }else if (command == "REQUEST") {
        handleRequest(tokens, count);

    }else if (command == "RETURN") {
        handleReturn(tokens, count);

    }else if (command == "RETURN_ALL") {
         handleReturnAll(tokens, count);

    }else if (command == "RESEARCHER_LOAD") {
         handleResearcherLoad(tokens, count);

    }else if (command == "MATCH_RARITY") {
         handleMatchRarity(tokens, count);

    }else if (command == "PRINT_UNASSIGNED") {
        handlePrintUnassigned(nullptr,count);

    }else if (command == "PRINT_STATS") {
        handlePrintStats(nullptr,count);

    }else if (command == "CLEAR") {
        handleClear(nullptr,count);

    }else {
        std::cout << "Error: Unknown command '"<<command<<"'."<< std::endl;
    }
}

// =================== COMMAND HANDLERS ===================

void ArtifactManager::handleAddArtifact(const std::string tokens[], int count)
{
    // Expected: ADD_ARTIFACT <id> <name> <rarity> <value>
    // TODO:
    // 1) Check parameter count.
    // 2) Convert <id>, <rarity>, <value> to integers.
    // 3) Create Artifact and attempt to insert into AVL tree.
    // 4) On success: print "Artifact <id> added."
    // 5) On duplicate: print appropriate error (as in the PDF).
    if(count==5){
        int id = std::stoi(tokens[1]);      
        int rarity = std::stoi(tokens[3]);  
        int value = std::stoi(tokens[4]);   

        Artifact node(id, tokens[2], rarity, value); //creating the artifact
        bool inserted = artifactTree.insertArtifact(node);  // attempting to insert into tree

        if(inserted){
            std::cout << "Artifact " << id << " added." << std::endl;
        }else{
            std::cout << "Error: Artifact " << id << " already exists." << std::endl;
        }
        
    }
}

void ArtifactManager::handleRemoveArtifact(const std::string tokens[], int count)
{
    // Expected: REMOVE_ARTIFACT <id>
    // TODO:
    // 1) Parse id.
    // 2) Find artifact in AVL; if not found, print error.
    // 3) If artifact is assigned , find researcher and
    //    remove artifact from their list.
    // 4) Remove artifact from AVL; print success or error message.

    if(count==2){
        int id = std::stoi(tokens[1]);
        ArtifactNode *node = artifactTree.findArtifact(id);
        if(node==nullptr){
            std::cout<<"Error: Artifact "<<id<<" not found."<<std::endl;
            return;
        }
        // checking if artifact is assigned
        if(node->data.assignedToName!=""){
            ResearcherNode *resNode = researcherTree.findResearcher(node->data.assignedToName);
            if (resNode != nullptr) {
                resNode->data.removeArtifact(id);
            }
        }
        // removing artifact
        bool removed = artifactTree.removeArtifact(id);
        if(removed){
            std::cout << "Artifact " << id << " removed." << std::endl;
        }else{
            std::cout << "Error: Failed to remove artifact " << id << "." << std::endl;
        }
    }
}

void ArtifactManager::handleHireResearcher(const std::string tokens[], int count)
{
    // Expected: HIRE_RESEARCHER <name> <capacity>
    // TODO:
    // 1) Parse name and capacity.
    // 2) Create Researcher and insert into RedBlackTree.
    // 3) On success: "Researcher <name> hired."
    // 4) On duplicate: error message.

    if(count==3){
        std::string name = tokens[1];
        int capacity = std::stoi(tokens[2]);
        // creating the researcher
        Researcher node(name,capacity);
        bool inserted = researcherTree.insertResearcher(node);
        if(inserted){
            std::cout<<"Researcher "<<name<< " hired."<<std::endl;
        }else{
            std::cout << "Error: Researcher " << name << " already exists." << std::endl;
        }
    }
}

void ArtifactManager::handleFireResearcher(const std::string tokens[], int count)
{
    // Expected: FIRE_RESEARCHER <name>
    // TODO:
    // 1) Find researcher by name. If not found, print error.
    // 2) For each artifact ID in their assignment list:
    //      - clear assignedToName in AVL.
    // 3) Remove researcher from RBT.
    // 4) Print success message.

    if(count==2){
        std::string name = tokens[1];
        ResearcherNode *researcher = researcherTree.findResearcher(name);
        if(researcher==nullptr){
            std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
            return;
        }
        // updating artifacts to be unassigned
        for(int i = 0; i<researcher->data.numAssigned; i++){
            int id = researcher->data.assignedArtifacts[i];
            ArtifactNode *artifact = artifactTree.findArtifact(id);
            if(artifact){
                artifact->data.assignedToName="";
            }
        }
        // removing the researcher
        bool removed = researcherTree.removeResearcher(name);
        if(removed){
            std::cout << "Researcher " << name << " fired." << std::endl;
        }else {
        std::cout << "Error: Failed to fire researcher " << name << "." << std::endl;
        }
    }
}

void ArtifactManager::handleRequest(const std::string tokens[], int count)
{
    // Expected: REQUEST <researcherName> <artifactID>
    // TODO:
    // 1) Find researcher by name; error if missing.
    // 2) Find artifact by ID; error if missing.
    // 3) Check artifact is unassigned; error if already assigned.
    // 4) Check researcher capacity; error if at full capacity.
    // 5) On success: add artifact to researcher list AND set assignedToName in AVL.
    //    Print "Artifact <id> assigned to <name>."

    if(count==3){
        std::string name = tokens[1];
        int id = std::stoi(tokens[2]);
        ResearcherNode *researcher = researcherTree.findResearcher(name);
        if(researcher==nullptr){
            std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
            return;
        }
        ArtifactNode *artifact = artifactTree.findArtifact(id);
        if(artifact==nullptr){
            std::cout<<"Error: Artifact "<<id<<" not found."<<std::endl;
            return;
        }
        // checking the artifact if it is unassigned
        if(artifact->data.assignedToName!=""){
            std::cout << "Error: Artifact " << id<< " is assigned to "<< artifact->data.assignedToName << "." << std::endl;
            return;
        }
        // checking the researcher's capacity
        if(researcher->data.numAssigned==researcher->data.capacity){
            std::cout<<"Error: Researcher "<<name<< " is at full capacity."<<std::endl;
            return;
        }
        // assigning the researcher to artifact
        researcher->data.addArtifact(id);
        artifact->data.assignedToName=name;
        artifact->data.updateValueBasedOnUsage();
        std::cout<<"Artifact "<<id<<" assigned to "<<name<<"."<<std::endl;
    }
}

void ArtifactManager::handleReturn(const std::string tokens[], int count)
{
    // Expected: RETURN <researcherName> <artifactID>
    // TODO:
    // 1) Validate existence of researcher and artifact.
    // 2) Check that artifact.assignedToName == researcherName.
    // 3) If not, print error.
    // 4) Otherwise, remove artifact from researcher list, clear assignedToName in AVL.
    //    Print "Artifact <id> returned by <name>."

    if(count==3){
        std::string name = tokens[1];
        int id = std::stoi(tokens[2]);
        ResearcherNode *researcher = researcherTree.findResearcher(name);
        if(researcher==nullptr){
            std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
            return;
        }
        ArtifactNode *artifact = artifactTree.findArtifact(id);
        if(artifact==nullptr){
            std::cout<<"Error: Artifact "<<id<<" not found."<<std::endl;
            return;
        }
        // checking if the researcher that artifact is assigned is matching
        if(artifact->data.assignedToName!=name){
            std::cout << "Error: Artifact " << id<< " is not assigned to " << name << "." << std::endl;
            return;
        }
        // making artifact unassigned
        researcher->data.removeArtifact(id);
        artifact->data.assignedToName="";
        std::cout<<"Artifact "<<id<<" returned by "<<name<<"."<<std::endl;
    }
}

void ArtifactManager::handleReturnAll(const std::string tokens[], int count)
{
    // Expected: RETURN_ALL <researcherName>
    // TODO:
    // 1) Find researcher; error if missing.
    // 2) For each artifact they supervise, clear assignedToName in AVL.
    // 3) Clear researcher's assignment list (removeAllArtifacts()).
    // 4) Print appropriate confirmation message.

    if(count==2){
        std::string name = tokens[1];
        ResearcherNode *researcher = researcherTree.findResearcher(name);
        if(researcher==nullptr){
            std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
            return;
        }
        // updating artifacts to be unassigned
        for(int i=0; i<researcher->data.numAssigned;i++){
            int id = researcher->data.assignedArtifacts[i];
            ArtifactNode *artifact = artifactTree.findArtifact(id);
            if(artifact){
                artifact->data.assignedToName="";
            }
        }
        // clearing the assignment list
        researcher->data.removeAllArtifacts();
        std::cout<<"All artifacts returned by "<<name<<"."<<std::endl;
    }
}

void ArtifactManager::handleResearcherLoad(const std::string tokens[], int count)
{
    // Expected: RESEARCHER_LOAD <name>
    // TODO:
    // 1) Find researcher by name.
    // 2) If not found, print error.
    // 3) Otherwise, print number of supervised artifacts in required format.

    if(count==2){
        std::string name = tokens[1];
        ResearcherNode *researcher = researcherTree.findResearcher(name);
        if(researcher==nullptr){
            std::cout<<"Error: Researcher "<<name<<" not found."<<std::endl;
            return;
        }
        // numAssigned = supervised artifact number
        std::cout<<"Researcher "<<name<<" is supervising "<<researcher->data.numAssigned<<" artifacts."<<std::endl;
    }

}

void ArtifactManager::handleMatchRarity(const std::string tokens[], int count)
{
    // Expected: MATCH_RARITY <minRarity>
    // TODO:
    // Traverse AVL tree and print all artifacts with rarity >= minRarity.
    // You may choose any reasonable order (probably inorder) unless specified otherwise
    // in your PDF. Artifacts may be assigned or unassigned; print as required.

    if(count==2){
        int minRarity = std::stoi(tokens[1]);
        std::cout<<"=== MATCH_RARITY "<<minRarity<<" ==="<<std::endl;
        // helper function to traverse AVL tree
        artifactTree.printRarity(minRarity);
    }
}


void ArtifactManager::handlePrintUnassigned(const std::string tokens[], int count)
{
    // Expected: PRINT_UNASSIGNED
    // TODO:
    // Print a header if needed, then call artifactTree.printUnassigned().

    if(count==1){
        std::cout<<"Unassigned artifacts:"<<std::endl;
        artifactTree.printUnassigned();
    }
}

void ArtifactManager::handlePrintStats(const std::string tokens[], int count)
{
    // Expected: PRINT_STATS
    // TODO:
    // 1) Compute:
    //    - totalArtifacts (artifactTree.getArtifactCount())
    //    - totalResearchers (researcherTree.getResearcherCount())
    //    - average artifact rarity (floor of totalRarity / totalArtifacts)
    //    - average researcher load (floor of totalLoad / totalResearchers)
    // 2) Print summary lines exactly as in the spec.
    // 3) Then:
    //    - Print researchers using preorder traversal:
    //      researcherTree.traversePreOrderForStats()
    //    - Print artifacts using postorder traversal:
    //      artifactTree.traversePostOrderForStats()
    //    (Exact formatting defined in your PDF.)

    if(count==1){
        std::cout<<"=== SYSTEM STATISTICS ==="<<std::endl;
        int totalArtifacts = artifactTree.getArtifactCount();
        int totalResearchers = researcherTree.getResearcherCount();
        int avgArtifactRarity = artifactTree.getTotalRarity()/totalArtifacts;
        int avgResearcherLoad = researcherTree.getTotalLoad()/totalResearchers;
        std::cout<<"Artifacts: "<<totalArtifacts<<std::endl;
        std::cout<<"Researchers: "<<totalResearchers<<std::endl;
        std::cout<<"Average rarity: "<<avgArtifactRarity<<std::endl;
        std::cout<<"Average load: "<<avgResearcherLoad<<std::endl;
        std::cout<<"Researchers:"<<std::endl;
        researcherTree.traversePreOrderForStats();
        std::cout<<"Artifacts:"<<std::endl;
        artifactTree.traversePostOrderForStats();
    }
}

void ArtifactManager::handleClear(const std::string tokens[], int count)
{
    // Expected: CLEAR
    // TODO:
    // Clear both trees and print confirmation message.
    
    if (count != 1) {
        return;
    }
    artifactTree.clear();
    researcherTree.clear();
    std::cout << "All data cleared." << std::endl;
    // e.g. "All data cleared."
}
