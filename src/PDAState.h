//
//  PDAState.h
//  2710program
//
//  Created by Philip Westrich on 4/22/14.
//  Copyright (c) 2014 Philip Westrich. All rights reserved.
//
/*
#ifndef _710program_PDAState_h
#define _710program_PDAState_h

#include <vector>
#include <string>

#include "TwoStackPDA.h"
#include "PDAStateTransition.h"
#include "PDAStateException.h"

using namespace std;

class PDAState {
   
private:
   
   //list of state transitions
   vector<PDAStateTransition*> transitions;
   
   //state's label (can be very simple
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
   
   int getNumber();
   bool isFinalState();
   bool isStartState();
   
   //add a transition rule
   void addTransition(PDAStateTransition *newTransition);
   
   void processChar(char input);
   
};

PDAState::PDAState(int newNumber, TwoStackPDA *newPDA, bool start, bool final){
   
   //check incoming args
   if (newNumber < 0){
      
      throw new PDAStateException("Error: State cannot be numbered less than zero");
      
   }
   
   else if (newPDA == NULL){
      
      throw new PDAStateException("Error: Cannot have null PDA in state: " + to_string(newNumber));
      
   }
   
   number = newNumber;
   pda = newPDA;
   isStart = start;
   isFinal = final;
   
}

PDAState::~PDAState(){
   
   //delete all the transitions
   for (int i = 0; i < transitions.size(); i++){
      
      delete transitions[i];
      
   }
   
}

//what's this state's number?
int PDAState::getNumber(){
   
   return number;
   
}

//is it a final state?
bool PDAState::isFinalState(){
   
   return isFinal;
   
}

//is is an ending state?
bool PDAState::isStartState(){
   
   return isStart;
   
}

//add transitions to this state
void PDAState::addTransition(PDAStateTransition *newTransition){
   
   //make sure it doesn't already exist
   for (int i = 0; i < transitions.size(); i++){
      
      if (transitions[i] == newTransition){
         
         throw new PDAStateException("Error: Cannot add identical transition to state ");
         
      }
      
      else if (transitions[i]->isEqualTo(newTransition)){
         
         throw new PDAStateException("Error: Incoming transition already exists!");
         
      }
      
   }
   
   transitions.push_back(newTransition);
   
}

//processes a character
void PDAState::processChar(char input){
   
   //check all transitions for this character
   for (int i = 0; i < transitions.size(); i++){
      
      if (transitions[i]->toEat == input){
         
         //found a match, do work
         
         //pop from stack one
         if (pda->stackOne.top() == transitions[i]->popStackOne){
            
            pda->stackOne.pop();
            
         }
         
         //change states
         pda->currentState = transitions[i]->nextState;
         
         break;
         
      }
      
   }
   
   //no transition found
   
}

#endif*/
