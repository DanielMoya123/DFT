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
 * 
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
void  FilterImage::FreqSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter){

      	// shift the spectrum (in the space domain)
	for (int y=0;y<imgToFilter.rows();++y) {
        	float s=lti::even(y) ? 1.f : -1.f;
        	for (int x=0;x<imgToFilter.columns();++x) {
          		imgToFilter.at(y,x)*=s;
          		s*=-1.f;
        	}
      	}
	// end of the shift (in the space domain)

	


	// Transform to frequency spectrum
	static const lti::eCoordinateSystem cordSys = lti::Polar;

	lti::fft fft2d;     // for 2-dimensional FFT
	lti::ifft ifft2d;   // for 2-dimensional inverse FFT

	// The real and imaginary parts of the image and filter
	lti::channel re,im;
	lti::channel reF,imF;

	cout << " ok 1 " << endl;

	fft2d.apply(imgToFilter,cordSys,re,im);

	cout << " ok 2 " << endl;

	fft2d.apply(kernel,cordSys,reF,imF);

	cout << " ok 3 " << endl;

	// The result of the multiply
	lti::channel rRe,rIm(imgToFilter.columns(),imgToFilter.rows(),0.0f);

	rRe.emultiply(re,reF);
	rIm.copy(im);
		
	lti::matrix<float> imgResult;

	ifft2d.apply(rRe,rIm,cordSys,imgResult);

        // shift back the result (in the space domain)
        for (int y=0;y<imgResult.rows();++y) {
          float s=lti::even(y) ? 1.f : -1.f;
          for (int x=0;x<imgResult.columns();++x) {
            imgResult.at(y,x)*=s;
            s*=-1.f;
          }
        }
      	
}

void  FilterImage::SetPadding(lti::matrix<float> imgToFilter, int kSize){

	// Set the padding
	const int n = lti::iround(lti::pow(2.0f,ceil(lti::log(2*lti::max(imgToFilter.rows(),
					imgToFilter.columns())+kSize-1)/lti::log(2.0f))));

	lti::eBoundaryType padding=lti::Zero;
	lti::boundaryExpansion::parameters bepar;
     	bepar.boundaryType = padding;

     	bepar.topBorder = 0;
     	bepar.leftBorder = 0;
     	bepar.bottomBorder = n-imgToFilter.rows();
     	bepar.rightBorder = n-imgToFilter.columns();

	lti::boundaryExpansion be(bepar);
      	be.apply(imgToFilter);

}


void FilterImage::SetPaddingKernel(lti::kernel2D<float> kernelToFilter, int kpSize){
	// Set the padding
	

	lti::eBoundaryType padding=lti::Zero;
	lti::boundaryExpansion::parameters bepar;
     	bepar.boundaryType = padding;

     	bepar.topBorder = (kpSize - kernelToFilter.rows()-1)/2;
     	bepar.leftBorder = (kpSize - kernelToFilter.rows()-1)/2;
     	bepar.bottomBorder = (kpSize + kernelToFilter.rows()-3)/2;
     	bepar.rightBorder = (kpSize + kernelToFilter.rows()-3)/2;

	lti::boundaryExpansion be(bepar);
      	be.apply(kernelToFilter);
}
