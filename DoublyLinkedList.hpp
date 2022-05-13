#ifndef MY_DOUBLY_LINKED_LIST_HPP
#define MY_DOUBLY_LINKED_LIST_HPP


/**
 * TODO: Implement DoublyLinkedList, its Node, and its Iterator!
 *
 * I've left some methods filled out for you,
 * 	and stubbed out some structure, to reduce difficulty.
 *
 * You may add or remove methods as you see fit,
 * 	as long as you can still pass all unit tests.
 * Although, it may be more difficult to do so. Your choice!
 *
 * Notice we're inside a namespace here.
 * The DLL is inside a namespace called DoublyLinkedList,
 * 	which is itself inside a namespace called CPSC131
 * This means, if you'd like to play around with your class later,
 * 	you'll need to access it like so:
 * ::CPSC131::DoublyLinkedList::DoublyLinkedList<int> list;
 *
 * Look into main.cpp and CPP_Tests.cpp for examples of using
 * 	the DLL and your BookStore. But don't worry too much, as you
 * 	only need to implement these classes
 * (main and tests are already done for you)
 */


//
#include <iostream>
#include <stdlib.h>
#include <stdexcept>


/**
 * Namespace for our classroom !
 */
namespace CPSC131 {
	/**
	 * Namespace to hold all things related to our DLL
	 */
	namespace DoublyLinkedList {
		/**
		 * Node class, which our DLL will use
		 *
		 * The Professor realizes he could have made this an inline class (like the Iterator),
		 * but he is lazy.
		 */
		template <class T>
		class Node {
			public:

				/// CTORS: YOUR WELCOME
				Node() : prev_(nullptr), next_(nullptr) {}
				Node(T element) : element_(element), prev_(nullptr), next_(nullptr) {}
				Node(T element, Node* prev, Node* next) : element_(element), prev_(prev), next_(next) {}

				/// Set the pointer to the previous element
				void setPrevious(Node* prev) {
					prev_ = prev;
				}

				/// Set the pointer to the previous element
				void setPrev(Node* prev) {
					prev_ = prev;
				}

				/// Get a pointer to the previous element
				Node* getPrevious() {
					return prev_;
				}

				/// Get a pointer to the previous element
				Node* getPrev() {
					return prev_;
				}

				/// Set the pointer to the next node
				void setNext(Node* next) {
					next_ = next;
				}

				/// Get a pointer to the next node
				Node* getNext() {
					return next_;
				}

				/// Set the element this node holds
				void setElement(T element) {
					element_ = element;
				}

				/// Get the element this node holds
				T& getElement() {
					return element_;
				}

				/// Return a reference to the element
				T& operator*() {
					return element_;
				}

			private:
				T element_;
				Node* prev_;
				Node* next_;
		};

		/**
		 * Implement our DoublyLinkedList class !
		 */
		template <class T>
		class DoublyLinkedList {
			public:

				/**
				 * Nested Iterator class.
				 * This allows user code to refer to the Iterator's type as:
				 *
				 * CPSC131::DoublyLinkedList::DoublyLinkedList<int>::Iterator
				 *
				 * (as opposed to specifying the template argument two times)
				 */
				class Iterator {
					public:

						///	Constructor taking a head and tail pointer; YOUR WELCOME
						Iterator(Node<T>* head, Node<T>* tail) : head_(head), tail_(tail) {
							this->cursor_ = this->end();
						}

						///	Constructor taking a head, tail, and cursor pointer; YOUR WELCOME
						Iterator(Node<T>* head, Node<T>* tail, Node<T>* cursor) : head_(head), tail_(tail), cursor_(cursor) {}

						///	Get a pointer to the head node, or end() if this list is empty
						Node<T>* begin() {
							// Check if the list is empty
							if(empty()) {
								return end(); // List is empty
							} else {
								return head_; // If not then return head_ pointer
							}
						}

						///	Get a node pointer representing "end" (aka "depleted"). Probably want to just use nullptr.
						Node<T>* end() {
							return nullptr;
						}

						///	Get the node this iterator is currently pointing to
						Node<T>* getCursor() {
							return cursor_; // Current node points to iterator
						}


						/**
						 * Assignment operator
						 * Return a copy of this Iterator, after modification
						 */
						Iterator& operator=(const Iterator& other) {
							this->head_ = other.head_;
							this->tail_ = other.tail_;
							this->cursor_ = other.cursor_;
							// Return the copy
							return *this;

						}

