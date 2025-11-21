// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const
{
    int height = this->getHeight();
    if (this->root == NULL)
    {
        cout << "NULL\n";
        return;
    }
    queue<AVLNode *> q;
    q.push(root);
    AVLNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    int space = pow(2, height);
    printNSpace(space / 2);
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        if (temp == NULL)
        {
            cout << " ";
            q.push(NULL);
            q.push(NULL);
        }
        else
        {
            cout << temp->data;
            q.push(temp->pLeft);
            q.push(temp->pRight);
        }
        printNSpace(space);
        count++;
        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        if (level == height)
            return;
    }
}

// TODO: Implement all AVLTree<K, T> methods here

// XÂY DỰNG CÂY AVL MEDIAN-BASED TRỰC TIẾP (KHÔNG dùng insert)
// Thêm hàm helper để insert trực tiếp vào node (bypass insert logic)
template<class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::buildBalancedTree(const std::vector<std::pair<K, T>>& sortedPairs, int left, int right) {
     if (left > right) return nullptr;
    
    int mid = (left + right) / 2;
    AVLNode* node = new AVLNode(sortedPairs[mid].first, sortedPairs[mid].second);
    
    //  XÂY DỰNG CÂY CON TRÁI VÀ PHẢI (không gán pParent)
    node->pLeft = buildBalancedTree(sortedPairs, left, mid - 1);
    node->pRight = buildBalancedTree(sortedPairs, mid + 1, right);
    
    //  CẬP NHẬT HEIGHT
    node->height = 1 + std::max(height(node->pLeft), height(node->pRight));
    
    //  CẬP NHẬT BALANCE
    int leftHeight = height(node->pLeft);
    int rightHeight = height(node->pRight);
    int balance = rightHeight - leftHeight;
    
    if (balance == -1 || balance == 0 || balance == 1) {
        node->balance = static_cast<BalanceValue>(balance);
    }
    
    return node;
}

template<class K, class T> 
void AVLTree<K,T>:: clearHelper(AVLNode* node) {
	if (node == nullptr) return;
    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template<class K, class T> 
AVLTree<K,T>::AVLTree() {
	this->root=nullptr; 
}

template<class K, class T> 
AVLTree<K,T>::~AVLTree() {
	clearHelper(root); 
	this->root=nullptr;
}

template<class K, class T> 
int AVLTree<K,T>:: height(AVLNode* n) const {
if (n==nullptr) return 0; 
int hl = height(n->pLeft);
int hr = height(n->pRight);
return 1 + (hl > hr ? hl : hr);
}

template<class K, class T> 
int AVLTree<K,T>::getHeight(AVLNode* node) const {
    if (node == nullptr) return 0;
    return node->height;  // O(1) - chỉ đọc field
}

template<class K, class T>
void AVLTree<K,T>::updateHeight(AVLNode* node) {
    if (node == nullptr) return;
    int leftHeight = getHeight(node->pLeft);
    int rightHeight = getHeight(node->pRight);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

template<class K, class T> 
int AVLTree<K,T>:: getHeight() const {
return height(root);  
}

template<class K, class T> 
typename AVLTree<K,T> ::AVLNode*  AVLTree<K,T>::rotateRight(AVLNode*& node) {

	if (node==nullptr||node->pLeft==nullptr) return node; 

	AVLNode* leftChild=node->pLeft; 
	node->pLeft=leftChild->pRight; 
	leftChild->pRight=node; 

	return leftChild; 
}

template<class K,class T>
typename AVLTree<K,T>:: AVLNode* AVLTree<K,T>:: rotateLeft(AVLNode*& node) {

	if (node==nullptr||node->pRight==nullptr) return node; 

	AVLNode* rightChild=node->pRight; 
	node->pRight=rightChild->pLeft; 
	rightChild->pLeft=node; 

	return rightChild; 
}

template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::insertRec(AVLNode* node, const K& key, const T& value) {
    // 1. Chèn như BST
    if (node == nullptr)
        return new AVLNode(key, value);

    if (key < node->key)
        node->pLeft = insertRec(node->pLeft, key, value);
    else if (key > node->key)
        node->pRight = insertRec(node->pRight, key, value);
    else
        return node;

    
    int balance = getBalance(node);

    

    // Case 1: Left Left
    if (balance > 1 && key < node->pLeft->key)
        return rotateRight(node);

    // Case 2: Right Right
    if (balance < -1 && key > node->pRight->key)
        return rotateLeft(node);

    // Case 3: Left Right
    if (balance > 1 && key > node->pLeft->key) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // Case 4: Right Left
    if (balance < -1 && key < node->pRight->key) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    
    return node;
}

template <class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value) {
    root = insertRec(root, key, value);
}

template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::findMin(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->pLeft != nullptr)
        current = current->pLeft;
    return current;
}

template<class K, class T>
int AVLTree<K, T>::getBalance(AVLNode* node) {
if (node == nullptr) return 0;
return height(node->pLeft) - height(node->pRight);
}
 
template<class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeRec(AVLNode* node, const K& key) {
    if (node == nullptr)
        return node;

    // --- 1. Xóa như BST ---
    if (key < node->key)
        node->pLeft = removeRec(node->pLeft, key);
    else if (key > node->key)
        node->pRight = removeRec(node->pRight, key);
    else {
        // Node có 1 hoặc 0 con
        if (node->pLeft == nullptr || node->pRight == nullptr) {
            AVLNode* temp = node->pLeft ? node->pLeft : node->pRight;

            if (temp == nullptr) { // không có con
                temp = node;
                node = nullptr;
            } else { // có 1 con
                *node = *temp;
            }
            delete temp;
        } else {
            // Node có 2 con
            AVLNode* temp = findMin(node->pRight);
            node->key = temp->key;
            node->data = temp->data;
            node->pRight = removeRec(node->pRight, temp->key);
        }
    }

    if (node == nullptr)
        return node;

    // --- 2. Tính lại balance factor ---
    int balance = getBalance(node);

    // --- 3. 4 Trường hợp mất cân bằng sau khi xóa ---

    // Left Left
    if (balance > 1 && getBalance(node->pLeft) >= 0)
        return rotateRight(node);

    // Left Right
    if (balance > 1 && getBalance(node->pLeft) < 0) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // Right Right
    if (balance < -1 && getBalance(node->pRight) <= 0)
        return rotateLeft(node);

    // Right Left
    if (balance < -1 && getBalance(node->pRight) > 0) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    return node;
}

