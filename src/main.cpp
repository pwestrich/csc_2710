/*
 main.cpp
 for CSC 2710-001 Program #1
 by Philip Westrich
 Monday, May 5, 2014
 
 This is the main driver for the program. It handles all interface with the user.
 
 One known issue is that if you enter a bad file name the program crashes. I don't
 understand why. In the constructor for the TwoStackPDA, I throw a PDAStateException
 when the input file is bad. Every time I declare a new TwoStackPDA, I'm in a 
 try-catch block, which should catch the exception, right? But it doesn't. I have
 no idea why. Every other try-catch block I have in the program works, except the
 one for the TwoStackPDA constructoer. So, please only enter valid file names, or 
 you'll have a bad time.
 
 When you compile this, be sure to use C++11, as I make use of a few things that
 exist only in C++11. The compile command should be:
 
      g++ -std=c++11 -o program1 main.cpp
 
 The program compiles without issue on my Mac, and runs without error, to my knowledge.
 However, Apple wrote their own compiler for C, C++, and Objective-C, called clang.
 It has several different rules, and works differently than the GNU C++ compiler. I
 have done my best to ensure that there will be no compilation issues on a Windows
 machine with the GNU C++11 compiler, but I cannot gurantee that it will work flawlessly.
 I have included with the program the Xcode 5.1.1 project I wrote the program in, and
 I can gurantee that will work without issue. If it doesn't compile on your machine, 
 let me know; I'll do whatever I can in order to ensure that I do not recieve a zero,
 because zeroes are bad. I'll even let you borrow my own computer if I have to.
 
 \r's in files may be an issue, because Windows [i]still[\i] needs two newline 
 characters. Hopefully not, however.
*/

#include <cstdlib>
#include <iostream>
#include <string>

#include "TwoStackPDA.h"
#include "PDAStateException.h"

using namespace std;

int main(int argc, const char *argv[]){
   
   cout << "This program will test a Two-Stack Push-Down Automoton (PDA)." << endl;
   
   TwoStackPDA *pda = NULL;
   string fileName = "";
   string keepProcessing = "";
   string input = "";
   string quick = "";
   
   do { //make the user do this until they enter a valid filename
   
      try {
         
         //get first pda
         cout << "Enter a file to read: ";
         getline(cin, fileName, '\n');
      
         pda = new TwoStackPDA(fileName);
      
      } catch (PDAStateException &it) { //WHY DOES THIS NOT CATCH THE EXCEPTION I THROW???
      
         cerr << it.what() << endl;
      
      }
      
   } while (pda == NULL);
   
   //while the user wants to use this pda
   do {
      
      //while the user wants to keep entering input
      do {
         
         cout << "Enter a string to process: ";
         getline(cin, input, '\n');
         
         cout << "Would you like to do a quick run? (Y for yes, anything else for no): ";
         getline(cin, quick, '\n');
         
         if (quick == "Y" || quick == "y"){
            
            try {
               
               pda->processInput(input);
               
            } catch (PDAStateException &it){
               
               cout << it.what() << endl;
               
            }
               
            pda->printCurrentState(cout);
               
         }
            
         else {
               
            try {
                  
               //do each character, slow run
               for (int i = 0; i < input.size(); i++){
                     
                  pda->processInput(input[i]);
                  pda->printCurrentState(cout);
                  
                  cout << "Press any key to continue...";
                  cin.get();
                  
               }
               
            } catch (PDAStateException &it){
               
               cout << it.what() << endl;
               
            }
            
            pda->printCurrentState(cout);
            
         }
         
         //reset pda for next run
         pda->reset();
         
         cout << "Would you like to enter a new string? (Y for yes, anything else for no): ";
         getline(cin, keepProcessing, '\n');
         
      } while (keepProcessing == "Y" || keepProcessing == "y");
      
      do {
      
         cout << "Enter a file to read, or 'quit' to stop: ";
         getline(cin, fileName, '\n');
      
         if (fileName == "quit") break;
      
         delete pda;
         pda = NULL;
         
         try {
         
            pda = new TwoStackPDA(fileName);
            
         } catch (PDAStateException &it){
            
            cerr << it.what() << endl;
            
         }
         
      } while (pda == NULL);
      
      if (fileName == "quit") break;
      
   } while (true);
   
   cout << "Exiting..." << endl;
   
   if (pda != NULL){
      
      delete pda;
      
   }
   
   return 0;
   
}