						///	Comparison operator
						bool operator==(const Iterator& other) {
							// Iterator of cursor is equal
							return cursor_ == other.cursor_;
						}
						///	Inequality comparison operator
						bool operator!=(const Iterator& other) {
							// Iterator of cursor is not equal
							return cursor_ != other.cursor_;
						}

						/**
						 * Prefix increment operator
						 * Return a copy of this Iterator, after modification
						 */
						Iterator& operator++()
						{
							// Code is reference from Professor Bein
							if (cursor_ == nullptr){
								throw std::invalid_argument("Can not increase null iterator");
							}
							// Prefix cursor increase
							this->cursor_ = this->cursor_->getNext();
							// Return copy, after modified
							return *this;
						}

						/**
						 * Postfix increment
						 * Return a copy of this Iterator, BEFORE it was modified
						 */
						Iterator operator++(int)
						{
							// Code is reference from Professor Bein
							// Create temp copy for iterator
							Iterator temp_copy(*this);
							// Posfix increase of iterator
							operator++();
							// Return copy, before modified
							return temp_copy;
						}

						/**
						 * Prefix decrement operator
						 * Return a copy of this Iterator, after modification
						 */
						Iterator& operator--()
						{
							// Code is reference from Professor Bein
							if (cursor_ == nullptr){
								throw std::invalid_argument("Can not increase null iterator");
							}
							// Prefix decrease of iterator
							this->cursor_ = this->cursor_->getPrev();
							// Return copy, after modified
							return *this;
						}

						/**
						 * Postfix decrement operator
						 * Return a copy of this Iterator BEFORE it was modified
						 */
						Iterator operator--(int)
						{
							// Code is reference from Professor Bein
							Iterator temp_copy(*this);
							// Postfix decrease of iterator / Delegate
							operator--();
							// Return copy, before modified
							return temp_copy;
						}

						/**
						 * AdditionAssignment operator
						 * Return a copy of the current iterator, after modification
						*/
						Iterator operator +=(size_t add)
						{
							// For loop through the cursor
							for (size_t i = 0; i < add; i++){
								// Make cursor point to next one (addition)
								cursor_ = cursor_->getNext();
							}
							// Return copy, after modified
							return *this;
						}
						/**
						 * SubtractionAssignment operator
						 * Return a copy of the current iterator, after modification
						 */
						Iterator operator -=(size_t add)
						{
							// For loop through the cursor
							for (size_t i = 0; i < add; i++){
								// Make cursor point to prev one (subtraction)
								cursor_ = cursor_->getPrev();
							}
							// Return copy, after modified
							return *this;
						}

						/**
						 * AdditionAssignment operator, supporting positive or negative ints
						 */
						Iterator operator +=(int add)
						{
							// Create temp iterator
							Iterator temp_plus = *this;
							if (add > 0) {
								// Positive int
								for (int i = 0; i < add; i++) {
									// Exit the loop if cause any errors
									if (cursor_ == nullptr) {
										break;
									}
									// Add temp iterator to next of our cursor
									temp_plus.cursor_ = this->cursor_->getNext();
									// Make cursor point to next one
									this->cursor_ = this->cursor_->getNext();
								}
							} else {
								// Negative int
								for ( int i = 0; i < add; i++) {
									// Exit the loop if cause any errors
									if (cursor_ == nullptr) {
										break;
									}
									// Add temp iterator to prev of our cursor
									temp_plus.cursor_ = cursor_->getPrev();
									// Make cursor point to prev one
									this->cursor_ = this->cursor_->getPrev();
								}
							}
							return temp_plus;
						}

						/**
						 * SubtractionAssignment operator, supporting positive or negative ints
						 */
						Iterator operator -=(int subtract)
						{
							// Create temp iterator
							Iterator temp_minus = *this;
							if (subtract < 0) {
								// Positive int
								for (int i = 0; i < subtract; i++) {
									// Exit the loop if cause any errors
									if (cursor_ == nullptr) {
										break;
									}
									// Add temp iterator to next of our cursor
									temp_minus.cursor_ = cursor_->getNext();
									// Make cursor point to next one
									this->cursor_ = this->cursor_->getNext();
								}
							} else {
								// Negative int
								for ( int i = 0; i < subtract; i++) {
									// Exit the loop if cause any errors;
									if (cursor_ == nullptr) {
										break;
									}
									// Add temp iterator to prev of our cursor
									temp_minus.cursor_ = cursor_->getPrev();
									// Make cursor point to prev one
									this->cursor_ = this->cursor_->getPrev();
								}
							}
							return temp_minus;
						}