template<class K, class T> 
 void AVLTree<K,T>:: remove(const K& key) {
	 root=removeRec(root, key); 
 }

template<class K, class T> 
bool AVLTree<K,T>:: contains(const K& key) const {
	AVLNode* current=root; 
	while (current) {
		if (key==current->key) {
			return true; 
		}
		if (key<current->key) current=current->pLeft; 
		else current=current->pRight; 
	}
	return false; 
}

template<class K, class T>
int AVLTree<K,T>:: countNodes(AVLNode* node) const {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->pLeft) + countNodes(node->pRight);
    }

template<class K, class T> 
int AVLTree<K,T>:: getSize() const {
return countNodes(root); 
}

template<class K, class T>
bool AVLTree<K,T>:: empty() const {
return (root==nullptr); 
}

template<class K, class T>
void AVLTree<K,T>:: clear() {
clearHelper(root);
root=nullptr; 
}


template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLNode* node, void (*action)(const T&)) const {
    if (node == nullptr) return;
    inorderHelper(node->pLeft, action);   
    action(node->data);                   
    inorderHelper(node->pRight, action);  
}

template<class K, class T> 
void AVLTree<K,T>:: inorderTraversal(void (*action)(const T&)) const {
	inorderHelper(root,action); 
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================
template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const
{
    if (this->root == nullptr)
    {
        cout << "NULL\n";
        return;
    }

    queue<RBTNode *> q;
    q.push(root);
    RBTNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;

    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode *> heightQ;
    heightQ.push(root);
    while (!heightQ.empty())
    {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++)
        {
            RBTNode *node = heightQ.front();
            heightQ.pop();
            if (node->left)
                heightQ.push(node->left);
            if (node->right)
                heightQ.push(node->right);
        }
    }

    int space = pow(2, height);
    printNSpace(space / 2);

    while (!q.empty())
    {
        temp = q.front();
        q.pop();

        if (temp == nullptr)
        {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        }
        else
        {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED)
            {
                cout << "R)";
            }
            else
            {
                cout << "B)";
            }

            q.push(temp->left);
            q.push(temp->right);
        }

        printNSpace(space);
        count++;

        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }

        if (level == height)
        {
            return;
        }
    }
}

// TODO: Implement all other RedBlackTree<K, T> methods here
template <class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K &key, const T &value)
    : key(key), data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr)
{
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed()
{
    this->color = RED;
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack()
{
    this->color = BLACK;
}

// =====================================
// Rotation methods implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode* node)
{
    if (node == nullptr || node->right == nullptr) return;
    
    RBTNode* rightChild = node->right;
    
    // Step 1: rightChild's left becomes node's right
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }
    
    // Step 2: Update parent pointers
    rightChild->parent = node->parent;
    
    if (node->parent == nullptr) {
        // node was root, now rightChild becomes root
        root = rightChild;
    } else if (node == node->parent->left) {
        // node was left child
        node->parent->left = rightChild;
    } else {
        // node was right child
        node->parent->right = rightChild;
    }
    
    // Step 3: node becomes left child of rightChild
    rightChild->left = node;
    node->parent = rightChild;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode* node)
{
    if (node == nullptr || node->left == nullptr) return;
    
    RBTNode* leftChild = node->left;
    
    // Step 1: leftChild's right becomes node's left
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }
    
    // Step 2: Update parent pointers
    leftChild->parent = node->parent;
    
    if (node->parent == nullptr) {
        // node was root, now leftChild becomes root
        root = leftChild;
    } else if (node == node->parent->left) {
        // node was left child
        node->parent->left = leftChild;
    } else {
        // node was right child
        node->parent->right = leftChild;
    }
    
    // Step 3: node becomes right child of leftChild
    leftChild->right = node;
    node->parent = leftChild;
}

// =====================================
// Bound methods implementation
// =====================================

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBoundNode(const K& key) const
{
    RBTNode* current = root;
    RBTNode* result = nullptr;
    
    while (current != nullptr) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            result = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBoundNode(const K& key) const
{
    RBTNode* current = root;
    RBTNode* result = nullptr;
    
    while (current != nullptr) {
        if (key < current->key) {
            result = current;
            current = current->left;
        } else if (key == current->key) {
            current = current->right;
        } else {
            // key > current->key, continue going right
            current = current->right;
        }
    }
    return result;
}


//====================================
//Constructor, destructor
//====================================
template <class K, class T>
RedBlackTree<K, T>::RedBlackTree() : root(nullptr)
{
}

template <class K, class T>
RedBlackTree<K, T>::~RedBlackTree()
{
    clear();
}

template <class K, class T> 
bool RedBlackTree<K,T>:: empty() const {
return root==nullptr; 
}

// =====================================
// Size and Clear methods implementation
// =====================================

template <class K, class T>
int RedBlackTree<K, T>::size() const
{
    if (root == nullptr) {
        return 0;
    }
    
    std::queue<RBTNode*> q;
    q.push(root);
    int count = 0;
    
    while (!q.empty()) {
        RBTNode* current = q.front();
        q.pop();
        count++;
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    
    return count;
}

template <class K, class T>
void RedBlackTree<K, T>::clear()
{
    if (root == nullptr) {
        return;
    }
    
    std::queue<RBTNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        RBTNode* current = q.front();
        q.pop();
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
        
        delete current;
    }
    
    root = nullptr;
}

// =====================================
// Insert and balancing implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::insert(const K& key, const T& value)
{
    // Create new node with RED color
    RBTNode* newNode = new RBTNode(key, value);
    
    // Standard BST insertion
    if (root == nullptr) {
        // Case 1: Tree is empty, new node becomes root
        root = newNode;
        newNode->recolorToBlack(); // Root must be black
        return;
    }
    
    // Find insertion position
    RBTNode* current = root;
    RBTNode* parent = nullptr;
    
    while (current != nullptr) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            // Key already exists, do nothing (as per specification)
            delete newNode;
            return;
        }
    }
    
    // Insert new node
    newNode->parent = parent;
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    // Fix Red-Black Tree violations
    insertFixup(newNode);
}

