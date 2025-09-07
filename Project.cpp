#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//The class of the book, yes. Has title, author, isbn, and isBorrowed to check if it's... borrowed
class Book
{
private:
    //On private class so that it won't be manipulated easily-- based on what they said atleast, idk how that works
    string title, author, isbn;
    bool isBorrowed;


public:
    //gets and prints book details
    void getBookDetails()
    {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Status: ";
        if (isBorrowed) cout << "Borrowed" << endl;
        else cout << "Available" << endl; 
    }
    //constructor that sets the title, author, and isbn to "UK" or Unknown if book details weren't entered
    Book(string t = "UK", string a = "UK", string i = "UK")
    {
        title = t;
        author = a;
        isbn = i;
        isBorrowed = false;
    }

    //gets book info but only parts of it(getBookDetails but cut into parts)
    string getTitle(){ return title; }
    string getAuthor(){ return author; }
    string getIsbn(){ return isbn; }
    bool getStatus(){ return isBorrowed; }

    //sets book info but parts(use constructor instead of this pls, haven't tested this part much
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setIsbn(string i) { isbn = i; }

    //borrow and return methods, check LibraryUser class for info
    bool borrowBook()
    {
        if (!isBorrowed){
            isBorrowed = true;
            return true;
        }
        else return false;
    }

    bool returnBook()
    {
        if (isBorrowed){
            isBorrowed = false;
            return true;
        }
        else return false;
    }
};

//class of LibraryUser that has userId, name, and borrowedBooks array. The user can borrow upto a maximum of 5 books
//Q: Why is it 5 books only?   Ans: I want it to, don't want users borrowing every single book available, not that there's much by default
//I also added borrowCount, just so that I can keep track of user's borrowed book count, yep
class LibraryUser
{
//as usual, this part will be private, gonna rely on constructor to set things
private:
    string userId;
    string name;
    string borrowedBooks[5];
    int borrowCount;

public:
    //is a getter for userId to use it for the Library class
    string getUserId() { return userId; }

//constructor for LibraryUser, for loop is to set blank values to the array
    LibraryUser(string u, string n)
    {
        userId = u;
        name = n;
        borrowCount = 0;
        for (int i = 0; i < 5; i++)
        {
        borrowedBooks[i] = "";
        }
    }
//constructor but is the default one, this is used of not declared with params
    LibraryUser() {
    userId = "";
    name = "";
    borrowCount = 0;
    for (int i = 0; i < 5; i++) borrowedBooks[i] = "";
}

//gets and prints what user has borrowed already
//uses for loop to get the book ISBN by checking every index on borrowedBooks array
    void getUserBorrowed(){
        if (borrowCount == 0)
        {
            cout << userId << ": " << name << " hasn't borrowed any books." << endl; return;
        }
        else {
            cout << userId << ": " << name << " has borrowed: \n";
            for (int i = 0; i < borrowCount; i++)
            {
                if (borrowedBooks[i] != "")
                {
                cout << " - " << borrowedBooks[i] << endl;
                }
            }
        }
    }
//uses the "&b" or just the "&" itself + random letter that is "b"(based on my searches, it can be anything)
//to basically reference the Book class and alter values in that class instead of being independent and doing nothing with the Book class
//(sorry for that explanation, I didn't understand myself either)
//this is a bool that will have its value set after it's called
//either it borrows(true) or not(false)
    bool borrowBook(Book &b)
    {
        if (borrowCount >= 5)
        {
            cout << "Maximum borrow amount reached. Caanot borrow book." << endl;
            return false;
        }
        else if (b.borrowBook()){
            borrowedBooks[borrowCount] = b.getIsbn();
            borrowCount++;
            cout << userId << ": " << name << " has borrowed " << b.getIsbn() << ": " << b.getTitle() << endl;
            return true;
        }
        else {
            cout << "Book was already borrowed." << endl;
            return false; 
        }
    }
//this is a 2dimensional loop, the entire loop of this thing depends on the LibraryUser's borrowCount, it compares the isbn of the borrowedBooks indexes to the
//book's isbn, then does returnBook() if it does.
    bool returnBook(Book &b)
    {
        for (int i = 0; i < borrowCount; i++)
        {
            if (borrowedBooks[i] == b.getIsbn())
            {
                b.returnBook();

                for (int h = i; h < borrowCount - 1; h++)
                {
                    borrowedBooks[h] = borrowedBooks[h + 1];
                borrowedBooks[borrowCount - 1] = "";
                borrowCount--;
                cout << userId << ": " << name << " returned " << b.getIsbn() << ": " << b.getTitle() << endl;
                return true;
                }
            }
        }
        cout << userId << ": " << name << " cannot borrow " << b.getIsbn() << ": " << b.getTitle() << endl;
        return false;
    }
//the following parts are needed for saving/loading to file
//adds borrowed book if borrowCount is less than 5
    void addBorrowedBook(string isbn)
    {
        if (borrowCount < 5)
        {
            borrowedBooks[borrowCount] = isbn;
            borrowCount++;
        }
    }
//getter for borrowCount
    int getBorrowCount()
    {
        return borrowCount;
    }

//getter for borrowedBooks
    string getBorrowedBook(int i)
    {
        return borrowedBooks[i];
    }

//getter for name
    string getName()
    {
        return name;
    }
};

