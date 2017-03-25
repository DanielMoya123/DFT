#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>


#include <ltiTimer.h>
#include <ltiGaussKernels.h>
#include <ltiBinaryKernels.h>


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


    lti::loadImage loader; // functor to load images
	lti::image img;        // the image loaded will be left here.

	loader.load("Lenna.png",img);  // load PNG image

	//// CONVERSION FROM img TO channel IS MISSING !!

	lti::channel  imgGray;

	double variance;

	// kSize NEEDS TO BE DEFINED AS AN ENTRY PARAMETER
	variance = pow((kSize+2)/6, 2)

	//Guassian kernel
	lti::gaussKernel2D<lti::channel::value_type> gKernel(kSize,variance); 
 	lti::convolution gFilter;                        // convolution operator
 	lti::convolution::parameters gParam;             // parameters
 	gParam.setKernel(gKernel);                        // use the gauss kernel
 	gFilter.setParameters(gParam);                    // set parameters
	gFilter.apply(imgGray);

	//Octogonal kernel
	lti::octagonalKernel<lti::channel::value_type> oKernel(kSize,variance); 
 	lti::convolution oFilter;                        // convolution operator
 	lti::convolution::parameters oParam;             // parameters
 	oParam.setKernel(oKernel);                        // use the ortogonal kernel
 	oFilter.setParameters(oParam);                    // set parameters
	oFilter.apply(imgGray);







	


      

    chron.stop();


    cout << "Duration: " << chron.getTime() << " microseconds\n";

	return EXIT_SUCCESS;
  
}