template <class K, class T>
void RedBlackTree<K, T>::insertFixup(RBTNode* node)
{
    while (node != root && node->parent->color == RED) {
        RBTNode* parent = node->parent;
        RBTNode* grandparent = parent->parent;
        
        // Case A: Parent is left child of grandparent
        if (parent == grandparent->left) {
            RBTNode* uncle = grandparent->right;
            
            // Case 1: Uncle is RED (recoloring)
            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent; // Move up to grandparent
            } else {
                // Case 2: Node is right child (triangle case)
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                
                // Case 3: Node is left child (line case)
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateRight(grandparent);
            }
        } 
        // Case B: Parent is right child of grandparent (symmetric)
        else {
            RBTNode* uncle = grandparent->left;
            
            // Case 1: Uncle is RED (recoloring)
            if (uncle != nullptr && uncle->color == RED) {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent; // Move up to grandparent
            } else {
                // Case 2: Node is left child (triangle case)
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                
                // Case 3: Node is right child (line case)
                parent->recolorToBlack();
                grandparent->recolorToRed();
                rotateLeft(grandparent);
            }
        }
    }
    
    // Ensure root is black (Case 1 from specification)
    root->recolorToBlack();
}


//=================================
//DELETE AND FIXUP
//=================================

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::maximum(RBTNode* node) 
{
    while (node != nullptr && node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template <class K, class T>
void RedBlackTree<K, T>::transplant(RBTNode* u, RBTNode* v)
{
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::deleteFixup(RBTNode* x, RBTNode* xParent) {
    while (x != root && (x == nullptr || x->color == BLACK)) {
        if (x == xParent->left) {
            RBTNode* w = xParent->right;
            
            // Case 1: sibling is RED
            if (w != nullptr && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                rotateLeft(xParent);
                w = xParent->right;
            }
            
            // Case 2: both sibling's children are BLACK
            if ((w == nullptr || w->left == nullptr || w->left->color == BLACK) &&
                (w == nullptr || w->right == nullptr || w->right->color == BLACK)) {
                if (w != nullptr) {
                    w->color = RED;
                }
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3: sibling's left child is RED, right child is BLACK
                if (w != nullptr && (w->right == nullptr || w->right->color == BLACK)) {
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                    if (w != nullptr) {
                        w->color = RED;
                    }
                    rotateRight(w);
                    w = xParent->right;
                }
                
                // Case 4: sibling's right child is RED
                if (w != nullptr) {
                    w->color = xParent->color;
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                }
                xParent->color = BLACK;
                rotateLeft(xParent);
                x = root;
            }
        } else {
            // Symmetric case (x is right child)
            RBTNode* w = xParent->left;
            
            // Case 1: sibling is RED
            if (w != nullptr && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                rotateRight(xParent);
                w = xParent->left;
            }
            
            // Case 2: both sibling's children are BLACK
            if ((w == nullptr || w->left == nullptr || w->left->color == BLACK) &&
                (w == nullptr || w->right == nullptr || w->right->color == BLACK)) {
                if (w != nullptr) {
                    w->color = RED;
                }
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3: sibling's right child is RED, left child is BLACK
                if (w != nullptr && (w->left == nullptr || w->left->color == BLACK)) {
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                    }
                    if (w != nullptr) {
                        w->color = RED;
                    }
                    rotateLeft(w);
                    w = xParent->left;
                }
                
                // Case 4: sibling's left child is RED
                if (w != nullptr) {
                    w->color = xParent->color;
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                    }
                }
                xParent->color = BLACK;
                rotateRight(xParent);
                x = root;
            }
        }
    }
    
    if (x != nullptr) {
        x->color = BLACK;
    }
}

template <class K, class T>
void RedBlackTree<K, T>::remove(const K& key)
{
    RBTNode* z = find(key);
    if (z == nullptr) return;
    
    RBTNode* y = z;
    RBTNode* x = nullptr;
    RBTNode* xParent = nullptr;  
    Color y_original_color = y->color;
    
    if (z->left == nullptr) {
        x = z->right;
        xParent = z->parent;  
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        xParent = z->parent;  
        transplant(z, z->left);
    } else {
        y = maximum(z->left);
        y_original_color = y->color;
        x = y->left;
        xParent = y;  
        
        if (y->parent == z) {
            if (x != nullptr) x->parent = y;
            xParent = y;
        } else {
            xParent = y->parent;
            transplant(y, y->left);
            y->left = z->left;
            y->left->parent = y;
        }
        
        transplant(z, y);
        y->right = z->right;
        if (y->right != nullptr) {
            y->right->parent = y;
        }
        y->color = z->color;
    }
    
    delete z;
    
    if (y_original_color == BLACK) {
        deleteFixup(x, xParent);  
    }
}


//=================================
//OTHERS 
//=================================
template <class K, class T> 
typename RedBlackTree<K,T>::RBTNode * RedBlackTree<K,T>:: find(const K &key) const {
RBTNode* current = root;
    
    while (current != nullptr) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K,T>:: contains(const K &key) const {
return (find(key)!=nullptr); 
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::lowerBound(const K& key, bool& found) const
{
    RBTNode* node = lowerBoundNode(key);
    found = (node != nullptr && node->key == key);
    return node;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::upperBound(const K& key, bool& found) const
{
    RBTNode* node = upperBoundNode(key);
    found = (node != nullptr && node->key > key);
    return node;
}

// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream &operator<<(std::ostream &os, const VectorRecord &record)
{
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================
double VectorStore::distanceByMetric(
    const std::vector<float> &a,
    const std::vector<float> &b,
    const std::string &metric) const
{
    if (metric == "euclidean" || metric == "l2")
        return l2Distance(a, b);
    if (metric == "manhattan" || metric == "l1")
        return l1Distance(a, b);
    if (metric == "cosine")
        return 1.0 - cosineSimilarity(a, b); 
    throw std::invalid_argument("Unknown metric type: " + metric);
}

void VectorStore:: buildAVLFromSortedRecords(const std::vector<VectorRecord*>& sortedRecords,
                                   VectorRecord* newRoot, int left, int right) {
        if (left > right) return;
        int mid = (left + right) / 2;
        VectorRecord* rec = sortedRecords[mid];
        double distToRoot = l2Distance(*(rec->vector), *(newRoot->vector));
        vectorStore->insert(distToRoot, *rec);
        buildAVLFromSortedRecords(sortedRecords, newRoot, left, mid-1);  // nửa trái
        buildAVLFromSortedRecords(sortedRecords, newRoot, mid+1, right); // nửa phải
} 

void VectorStore::buildBalancedAVL(const std::vector<VectorRecord*>& sortedRecords, 
                                    int left, int right) {
    if (left > right) return;
    
    int mid = left + (right - left) / 2;
    VectorRecord* rec = sortedRecords[mid];
    
    // Insert vào AVL với key = distanceFromReference (đã update)
    vectorStore->insert(rec->distanceFromReference, *rec);
    
    // Đệ quy left và right
    buildBalancedAVL(sortedRecords, left, mid - 1);
    buildBalancedAVL(sortedRecords, mid + 1, right);
}

void VectorStore::rebuildTreeWithNewRoot(VectorRecord *newRoot) {
if (!newRoot) return;
    
    // Lấy danh sách từ allRecords
    std::vector<VectorRecord*> sortedRecords;
    for (auto& rec : allRecords) {
        if (rec.vector) {
            sortedRecords.push_back(&rec);
        }
    }
    
    // Thay std::sort bằng mergeSortVectorRecords
    if (!sortedRecords.empty()) {
        mergeSortVectorRecords(sortedRecords, 0, sortedRecords.size() - 1);
    }
    
    // Rebuild cây AVL
    vectorStore->clear();
    buildAVLFromSortedRecords(sortedRecords, newRoot, 0, sortedRecords.size() - 1);
}

void VectorStore::rebuildRootIfNeeded() {
    std::vector<VectorRecord*> sortedRecords = getAllVectorsSortedByDistance();
    size_t medianIdx = sortedRecords.size() / 2;
    VectorRecord* medianRoot = (sortedRecords.empty() ? nullptr : sortedRecords[medianIdx]);
    if (medianRoot && medianRoot != rootVector) {
        rootVector = medianRoot;
        rebuildTreeWithNewRoot(rootVector);
    }
}

VectorStore::VectorStore(int dimension, vector<float>* (*embeddingFunction)(const string&),
                        const vector<float>& referenceVector) :
dimension(dimension),
count(0),
averageDistance(0.0),
embeddingFunction(embeddingFunction),
rootVector(nullptr),
nextId(1)
{
    vectorStore= new AVLTree<double, VectorRecord>();
    normIndex= new RedBlackTree<double, VectorRecord>();
    this->referenceVector = new vector<float>(referenceVector);

    //=====================ADD IN TO ADVANCE METHOD================
    minDistanceFromRef = std::numeric_limits<double>::max();
    maxDistanceFromRef = 0.0;
    //=============================================================

   
    allRecords.reserve(10000);  
}

VectorStore::~VectorStore() {
    clear(); 
}

int VectorStore::size() {
    return count; 
}

bool VectorStore::empty() {
    return (size()==0); 
}

void VectorStore:: clear() {
    // Bước 1: Clear các cây chỉ mục (chỉ giải phóng node, data là bản sao nên an toàn) 
    if (vectorStore) vectorStore->clear();
    if (normIndex) normIndex->clear();
    
    // Bước 2: Clear allRecords - QUAN TRỌNG: giải phóng vector bên trong mỗi record
    for (VectorRecord& record : allRecords) {
        if (record.vector != nullptr) {
            delete record.vector;
            record.vector = nullptr;
        }
    }
    allRecords.clear();
    
    // Bước 3: Reset các thống kê
    count = 0;
    averageDistance = 0.0;
    if (rootVector) rootVector = nullptr;
}

vector<float> * VectorStore:: preprocessing(std::string rawText) {
    if (embeddingFunction == nullptr) return nullptr;  
    vector<float>* rawVector = embeddingFunction(rawText);
    if (rawVector == nullptr) {
        return new vector<float>(dimension, 0.0f);
    }
    vector<float>* result = new vector<float>();
    if (rawVector->size() > static_cast<size_t>(dimension)) {
        result->assign(rawVector->begin(), rawVector->begin() + dimension);
    }
    else if (rawVector->size() < static_cast<size_t>(dimension)) {
        *result = *rawVector;
        result->resize(dimension, 0.0f);
    }
    else {
        // Kích thước vừa đủ: sao chép nguyên bản
        *result = *rawVector;
    }
    // Bước 5: Giải phóng vector thô (nếu cần)
    delete rawVector;
    
    return result;
}

double VectorStore:: calculateNorm(const vector<float>& vec) const {
double sum = 0.0;
    for (float value : vec) {
        sum += value * value;
    }
    return sqrt(sum);
}

void VectorStore::addText(std::string rawText) {
    // Tính ID mới
    if (count==0) nextId=0; 
    if (count > 0) {
        // Tìm ID lớn nhất hiện có
        int maxId = -1;
    for (const auto& record : allRecords) {
        if (record.id > maxId) maxId = record.id;
    }
        nextId = maxId + 1;
    }

    vector<float>* processedVector = preprocessing(rawText);
    double distance = l2Distance(*processedVector, *referenceVector);

    //================ADD ADVANCED METHOD===============
     // Cập nhật min/max
    if (distance < minDistanceFromRef) {
        minDistanceFromRef = distance;
    }
    if (distance > maxDistanceFromRef) {
        maxDistanceFromRef = distance;
    }
    //==========================================

    double norm = calculateNorm(*processedVector);

    VectorRecord newRecord;
    newRecord.id=nextId; 
    newRecord.rawText = rawText;
    newRecord.vector = processedVector;
    newRecord.distanceFromReference = distance;
    newRecord.norm = norm;
    newRecord.rawLength = rawText.length();

    // Push to allRecords first
    allRecords.push_back(newRecord);
    VectorRecord& mainRecord = allRecords.back();

    // Update averageDistance and count
    averageDistance = (averageDistance * count + distance) / (count + 1);
    count++;

    // Insert into AVL with key = distance to reference (initially)
    vectorStore->insert(mainRecord.distanceFromReference, mainRecord);


    //THÊM VÀO ĐỂ DEBUG===============================================
    // SAU KHI INSERT, ĐỒNG BỘ rootVector với AVL root
    if (vectorStore->root != nullptr) {
        // Tìm record trong allRecords có ID = root->data.id
        int rootId = vectorStore->root->data.id;
        for (auto& rec : allRecords) {
            if (rec.id == rootId && rec.vector != nullptr) {
                rootVector = &rec;
                break;
            }
        }
    }

    // Insert into RBT with key = norm
    normIndex->insert(mainRecord.norm, mainRecord);
    updatenextId(); // đảm bảo lấy id mới hợp lệ, gọi đầu hoặc ngay trước khi gán id cho record mới
}

// Private helper để tìm VectorRecord* tại vị trí index inorder
VectorRecord* VectorStore::findInorderIndex(AVLTree<double, VectorRecord>::AVLNode* node, int& currentIndex,int targetIndex) {
    if (node == nullptr) return nullptr;
    
    // 1. Duyệt left
    VectorRecord* result = findInorderIndex(node->pLeft, currentIndex, targetIndex);
    if (result != nullptr) return result;
    
    // 2. Kiểm tra node hiện tại
    bool isMatch = (currentIndex == targetIndex);
    
    //LUÔN TĂNG INDEX (dù match hay không)
    currentIndex++;
    
    // 3. Nếu match, tìm và return
    if (isMatch) {
        int targetId = node->data.id;
        for (VectorRecord& record : allRecords) {
            if (record.id == targetId) {
                return &record;
            }
        }
        return nullptr; // Không tìm thấy
    }
    
    // 4. Duyệt right
    return findInorderIndex(node->pRight, currentIndex, targetIndex);
}

VectorRecord* VectorStore::getVector(int index) {
if (index<0 || index >=count) throw out_of_range("Index is invalid!"); 
int currentIndex = 0;
    AVLTree<double, VectorRecord>::AVLNode* root = vectorStore->getRoot();  // Access root (friend)
    VectorRecord* result = findInorderIndex(root, currentIndex, index); 
    if (result == nullptr) {
        // Nên không xảy ra nếu check count, nhưng an toàn
        throw std::out_of_range("Index is invalid!");
    }
    return result;
}

std::string VectorStore::getRawText(int index) {
    VectorRecord* rec = getVector(index);
    return rec->rawText;
}

int VectorStore:: getId(int index) {
VectorRecord* rec=getVector(index); 
return rec->id; 
}

VectorRecord* VectorStore::findVectorNearestToDistance(double targetDistance) const {
    if (count == 0 || allRecords.empty()) {
        return nullptr;
    }
    
    double minDiff = std::numeric_limits<double>::max();
    VectorRecord* bestRecord = nullptr;
    
    //TÌM TRỰC TIẾP TRONG allRecords
    for (const VectorRecord& rec : allRecords) {
        if (!rec.vector) continue;
        
        double diff = fabs(rec.distanceFromReference - targetDistance);
        
        if (bestRecord == nullptr) {
            minDiff = diff;
            bestRecord = const_cast<VectorRecord*>(&rec);
        } else if (diff < minDiff - EPSILON) {
            minDiff = diff;
            bestRecord = const_cast<VectorRecord*>(&rec);
        } else if (fabs(diff - minDiff) <= EPSILON && rec.id < bestRecord->id) {
            bestRecord = const_cast<VectorRecord*>(&rec);
        }
    }
    
    return bestRecord;
}

bool VectorStore::removeAt(int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    
    // Bước 1: Tìm record trong allRecords bằng ID (từ cây AVL hiện tại)
    int targetId = -1;
    try {
        targetId = getId(index);  // Duyệt inorder cây AVL để lấy ID tại index
    } catch (const out_of_range& e) {
        return false;
    }
    
    // Bước 2: Tìm và erase record khỏi allRecords trước
    auto it = allRecords.begin();
    VectorRecord* targetRecord = nullptr;
    int currentIndex = 0;
    
    for (; it != allRecords.end(); ++it) {
        if (it->id == targetId) {
            targetRecord = &(*it);
            break;
        }
        currentIndex++;
    }
    
    if (!targetRecord) return false;
    
    // Lưu thông tin cần thiết trước khi erase
    double dist = targetRecord->distanceFromReference;

    double nrm = targetRecord->norm;
    bool wasRoot = (targetRecord == rootVector);

    //SET NULLPTR TRƯỚC KHI ERASE
    if (wasRoot) {
        rootVector = nullptr;
    }

    // Kiểm tra xem record này có phải là min hoặc max không
    bool wasMinOrMax = false;
    if (fabs(dist - minDistanceFromRef) < EPSILON || 
        fabs(dist - maxDistanceFromRef) < EPSILON) {
        wasMinOrMax = true;
    }
    
    // Erase khỏi allRecords và giải phóng bộ nhớ vector
    if (targetRecord->vector) {
        delete targetRecord->vector;
        targetRecord->vector = nullptr;
    }
    allRecords.erase(it);

    //thiếu giảm count 
    count--; 


    // CASE 1: XÓA ROOT --> REBUILD TOÀN BỘ
    if (wasRoot && count > 0) {
        // Tính lại averageDistance
        double sum = 0.0;
        for (const auto& rec : allRecords) {
            sum += rec.distanceFromReference;
        }
        averageDistance = sum / count;
        
        // Chọn node trung bình làm root
        VectorRecord* newRoot = findVectorNearestToDistance(averageDistance);
        
        // Rebuild cây với root mới
        vectorStore->clear();
        normIndex->clear();
        
        //  BUILD BALANCED TREE (theo thuật toán của thầy)
        std::vector<VectorRecord*> sorted = getAllVectorsSortedByDistance();
        buildBalancedAVL(sorted, 0, sorted.size() - 1);
        
        // Rebuild normIndex
        for (const auto& rec : allRecords) {
            normIndex->insert(rec.norm, rec);
        }
        
        rootVector = newRoot;
    }
    // CASE 2: XÓA NODE KHÁC ==> CHỈ XÓA KHỎI CÂY, SAU ĐÓ ĐỒNG BỘ ROOT
    else if (count > 0) {
        // Xóa khỏi cây AVL (AVL tự quay để cân bằng)
        vectorStore->remove(targetRecord->distanceFromReference);
        normIndex->remove(targetRecord->norm);
        
        // Cập nhật averageDistance
        // Code đúng (tính lại từ đầu):
double sum = 0.0;
for (const auto& rec : allRecords) {
    if (rec.vector) sum += rec.distanceFromReference;
}
averageDistance = sum / count;
        
        // ĐỒNG BỘ rootVector với AVL root sau khi rotate
        if (vectorStore->root != nullptr) {
            int rootId = vectorStore->root->data.id;
            for (auto& rec : allRecords) {
                if (rec.id == rootId && rec.vector != nullptr) {
                    rootVector = &rec;
                    break;
                }
            }
        }
    }
    else {
        rootVector = nullptr;
    }
    updatenextId(); 
    return true;
}


void VectorStore::setReferenceVector(const std::vector<float> &newReference) {
    delete referenceVector;
    referenceVector = new std::vector<float>(newReference); 
    
    if (count == 0) return;
    
    // Bước 1: Update distance trong allRecords
    double sumDistance = 0.0;
    for (VectorRecord& rec : allRecords) {  // Dùng allRecords, KHÔNG dùng getAllVectorsSorted...
        if (rec.vector && referenceVector) {
            rec.distanceFromReference = l2Distance(*rec.vector, *referenceVector);
            sumDistance += rec.distanceFromReference;
        }
    }
    averageDistance = sumDistance / count;

    // Lấy sorted records
    std::vector<VectorRecord*> sortedRecords = getAllVectorsSortedByDistance();
    
    // Chuyển sang vector<pair<K, T>>
    std::vector<std::pair<double, VectorRecord>> pairs;
    for (auto* rec : sortedRecords) {
        pairs.push_back({rec->distanceFromReference, *rec});
    }
    
    // BUILD TRỰC TIẾP BẰNG buildBalancedTree (KHÔNG qua insert)
    vectorStore->clear();
    vectorStore->root = vectorStore->buildBalancedTree(pairs, 0, pairs.size() - 1);
    
    // Rebuild normIndex
    normIndex->clear();
    for (const auto& rec : allRecords) {
        normIndex->insert(rec.norm, rec);
    }
    
    // Update rootVector
    rootVector = findVectorNearestToDistance(averageDistance);
}

std::vector<float> * VectorStore:: getReferenceVector() const {
return referenceVector; 
}

VectorRecord * VectorStore::getRootVector() const {
//  Validate rootVector trước khi return
    if (rootVector == nullptr || count == 0) {
        return nullptr;
    }
    
    //  Check xem rootVector có còn valid không
    for (const auto& rec : allRecords) {
        if (&rec == rootVector && rec.vector != nullptr) {
            return rootVector;
        }
    }
    
    // Nếu không tìm thấy, return nullptr
    return nullptr;
}

double VectorStore:: getAverageDistance() const {
return averageDistance; 
}

void VectorStore:: setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &)) {
embeddingFunction = newEmbeddingFunction; 
}

void VectorStore:: forEach(void (*action)(std::vector<float> &, int, std::string &)) {
// Sử dụng phương thức getAllVectorsSortedByDistance() nếu đã triển khai
    std::vector<VectorRecord*> allRecords = getAllVectorsSortedByDistance();
    
    for (VectorRecord* record : allRecords) {
        if (record && record->vector) {
            action(*(record->vector), record->id, record->rawText);
        }
    }
}

std::vector<int> VectorStore:: getAllIdsSortedByDistance() const {
// Khởi tạo một vector để lưu trữ danh sách các ID
    std::vector<int> idList;

    std::vector<VectorRecord*> sortedRecords = getAllVectorsSortedByDistance();
    
    
    for (size_t i = 0; i < sortedRecords.size(); ++i) {
        VectorRecord* record = sortedRecords[i];
        if (record != nullptr) {
            // 3. Trích xuất trường 'id' từ mỗi bản ghi
            idList.push_back(record->id);
        }
    }
    
    return idList;
}

void VectorStore::getAllVectorsHelper(AVLTree<double, VectorRecord>::AVLNode* node, std::vector<VectorRecord*>& vec) const {
    if (node == nullptr) return;
    
    getAllVectorsHelper(node->pLeft, vec);
    
    // TÌM record tương ứng trong allRecords bằng ID
    int targetId = node->data.id;

    for (const VectorRecord& record : allRecords) {
        if (record.id == targetId && record.vector ) {
            vec.push_back(const_cast<VectorRecord*>(&record));
            break;
        }
    }
    
    getAllVectorsHelper(node->pRight, vec);
}

bool VectorStore::compareByDistance(const VectorRecord* a, const VectorRecord* b) {
    const double EPSILON = 1e-9;
    
    // So sánh theo distance trước
    if (fabs(a->distanceFromReference - b->distanceFromReference) > EPSILON) {
        return a->distanceFromReference < b->distanceFromReference;
    }
    
    // Nếu bằng nhau, so sánh theo id
    return a->id < b->id;
}

std::vector<VectorRecord*> VectorStore::getAllVectorsSortedByDistance() const {
    std::vector<VectorRecord*> result;
    
    for (auto& rec : allRecords) {
        if (rec.vector) {
            result.push_back(const_cast<VectorRecord*>(&rec));
        }
    }
    
    if (!result.empty()) {
        mergeSortVectorRecords(result, 0, result.size() - 1);
    }
    
    return result;
}

double VectorStore::cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2) const {
    if (v1.empty() || v2.empty()) return 0.0; // Tránh crash nếu có vector rỗng
    size_t len = std::min(v1.size(), v2.size());
    double dot = 0, normA = 0, normB = 0;
    for (size_t i = 0; i < len; ++i) {
        double a = v1[i], b = v2[i];
        dot += a * b;
        normA += a * a;
        normB += b * b;
    }
    if (normA == 0 || normB == 0) return 0.0;
    return dot / (sqrt(normA) * sqrt(normB));
}

double VectorStore::l1Distance(const std::vector<float> &v1, const std::vector<float> &v2) const {
    if (v1.empty() || v2.empty()) return 0.0;
    size_t len = std::min(v1.size(), v2.size());
    double result = 0.0;
    for (size_t i = 0; i < len; ++i) {
        result += std::fabs(v1[i] - v2[i]);
    }
    return result;
}

double VectorStore::l2Distance(const std::vector<float> &v1, const std::vector<float> &v2) const {
   if (v1.empty() || v2.empty()) return 0.0;
    size_t len = std::min(v1.size(), v2.size());
    double sum = 0.0;
    for (size_t i = 0; i < len; ++i) {
        double temp = v1[i] - v2[i];
        sum += temp * temp;
    }
    return sqrt(sum);
}

double VectorStore:: estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias, double c1_slope) {

    double dr = this->l2Distance(query, reference);
    

    
    double term1 = abs(dr - averageDistance);
    double term2 = c1_slope * averageDistance * static_cast<double>(k);
    
    double D = term1 + term2 + c0_bias;
    
    return D;
}

int VectorStore:: findNearest(const std::vector<float> &query, std::string metric) {
if (empty()) return -1; 
int idxBest=-1; 
double bestVal=0.0; 
if (metric == "cosine") bestVal = -2.0;
    else bestVal = 1e100;
int n=allRecords.size(); 
for (int i=0; i<n; ++i) {
    const VectorRecord& rec = allRecords[i];
        if (!rec.vector) continue;  // skip deleted or invalid vector

        double val;
        if (metric == "cosine") {
            val = cosineSimilarity(query, *(rec.vector));
        } else if (metric == "euclidean" || metric=="l2") {
            val = l2Distance(query, *(rec.vector));
        } else if (metric == "manhattan" || metric=="l1") {
            val = l1Distance(query, *(rec.vector));
        } else {
            throw std::invalid_argument("invalid_metric");
        }

        if (idxBest == -1) {
            // Khởi tạo kết quả đầu tiên
            bestVal = val;
            idxBest = i;
            continue;
        }

        if (metric == "cosine") {
            if (val > bestVal + EPSILON) {
                bestVal = val;
                idxBest = i;
            } else if (fabs(val - bestVal) <= EPSILON && i < idxBest) {
                idxBest = i;
            }
        } else { // Euclidean / Manhattan: nhỏ hơn là tốt nhất
            if (val < bestVal - EPSILON) {
                bestVal = val;
                idxBest = i;
            } else if (fabs(val - bestVal) <= EPSILON && i < idxBest) {
                idxBest = i;
            }
        }
    }

    if (idxBest == -1) return -1;
    return allRecords[idxBest].id;
}

//==================hàm merger được overload===================
void VectorStore:: merge(std::vector<std::pair<double, int>>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Tạo mảng tạm
    std::vector<std::pair<double, int>> L(n1), R(n2);
    
    // Copy dữ liệu vào mảng tạm
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge hai mảng tạm về mảng gốc
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        // So sánh theo first, nếu bằng thì so sánh theo second
        bool leftSmaller = (L[i].first < R[j].first) || 
                          (L[i].first == R[j].first && L[i].second < R[j].second);
        
        if (leftSmaller) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy phần tử còn lại của L[] (nếu có)
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy phần tử còn lại của R[] (nếu có)
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void VectorStore:: mergeSort(std::vector<std::pair<double, int>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Sắp xếp nửa đầu
        mergeSort(arr, left, mid);
        
        // Sắp xếp nửa sau
        mergeSort(arr, mid + 1, right);
        
        // Merge hai nửa đã sắp xếp
        merge(arr, left, mid, right);
    }
}

void VectorStore::mergeVectorRecords( std::vector<VectorRecord*>& arr,int left, int mid, int right) const 
{
    const double EPSILON = 1e-9;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<VectorRecord*> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        bool leftSmaller = false;
        
        // So sánh theo distanceFromReference
        if (fabs(L[i]->distanceFromReference - R[j]->distanceFromReference) > EPSILON) {
            leftSmaller = (L[i]->distanceFromReference < R[j]->distanceFromReference);
        } else {
            // Nếu bằng nhau, so sánh theo id
            leftSmaller = (L[i]->id < R[j]->id);
        }
        
        if (leftSmaller) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void VectorStore::mergeSortVectorRecords (std::vector<VectorRecord*>& arr, int left, int right) const
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortVectorRecords(arr, left, mid);
        mergeSortVectorRecords(arr, mid + 1, right);
        mergeVectorRecords (arr, left, mid, right);
    }
}
//=============================================================