						/**
						 * Dereference operator returns a reference to the ELEMENT contained with the current node
						 */
						T& operator*() {
							return cursor_->getElement();
						}

					private:

						/// Pointer to the head node
						Node<T>* head_ = nullptr;

						/// Pointer to the tail node
						Node<T>* tail_ = nullptr;

						/**
						 * Pointer to the cursor node.
						 * This is only one way of letting the iterator traverse the linked list.
						 * You can change to a different method if you wish (and can still pass unit tests)
						 */
						Node<T>* cursor_ = nullptr;

					/// YOUR WELCOME
					friend class DoublyLinkedList;
				};

				/// Your welcome
				DoublyLinkedList() {}

				///	Copy Constructor
				DoublyLinkedList(DoublyLinkedList& other) {
					/*
					if (other.head() == nullptr) {
						return;
					}
					*/

					// For loop through the lists to create copy
					for (auto & value : other) {
						// Increase the size so that iterator would copy everything over
						push_back(value);
					}

				}

				/// DTOR: Your welcome
				~DoublyLinkedList() {
					this->clear();
				}

				/**
				 * Clear the list and assign the same value, count times.
				 * If count was 5, T was int, and value was 3,
				 * 	we'd end up with a list like {3, 3, 3, 3, 3}
				 */
				void assign(size_t count, const T& value) {
					clear();
					// For loop through the list
					for (size_t i = 0; i < count; i++) {
						// Increase the size of the list which makes the count continue after the loop
						// Purpose is we do not need to assign count again
						push_front(value);
					}
				}

				/**
				 * Clear the list and assign values from another list.
				 * The 'first' iterator points to the first item copied from the other list.
				 * The 'last' iterator points to the last item copied from the other list.
				 *
				 * Example:
				 * 	Suppose we have a source list like {8, 4, 3, 2, 7, 1}
				 * 	Suppose first points to the 4
				 *	Suppose last points to the 7
				 * 	We should end up with our list becoming: {4, 3, 2, 7}
				 *
				 * If the user code sends out-of-order iterators,
				 * 	just copy from 'first' to the end of the source list
				 * Example: first=7, last=4 from the list above would give us:
				 * 	{7, 1}
				 */
				void assign(Iterator first, Iterator last) {
					clear();
					// Create temp point to the head
					auto temp = head_;
					// Iterate through the list
					for (Iterator i = first; i != last; i++) {
						// Copy from 'first' to end of list
						temp->setElement(i->getElement());
						temp = temp->getNext();
					}
				}

				/// Return a pointer to the head node, if any
				Node<T>* head() {
					return head_;
				}

				/// Return a pointer to the tail node, if any
				Node<T>* tail() {
					return tail_;
				}

				/**
				 * Return an iterator that points to the head of our list
				 */
				Iterator begin() {
					return Iterator(head_, tail_, head_);
				}

				/**
				 * Return an iterator that points to the last element (tail) of our list
				 */
				Iterator last() {
					return Iterator(head_, tail_, tail_);
				}

				/**
				 * Should return an iterator that represents being past the end of our nodes,
				 * or just that we are finished.
				 * You can make this a nullptr or use some other scheme of your choosing,
				 * 	as long as it works with the logic of the rest of your implementations.
				 */
				Iterator end() {
					return Iterator(nullptr, nullptr, nullptr);
				}

				/**
				 * Returns true if our list is empty
				 */
				bool empty() const {
					if (head_ == nullptr) {
						return true;
					}
					return false;
				}

				/**
				 * Returns the current size of the list
				 * Should finish in constant time!
				 * (keep track of the size elsewhere)
				 */
				size_t size() const {
					return size_;
				}

				/**
				 * Clears our entire list, making it empty
				 * Remember: All removal operations should be memory-leak free.
				 */
				void clear() {
					// Making the list becomes empty
					while(!empty()) {
						pop_front();
					}
					// Reset all values
					size_ = 0;
					head_ = nullptr;
					tail_ = nullptr;
				}

