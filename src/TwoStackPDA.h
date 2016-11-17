/*
 TwoStackPDA.h
 for CSC 2710-001 Program #1
 by Philip Westrich
 Monday, May 5, 2014
 
 This is the class specification and implementation for the TwoStackPDA. I know 
 it's poor programming practice to put the implementation in the same file as the 
 definition, but I had linker issues otherwise, so I put both in one file. This is
 a small program, so it should not cause any massive issues.
 
 The definition of the TwoStackPDA object has a private class, PDAState, and a 
 private struct, PDAStateTransition. The both are defined here because I ran into 
 compiler issues when I tried to write them as their own standalone objects, in 
 their own header files. PDAStateTransition needed to be able to use data members
 and functions from TwoStackPDA, and the compiler wouldn't have it. I had a similar
 issue with PDAStateTransition. So, they are also defined here. I know, not a good 
 practice, but again, this is a smaller program, and it should not cause much issue.
 I implemented the objects in the order I specified them in, and did my best to 
 clearly mark that.
*/

#ifndef TWO_STACK_PDA_H
#define TWO_STACK_PDA_H

#define EMPTY_STRING "!"
#define END_OF_STACK "\0"

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <vector>

#include "PDAStateException.h"
#include "StringExplode.h"

using namespace std;

/*
   This is the object for the Two-Stack Push-Down Automoton (PDA). It very closley
   follows the State design pattern we studied in CSC 2120.
*/
class TwoStackPDA {
   
private:
   
   //forward definition of the transition, so the compiler is happy
   struct PDAStateTransition;
   
   /*
    This is the object for individual states in the PDA. Each one has a list of transitions
    and a reference to the PDA it is a member of.
    */
   class PDAState {
      
   private:
      
      //list of state transitions
      vector<PDAStateTransition*> transitions;
      
      //state's label (can be very simple)
      int number;
      
      //reference to the state's machine
      TwoStackPDA *pda;
      
      //is it final?
      bool isFinal;
      
      //is it the start state?
      bool isStart;
      
   public:
      
      //constructor and destructor
      PDAState(int newNumber, TwoStackPDA *newPDA, bool start, bool final);
      ~PDAState();
      
      int getNumber() const noexcept;
      bool isFinalState() const noexcept;
      bool isStartState() const noexcept;
      
      //add a transition rule
      void addTransition(PDAStateTransition *newTransition);
      
      void processChar(char input);
      
   };
   
   /*
    Struct for a state transition.
    */
   struct PDAStateTransition {
      
      //state to switch to
      int nextState;
      
      //what character this state accepts
      char toEat;
      
      //what to push on to each stack
      string popStackOne;
      string popStackTwo;
      
      //what to pop from each stack
      string pushStackOne;
      string pushStackTwo;
      
      //method to determine if two states are equal
      bool isEqualTo(PDAStateTransition *other){
         
         return ((nextState == other->nextState) && (toEat == other->toEat) && (pushStackOne == other->pushStackOne)
                 && (pushStackTwo == other->pushStackTwo) && (popStackOne == other->popStackOne) && (popStackTwo == other->popStackTwo));
         
      }
      
   };
   
   //first stack
   stack<string> stackOne;
   
   //second stack
   stack<string> stackTwo;
   
   //list of states this machine has
   vector<PDAState*> states;
   
   //number of states
   int numStates;
   
   //machine's alphabet
   vector<string> alphabet;
   
   //how many characters in the alphabet
   int numCharacters;
   
   //current state
   PDAState *currentState;
   
   //current input
   string currentInput;
   
   //is the current input accepted?
   bool isStringAccepted;
   
public:
   
   //constructor and destructor
   TwoStackPDA(const string &fileName);
   ~TwoStackPDA();
   
   //prints current state of the PDA
   void printCurrentState(ostream &out) const noexcept;
   
   //process an input string
   bool processInput(string input);
   
   //process an individual character, one-by-one
   void processInput(char input);
   
   //was the last string accepted?
   bool isAccepted() const noexcept;
   
   //resets the PDA
   void reset() noexcept;
   
   //what is the current input?
   string getCurrentInput() const noexcept;
   
};

// --------- Begin implementation of TwoStackPDA::PDAState ----------------------------------------------------------

/*
 Constructor. Sets up the object.
 
 Parameters:
   newNumber:  This state's number
   *newPDA:    Reference to the PDA this state belongs to
   start:      Is this the starting state?
   final:      Is this a final state?
 
*/
TwoStackPDA::PDAState::PDAState(int newNumber, TwoStackPDA *newPDA, bool start, bool final){
   
   //check incoming args
   if (newNumber < 0){
      
      throw new PDAStateException("Error: State cannot be numbered less than zero");
      
   }
   //to_string(int) is not in the windows version of the compiler. I know this will never happen, so I just commented it out
   /*else if (newPDA == NULL){
      
      throw new PDAStateException("Error: Cannot have null PDA in state: " + to_string(newNumber));
      
   }*/
   
   number = newNumber;
   pda = newPDA;
   isStart = start;
   isFinal = final;
   
}

