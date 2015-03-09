#include <cstdlib>
#include <iostream>

// Defines blueprint of a RList objecct.
class RList {
	public:
		// Data
		int first;
		RList* rest; // Recursive definition, an RList can only be defined in terms of another RList
					// i.e. "A person is a creature whose mother was a person."
		// Behavior
		RList( int first, RList* rest ) { // builds
			this->first = first; // 'this' is a pointer to the object we are building/using
			this->rest = rest; // 'this' is a keyword in C++, can only be used when defining objects.
		}
		~RList() { // destroys
			delete this->rest; // frees memory being used by the rest of list
			// If you dont want the rest of the list to be freed, store rest in another pointer and then
			// set this->rest = NULL before calling delete.
			std::cout << "Destroying RList..." << std::endl;
		}
		bool isEmpty() { // tells you if the list is empty
			return this == NULL; // If the list is pointing to nothing, its empty
		}
		void print() { // prints the contents of the list using recursion
			// If list is empty, stop and just return.
			if ( this->isEmpty() ) return ;
			// Otherwise, print first element in list, and call print() on the rest of the list.
			std::cout << this->first << std::endl;
			this->rest->print(); // calls the function again, on the "rest" of the list
		}

		void printBackwards() {
			if ( this->isEmpty() ) return ;
			this->rest->printBackwards();
			std::cout << (char) this->first << std::endl;
		}
};

// Defines blueprint of a MyStack object.
class MyStack {
	public:
		// Data
		RList* data; // Uses RList pointer to hold stack data
		// Behavior
		MyStack() { // builds
			this->data = NULL;
		}
		~MyStack() { // destroys
			delete this->data; // destroys RList holding stack data
			std::cout << "Destroying MyStack..." << std::endl;
		}
		void push( int datum ) { // puts an item of data (datum) on the top of the stack
			this->data = new RList( datum, data ); // creates memory for object to store datum
		}
		int pop() { // remove an item of data from the top of the stack
			int datum = this->data->first; // item being removed
			RList* oldDatasAddress = this->data; // store address of object who held the item being removed
			this->data = this->data->rest; // now the stack only has the remaining portion of the stack
			oldDatasAddress->rest = NULL; // we dont want the rest of the stack to be deleted
			delete oldDatasAddress; // delete object that stored the removed item
			return datum; // return the removed item
		}
		bool isEmpty() { // tells you if stack is empty
			return this->data->isEmpty();
		}
};

// Prints a blank line, for aesthetic purposes
void printNewline() {
	std::cout << "" << std::endl;
}

// Main function, the actual code that gets run.
// It uses the functions and objects that are defined above.
int main() {

	// Normal object -> read only access to object's address
	// Pointer object -> read/write access to object's address
	// A pointer is an address in memory.
	// The address the pointer holds, is where the actual object is in memory.
	// So we say the pointer 'points' to the object. If you want to access an object's
	// data or behavior, use '->' to get/call the data/function 'pointed' to by the pointer

	RList* list = NULL; // pointer to nothing, which I defined as an empty list (see isEmpty)
	int size; // size of the list, determined by user
	
	// Gets size of list from user
	std::cout << "How many elements should the list be? (pick an positive whole number) ";
	std::cin >> size;
	
	// Builds recursive list given a size.
	// Last element in the list is 0.
	// Second to last element is 1.
	// ...
	// Second element is size - 2.
	// First element is size - 1. 
	for ( int i = 0; i < size; i++ ) {
		list = new RList( i, list ); // Builds a new RList with the current element and the already built portion of the list
	}
	
	// Prints list using recursive print function
	std::cout << "Printing contents of list using list->print():" << std::endl;
	list->print(); // calls the print function for an RList object
	printNewline();
	
	// Prints list while emptying the list, putting each list element on the stack.
	// Because each RList contains another RList, we must destroy the old portions of the list as we empty it.
	MyStack* stack = new MyStack(); // Builds object. Allocates memory for a MyStack object which 'stack' points to.
	std::cout << "Printing contents of your list while putting them on a stack:" << std::endl;
	RList* temp = NULL; // Pointer used to save the address of the old portion of list so we can free that memory
	while ( !list->isEmpty() ) { // Print until list is empty
		std::cout << list->first << std::endl; // prints first element in the current portion of the list
		stack->push( list->first );	// puts element on the top of stack
		temp = list; // temp now points to the address of the old portion of list
		list = list->rest; // list now points to address of the rest of list, forgot about old portion
		temp->rest = NULL; // If old portion still pointed to the rest of the list, the whole list would be destroyed.
		delete temp; // frees memory at old portion's address, calls destroyer function for RList
	}
	printNewline();
	delete list; // deletes anything remaining in the list, which is nothing, but I do it anyways to be safe.
	list = NULL; // guarantees list doesn't point to a freed address
	
	
	// Prints contents of the stack, which is the list backwards.
	std::cout << "Printing contents of list backwards using the stack we put them on." << std::endl;
	while ( !stack->isEmpty() ) { // keep printing until the stack is empty
		std::cout << stack->pop() << std::endl; // removes element from top of stack
	}
	printNewline();

	//system( "pause" );
	// Pointers are like raw memory, when we're done with them we have to "free" the memory.
	delete stack; // When we free the memory, the destroyer function for the object is called.
	stack = NULL; // guarantees stack doesn't point to freed address.
	
	return 0;
}
