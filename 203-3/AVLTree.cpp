#include "AVLTree.h"

AVLTree::AVLTree()
    : root(nullptr)
{
}

AVLTree::~AVLTree()
{
    clear();
}

void AVLTree::clear()
{
    clear(root);
    root = nullptr;
}

void AVLTree::clear(ArtifactNode *node)
{
    //TODO

    if(node==nullptr){
        return;
    }

    clear(node->right);
    clear(node->left);

    delete node;
}

int AVLTree::height(ArtifactNode *node) const
{
    return node ? node->height : -1;
}

int AVLTree::getBalance(ArtifactNode *node) const
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

ArtifactNode *AVLTree::rotateLeft(ArtifactNode *x)
{
    // TODO: Standard AVL left rotation.
    //return new root

    ArtifactNode* rightNode = x->right; // new root after rotation
    ArtifactNode* leftNode = rightNode->left; 

    rightNode->left=x;
    x->right=leftNode;
    
    // updating height of x
    if(height(x->left)>height(x->right)){
        x->height=height(x->left)+1;
    }else{
        x->height=height(x->right)+1;
    }
    
    // updating height of new root
    if(height(rightNode->left)>height(rightNode->right)){
        rightNode->height=height(rightNode->left)+1;
    }else{
        rightNode->height=height(rightNode->right)+1;
    }
    x = rightNode;
    return x; // returning new root
}

ArtifactNode *AVLTree::rotateRight(ArtifactNode *y)
{
    // TODO: Standard AVL right rotation (mirror of rotateLeft).

    ArtifactNode* leftNode =  y->left;  // new root after rotation
    ArtifactNode* rightNode = leftNode->right;

    leftNode->right=y;
    y->left=rightNode;

    // updating height of y
    if(height(y->left)>height(y->right)){
        y->height=height(y->left)+1;
    }else{
        y->height=height(y->right)+1;
    }

    // updating height of new root
    if(height(leftNode->left)>height(leftNode->right)){
        leftNode->height=height(leftNode->left)+1;
    }else{
        leftNode->height=height(leftNode->right)+1;
    }
    y = leftNode;
    return y;  // returning new root
}

ArtifactNode *AVLTree::findMinNode(ArtifactNode *node) const
{
    // TODO: Return leftmost node in this subtree.

    if (node == nullptr){
        return nullptr;
    }
        
    while(node->left!=nullptr){
        node=node->left;  // finding leftmost node
    }
    return node;
}

