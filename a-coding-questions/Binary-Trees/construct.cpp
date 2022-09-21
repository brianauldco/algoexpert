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
};

int main( int argc, char ** argv ) {

  int rootval;
  cout << "Enter value for BST root node: ";
  cin >> rootval;
  BST bst(rootval);
  bool abort = false;
  printf("bst:%p\n", &bst);
  
  while ( true ) {
    char op;
    int val;
    cout << endl;
    cout << "Enter next op: c(contains) e(exit) i(insert) p(print) r(remove): ";
    cin >> op;

    if        ( op == 'c' ) {
      cout << "Contains what value: ";
      cin >> val;
      printf("Contains:%d --> %s\n", val, bst.contains(val) ? "T":"F");
    } else if ( op == 'e' ) {
      printf("Exitting...\n");
      abort = true;
    } else if ( op == 'i' ) {
      cout << "Enter value to insert: ";
      cin >> val;
      bst.insert(val);
      printf("Inserting %d... bst:%p val:%d\n", val, &bst, bst.value);
      
    } else if ( op == 'p' ) {
      bst.printTree();
    } else if ( op == 'r' ) {
      cout << "Enter value to remove: ";
      cin >> val;
      printf("Removing %d... bst:%p val:%d\n", val, &bst, bst.value);
      bst.remove(val);
    } else {
      cout << "Invalid entry - try again" << endl;
    }

    if ( !abort )
      continue;
    break;
  }
  return 0;
}
