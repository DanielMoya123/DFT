#include "FilterImage.h"

/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/

using namespace std;
using namespace lti;

/***************************************
 * Constants
 * 
 ***************************************/
#define PI 3.14159265


/***************************************
 * Attributes
 * 
 ***************************************/



/******************************************************
 * 
 * Constructor of the class Campbell-Robson with default values
 * 
 * **********************************************
 * 
 * Inputs: 
 * 		-------
 * 
 * Outputs:
 * 		-------
 * 
 * Restrinctions:
 * 		-------
 *****/
FilterImage::FilterImage(void) {
	
	
}

/******************************************************
 * 
 * 
 * 
 * **********************************************
 * 
 * Inputs: 
 * 			
 * 
 * Outputs:
 * 		
 * 
 * Restrinctions:
 * 			
 *****/
image FilterImage::GenerateSquareOddGaussianFilter(int size,string path){ 
	
}

void  FilterImage::ConvolutionSquareFilter2D(int size,image Gaussian,image imgToFilter){}

void  FilterImage::ConvolutionSquareFilterOctogonal(int size,image Gaussian,image imgToFilter){}

void  FilterImage::SpaceSquareFilter(int size,string pathGaussian,image Gaussian,image imgToFilter){}
