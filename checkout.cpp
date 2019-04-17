#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;

void printMenu();
int readPersons(vector <Person *> & myCardholders);
void readBooks(vector <Book * > & myBooks);
void readRentals(vector <Book * > & myBooks, vector <Person *> & myCardholders);
bool checkPersonID(vector <Person * > & myCardholders, int cardID);
void Checkout(vector < Book * > & myBooks, vector <Person *> & myCardholders);
bool checkBookID(vector <Book * > & myBooks, int bookID);
bool checkedOut(vector <Book * > & myBooks, int bookID);
void setPersonPtr(vector <Book *> &myBooks, vector <Person *> & myCardholders, int bookID, int cardID);
void bookReturn(vector <Book * > &myBooks);
void removePersonPtr(vector < Book * > & myBooks, int bookID);
void viewAvailableBooks(vector <Book *> & myBooks);
void viewRentedBooks(vector <Book * > & myBooks);
void viewYourRentals(vector <Book * > & myBooks, vector <Person *> & myCardholders);
Person * getPersonPtr(int cardID, vector <Person *> & myCardholders);
int OpenCard(vector <Person *> & myCardholders, int cardID);
void closeCard(vector <Person *> & myCardholders);
void writeBack(vector <Person *> & myCardholders, vector <Book * > myBooks);


