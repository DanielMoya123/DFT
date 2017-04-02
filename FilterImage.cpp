#include "FilterImage.h"

/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/

using namespace std;

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
lti::kernel2D<float> FilterImage::GenerateSquareOddGaussianFilter(int kSize, int variance, bool octagonal){ 
	
	lti::kernel2D<float> kern;
	
	if (octagonal){
		
		lti::gaussKernel2D<float> gauss(kSize,variance); 
		kern.castFrom(gauss);
		
	}else{
	
		lti::octagonalKernel<float> gauss(kSize,variance); 
		kern.castFrom(gauss);
	
	}
	
	return kern;
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
void  FilterImage::ConvolutionSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter){
		
		lti::convolution filter;
		lti::convolution::parameters param;
		param.setKernel(kernel);
		filter.setParameters(param);
		filter.apply(imgToFilter);
		
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
void  FilterImage::SpaceSquareFilter(lti::kernel2D<float> kernel,lti::image imgToFilter){
	
		// NEED TO APPLY BOUNDARY EXPANSION

		//lti::fft fft2d;     // for 2-dimensional FFT
		//lti::ifft ifft2d;   // for 2-dimensional inverse FFT


		/* NOT SURE WHY */
		//par2d.mode = lti::fft::Polar;
		//ifft2d.setParameters(par2d);
		//fft2d.setParameters(par2d);
		/**************************/

		//lti::channel back;
		//lti::channel re,im;

		//fft2d.apply(R, re, im);       // the actual FFT, Parameters: source, real result part, imaginary result part
		
		// Multiply 2 channels
		//lti::channel  result;

		//result = lti::matrix< T >::emultiply(a,b)//not sure if so for channel instead of matrix
		
		//ifft2d.apply(re, im, back);   // inverse FF, Parameters: real part, imaginary part, destination		
	
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
lti::matrix<float> FilterImage::GetPadded(lti::matrix<float> m, int pSize){

	lti::matrix<float> result;
	lti::boundaryExpansion b = lti::boundaryExpansion(pSize);
	b.apply(m, result);
	return result;
}