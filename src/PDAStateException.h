/*
 PDAStateException.h
 for CSC 2710-001 Program #1
 by Philip Westrich
 Monday, May 5, 2014
 
 This is a simple exception for the TwoStackPDA to throw around. It extends the
 standard library exception class. It simply holds a message to be passed to the 
 user, usually telling why their string was not accepted.
 
 Again, I should define the object in the header file, and implement it in a seperate
 .cpp file, but I had linker issues. Same as the others.
*/

#ifndef PDAStateException_h
#define PDAStateException_h

#include <exception>
#include <string>

using namespace std;

class PDAStateException : public exception {
   
private:
   
   //what went wrong
   string message;
   
public:
   
   //give me an error message to report
   PDAStateException(const string &message);
   ~PDAStateException();
   
   //what happened?
   const char *what() const noexcept;
   
};

//make a new object with a message
PDAStateException::PDAStateException(const string &newMessage){
   
   message = newMessage;
   
}

//destructor. Nothing to do here.
PDAStateException::~PDAStateException(){}

//Get the mesage as a const char*
const char *PDAStateException::what() const noexcept{
   
   return message.c_str();
   
}

#endif
