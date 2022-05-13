
/**
 * TODO: Implement all the BookStore methods below.
 * Note you'll have to remain inside the CPSC131::BookStore namespace.
 */

//
#include "BookStore.hpp"


//
#include <iostream>
#include <sstream>
#include <string>


//
using std::cout, std::cin, std::endl;
using std::string, std::to_string;
using std::stringstream;


//
namespace CPSC131::BookStore
{

/// Your welcome
BookStore::BookStore() {}

/**
* Adjust the store's account balance
* Should accept positive or negative adjustments
*/
void BookStore::adjustAccountBalance(int adjustment)
{
	account_balance_ += adjustment;
}

/**
* Return the store's current account balance
*/
int BookStore::getAccountBalance() {
	return account_balance_;
}

/**
* Find a book by its ISBN
*
* Return this->bookList.end() if the book isn't found.
*
* Return an interator pointing to the Book if it is found.
*/
DoublyLinkedList::DoublyLinkedList<Book>::Iterator BookStore::findBook(std::string isbn)
{
	/*
	// should be iterator not node
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *tail = bookList.tail();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	*/

	// Iterate/Go through the whole list of the book
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book(bookList.head(), bookList.tail(), bookList.head());
	while (book.getCursor() != nullptr) {
		// If can find the book by isbn then return
		if (book.getCursor()->getElement().getIsbn() == isbn) {
			return book;
		}
		// book = book.getCursor()->getElement()->getNext();
		book++; // Keep track number of book
	}
	// If book is not found
	return bookList.end();
}

/**
* Check whether a book exists, by its ISBN
*
* Return true if it exists, or false otherwise
*/
bool BookStore::bookExists(std::string isbn)
{
	/*
	int count = 0; // keep track of the book
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	*/

	// Iterate/Go through the whole list of the book
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book_exist(bookList.head(), bookList.tail(), bookList.head());
	while (book_exist.getCursor() != nullptr) {
		// Check whether the book exist by isbn
		if (book_exist.getCursor()->getElement().getIsbn() == isbn) {
			return true; // Book exists
		}
		book_exist++; // Keep track number of the book
	}
	return false; // Book does not exist
}

/**
* Check the quantity of stock we have for a particular book, by ISBN
*
* If the book doesn't exist, just return 0
*/
size_t BookStore::getBookStockAvailable(std::string isbn)
{
	/*
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	*/

	// Iterate/Go through the whole list of the book
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book_stock(bookList.head(), bookList.tail(), bookList.head());
	while (book_stock.getCursor() != nullptr) {
		// If the book is in the stock by isbn
		if (book_stock.getCursor()->getElement().getIsbn() == isbn ) {
			// Get the book in the stock
			return book_stock.getCursor()->getElement().getStockAvailable();
		}
		book_stock++; // Keep track number of book in the stock
	}
	return 0; // Book does not exist

}

/**
* Locate a book by ISBN and return a reference to the Book
*
* If the book doesn't exist, throw an exception
*/
Book& BookStore::getBook(std::string isbn)
{

	/*for (CPSC131::DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator i = bookList.begin(); i != bookList.end(); i++) {

	::CPSC131::DoublyLinkedList::Node<Book> *temp = bookList.head();
	// ::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	*/

	// Iterate/Go through the whole list of the book
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book_locate(bookList.head(), bookList.tail(), bookList.head());
	while (book_locate.getCursor() != nullptr) {
		// If the book location find by isbn
		if (book_locate.getCursor()->getElement().getIsbn() == isbn) {
			// Return reference to the Book
			return book_locate.getCursor()->getElement();
		}
		book_locate++; // Keep track number of book locates in the list
	}
	// Book does not exist
	throw std::range_error ("Book does not exist");

}

/**
* Take a Book instance and add it to inventory
*
* If the book's ISBN already exists in our store,
* 	simply adjust account balance by the book's price and quantity,
* 	but ignore other details like title and author.
*
* If the book's ISBN doesn't already exist in our store,
* 	adjust our account balance and push the book into our list
*/
void BookStore::purchaseInventory(const Book& book)
{
	/*
	for (CPSC131::DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator i = bookList.begin(); i != bookList.end(); i++) {
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	::CPSC131::BookStore::Book book_temp = temp->getElement();
	while (temp != nullptr) {
	if (bookExists(book.getIsbn()) {
		account_balance_ -= book_temp.getPriceCents();
		account_balance_ -= book_temp.getStockAvailable();
	}
	}
	account_balance_ += book.getPriceCents();
	book.adjustStockAvailable(-stock_available_);
	bookList.push_back(book);
	*/

	// Iterate/Go through the whole list of the book to find whether book exist or not
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book_exist = findBook(book.getIsbn());
	/*
		Case when book exists
		Adjust the account balance by price and quantity(stock)
	*/
	adjustAccountBalance(-(book.getPriceCents() * book.getStockAvailable()));
	/*
		Case when book does not exists
		We need to adjust the quantity to get to book to our list
	*/
	if (book_exist.getCursor() != nullptr) {
		book_exist.getCursor()->getElement().adjustStockAvailable(book.getStockAvailable());
	} else {
		// push the book to list
		bookList.push_back(book);
	}
}

/**
* Take some book details and add the book to our inventory.
*
* Use the same rules as the other overload for this function.
*
* You might want to avoid repeating code by simply building a Book
* 	object from the details, then calling the other overload
* 	with the new Book object.
*/
void BookStore::purchaseInventory(
	std::string title, std::string author, std::string isbn,
	size_t price_cents,
	size_t unit_count
)
{
	/*
	for (CPSC131::DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator i = bookList.begin(); i != bookList.end(); i++ ){
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *tail = bookList.tail();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	while (temp != nullptr) {
		::CPSC131::BookStore::Book book = temp->getElement();
		if (book.getIsbn() == isbn ) {
			account_balance_ += book.getPriceCents();
			account_balance_ += book.getStockAvailable();
		}
	}
	bookList.push_back(Book(title,author,isbn,price_cents,unit_count));
	*/

	// Concise way
	// Rely on purchaseInventory(const Book& book)
	// Add book to inventory
	Book newBook(title, author, isbn, price_cents,unit_count);
	// Calling the overload method
	purchaseInventory(newBook);

}

/**
* Print out inventory.
* Should be in a particular way to earn unit test points.
* Example (ignore the asterisks at the left of this comment block):
*
* *** Book Store Inventory ***
* "Book1", by Author1 [123] (5 in stock)
* "Book2", by Author2 [456] (19 in stock)
*
* Should print a trailing std::endl after the last book line, for the unit tests
*/
void BookStore::printInventory()
{
	/*
	for (CPSC131::DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator i = bookList.begin(); i != bookList.end(); i++) {
	*/
	/*
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	while (temp != nullptr) {
		::CPSC131::BookStore::Book book = temp->getElement();
	*/

	cout << "*** Book Store Inventory ***" << endl;
	// Iterate/Go through the whole list
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator book(bookList.head(), bookList.tail(), bookList.head());
	while (book.getCursor() != nullptr) {
	// Print out the inventory
		cout << "\"" << book.getCursor()->getElement().getTitle() << "\", by " << book.getCursor()->getElement().getAuthor() << " [" << book.getCursor()->getElement().getIsbn() << "] (" << book.getCursor()->getElement().getStockAvailable() << " in stock)" << endl;
		book++;
	}

}

/**
* Sell a book to a customer!
*
* Takes the ISBN of the book, the selling price of the book, and the quantity of books sold
*
* Uses the same rules as the other overload.
*
* You may wish to just grab a reference to the book and call the other overload,
* 	to avoid repeating code
*/
void BookStore::sellToCustomer(std::string isbn, size_t price_cents, size_t quantity)
{
	/*
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	while (temp != nullptr) {
		::CPSC131::BookStore::Book book = temp->getElement();
		if (book.getIsbn() == isbn ) {
			stock_available_ = book.adjustStockAvailable(-quantity);
			account_balance_ += (quantity * price_cents);
		}
	}
	*/

	// Iterate/Go through the whole list of the book to find book to sell to customer
	DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator sell_book = findBook(isbn);
	// Check if the list has enough book to sell
	if (quantity > sell_book.getCursor()->getElement().getStockAvailable()) {
		throw std::range_error("Does not have enough book");
	}
	// Enough book -> sell to customer and change the amount we have in stock
	sell_book.getCursor()->getElement().adjustStockAvailable(-quantity);
	// Receive money from customer
	adjustAccountBalance(quantity * price_cents);

}

/**
* Sell a book to a customer!
*
 Takes a Book reference, the selling price of the book, and the quantity of books sold
*
* If we don't have enough of this book in stock for the quantity the customer wants to purchase,
* 	throw an std::range_error
*
* Otherwise, adjust the stock available in our store, and update our account balance.
*/
void BookStore::sellToCustomer(Book& book, size_t price_cents, size_t quantity)
{

	/*
	for (CPSC131::DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator i = bookList.begin(); i != bookList.end(); i++) {
	::CPSC131::DoublyLinkedList::Node<Book> *head = bookList.head();
	::CPSC131::DoublyLinkedList::Node<Book> *temp = head;
	while (temp != nullptr) {
		::CPSC131::BookStore::Book book_temp = temp->getElement();
	*/

	// We do not need to Iterate/go to whole list since we already have book instance
	// Check if we have enough book to sell
	if (quantity > book.getStockAvailable()) {
		throw std::range_error ("Doesn't have enough book");
	}
	/*
	Sucessfully sell book to sell to customer
	Change the amount of stock we have in store
	Update our account balance since we receive money of selling the book from customer
	*/
	book.adjustStockAvailable(-quantity);
	adjustAccountBalance(quantity * price_cents);
	}

}