ArtifactNode *AVLTree::insert(ArtifactNode *node, const Artifact &artifact, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by artifactID.
    // 2) If duplicate ID, set inserted = false and return node unchanged.
    // 3) On actual insertion, update node->height.
    // 4) Compute balance and apply AVL rotations if needed.
    // 5) Return the (possibly new) root of this subtree.

    // base case: empty subtree
    if(node==nullptr){
        inserted=true;
        return new ArtifactNode(artifact); // inserting new node
    }
    
    // duplicate key case : not inserting
    if(node->data.artifactID==artifact.artifactID){
        inserted = false;
        return node;
    }
    
    if(artifact.artifactID<node->data.artifactID){
        node->left=insert(node->left,artifact,inserted);
    }else{
        node->right=insert(node->right,artifact,inserted);
    }
    
    // updating height
    if(height(node->left)<height(node->right)){
        node->height=height(node->right)+1;
    }else{
        node->height=height(node->left)+1;
    }
    
    // computing balance factor
    int balance = getBalance(node);

    // insertion to left subtree of left child
    if(balance >1 && artifact.artifactID<node->left->data.artifactID){
        return rotateRight(node);
    }

    // insertion to right subtree of left child
    if(balance>1 && artifact.artifactID>node->left->data.artifactID){
        node->left=rotateLeft(node->left);
        return rotateRight(node);
    }

    // insertion to right subtree of right child
    if(balance<-1 && artifact.artifactID>node->right->data.artifactID){
        return rotateLeft(node);
    }

    // insertion to left subtree of right child
    if(balance<-1 && artifact.artifactID<node->right->data.artifactID){
        node->right=rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

ArtifactNode *AVLTree::remove(ArtifactNode *node, int artifactID, bool &removed)
{
    // TODO:
    // 1) Standard BST deletion by artifactID.
    // 2) If node not found, leave removed = false.
    // 3) Upon deletion, update heights and rebalance with rotations.
    // 4) Return (possibly new) root of this subtree.

    // base case : empty tree
    if(node==nullptr){
        removed = false;
        return node;
    }
    if(node->data.artifactID>artifactID){
        node->left=remove(node->left,artifactID,removed);
    }else if(node->data.artifactID<artifactID){
        node->right=remove(node->right,artifactID,removed);
    }else{
        //test for a leaf
        if(node->left==nullptr && node->right==nullptr){
            removed=true;
            delete node;
            return nullptr;

        // test for no left child
        }else if(node->left==nullptr){
            ArtifactNode* temp = node;
            node = node->right;
            temp->right = nullptr;
            removed=true;
            delete temp;

        // test for no right child
        }else if(node->right==nullptr){
            ArtifactNode* temp = node;
            node = node->left;
            temp->left = nullptr;
            removed=true;
            delete temp;

        // node with two child
        }else{
            ArtifactNode* temp = findMinNode(node->right);  // finding inorder successor
            node->data = temp->data; 
            node->right = remove(node->right, temp->data.artifactID, removed); // deleting the inorder successor from right subtree
        }

    }
    if (node == nullptr){
        return node;
    }

    // updating height
    if(height(node->left)<height(node->right)){
        node->height=height(node->right)+1;
    }else{
        node->height=height(node->left)+1;
    }
    
    int balance = getBalance(node);
        
    // left subtree of left child
    if(balance >1 && getBalance(node->left)>=0){
        return rotateRight(node);
    }

    // right subtree of left child
    if(balance<-1 && getBalance(node->right)>0){
        node->right=rotateRight(node->right);
        return rotateLeft(node);
    }

    // right subtree of right child
    if(balance<-1 && getBalance(node->right)<=0){
        return rotateLeft(node);
    }

    // left subtree of right child
    if(balance>1 && getBalance(node->left)<0){
        node->left=rotateLeft(node->left);
        return rotateRight(node);
    }

    return node;
}

bool AVLTree::insertArtifact(const Artifact &artifact)
{
    bool inserted = false;//tracks whether operation is succesful
    root = insert(root, artifact, inserted);
    return inserted;
}

bool AVLTree::removeArtifact(int artifactID)
{
    bool removed = false;//tracks whether operation is succesful
    root = remove(root, artifactID, removed);
    return removed;
}

ArtifactNode *AVLTree::findArtifact(int artifactID) const
{
    return find(root, artifactID);
}

ArtifactNode *AVLTree::find(ArtifactNode *node, int artifactID) const
{
    //TODO

    // empty tree
    if(node==nullptr){
        return nullptr;
    }
    
    if(node->data.artifactID==artifactID){
        return node;
    }else if (node->data.artifactID > artifactID) {
        return find(node->left,artifactID);
    }else{
        return find(node->right,artifactID);
    }

    // if not found
    return nullptr;
}

void AVLTree::setAssignedTo(int artifactID, const std::string &researcherName)
{

    //assign artifact to researcher

    ArtifactNode* node = findArtifact(artifactID);
    if(node!=nullptr){
        node->data.assignedToName = researcherName;
    }

}

void AVLTree::clearAssignedTo(int artifactID)
{
    //TODO
    ArtifactNode* node = findArtifact(artifactID); // checking if artifact exist
    if (node != nullptr) {
        node->data.assignedToName = "";
    }
}

void AVLTree::printUnassigned() const
{
    printUnassigned(root);
}

void AVLTree::printUnassigned(ArtifactNode *node) const
{
    // TODO:
    // Inorder traversal.
    // For each node with data.assignedToName == "", print in required format, e.g.:
    // <id> <name> <rarity> <value>

    if(node!=nullptr){
        printUnassigned(node->left);
        if(node->data.assignedToName==""){
            std::cout << node->data.artifactID << " "
                  << node->data.name << " "
                  << node->data.rarityLevel << " "
                  << node->data.researchValue << std::endl;
        }
        printUnassigned(node->right);

    }
    
}

int AVLTree::getArtifactCount() const
{
    return getArtifactCount(root);
}

int AVLTree::getArtifactCount(ArtifactNode *node) const
{
    // TODO: return size of subtree.

    int count = 0;
    if(node==nullptr){
        return 0;
    }
    count = 1 + getArtifactCount(node->left)+ getArtifactCount(node->right);
    return count;
}

int AVLTree::getTotalRarity() const
{
    return getTotalRarity(root);
}

int AVLTree::getTotalRarity(ArtifactNode *node) const
{
    // TODO: sum of rarityLevel over subtree.

    int count = 0;
    if(node==nullptr){
        return 0;
    }
    count = node->data.rarityLevel + getTotalRarity(node->left)+ getTotalRarity(node->right);
    return count;
}

void AVLTree::traversePostOrderForStats() const
{
    traversePostOrderForStats(root);
}

void AVLTree::traversePostOrderForStats(ArtifactNode *node) const
{
    // TODO:
    // Post-order traversal (left, right, node).
    // Students will decide what exactly to print in PRINT_STATS.

    // empty tree
    if (node == nullptr) {
        return;
    }
    traversePostOrderForStats(node->left);
    traversePostOrderForStats(node->right);
    
    std::cout<< node->data.artifactID<<" "<<
                node->data.name<<" "<<
                node->data.rarityLevel<<" "<<
                node->data.researchValue<<" ";
    if(node->data.assignedToName==""){
        std::cout<<"UNASSIGNED"<<std::endl;
    }else{
        std::cout<<node->data.assignedToName<<std::endl;
    }
}

void AVLTree::printRarity(int minRarity)
{
    printRarity(root, minRarity);
}

// helper function for MATCH_RARITY <minRarity>
void AVLTree::printRarity(ArtifactNode* node, int minRarity) {
    if (node == nullptr) return;

    // Inorder traversal: left, root, right
    printRarity(node->left, minRarity);

    if (node->data.rarityLevel >= minRarity) {
        std::cout<<node->data.artifactID<<" "<<node->data.name<<" "<<
                    node->data.rarityLevel<<" "<<node->data.researchValue<<" ";
        if(node->data.assignedToName==""){
            std::cout<<"UNASSIGNED"<<std::endl;
        }else{
            std::cout<<"ASSIGNED:"<<node->data.assignedToName<<std::endl;
        }
    }

    printRarity(node->right, minRarity);
}