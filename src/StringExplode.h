/*
 StringExplode.h
 for CSC 2710-001 Program #1
 by Philip Westrich
 Monday, May 5, 2014
 
 This function splits a string by a given delimeter into a vector of strings.
 
 I actually didn't write this. I borrowed it from the following StackOverflow article:
 
   http://stackoverflow.com/questions/890164/how-can-i-split-a-string-by-a-delimiter-into-an-array/
 
 I modified it a bit, made the code a little cleaner. However, it remains mostly unchanged
 from the original on the website. If I remember correctly, it is from the first solution
 on the webpage.
*/

#ifndef StringExplode_h
#define StringExplode_h

#include <string>
#include <vector>

using namespace std;

vector<string> explode(const string &delimiter, const string &str){
   
   vector<string> arr;
   
   int strleng = static_cast<int>(str.length());
   int delleng = static_cast<int>(delimiter.length());
   
   if (delleng == 0) {
      
      return arr; //no change
      
   }
   
   int i = 0;
   int k = 0;
   
   while(i < strleng) {
      
      int j=0;
      while (i+j<strleng && j<delleng && str[i+j]==delimiter[j]) {
         
         j++;
         
      }
      
      //found delimiter
      if (j == delleng) {
         
         arr.push_back(str.substr(k, i - k));
         i += delleng;
         k = i;
      }
      
      else {
         
         i++;
         
      }
      
   }
   
   arr.push_back(str.substr(k, i - k));
   return arr;
   
}

#endif