int * VectorStore:: topKNearest(const std::vector<float> &query, int k, std::string metric) {
    if (k<0||k>count) throw std::invalid_argument("invalid_k_value");
    if (empty()) return new int[0]; 
     // Bước 1: Tính chuẩn query
    double nq = calculateNorm(query);
    
    // Bước 2: Ước lượng D
    double D = estimateD_Linear(query, k, averageDistance, *referenceVector);
     // Bước 3: Lọc bằng normIndex
    bool foundLower, foundUpper;
    auto lowerNode = normIndex->lowerBound(nq - D, foundLower);
    auto upperNode = normIndex->upperBound(nq + D, foundUpper);

    std::vector<std::pair<double, int>> candidates;
    // Duyệt allRecords và lọc theo norm (hoặc duyệt RBT)
    for (const auto& rec : allRecords) {
        if (!rec.vector) continue;
        double recNorm = rec.norm;
        if (recNorm >= nq - D && recNorm <= nq + D) {
            double dist;
            if (metric == "cosine") {
                dist = -cosineSimilarity(query, *rec.vector);
            } else if (metric == "euclidean"|| metric=="l2") {
                dist = l2Distance(query, *rec.vector);
            } else {
                dist = l1Distance(query, *rec.vector);
            }
            candidates.push_back({dist, rec.id});
        }
    }
    
    std::cout << "Value m = " << candidates.size() << std::endl;
    
    // Bước 5: Sort và lấy top-k
    mergeSort(candidates, 0, candidates.size() - 1);
    int actualK = std::min(k, (int)candidates.size());
    int* result = new int[actualK];
    
    for (int i = 0; i < actualK; ++i) {
        result[i] = candidates[i].second;
    }
    
    return result;
}

