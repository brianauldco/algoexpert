#include <vector>
#include <list>
#include <cstdio>
#include <iostream>
using namespace std;

// Do not edit the class below except for
// the insert, contains, and remove methods.
// Feel free to add new properties and methods
// to the class.
class BST {
public:
  int value;
  BST *left;
  BST *right;

  BST(int val) {
    value = val;
    left = nullptr;
    right = nullptr;
  }

  BST( void ) {
    left = nullptr;
    right = nullptr;
  }

  BST &insert(int val) {

    if ( val < value ) {
      if ( left ) {
        left->insert( val );
      } else {
	BST *node = new BST(val);
	left = node;
      }
    } else if ( val >= value ) {
      if ( right ) {
        right->insert( val );
      } else {
	BST *node = new BST(val);
	right = node;
      }
    } 
    // Write your code here.
    // Do not edit the return statement of this method.
    return *this;
  }

  bool contains(int val) {
    // Write your code here.
    if ( value == val ) {
      return true;
    } else if ( val < value ) {
      return left ? left->contains( val ) : false;
    } else {
      return right ? right->contains( val ) : false;
    }
  }

  // 1. find node to remove
  // 2. detach highest/lowest entry from left/right subtree
  //    of node to delete.
  // 2a. purge swap node but save value
  // 2b. when returned to foundNode, sub swapVal
  BST &remove(int val) {
    int swapVal = value;
    findAndSwap( val, swapVal );
    return *this;
  }

  BST * findAndSwap( int val, int &swapVal ) {

    if ( val == value ) {
      if ( right ) {
        right = right->detachAndReturn(swapVal, false);
      } else if ( left ) {
        left = left->detachAndReturn(swapVal, true);
      } else {
        return nullptr;
      }
      // swap Val here?
      value = swapVal;
    } else if ( val < value ) {
      left = left->findAndSwap( val, swapVal );
    } else if ( val > value ) {
      right = right->findAndSwap( val, swapVal );
    }

    return this;
  }

  BST * detachAndReturn( int & swapVal, bool swapLargest ) {
    // find smallest ( or largest if smallest == false ), detach, and return it
    if ( swapLargest ) {
      // search for largest ( left subtree of node to delete )
      if ( right ) {
        right = right->detachAndReturn( swapVal, swapLargest );
      } else {
        swapVal = value;
        return left;
      }
    } else {
      // searching for smallest ( in right subtree of node to delete )
      if ( left ) {
        left = left->detachAndReturn( swapVal, swapLargest );
      } else {
        swapVal = value;
        return right;
      }
    }
    return this;
  }

  void printTree( void ) {

    list<BST> nodes;

    nodes.push_back(*this);
    while ( !nodes.empty() ) {
      BST node = nodes.front();
      nodes.pop_front();
      if ( node.left ) nodes.push_back(*node.left);
      if ( node.right ) nodes.push_back(*node.right);
      printf("node:%d left:%d right:%d value:%d\n", node.value, node.left ? node.left->value : -1, node.right ? node.right->value : -1, node.value );
    }
  }

  static void Usage(void) {
    printf("\
Binary tree construct/helper           \n\
----------------------------             \
    \n");

  }

  // Find first matching entry based on in-order search
  BST * findNode( int val ) {

    BST *node;
      
    if ( left ) {
      node = left->findNode( val );
      if ( node ) return node;
    }

    if ( val == value ) {
      return this;
    }
    
    if ( right ) {
      node = right->findNode( val );
      if ( node ) return node;
    }

    return nullptr;
  }

};

// =============================================================================
// ================= validate BST (start)
// =============================================================================
void validateBstAsList(BST *tree, vector<int> &valueList) {

  if ( tree == NULL )
    return;
  
  validateBstAsList(tree->left, valueList);
  valueList.push_back(tree->value);
  validateBstAsList(tree->right, valueList);
  return;
}

bool validateBst(BST *tree) {
  // Write your code here.
  vector<int> valueList;
  validateBstAsList(tree, valueList);

  bool visited = false;
  int last;
  bool isBst = true;
  for ( auto iter = valueList.begin(); iter != valueList.end(); iter++ ) {
    if ( !visited) {
      last = *iter;
      continue;
    }
    if ( *iter < last ) {
      isBst = false;
      break;
    }
    last = *iter;
  }
  return true;
}

// =============================================================================
// ================= validate BST (end)
// =============================================================================

int main( int argc, char ** argv ) {

  BST *root = nullptr;
  bool abort = false;
  bool rootSet = false;
  
  while ( true ) {
    char op;
    cout << "Enter next op: b(build tree) c(contains) e(exit) i(insert) p(print) r(remove) v(validateBst): ";
    cin >> op;
    printf("read %c\n", op);

    if ( op == 'b' ) {
      int value;
      cout << "Entry node value: ";
      cin >> value;
      if ( !rootSet ) {
	// initialize as root node
	rootSet = true;
	cout << "This is first node added, so treating as root node" << endl;
	root = new BST(value);
      } else {
	int parent;
	cout << "Entry parent node value: ";
	cin >> parent;
	BST *parentNode = root->findNode( parent );
	if ( parentNode ) {
	  char leftOrRight;
	  cout << "Enter 'l' for left child, or 'r' for right child: ";
	  cin >> leftOrRight;
	  if ( leftOrRight == 'l' ) {
	    if ( parentNode->left ) {
	      cout << "left node is already set for node: " << parentNode->value << " - cannot overwrite - Try again" << endl;
	    } else {
	      parentNode->left = new BST(value);
	    }
	  } else if ( leftOrRight == 'r' ) {
	    if ( parentNode->right ) {
	      cout << "right node is already set for node: " << parentNode->value << " - cannot overwrite - Try again" << endl;
	    } else {
	      parentNode->right = new BST(value);
	    }
	  } else {
	    cout << "Invalid entry. Must enter 'l' or 'r'. Try again." << endl;
	    root->printTree();
	    cout << endl;
	  }
	} else {
	  cout << "Cannot find parent - try again" << endl;
	} 
	  
      }
      cout << "Tree:" << endl;
      root->printTree();
      cout << endl;      
    } else if ( op == 'c' ) {
      int value;
      cout << "Contains what value: ";
      cin >> value;
      printf("Contains:%d --> %s\n", value, root->contains(value) ? "T":"F");
    } else if ( op == 'e' ) {
      printf("Exitting...\n");
      abort = true;
    } else if ( op == 'i' ) {
      int value;
      cout << "Enter value to insert: ";
      cin >> value;
      root->insert(value);
      printf("Inserting %d... bst:%p val:%d\n", value, root, root->value);
    } else if ( op == 'p' ) {
      root->printTree();
    } else if ( op == 'r' ) {
      int value;
      cout << "Enter value to remove: ";
      cin >> value;
      printf("Removing %d... bst:%p val:%d\n", value, root, root->value);
      root->remove(value);
    } else if ( op == 'v' ) {
      bool result = validateBst( root );
      printf("Validate Bst: %s\n", result ? "T":"F");
    } else {
      cout << "Invalid entry - try again" << endl;
    }

    if ( !abort )
      continue;
    break;
  }
  return 0;
}