class Library
{
//we start again with a private one, initiating the attributes but this time, instead of using data types like int 
//or string or char and the likes, we declare the array by using the class, by doing so... we create an array of objects
//the example is the one below, Book books[100], Book is the class in this and we create 500 array of objects for it
//(I'm basically setting 500 books limit and 200 users limit btw)
private:
    Book books[500];
    int bookCount;

    LibraryUser users[200];
    int userCount;

public: 

    Library()
    {
        bookCount = 0;
        userCount = 0;
    }

    void addBook(Book b)
    {
        for (int i = 0; i < bookCount; i++) {
        if (books[i].getIsbn() == b.getIsbn()) {
            cout << "Book with ISBN " << b.getIsbn() << " already exists.\n";
            return;
        }
    }
    if (bookCount < 500) {
        books[bookCount++] = b;
    }
    }

    void registerUser(LibraryUser u) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].getUserId() == u.getUserId()) {
            cout << "User with ID " << u.getUserId() << " already exists.\n";
            return;
        }
    }
    if (userCount < 200) {
        users[userCount++] = u;
    }
    }

    void removeBook(string isbn)
    {
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getIsbn() == isbn)
            {
                for (int h = i; h < bookCount - 1; h++)
                {
                    books[h] = books[h + 1];
                }
                bookCount--;
                cout << "Book with ISBN: " << isbn << " has been removed." << endl;
                return;
            }
        }
        cout << "Book with ISBN: " << isbn << " not found." << endl;
    }

    void removeUser(string userId)
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].getUserId() == userId)
            {
                for (int h = i; h < userCount - 1; h++)
                {
                    users[h] = users[h + 1];
                }
                userCount--;
                cout << "User with ID: " << userId << " has been removed." << endl;
                return;
            }
        }
        cout << "User with ID: " << userId << " not found." << endl;
    }

    void displayAllBooks()
    {
        for (int i = 0; i < bookCount; i++)
        {
            books[i].getBookDetails(); cout << endl;
        }
    }

    void displayAllUsers()
    {
        for (int i = 0; i < userCount; i++)
        {
            users[i].getUserBorrowed(); cout << endl;
        }
    }