void VectorStore::rangeQueryHelper(AVLTree<double, VectorRecord>::AVLNode* node,double minDist, double maxDist, std::vector<int>& result) const {
    if (node == nullptr) return;
    
    // Chỉ duyệt cây con trái nếu key của node > minDist
    // (vì bên trái có thể có giá trị >= minDist)
    if (node->key > minDist) {
        rangeQueryHelper(node->pLeft, minDist, maxDist, result);
    }
    
    // Thu thập node hiện tại nếu nằm trong [minDist, maxDist]
    if (node->key >= minDist && node->key <= maxDist) {
        result.push_back(node->data.id);
    }
    
    // Chỉ duyệt cây con phải nếu key của node < maxDist
    // (vì bên phải có thể có giá trị <= maxDist)
    if (node->key < maxDist) {
        rangeQueryHelper(node->pRight, minDist, maxDist, result);
    }
}

int * VectorStore:: rangeQueryFromRoot(double minDist, double maxDist) const {
    if (rootVector == nullptr || count==0) {
        int* result = new int[1];
        result[0] = 0; // size = 0
        return result;
    }

    if (minDist < 0 || maxDist < minDist) {
        throw std::invalid_argument("Invalid distance range");
    }

    // Vector để lưu kết quả tạm
    std::vector<int> resultIds;

    // Gọi helper với root của vectorStore (AVL tree)
    rangeQueryHelper(vectorStore->getRoot(), minDist, maxDist, resultIds);

    // Chuyển sang mảng động
    int size = resultIds.size();
    int* result = new int[size + 1];
    
    for (int i = 0; i < size; i++) {
        result[i] = resultIds[i];
    }
    
    return result;
}

