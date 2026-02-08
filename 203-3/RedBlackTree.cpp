#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if(node==nullptr){
        return;
    }
    clear(node->right);
    clear(node->left);

    delete node;
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
    if(node==nullptr){
        return nullptr;  // not found
    }
    if(node->data.fullName==fullName){
        return node;  // found
    }else if(node->data.fullName<fullName){ // search right subtree
        return find(node->right,fullName);
    }else{
        return find(node->left,fullName);  // search left subtree
    }
        return nullptr;
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise

    ResearcherNode *node = new ResearcherNode(researcher);
    bool inserted = false;
    root = bstInsert(root,node,inserted);
    if(inserted){
        insertFixup(node);
        return true;
    }
    delete node;
    return false;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).

    // empty position : insert node
    if(current==nullptr){
        current=node;
        inserted=true;
        return current;
    }
    // duplicate key
    if(current->data.fullName==node->data.fullName){
        inserted=false;
        return current;
    }
    // move left subtree
    if(current->data.fullName>node->data.fullName){
        current->left=bstInsert(current->left,node,inserted);
        if(current->left){
            current->left->parent=current;
        }
    // move right subtree
    }else{
        current->right=bstInsert(current->right,node,inserted);
        if(current->right){
            current->right->parent=current;
        }
    }

    return current; 
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.

    while(node != root && node->parent->color==RED){

        // parent is the left child of grandparent
        if(node->parent==node->parent->parent->left){
            ResearcherNode *uncle = node->parent->parent->right;

            // uncle is RED : only flipping colors
            if(uncle != nullptr && uncle->color==RED){
                node->parent->color=BLACK;
                uncle->color=BLACK;
                node->parent->parent->color=RED;
                node=node->parent->parent;
            }

            // uncle is BLACK
            else{
                // node is the left child of parent 
                if(node->parent->left==node){
                    // flipping colors
                    node->parent->color=BLACK;
                    node->parent->parent->color=RED;
                    rotateRight(node->parent->parent);
                }
                // node is the right child of parent 
                else{
                    node = node->parent;
                    rotateLeft(node);
                    node->parent->color=BLACK;
                    node->parent->parent->color=RED;
                    rotateRight(node->parent->parent);
                }

            }
        }
        // parent is the right child of grandparent
        else{
            ResearcherNode *uncle = node->parent->parent->left;

            // uncle is RED : only flipping colors
            if(uncle != nullptr && uncle->color==RED){
                node->parent->color=BLACK;
                uncle->color=BLACK;
                node->parent->parent->color=RED;
                node = node->parent->parent;
            }

            // uncle is BLACK 
            else{
                  // node is the left child of parent
                if(node->parent->left==node){
                    // flipping colors
                    node = node->parent;
                    rotateRight(node);
                    node->parent->color=BLACK;
                    node->parent->parent->color=RED;
                    rotateLeft(node->parent->parent);
                }
                // node is the right child of parent 
                else{
                    node->parent->color=BLACK;
                    node->parent->parent->color=RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
    }
    root->color=BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.

    ResearcherNode *z = findResearcher(fullName);
    if(z){
        ResearcherNode *removedNode = z; // will be removed
        Color origColor = removedNode->color; 
        ResearcherNode *replacementNode = nullptr;  // will come at z's position
        ResearcherNode *repParent = nullptr;  // parent of replacement node

        // no left child
        if(z->left==nullptr){
            replacementNode = z->right;
            repParent = z->parent;
            if(z->parent==nullptr){
                root=replacementNode;
            }else if(z==z->parent->left){
                z->parent->left=replacementNode;
            }else{
                z->parent->right=replacementNode;
            }

            if(replacementNode){
                replacementNode->parent = z->parent;
            }
        }

        // no right child
        else if(z->right==nullptr){
            replacementNode = z->left;
            repParent = z->parent;
            if(z->parent==nullptr){
                root=replacementNode;
            }else if(z==z->parent->left){
                z->parent->left=replacementNode;
            }else{
                z->parent->right=replacementNode;
            }

            if(replacementNode){
                replacementNode->parent = z->parent;
            }
        }

        // two children
        else{
            // finding successor
            removedNode = minimum(z->right);
            origColor = removedNode->color;
            replacementNode = removedNode->right;  // successor can only have right child

            if(removedNode->parent==z){
                repParent=removedNode;  
            }else{
                // Detach successor from its original position
                repParent = removedNode->parent;
                if(removedNode==removedNode->parent->left){
                    removedNode->parent->left=replacementNode;
                }else{
                    removedNode->parent->right=replacementNode;
                }

                if(replacementNode){
                    replacementNode->parent = removedNode->parent;
                }
                removedNode->right=z->right;
                removedNode->right->parent=removedNode;
            }
            
            // replace z with successor
            if(z->parent==nullptr){
                root=removedNode;
            }else if(z==z->parent->left){
                z->parent->left=removedNode;
            }else{
                z->parent->right=removedNode;
            }

            removedNode->parent = z->parent;
            removedNode->left=z->left;
            removedNode->left->parent=removedNode;
            removedNode->color=z->color;

        }
        // free removed node
        delete z;

        if(origColor==BLACK){
            deleteFixup(replacementNode,repParent);
        }
        return true;
    }
    return false;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    while(node != root && (node == nullptr || node->color == BLACK)){
        if(node==parent->left){
            ResearcherNode *sibling = parent->right;

            if (sibling == nullptr) {
                node = parent;
                parent = node->parent;
                continue;
            }

            // case1: sibling is red
            if(sibling->color==RED){
                sibling->color=BLACK;
                parent->color=RED;
                rotateLeft(parent);
                sibling=parent->right;

            // case2: sibling is black and its children are black
            }else if((sibling->left==nullptr || sibling->left->color==BLACK) && (sibling->right==nullptr || sibling->right->color==BLACK)){
                sibling->color=RED;
                node = parent;
                parent = node->parent;

            }else{

                // case3: sibling is black and sibling's left child is red
                if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                    (sibling->left != nullptr && sibling->left->color == RED)){
                    sibling->color=RED;
                    if(sibling->left){
                        sibling->left->color=BLACK;
                    }
                    rotateRight(sibling);
                    sibling=parent->right;
                }

                // case4: sibling is black and siblings's right child is red
                sibling->color=parent->color;
                parent->color=BLACK;
                if(sibling->right){
                    sibling->right->color=BLACK;
                }
                rotateLeft(parent);
                break;  
            }
        }else{
            ResearcherNode *sibling = parent->left;

            if (sibling == nullptr) {
                node = parent;
                parent = node->parent;
                continue;
            }

            // case1: sibling is red
            if(sibling->color==RED){
                sibling->color=BLACK;
                parent->color=RED;
                rotateRight(parent);
                sibling=parent->left;
            

            // case2:  sibling is black and its children are black
            }else if((sibling->left==nullptr || sibling->left->color==BLACK) && (sibling->right==nullptr || sibling->right->color==BLACK)){
                sibling->color=RED;
                node = parent;
                parent = node->parent;

            }else{

                //  case3: sibling is black and sibling's right child is red
                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    ( sibling->right != nullptr && sibling->right->color == RED)){
                    sibling->color=RED;
                    if(sibling->right){
                        sibling->right->color=BLACK;
                    }
                    rotateLeft(sibling);
                    sibling=parent->left;
                }

                // case4: sibling is black and siblings's left child is red
                sibling->color=parent->color;
                parent->color=BLACK;
                if(sibling->left){
                    sibling->left->color=BLACK;
                }
                rotateRight(parent);
                break;
            }
        }
    }
    if(node){
        node->color=BLACK;
    }
}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.

    if (node == nullptr){
        return nullptr;
    }
    // moving to the leftmost child
    while(node->left!=nullptr){
        node=node->left;
    }
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    
    ResearcherNode *node = x->right; // new root
    x->right=node->left;
    if(node->left){
        node->left->parent=x;
    }
    node->parent=x->parent;
    if(x->parent==nullptr){
        root=node;  // x was root
    }else if(x==x->parent->right){
        x->parent->right=node;
    }else{
        x->parent->left=node;
    }
    node->left=x;
    x->parent=node;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    ResearcherNode *node = y->left; // new root
    y->left=node->right;
    if(node->right){
        node->right->parent=y;
    }
    node->parent=y->parent;
    if(y->parent==nullptr){
        root=node;  // y was root
    }else if(y==y->parent->right){
        y->parent->right=node;
    }else{
        y->parent->left=node;
    }
    node->right=y;
    y->parent=node;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    int count = 0;
    if(node==nullptr){
        return 0;
    }
    count = 1 + getResearcherCount(node->left)+ getResearcherCount(node->right);
    return count;
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    int count = 0;
    if(node==nullptr){
        return 0;
    }
    count = node->data.numAssigned + getTotalLoad(node->left) + getTotalLoad(node->right);
    return count;
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.

    if (node == nullptr) {
        return;
    }

    std::cout<< node->data.fullName<< " "
            <<node->data.capacity<<" "
            <<node->data.numAssigned<<std::endl;

    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);

}