				/**
				 * Insert an element after the node pointed to by the pos Iterator
				 *
				 * If the list is currently empty,
				 * 	ignore the iterator and just make the new node at the head/tail (list of length 1).
				 *
				 * If the incoming iterator is this->end(), insert the element at the tail
				 *
				 * Should return an iterator that points to the newly added node
				 *
				 * To avoid repeated code, it might be a good idea to have other methods
				 * 	rely on this one.
				 */
				Iterator insert_after(Iterator pos, const T& value) {
					// Code is reference from Professor Bein of insertBefore()
					// Create a node that point to current position of cursor
					Node<T> *insert = pos.getCursor();
					// Create new node represents value
					Node<T> *node = new Node<T>(value);
					// If list is not empty
					if (size_ != 0) {
						// Get the node to the value
						node->setPrev(insert);
						node->setNext(insert->getNext());
						// Insert the current node to value
						insert->getNext()->setPrev(node);
						insert->setNext(node);
						size_++; // Keep track of the size in the list
					}
					else if (pos == this->end()) {
						// Insert at tail
						push_back(value);
					}
					else {
						head_ = node;
						tail_ = node;
						size_++;
					}
					// Iterator that points to newly added node
					Iterator newAddedNode(head_, tail_, node);
					return newAddedNode;
				}

				/**
				 * Insert a new element after the index pos.
				 * Should work with an empty list.
				 *
				 * Should return an iterator pointing to the newly created node
				 *
				 * To reduce repeated code, you may want to simply find
				 * 	an iterator to the node at the pos index, then
				 * 	send it to the other overload of this method.
				*/
				Iterator insert_after(size_t pos, const T& value) {
					// New element of index pos
					Iterator index_pos(head_, tail_, head_);
					index_pos += pos;
					// Send it to overload method which is insert_after(Iterator pos, const T& value)
					return insert_after(index_pos, value);
				}

				/**```````````
				 * Erase the node pointed to by the Iterator's cursor.
				 *
				 * If the 'pos' iterator does not point to a valid node,
				 * 	throw an std::range_error
				 *
				 * Return an iterator to the node AFTER the one we erased,
				 * 	or this->end() if we just erased the tail
				 */
				Iterator erase(Iterator pos) {
					// Pos iterator does not point to a valid node
					if (pos.getCursor() == nullptr) {
						throw std::range_error("Invalid node");
					}

					// Code is reference from Professor Bein of remove()
					Node<T> *temp = pos.getCursor();
					// Check for current position of cursor is head
					if (pos.getCursor() == head_) {
						// Current node is at both head and tail
						if (head_ == nullptr || tail_ == nullptr || tail_ == head_) {
							head_ = nullptr;
							tail_ = nullptr;
						}
						else {
							// Current node is head
							head_ = head_->getNext();
							head_->setPrev(nullptr);
						}
						// Check for current postion of cursor is tail
					} else if (pos.getCursor() == tail_) {
						tail_ = tail_->getPrev();
						tail_->setNext(nullptr);
					} else {
						// Current node is between head and tail
						temp->getNext()->setPrev(temp->getPrev());
						temp->getPrev()->setNext(temp->getNext());
					}
					size_ --;
					// Create Iterator to return the node after one removed
					Iterator new_node(head_, tail_, temp->getNext());
					delete pos.getCursor(); // delete old node
					return new_node;
				}

				/**
				 * Add an element just after the one pointed to by the 'pos' iterator
				 *
				 * Should return an iterator pointing to the newly created node
				 */
				Iterator push_after(Iterator pos, const T& value) {
					return insert_after(pos,value);
				}

				/**
				 * Add a new element to the front of our list.
				 */
				void push_front(const T& value) {
					/*
					head_ = new Node<T>(value, nullptr, head_);
					if (tail_ == nullptr){
						tail_ = head_;
					}
					*/
					// Create new node for the head
					Node<T> *add_new_node = new Node<T>(value, nullptr, head_);
					// Check if head_ exist in the node
					if (head_ == nullptr) {
						head_ = add_new_node;
						tail_ = add_new_node;
						size_ ++;
						return;
					}
					// Add new element
					head_->setPrev(add_new_node);
					head_ = add_new_node;
					add_new_node->setPrev(nullptr);
					size_++;

				}

				/**
				 * Add an element to the end of this list.
				 *
				 * Should return an iterator pointing to the newly created node.
				 */
				Iterator push_back(const T& value) {
					/*
					Node<T> *add_end_list = new Node<T>(value,tail_,nullptr);
					if (tail_ == nullptr) {
						head_ = tail_ = add_end_list;
					}
					*/

					// Create new node for the tail
					Node<T> *add_end_list = new Node<T>(value, tail_, nullptr);
					// If list is not empty then insert the new node to tail
					if (!empty()) {
						add_end_list->setPrev(tail_);
						tail_->setNext(add_end_list);
						tail_ = add_end_list;
					} else {
						head_ = add_end_list;
						tail_ = add_end_list;
					}
					// Return iterator pointing to new tail
					size_ ++;
					return Iterator (head_, tail_, tail_);
				}