int main() {
  vector<Book *> books;
  vector<Person *> cardholders;

  //fills vectors with current cardholders, available books
  //and which books have been checked out previously
  int largestID = readPersons(cardholders);
  readBooks(books);
  readRentals(books, cardholders);
  
  int cardID;
  int bookID;  
  int choice;

 


  
  
  do {
    
  
    printMenu();
    
    cin >> choice;
   
    
   
 
    
    switch(choice) {
    case 1: Checkout(books, cardholders);   
      break;

    case 2: bookReturn(books);
      // Book return
      break;

    case 3: viewAvailableBooks(books);
      // View all available books
      break;

    case 4: viewRentedBooks(books);
      // View all outstanding rentals
      break;

    case 5: viewYourRentals (books, cardholders);
      // View outstanding rentals for a cardholder
      break;

    case 6: largestID = OpenCard(cardholders, largestID);
                // Open new library card
                break;

    case 7: closeCard(cardholders);
      // Close library card
      break;
                
    case 8: writeBack(cardholders, books);
                // update records in files here before exiting the program
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);


  for (int i =0; i < cardholders.size(); i++) {
    delete cardholders.at(i);
    cardholders.at(i) = NULL;
  }

   for (int i =0; i < books.size(); i++) {
    delete books.at(i);
    books.at(i) = NULL;
  }




  return 0;


}

void printMenu(){
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}


//reads in persons.txt file which holds information about each cardholder
//then saves each Person pointer into the cardholders vector
int readPersons(vector <Person *> & myCardholders){
  Person * personptr = NULL;
  int cardNum = 0;
  bool act = 0;
  string firstName = " ";
  string lastName = " ";
  int largestID = 0;

 

  
  ifstream inFile;
  inFile.open("persons.txt");

  while ( inFile >> cardNum >> act >> firstName >> lastName) {
    personptr = new Person(cardNum, act, firstName, lastName);
    
    
   
   
    if (personptr != NULL){
     
      myCardholders.push_back(personptr);
    }
    
    largestID = cardNum;
    
    
    
   
    
  }
  personptr = NULL;
 
  inFile.close();
 
  return (largestID +1);
   
  
}

/*void checkCardholders(vector <Person * > & mycardh){
  for ( vector <Person * >:: iterator it = mycardh.begin(); it != mycardh.end(); it++){
    if(*it != NULL){
      cout << (*it)->fullName()<< endl;
    }
  }
}
*/
  

//reads in books.txt which holds information about each book
//then saves each Book pointer into the books vector
void readBooks(vector <Book * > & myBooks){
  Book * bookPtr;
  int bookID = 0;
  string bookName, author, cat, line, emptyline;
  ifstream inFile;
  inFile.open("books.txt");
  while (!inFile.eof()){
    //reading from file one line at a time, skipping any information on every 5th line

    inFile >> bookID;
    getline(inFile, emptyline, '\n');    
    getline(inFile, bookName, '\n');
    getline(inFile, author, '\n');
    getline(inFile, cat, '\n');
    getline(inFile, emptyline, '\n');
    
    bookPtr = new Book(bookID, bookName, author, cat);
    myBooks.push_back(bookPtr);

  
   
  }
  bookPtr = NULL;

  inFile.close();
}


//reads in rentals.txt and saves the Person pointer to the myBooks vector
//for each book that has already been rented to specify which cardholder has rented it
void readRentals(vector <Book * > & myBooks, vector <Person *> & myCardholders){

  int bookID, cardID;
  Person *personPtr = NULL;
 
  
  ifstream inFile;
  inFile.open("rentals.txt");

  while (inFile >> bookID >> cardID) {
   
    for (int i = 0; i < myCardholders.size(); i++) {
      if (myCardholders.at(i)->getId() == cardID) {
	personPtr = myCardholders.at(i);
      }
    }
    for (int j = 0; j < myBooks.size(); j++) {
      if (myBooks.at(j)->getId() == bookID) {
	myBooks.at(j)->setPersonPtr(personPtr);
      }
    }

   
   
  }
  personPtr = NULL;
  inFile.close();
}


//confirm if cardID is in system,then confirm if it is active
//if both, print out cardholder name
bool checkPersonID(vector <Person * > & myCardholders,int cardID) {

  int j = 0;
  
  for ( int i =0; i < myCardholders.size(); i++) {
    if ( myCardholders.at(i)->getId() == cardID) {
      j = i;
    }
        
  }
  if(myCardholders.at(j)->getId() == cardID){
    if (myCardholders.at(j)->isActive()) {
	cout << myCardholders.at(j)-> fullName() << endl;
	return true;
    }
    else{
      cout<< "inactive card ID" << endl;
    }
  }
  else{
    cout << "invalid card ID" << endl;
  }
    
    

  return false;
}

//checks to see if book exists in system
bool checkBookID(vector <Book * > & myBooks, int bookID) {
  for (int i = 0; i < myBooks.size(); i++) {
    if (myBooks.at(i)->getId() == bookID) {
      cout << "Title: " <<  myBooks.at(i)->getTitle() << endl;
      return true;
    }
  }
  cout << "Invalid Book ID" << endl;
  return false;
}

//checks to see if book has been previously checked out
bool checkedOut(vector <Book * > & myBooks, int bookID) {
  for (int i = 0; i < myBooks.size(); i++) {
    if (myBooks.at(i)->getId() == bookID) {
      if (myBooks.at(i)->getPersonPtr() == nullptr) {

	return true;
    }
  }
  }
  
  return false;
  
}

//sets person ptr into book object- allows cardholder to checkout books
void setPersonPtr(vector <Book *> &myBooks, vector <Person *> & myCardholders, int bookID, int cardID){
  for (int i =0; i < myBooks.size(); i++){
    if (myBooks.at(i)->getId() == bookID) {
      for (int j =0; j < myCardholders.size(); j++) {
	if (myCardholders.at(j)->getId() == cardID) {
	  myBooks.at(i)->setPersonPtr(myCardholders.at(j));
	  cout << "Checkout Complete" << endl;
	  return;
	}
      }
    }
  }
  
  cout << "Book could not be checked out" << endl;
}

//confirms if book is available by making sure it exists in the system
//as well making sure it has not already been checked out
//if it is available then it allows the cardholder to check the book out

void Checkout(vector < Book * > & myBooks, vector <Person *> & myCardholders){

  int cardID;
  int bookID;
  cout << "Please enter the card ID: ";
  cin >> cardID;
  cout << "\n";

  if(checkPersonID(myCardholders, cardID)){
    cout << "Please enter book ID: " << endl;
    cin >> bookID;
    if(checkBookID(myBooks, bookID)){
      if(checkedOut(myBooks, bookID)){
	cout << "Book Available" << endl;
	setPersonPtr(myBooks, myCardholders, bookID, cardID);
      }
      else {
	cout << "Book already checked out" << endl;
      }
    }
  }
}

void removePersonPtr(vector < Book * > & myBooks, int bookID) {
  for (int i =0; i < myBooks.size(); i++) {
    if (myBooks.at(i)->getId() == bookID) {
      myBooks.at(i)->setPersonPtr(NULL);
    }
  }
}

void bookReturn(vector <Book * > &myBooks) {
  int bookID;
  cout << "Please enter the book ID to return: " << endl;
  cin >> bookID;
  if (checkBookID(myBooks, bookID)) {
    removePersonPtr(myBooks, bookID);
    cout << "Return Completed" << endl;
    
  }
  else{
    cout << "Return could not be completed" << endl;
  }
}

void viewAvailableBooks(vector <Book *> & myBooks){
  cout << "Here are the books that are currently available: " << endl;
  for (int i =0; i < myBooks.size(); i++) {
    if (checkedOut(myBooks, myBooks.at(i)->getId())) {
	cout << "Book ID: " << myBooks.at(i)->getId() << endl;
	cout << "Title: "   << myBooks.at(i)->getTitle() << endl;
	cout << "Author: "  << myBooks.at(i)->getAuthor() << endl;
	cout << "Category:" << myBooks.at(i)->getCategory() << endl;
	cout << endl;
      }
  }
      
}


void viewRentedBooks(vector <Book * > & myBooks) {
  

cout << "Here are the books that are currently already checked out: " << endl;
  for (int i =0; i < myBooks.size(); i++) {
    if (!checkedOut(myBooks, myBooks.at(i)->getId())) {
	cout << "Book ID: " << myBooks.at(i)->getId() << endl;
	cout << "Title: "   << myBooks.at(i)->getTitle() << endl;
	cout << "Author: "  << myBooks.at(i)->getAuthor() << endl;
	cout << "Category:" << myBooks.at(i)->getCategory() << endl;
	cout << endl;
      }
  }

}

Person * getPersonPtr(int cardID, vector <Person *> & myCardholders) {
  Person *personptr = NULL;
  for (int i =0; i < myCardholders.size(); i++) {
    if (myCardholders.at(i)->getId() == cardID) {
      personptr = myCardholders.at(i);
    }
  }
  return personptr;
}
  


void viewYourRentals(vector <Book * > & myBooks, vector <Person *> & myCardholders) {

  int cardID;
  cout << "Please enter the card ID: " << endl;
  cin >> cardID;
  checkPersonID(myCardholders, cardID);
  cout << endl;
  Person * personptr = getPersonPtr(cardID, myCardholders);
  cout << "These are your rentals: " << endl;
  cout << endl;
  for (int i =0; i <myBooks.size(); i++){
    if (myBooks.at(i)->getPersonPtr() == personptr) {
      cout << "Book ID: " << myBooks.at(i)->getId() << endl;
      cout << "Title: "   << myBooks.at(i)->getTitle() << endl;
      cout << "Author: "  << myBooks.at(i)->getAuthor() << endl;
      cout << endl;
    }
  }
}
    

    
int OpenCard(vector <Person *> & myCardholders, int cardID) {
  string firstName;
  string lastName;
  
  
  cout << "Please enter the first name: ";
  cin >> firstName;
  cout << "Please enter the last name: ";
  cin >> lastName;

  Person * personptr = new Person(cardID, 1, firstName, lastName);
  myCardholders.push_back(personptr);
  
  cout << "Card ID " << cardID << " active" << endl;
  cout << "Cardholder: " << (firstName + " " + lastName) << endl;

  personptr = NULL;
  
  return (cardID + 1);
}

void closeCard(vector <Person *> & myCardholders) {
  int cardID;
  char answer;
  cout << "Please enter the card ID: ";
  cin >> cardID;
  if (checkPersonID(myCardholders, cardID)) {
    cout << "Are you sure you want to deactive card (y/n)? ";
      cin >> answer;
    if (answer == 'y') {
      for (int i =0; i < myCardholders.size(); i++) {
	if (myCardholders.at(i)->getId() == cardID) {
	  myCardholders.at(i)->setActive(false);
	  cout << "Card ID deactivated" << endl;
	}
      }
    }
  }
}

void writeBack(vector <Person *> & myCardholders, vector <Book * > myBooks) {

  ofstream outFile;
  outFile.open("persons.txt");
  for (int i =0; i < myCardholders.size(); i++) {
    outFile << myCardholders.at(i)->getId() << " ";
    if (myCardholders.at(i)->isActive()) {
      outFile << 1 << " ";
    }
    else{
      outFile << 0 << " ";
    }
    outFile << myCardholders.at(i)->getFirstName()<< " " << myCardholders.at(i)->getLastName() << endl;
  }
  outFile.close();
  
  
  outFile.open("books.txt");
  for (int j =0; j < myBooks.size(); j++) {
    outFile << myBooks.at(j)->getId() << endl;
    outFile << myBooks.at(j)->getTitle()<< endl;
    outFile << myBooks.at(j)->getAuthor() << endl;
    outFile << myBooks.at(j)->getCategory()<< endl;
    outFile << endl;
  }
  outFile.close();

  outFile.open("rentals.txt");

  for (int k =0; k <myBooks.size(); k++) {
    if (myBooks.at(k)->getPersonPtr() != NULL){
      for (int l =0; l<myCardholders.size(); l++){
	if(myBooks.at(k)->getPersonPtr() == myCardholders.at(l)){
	  outFile << myBooks.at(k)->getId() << " " << myCardholders.at(l)->getId() << endl;
	}
      }
    }
  }

  outFile.close();
}
    