TwoStackPDA::PDAState::~PDAState(){
   
   //delete all the transitions
   for (int i = 0; i < transitions.size(); i++){
      
      delete transitions[i];
      
   }
   
}

//what's this state's number?
int TwoStackPDA::PDAState::getNumber() const noexcept {
   
   return number;
   
}

//is it a final state?
bool TwoStackPDA::PDAState::isFinalState() const noexcept {
   
   return isFinal;
   
}

//is is an ending state?
bool TwoStackPDA::PDAState::isStartState() const noexcept {
   
   return isStart;
   
}

//add transitions to this state
void TwoStackPDA::PDAState::addTransition(PDAStateTransition *newTransition){
   
   //make sure it doesn't already exist
   for (int i = 0; i < transitions.size(); i++){
      
      //check if identical
      if (transitions[i]->isEqualTo(newTransition)){
         
         throw new PDAStateException("Error: Cannot add identical transition to state.");
         
      }
      
      //check if they accept the same character, nondeterminism NOT ALLOWED
      else if (transitions[i]->toEat == newTransition->toEat){
         
         //throw new PDAStateException("Error: Two transitions cannot accept the same character!");
         cerr << "Bad transition, using anyway." << endl;
         
      }
      
   }
   
   //otherwise, add it
   transitions.push_back(newTransition);
   
}

//processes a character
void TwoStackPDA::PDAState::processChar(char input){
   
   //check all transitions for this character
   for (int i = 0; i < transitions.size(); i++){
      
      //check character match
      if (transitions[i]->toEat == input){
         
         //check top of stack 1 match or empty string
         if ((transitions[i]->popStackOne == EMPTY_STRING) || (pda->stackOne.top() == transitions[i]->popStackOne)){
            
            //check top of stack 2 match or empty string
            if ((transitions[i]->popStackTwo == EMPTY_STRING) || (pda->stackTwo.top() == transitions[i]->popStackTwo)){
               
               //complete match found, use this rule
               
               //pop both stacks, if needed
               if (transitions[i]->popStackOne != EMPTY_STRING){
                  
                  pda->stackOne.pop();
                  
               }
               
               if (transitions[i]->popStackTwo != EMPTY_STRING){
                  
                  pda->stackTwo.pop();
                  
               }
               
               //push stuff to stacks, if needed
               if (transitions[i]->pushStackOne != EMPTY_STRING){
                  
                  pda->stackOne.push(transitions[i]->pushStackOne);
                  
               }
               
               if (transitions[i]->pushStackTwo != EMPTY_STRING){
                  
                  pda->stackTwo.push(transitions[i]->pushStackTwo);
                  
               }
               
               //set next state
               pda->currentState = pda->states[transitions[i]->nextState];
               
               //get out of here
               return;
               
            }
            
         }
         
      }
      
   }
   
   //no transition found, problem
   throw new PDAStateException("Stuck in a state without a transition rule.");
   
}

// --------- Begin implementation of TwoStackPDA ------------------------------------------------------------------------------------

/*
 The constructor. Reads the information for the PDA from the file to set up the object.
 
 Parameters:
   fileName:   Name of the file to read
*/
TwoStackPDA::TwoStackPDA(const string &fileName){
   
   //open infile
   fstream inFile(fileName);
   
   //if it failed, problem
   if (!inFile){
      
      throw new PDAStateException("Error: Could not open input file.");
      
   }
   
   //buffer for reading input, will reuse a lot
   string buffer;
   
   //read the first line
   getline(inFile, buffer, '\n');
   
   //split it by the delimiter
   alphabet = explode(" ", buffer);
   
   //next line: number of states
   getline(inFile, buffer, '\n');
   numStates = atoi(buffer.c_str());
   
   //have to have at least one state
   if (numStates < 1){
      
      throw new PDAStateException("Error: Cannot have less than one state!");
      
   }
   
   //next line: what will be final states
   getline(inFile, buffer, '\n');
   
   //split it
   vector<string> finalStates = explode(" ", buffer);
   
   //must have at least one final state
   if (finalStates.size() < 1){
      
      throw new PDAStateException("Error: Must have at least one final state!");
      
   }
   
   //can't have too many final states
   else if (finalStates.size() > numStates){
      
      throw new PDAStateException("Error: Number of final states cannot exceed the total number of states!");
      
   }
   
   //for each state
   for (int currentStateNumber = 0; currentStateNumber < numStates; currentStateNumber++){
      
      bool shouldBeFinal = false;
      
      //determine if this state should be final
      for (int i = 0; i < finalStates.size(); i++){
         
         if (currentStateNumber == atoi(finalStates[i].c_str())){
            
            shouldBeFinal = true;
            break;
            
         }
         
      }
      
      //make the new state
      PDAState *newState = new TwoStackPDA::PDAState(currentStateNumber, this, (currentStateNumber == 0), shouldBeFinal);
      
      //read the line of transitions
      getline(inFile, buffer, '\n');
      
      //split into individual transitions
      vector<string> transitions = explode(" ", buffer);
      
      //there should be one transition for each char in the alphabet
      if (transitions.size() != alphabet.size()){
         
         throw new PDAStateException("Error: Need one transition rule per character in the alphabet.");
         
      }
      
      //for each transition
      for (int currentTransition = 0; currentTransition < transitions.size(); currentTransition++){
         
         vector<string> transitionData = explode(",", transitions[currentTransition]);
         
         //create the transition, and set its data
         PDAStateTransition *newTransition = new PDAStateTransition;
         
         newTransition->toEat          = *(alphabet[currentTransition].c_str());
         newTransition->nextState      = atoi(transitionData[0].c_str());
         newTransition->popStackOne    = transitionData[1];
         newTransition->pushStackOne   = transitionData[2];
         newTransition->popStackTwo    = transitionData[3];
         newTransition->pushStackTwo   = transitionData[4];
         
         newState->addTransition(newTransition);
         
      }
      
      //add the state to the list
      states.push_back(newState);
         
   }
   
   //set up for first run
   reset();
      
}