				/**
				 * Remove the node at the front of our list
				 *
				 * Should throw an exception if our list is empty
				 */
				void pop_front() {
					// If list is empty then throw an exception
					if (empty()) {
						throw std::range_error("List is empty");
					}
					/*
					if (head_ == tail_) {
						Node<T> *value = head_;
						head_->setNext(head_->getNext());
						delete value;
					}
					head_->setNext(head_->getNext());
					head_->setPrev(nullptr);
					*/

					// Create new Iterator to remove the node
					Iterator remove(head_, tail_, head_);
					erase(remove);

				}

				/**
				 * Return a reference to the element at the front.
				 *
				 * Throw an exception if the list is empty
				 */
				T& front() {
					if (empty()) {
						throw std::range_error("List is empty");
					}
					// Reference of the element at the front
					return head_->getElement();
				}

				/**
				 * Return a reference to the element at the back.
				 *
				 * Throw an exception if the list is empty
				 */
				T& back() {
					if (empty()) {
						throw std::range_error("List is empty");
					}
					// Reference of element at the back
					return tail_->getElement();
				}

				/**
				 * Return the element at an index
				 *
				 * Should throw a range_error is out of bounds
				 */
				T& at(size_t index) {
					if (index < 0 || index >= size_) {
						throw std::range_error("Index out of bounds");
					}
					// Create new node to point to head
					Node<T> *value = head_;
					// For loop through the list
					for (size_t i = 0; i < index; i++) {
						// Get the new node to the index that it want to stop
						value = value->getNext();
					}
					// Element at index
					return value->getElement();
				}

				/**
				 * Reverse the current list
				 *
				 * It might be easy to consider the following:
				 * - Create a temp empty list
				 * - Iterate through the current list
				 * - For each item in the current list, push to the FRONT (not back)
				 * - Assign the current list to the temp list
				 * - Discard the temp list
				 */
				void reverse() {
					// Check if there at least one node
					if (this->head_ != nullptr) {
						// Create two new nodes point to the head
						Node<T> *temp_list = this->head_;
						Node<T> *value = this->head_;
						while (value != nullptr) {
							// First node get to prev of second node
							temp_list = value->getPrev();
							// Set the second node to next list
							value->setPrev(value->getNext());
							// Set the second node to first node
							value->setNext(temp_list);
							// Get the prev second node
							value = value->getPrev();
						}
						// Start reverse
						temp_list = head_;
						head_ = tail_;
						tail_ = temp_list;
					}
				}

				/**
				 * I bet you're happy I'm not making you do this.
				 * No tests will be run against this function,
				 * 	but feel free to try it out, as a challenge!
				 *
				 * If I were doing this and didn't care too much for efficiency,
				 * 	I would probably create an extra helper function to swap two
				 * 	positions in the current list.
				 * Then I would simply sweep through the list bubble-sort style.
				 * Perhaps selection sort.
				 *
				 * If you want a huge challenge, try implementing quicksort.
				 *
				 * (but again, don't worry about this method; it will not be tested)
				 */
				void sort() {
					// thank you very much !!!
				}

				/**
				 * Assignment operator
				 *
				 * Clear this list and fill it with the others' values
				 * (by value, not by reference)
				 *
				 * Return a reference to this list
				 */
				DoublyLinkedList<T>& operator =(DoublyLinkedList<T>& other) {
						clear();
						// Create new node point to head
						Node<T> *value = other.head_;
						while (value != nullptr) {
							// Fill it with others' values
							push_back(value->getElement());
							value = value->getNext();
						}
					// Reference to the list
					return *this;
				}

				/**
				 * Return true if the lists are "equal"
				 *
				 * "Equal" here is defined as:
				 * - Same size
				 * - Elements at the same indexes would return true for their own comparison operators
				 *
				 * In other words: "They contain all the same values"
				 * (no need to be references to each other)
				 */
				bool operator ==(DoublyLinkedList<T>& other) {
					// Code is reference from Professor Bein
					// Lists are equal
					return this->size_ == other.size_;
				}

				/**
				 * Return true if the lists are "not equal"
				 *
				 * See the operator== stub for definition of "equal"
				 *
				 * Probably want to avoid repeated code by relying on the other operator
				 */
				bool operator !=(DoublyLinkedList<T>& other) {
					// Code is reference from Professor Bein
					// List are not equal
					return this->size_ != other.size_;
				}

			private:
				Node<T>* head_ = nullptr;
				Node<T>* tail_ = nullptr;
				size_t size_ = 0;
		};
	}
}

#endif















































/// Yes, I'm aware it's spelled you're*
