#include <vector>
#include <list>
#include <cstdio>
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
        return left->insert( val );
      }
      BST *node = new BST(val);
      left = node;
    } else if ( val >= value ) {
      if ( right ) {
        return right->insert( val );
      }
      BST *node = new BST(val);
      right = node;
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
      if ( left ) {
        left = left->detachAndReturn(swapVal, true);
      } else if ( right ) {
        right = right->detachAndReturn(swapVal, false);
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
      printf("node:%d left:%d right:%d\n", node.value, node.left ? node.left->value : -1, node.right ? node.right->value : -1 );
    }
  }
};

int main( int argc, char ** argv ) {
  
  BST root(10);
  printf("\nroot\n");
  root.printTree();
  
  root.insert(5);
  printf("\ninsert 5\n");
  root.printTree();


  root.insert(15);
  printf("\ninsert 15\n");
  root.printTree();


  root.insert(2);
  printf("\ninsert 2\n");
  root.printTree();

  root.insert(5);
  printf("\ninsert 5\n");
  root.printTree();

  root.insert(13);
  printf("\ninsert 13\n");
  root.printTree();

  root.insert(22);
  printf("\ninsert 22\n");
  root.printTree();

  root.insert(1);
  printf("\ninsert 1\n");
  root.printTree();

  root.insert(14);
  printf("\ninsert 14\n");
  root.printTree();

  root.insert(12);
  printf("\ninsert 12\n");
  root.printTree();

  root.remove(1);
  printf("\nremove 1\n");
  root.printTree();

  printf("contains 15:%s\n", root.contains(15) ? "T":"F");
}