int * VectorStore:: rangeQuery(const std::vector<float> &query, double radius, std::string metric) const {
if (count==0) {
        int* result = new int[1];
        result[0] = 0;
        return result;
    }
    
    if (radius < 0) {
        throw std::invalid_argument("Radius must be non-negative");
    }
    
    std::vector<int> resultIds;
    
    // Duyệt toàn bộ allRecords và tính khoảng cách thực
    for (const auto& rec : allRecords) {
        if (!rec.vector) continue;
        
        double dist;
        if (metric == "cosine") {
            // Cosine: dùng similarity, cần chuyển đổi
            double sim = cosineSimilarity(query, *rec.vector);
            dist = 1.0 - sim; // Chuyển similarity thành distance
            // Hoặc dùng: dist = acos(sim) nếu muốn góc
        } else if (metric == "euclidean" || metric=="l2") {
            dist = l2Distance(query, *rec.vector);
        } else if (metric == "manhattan"|| metric=="l1") {
            dist = l1Distance(query, *rec.vector);
        } else {
            throw std::invalid_argument("Invalid metric");
        }
        
        // Kiểm tra nếu nằm trong bán kính
        if (dist <= radius) {
            resultIds.push_back(rec.id);
        }
    }
    
    // Chuyển sang mảng động
    int size = resultIds.size();
    int* result = new int[size + 1];
    
    for (int i = 0; i < size; i++) {
        result[i] = resultIds[i];
    }
    
    return result;
}

