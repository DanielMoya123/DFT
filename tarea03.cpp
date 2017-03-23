#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <ltiTimer.h>



/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
  cout << "usage: \n" << endl;   
}



/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{

	lti::timer chron;

      chron.start();

      

      chron.stop();


      cout << "Duration: " << chron.getTime() << " microseconds\n";

	return EXIT_SUCCESS;
  
}