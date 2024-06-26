//Neel Madala
//5/3/2024
//This code creates a red black search tree which automatically balances itself in order to optimize search time where any leaf of the tree can only be one additional level deeper than every other leaf. If not it balances itself with rotation to make it so

//Assistance from Nathan Zou, Lucas Johnson, Roger Li, Noam Yaffe


#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "node.h"

//Colors
#define RESET "\033[0m"
#define RED   "\033[31m"
#define BLUE  "\033[34m"

using namespace std;

struct Trunk { //Used for printing
    Trunk* previous;
    char* str;

    Trunk(Trunk* previous, char* str) {
        this->previous = previous;
        this->str = str;
    }
};

void showTrunks(Trunk* p) {
    if (p == NULL)
        return;

    showTrunks(p->previous);

    cout << p->str;
}

//Prototypes
void ADD(Node*& head, Node*& current, Node*& previous, int value);
void PRINT(Node* root, Trunk* previous, bool isLeft);

void BALANCE(Node*& head, Node*& current);
void rotateLeft(Node*& head, Node*& current);
void rotateRight(Node*& head, Node*& current);

int main(){
  cout << "Welcome to BST" << endl << endl;
  bool looping = true;
  bool adding = true;
  char command[10];
  char input[3200];
  int value = -1;
  Node* tree = nullptr;
  while(looping == true) {
    cout <<"ADD, SEARCH, PRINT, or QUIT?" << endl;
    cin.get(command, 10);
    cin.clear();
    cin.ignore(10000, '\n');
    if(strcmp(command, "ADD") == 0) {//Add to Red black tree
      adding = true;
      cout << "Are you adding by console or file? (c/f)" << endl;//console or file
      cin.get(command, 2);
      cin.clear();
      cin.ignore(10000, '\n');
      if(strcmp(command, "c") == 0) {
	cout << "Enter you input of numbers seperated by spaces and put a -1 at end of the list" << endl;
	while(adding == true) {
	  cin >> value;
	  if(value == -1) {
	    adding = false;
	  }
	  else {
	    Node* current = tree;
	    Node* previous = NULL;
	    ADD(tree, current, previous, value);
	  }
	}
	cin.clear();
	cin.ignore(10000, '\n');
      }
      else if(strcmp(command, "f") == 0) {//adds by file max being 10 in the file I am testing with add another file call it number.txt and it can be any max
	int inputNum;
	cout << "How many numbers are you gonna input?" << endl;
	cin >> inputNum;
	cin.clear();
	cin.ignore(1000, '\n');
        
	ifstream numbers("numbers.txt");
	int numdone = 0;
	while(numbers >> value && numdone < inputNum) {//go through the file and add to BST and then go to the next int until it adds how many the user wanted
	  Node* current = tree;
	  Node* previous = NULL;
	  ADD(tree, current, previous, value);
	  numdone++;
	}
      }
      else {
	cout << "Invalid input" << endl;
      }
    }
    else if(strcmp(command, "PRINT") == 0) {//PRINT out tree
      PRINT(tree, NULL, false);
    }
    else if(strcmp(command, "QUIT") == 0) {//quits
      looping = false;
      cout << "Thanks for utilizing this code" << endl;
    }
  }
}
void ADD(Node*& head, Node*& current, Node*& previous, int value) { //Add function, manually add in a node
    if (head == NULL) {
        head = new Node();
        current = head;
        head->setData(value);
        head->setColor(0); //Head is always black
    }
    else {
        if (value < current->getData()) {
            previous = current;
            current = current->getLeft();
            if (current == NULL) {
                current = new Node();
                current->setData(value);
                previous->setLeft(current);
                current->setParent(previous);
                BALANCE(head, current);
                return;
            }
            else {
                ADD(head, current, previous, value);
            }
        }
        else {
            previous = current;
            current = current->getRight();
            if (current == NULL) {
                current = new Node();
                current->setData(value);
                previous->setRight(current);
                current->setParent(previous);
                BALANCE(head, current);
                return;
            }
            else {
                ADD(head, current, previous, value);
            }
        }
    }
}
void BALANCE(Node*& head, Node*& current) { //Balance function, for Red-Black Tree properties
    Node* parent = NULL;
    Node* grandParent = NULL;
    while ((current != head) && (current->getColor() != 0) && ((current->getParent())->getColor() == 1)) {
        parent = current->getParent();
        grandParent = parent->getParent();
        //Case A: Parent = left child of grandparent
        if (parent == grandParent->getLeft()) {
            Node* uncle = grandParent->getRight();
            //Case 1A: Uncle = red, only recolor
            if (uncle != NULL && uncle->getColor() != 0) {
                grandParent->setColor(1); //Red
                parent->setColor(0); //Black
                uncle->setColor(0); //Black
                current = grandParent;
            }
            else {
                //Case 2A: Current = right child of parent, rotate left
                if (current == parent->getRight()) {
                    rotateLeft(head, parent);
                    current = parent;
                    parent = current->getParent();
                }
                //Case 3A: Current = left child of parent, rotate right
                rotateRight(head, grandParent);
                //swap colors of parent and grandparent
                int tempCol = parent->getColor();
                parent->setColor(grandParent->getColor());
                grandParent->setColor(tempCol);
                current = parent;
            }
        }
        //Case B: Parent = right child of grandparent
        else {
            Node* uncle = grandParent->getLeft();
            //Case 1B: Uncle = red, only recolor
            if (uncle != NULL && uncle->getColor() != 0) {
                grandParent->setColor(1); //Red
                parent->setColor(0); //Black
                uncle->setColor(0); //Black
                current = grandParent;
            }
            else {
                //Case 2B: Current = left child of parent, rotate right
                if (current == parent->getLeft()) {
                    rotateRight(head, parent);
                    current = parent;
                    parent = current->getParent();
                }
                //Case 3B: Current = right child of parent, rotate left
                rotateLeft(head, grandParent);
                //swap color of parent and grandparent
                int tempCol = parent->getColor();
                parent->setColor(grandParent->getColor());
                grandParent->setColor(tempCol);
                current = parent;
            }
        }
    }
    head->setColor(0); //head is black
}
void rotateLeft(Node*& head, Node*& current) { //Rotate Left
    Node* rightPointer = current->getRight();
    //begin rotation
    current->setRight(rightPointer->getLeft());
    if (current->getRight() != NULL) {
        (current->getRight())->setParent(current);
    }
    rightPointer->setParent(current->getParent());
    //if working with head
    if (current->getParent() == NULL) {
        head = rightPointer;
    }
    else if (current == (current->getParent())->getLeft()) {
        (current->getParent())->setLeft(rightPointer);
    }
    else {
        (current->getParent())->setRight(rightPointer);
    }
    rightPointer->setLeft(current);
    current->setParent(rightPointer);
}

