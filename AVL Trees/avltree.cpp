/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K & key) const
{
	return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node * subtree, const K & key) const
{
	if (subtree == NULL)
		return V();
	else if (key == subtree->key)
		return subtree->value;
	else
	{
		if (key < subtree->key)
			return find(subtree->left, key);
		else
			return find(subtree->right, key);
	}
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node * & t)
{
	*_out << __func__ << endl; // Outputs the rotation name (don't remove this)
    // your code here
    Node * tempLeft = t->left;
	Node * tempRight = t->right;
    
    t->left = new Node(t->key, t->value);
    t->right = tempRight->right;
    t->key = tempRight->key;
    t->value = tempRight->value;
    
    t->left->left = tempLeft;
    t->left->right = tempRight->left;
    
    t->height = getHeight(t) - 1;
    if (t->right != NULL) {t->right->height = getHeight(t->right) - 1;}
    if (t->left != NULL) {t->left->height = getHeight(t->left) - 1;}
    
    delete tempRight;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node * & t)
{
	*_out << __func__ << endl; // Outputs the rotation name (don't remove this)
	// Implemented for you:
	rotateLeft(t->left);
	rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node * & t)
{
	*_out << __func__ << endl; // Outputs the rotation name (don't remove this)
	Node * tempLeft = t->left;
	Node * tempRight = t->right;

    t->right = new Node(t->key, t->value);
    t->left = tempLeft->left;
    t->key = tempLeft->key;
    t->value = tempLeft->value;
    
    t->right->right = tempRight;
    t->right->left = tempLeft->right;
    
    t->height = getHeight(t) - 1;
    if (t->right != NULL) {t->right->height = getHeight(t->right) - 1;}
    if (t->left != NULL) {t->left->height = getHeight(t->left) - 1;}
    
    delete tempLeft;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node * & t)
{
	*_out << __func__ << endl; // Outputs the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
	insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node* & subtree, const K & key, const V & value)
{
	if (subtree == NULL) {subtree = new Node(key, value);}
    else if (key < subtree->key) {
    	if (subtree->left == NULL) {subtree->left = new Node(key, value);}
    	else {insert(subtree->left, key, value);}
    	
    	if (subtree->right != NULL) {
			if (subtree->left->height > subtree->right->height) {subtree->height = subtree->left->height + 1;}
			else {subtree->height = subtree->right->height + 1;}
			
			if ((subtree->left->height - subtree->right->height) > 1) {
				if (subtree->left->left == NULL) {rotateLeftRight(subtree);}
				else if (subtree->left->right == NULL) {rotateRight(subtree);}
				else if ((subtree->left->left->height - subtree->left->right->height) == 1) {
					rotateRight(subtree);
				}
				else {rotateLeftRight(subtree);}
			}
			else if ((subtree->left->height - subtree->right->height) < -1) {
				if (subtree->right->left == NULL) {rotateLeft(subtree);}
				else if (subtree->right->right == NULL) {rotateRightLeft(subtree);}
				else if ((subtree->right->left->height - subtree->right->right->height) == 1) {
					rotateRightLeft(subtree);
				}
				else {rotateLeft(subtree);}
			}
		}
		else {
			subtree->height = subtree->left->height + 1;
			if ((subtree->left->height) >= 1) {
				if (subtree->left->left == NULL) {rotateLeftRight(subtree);}
				else if (subtree->left->right == NULL) {rotateRight(subtree);}
				else if ((subtree->left->left->height - subtree->left->right->height) == 1) {
					rotateRight(subtree);
				}
				else {rotateLeftRight(subtree);}
			}
		}
	}
    else {
    	if (subtree->right == NULL) {subtree->right = new Node(key, value);}
    	else {insert(subtree->right, key, value);}
    	
    	if (subtree->left != NULL) {
			if (subtree->left->height > subtree->right->height) {subtree->height = subtree->left->height + 1;}
			else {subtree->height = subtree->right->height + 1;}
			
			if ((subtree->left->height - subtree->right->height) > 1) {
				if (subtree->left->left == NULL) {rotateLeftRight(subtree);}
				else if (subtree->left->right == NULL) {rotateRight(subtree);}
				else if ((subtree->left->left->height - subtree->left->right->height) == 1) {
					rotateRight(subtree);
				}
				else {rotateLeftRight(subtree);}
			}
			else if ((subtree->left->height - subtree->right->height) < -1) {
				if (subtree->right->left == NULL) {rotateLeft(subtree);}
				else if (subtree->right->right == NULL) {rotateRightLeft(subtree); }
				else if ((subtree->right->left->height - subtree->right->right->height) == 1) {
					rotateRightLeft(subtree);
				}
				else {rotateLeft(subtree);}
			}
		}
		else {
			subtree->height = subtree->right->height + 1;
		
			if (subtree->right->height >= 1) {
				if (subtree->right->left == NULL) {rotateLeft(subtree);}
				else if (subtree->right->right == NULL) {rotateRightLeft(subtree);}
				else if ((subtree->right->left->height - subtree->right->right->height) == 1) {
					rotateRightLeft(subtree);
				}
				else {rotateLeft(subtree);}
			}
		}
    }
}