//On this part, we'll use Book*, the asterisk is used to get the memory address of an object in the class Book (if I understood it correctly)
//it's like the Book &b in a way, check the LibraryUser class if you missed that part, otherwise continue reading this mess of a code
//Well, not actually that messy now since I used VSCode's "Format Document" button
//nullptr is used to indicate that it points to nothing yet, I read somewhere that not initizialing where it points first may
//crash the program but I added it just to be sure

    bool borrowBook(string isbn, string userId)
    {
        Book* bookPtr = nullptr;
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getIsbn() == isbn)
            {
                bookPtr = &books[i];
                break;
            }
        }
        if (!bookPtr)
        {
            cout << "Book with ISBN: " << isbn << " not found." << endl;
            return false;
        }
        
        LibraryUser* userPtr = nullptr;
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].getUserId() == userId)
            {
                userPtr = &users[i];
                break;
            }
        }
        if (!userPtr)
        {
            cout << "User with ID: " << userId << " not found." << endl;
        }

        return userPtr -> borrowBook(*bookPtr);
    }

    bool returnBook(string isbn, string userId)
    {
        Book* bookPtr = nullptr;
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getIsbn() == isbn)
            {
                bookPtr = &books[i];
                break;
            }
        }
        if (!bookPtr)
        {
            cout << "Book with ISBN: " << isbn << " not found." << endl;
            return false;
        }
        
        LibraryUser* userPtr = nullptr;
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].getUserId() == userId)
            {
                userPtr = &users[i];
                break;
            }
        }
        if (!userPtr)
        {
            cout << "User with ID: " << userId << " not found." << endl;
            return false;
        }

        return userPtr -> returnBook(*bookPtr);
    }

    void saveBooksToFile()
    {
        ofstream saveFile("books.txt");
        for (int i = 0; i < bookCount; i++)
        {
            saveFile << books[i].getIsbn() << "|" << books[i].getTitle() << "|"
                     << books[i].getAuthor() << "|" << books[i].getStatus() << endl;
        }
        saveFile.close();
    }

    void loadBooksFromFile()
    {
        ifstream loadFile("books.txt");
        string isbn, title, author, statusStr;

        while (getline(loadFile, isbn, '|'))
        {
            getline(loadFile, title, '|');
            getline(loadFile, author, '|');
            getline(loadFile, statusStr);

            Book b(title, author, isbn);
            if (statusStr == "1") b.borrowBook();
            addBook(b);
        }
        loadFile.close();
    }

    void saveUsersToFile()
    {
        ofstream saveFile("users.txt");
        for (int i = 0; i < userCount; i++)
        {
            saveFile << users[i].getUserId() << "|" << users[i].getName() << "|"
                     << users[i].getBorrowCount();
            for (int h = 0; h < users[i].getBorrowCount(); h++)
            {
                saveFile << "|" << users[i].getBorrowedBook(h);
            }
            saveFile << endl;
        }
        saveFile.close();
    }

    void loadUsersFromFile()
    {
        ifstream loadFile("users.txt");
        string userId, name, borrowCountStr, isbn;

        while (getline(loadFile, userId, '|'))
        {
            getline(loadFile, name, '|');
            getline(loadFile, borrowCountStr, '|');
            int borrowCount = stoi(borrowCountStr);

            LibraryUser u(userId, name);
            for (int i = 0; i < borrowCount; i++)
            {
                getline(loadFile, isbn, '|');
                u.addBorrowedBook(isbn);
            }
            string skip;
            getline(loadFile, skip);
            registerUser(u);
        }
        loadFile.close();
    }
};


int main()
{
    
    Library library;

    // Load existing data if available
    library.loadBooksFromFile();
    library.loadUsersFromFile();

    int choice;
    do {
        cout << "\n=== Library Menu ===\n";
        cout << "1. Add Book\n";
        cout << "2. Register User\n";
        cout << "3. Borrow Book\n";
        cout << "4. Return Book\n";
        cout << "5. Display All Books\n";
        cout << "6. Display All Users\n";
        cout << "7. Save and Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // consume newline

        if (choice == 1) {
            string title, author, isbn;
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            library.addBook(Book(title, author, isbn));
            cout << "Book added successfully.\n";
        }
        else if (choice == 2) {
            string userId, name;
            cout << "Enter user ID: ";
            getline(cin, userId);
            cout << "Enter user name: ";
            getline(cin, name);
            library.registerUser(LibraryUser(userId, name));
            cout << "User registered successfully.\n";
        }
        else if (choice == 3) {
            string isbn, userId;
            cout << "Enter ISBN of the book: ";
            getline(cin, isbn);
            cout << "Enter user ID: ";
            getline(cin, userId);
            library.borrowBook(isbn, userId);
        }
        else if (choice == 4) {
            string isbn, userId;
            cout << "Enter ISBN of the book: ";
            getline(cin, isbn);
            cout << "Enter user ID: ";
            getline(cin, userId);
            library.returnBook(isbn, userId);
        }
        else if (choice == 5) {
            library.displayAllBooks();
        }
        else if (choice == 6) {
            library.displayAllUsers();
        }
        else if (choice == 7) {
            library.saveBooksToFile();
            library.saveUsersToFile();
            cout << "Data saved. Exiting program.\n";
        }
        else {
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != 7);

    return 0;
}