//destructor. Deletes all states, and the alphabet
TwoStackPDA::~TwoStackPDA(){
   
   //delete every state
   for (int i = 0; i < numStates; i++){
      
      delete states[i];
      
   }
   
}

//prints the PDA's current state
void TwoStackPDA::printCurrentState(ostream& out) const noexcept {
   
   //copy the stacks, don't want to screw them up
   stack<string> copyStackOne(stackOne);
   stack<string> copyStackTwo(stackTwo);
   
   //print the machine's state
   out << "Current state of the PDA:" << endl;
   out << "-------------------------------------------------------------" <<endl;
   out << "Current state: " << currentState->getNumber() << endl;
   out << "Is the current state final? ";
   
   if (currentState->isFinalState()){
      
      cout << "Yes" << endl;
      
   }
      
   else {
         
      cout << "No" << endl;
         
   }
   
   out << "Input remaining to process: " << currentInput << endl;
   out << "Is the string accepted? ";
   
   if (isStringAccepted){
      
      cout << "Yes" << endl;
      
   }
   
   else {
      
      cout << "No" << endl;
      
   }
   
   out << "Contents of stack 1 (top to bottom): ";
   
   while (!copyStackOne.empty()){
      
      out << copyStackOne.top() << " ";
      copyStackOne.pop();
      
   }
   
   out << endl;
   out << "Contents of stack 2 (top to bottom): ";
   
   while (!copyStackTwo.empty()){
      
      out << copyStackTwo.top() << " ";
      copyStackTwo.pop();
      
   }
   
   out << endl;
   out << "-------------------------------------------------------------" <<endl;
   
}

//processes a string of characters, returns whether or not it is accepted
bool TwoStackPDA::processInput(string input){
   
   //reset the PDA
   reset();
   
   //make a copy to keep
   currentInput = input;
   
   try {
   
      //process every character
      while (!currentInput.empty()){
         
         if (input[0] == '\0') break;
      
         currentState->processChar(currentInput[0]);
         currentInput.erase(currentInput.begin());
      
      }
      
   } catch (PDAStateException &it){
      
      //rejected string, stuck in a state
      return false;
      
   }
   
   //check ending conditions
   isStringAccepted = (currentInput.empty() && currentState->isFinalState() && (stackOne.top() == END_OF_STACK) && (stackTwo.top() == END_OF_STACK));
   return isStringAccepted;
   
}

//processes an individual character
void TwoStackPDA::processInput(char input){
   
   //do work on the one character
   currentState->processChar(input);
   
   //check accepting conditions
   isStringAccepted = (currentState->isFinalState() && (stackOne.top() == END_OF_STACK) && (stackTwo.top() == END_OF_STACK));
   
}
   
bool TwoStackPDA::isAccepted() const noexcept{
      
   return isStringAccepted;
      
}
   
void TwoStackPDA::reset() noexcept{
      
   //reset the stacks
   while (!stackOne.empty()){
      
      stackOne.pop();
      
   }
   
   while (!stackTwo.empty()){
      
      stackTwo.pop();
      
   }
   
   //push the end-of-stack character back on
   stackOne.push(END_OF_STACK);
   stackTwo.push(END_OF_STACK);
   
   //reset other instance variables
   currentInput = EMPTY_STRING;
   isStringAccepted = false;
   currentState = states[0];
   
}
   
string TwoStackPDA::getCurrentInput() const noexcept{
      
   return currentInput;
      
}

#endif