void rotateRight(Node*& head, Node*& current) { //Rotate Right
    Node* leftPointer = current->getLeft();
    //being rotation
    current->setLeft(leftPointer->getRight());
    if (current->getLeft() != NULL) {
        (current->getLeft())->setParent(current);
    }
    leftPointer->setParent(current->getParent());
    //if working with head
    if (current->getParent() == NULL) {
        head = leftPointer;
    }
    else if (current == (current->getParent())->getLeft()) {
        (current->getParent())->setLeft(leftPointer);
    }
    else {
        (current->getParent())->setRight(leftPointer);
    }
    leftPointer->setRight(current);
    current->setParent(leftPointer);
}
//Adopted from Nathan Zou
void PRINT(Node* root, Trunk* previous, bool isLeft) { //Print functions, prints out tree
    if (root == NULL) {
        return;
    }
    char* prevStr = (char*)("    ");
    Trunk* trunk = new Trunk(previous, prevStr);
    PRINT(root->getLeft(), trunk, true);
    //Formatting tree
    if (!previous) {
        trunk->str = (char*)("---");
    }
    else if (isLeft) {
        trunk->str = (char*)(".---");
        prevStr = (char*)("   |");
    }
    else {
        trunk->str = (char*)("'---");
        previous->str = prevStr;
    }
    showTrunks(trunk);
    if (root->getColor() == 0) { //if Black
        cout << BLUE << root->getData() << RESET << endl;
    }
    else { //Red
        cout << RED << root->getData() << RESET << endl;
    }
    if (previous) {
        previous->str = prevStr;
    }
    trunk->str = (char*)("   |");
    PRINT(root->getRight(), trunk, false);
}