int * VectorStore:: boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const {
    // Kiểm tra điều kiện
    if (count==0) {
        int* result = new int[1];
        result[0] = 0;
        return result;
    }
    
    if (minBound.size() != dimension || maxBound.size() != dimension) {
        throw std::invalid_argument("Bound dimensions do not match");
    }
    
    std::vector<int> resultIds;
    
    // Duyệt toàn bộ allRecords
    for (const auto& rec : allRecords) {
        if (!rec.vector) continue; // Bỏ qua record đã xóa
        
        bool inBox = true;
        
        // Kiểm tra từng chiều
        for (int i = 0; i < dimension; i++) {
            float val = (*rec.vector)[i];
            if (val < minBound[i] || val > maxBound[i]) {
                inBox = false;
                break; // Thoát sớm nếu 1 chiều không thỏa
            }
        }
        
        // Nếu tất cả chiều đều thỏa, thêm vào kết quả
        if (inBox) {
            resultIds.push_back(rec.id);
        }
    }
    
    // Chuyển sang mảng động
    int size = resultIds.size();
    int* result = new int[size + 1];
  
    for (int i = 0; i < size; i++) {
        result[i] = resultIds[i];
    }
    
    return result;
}

 void VectorStore::recalculateMinMaxDistance() {
    if (allRecords.empty()) {
        minDistanceFromRef = 0.0;
        maxDistanceFromRef = 0.0;
        return;
    }
    
    minDistanceFromRef = std::numeric_limits<double>::max();
    maxDistanceFromRef = 0.0;
    
    for (const auto& rec : allRecords) {
        if (!rec.vector) continue; // Bỏ qua record đã xóa
        
        if (rec.distanceFromReference < minDistanceFromRef) {
            minDistanceFromRef = rec.distanceFromReference;
        }
        if (rec.distanceFromReference > maxDistanceFromRef) {
            maxDistanceFromRef = rec.distanceFromReference;
        }
    }
}

double VectorStore::getMinDistance() const {
    if (count==0) {
        return 0.0;
    }
    return minDistanceFromRef;
}

double VectorStore::getMaxDistance() const {
    if (count==0) {
        return 0.0;
    }
    return maxDistanceFromRef;
}

VectorRecord VectorStore::computeCentroid(const std::vector<VectorRecord*>& records) const 
{
    if (records.empty()) {
        throw std::invalid_argument("Cannot compute centroid of empty set");
    }
    
    int m = records.size();
    int d = dimension; // Số chiều của vector trong store
    
    // Khởi tạo vector tổng
    std::vector<float> sum(d, 0.0f);
    
    // Cộng tất cả các vector
    for (int i = 0; i < m; i++) {
        if (!records[i]->vector) continue; // Bỏ qua vector null
        
        for (int j = 0; j < d; j++) {
            sum[j] += (*(records[i]->vector))[j];
        }
    }
    
    // Chia cho số lượng để lấy trung bình
    std::vector<float>* centroidVec = new std::vector<float>(d);
    for (int j = 0; j < d; j++) {
        (*centroidVec)[j] = sum[j] / m;
    }
    
    // Tạo VectorRecord cho centroid
    VectorRecord centroidRecord;
    centroidRecord.id = -1; // ID đặc biệt cho centroid
    centroidRecord.rawText = "centroid";
    centroidRecord.vector = centroidVec;
    centroidRecord.distanceFromReference = l2Distance(*centroidVec, *referenceVector);
    centroidRecord.norm = calculateNorm(*centroidVec);
    
    return centroidRecord;
}

void VectorStore::updatenextId() {
    if (allRecords.empty()) {
        nextId = 1;  // Reset về 1 khi rỗng
        return;
    }
    
    int maxId = -1;
    for (const auto& record : allRecords) {
        if (record.id > maxId) maxId = record.id;
    }
    nextId = maxId + 1;  // Luôn tăng dần, KHÔNG reuse
}

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;
