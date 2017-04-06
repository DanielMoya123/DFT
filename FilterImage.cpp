#include "FilterImage.h"

/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/
using namespace std;



/******************************************************
 * 
 * Constructor of the class
 * 
 * **********************************************/
FilterImage::FilterImage(void) {}

/******************************************************
 * 
 * Function to generate the Square Odd Gaussian Filter
 * 
 * **********************************************
 * 
 * Inputs: 
 * 	int kSize: the size of the filter
 * 	int variance: the variance of the gaussian function	
 * 	bool otoginal: if the filter is octogonal
 * 
 * Outputs:
 * 	lti::kernel2D<float> filter: the filter that response to the inputs
 * 
 * Restrinctions:
 * 	ksize > 0
 * 	variance > 0	
 *****/
lti::kernel2D<float> FilterImage::GenerateSquareOddGaussianFilter(int kSize, int variance, bool octagonal){ 
	
	// We create the kernel to response
	lti::kernel2D<float> kern;
	
	// We ask if the filter is octogonal or not
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
 * Function to generate a convolution square filter
 * 
 * **********************************************
 * 
 * Inputs: 
 * 	lti::kernel2D<float> kernel: The kernel
 *	lti::matrix<float> imgToFilter: The image to filter 
 * 
 * Outputs:
 * 	------------------	
 * 
 * Restrinctions:
 *      -----------------
 * 			
 *****/
lti::matrix<float>  FilterImage::ConvolutionSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter){
		
		// Create the convolution object
		lti::convolution convolutionO;
		lti::convolution::parameters param;
		// Set the kernel to convolute
		param.setKernel(kernel);
		// We do the convolution
		convolutionO.setParameters(param);
		convolutionO.apply(imgToFilter);

		return imgToFilter;
		
}

/******************************************************
 * 
 * The square filter in frequency
 * 
 * **********************************************
 * 
 * Inputs: 
 * 	lti::kernel2D<float> kernel: The kernel
 *	lti::matrix<float> imgToFilter: The image to filter 		
 * 
 * Outputs:
 * 	------------------	
 * 
 * Restrinctions:
 *	------------------
 * 			
 *****/
lti::matrix<float>  FilterImage::FreqSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter){

	// The size of the image
	int row = imgToFilter.rows();
	int col = imgToFilter.columns();

      	// shift the spectrum (in the space domain)
	for (int y=0;y<row;y++) {
        	float s=lti::even(y) ? 1.f : -1.f;
        	for (int x=0;x<col;x++) {
          		imgToFilter.at(y,x)*=s;
          		s*=-1.f;
        	}
      	}
	// end of the shift (in the space domain)


	// Transform to frequency spectrum
	static const lti::eCoordinateSystem cordSys = lti::Polar;

	//lti::fft fft2d;   // for 2-dimensional FFT
	//lti::ifft ifft2d; // for 2-dimensional inverse FFT

	lti::realFFT fft2d;       // for 2-dimensional FFT
  	lti::realInvFFT ifft2d;   // for 2-dimensional inverse FFT

	// The real and imaginary parts of the image and filter
	lti::channel re,im;
	lti::channel reF,imF;

	// Apply the cordSys to an image and filter
	fft2d.apply(imgToFilter,cordSys,re,im);
	fft2d.apply(kernel,cordSys,reF,imF);

	// The result of the multiply
	lti::channel rRe, rRe2, rIm(col,row,0.0f), rIm1(col,row,0.0f), rIm2(col,row,0.0f);

	// Multiply the real parts
	rRe.emultiply(re,reF);
	rRe2.emultiply(im,imF);

	rRe-= rRe2;


	//Multiply the imaginary parts
	rIm1.emultiply(re,imF);
	rIm2.emultiply(im,reF);

	rIm = rIm1 + rIm2;

	// As the gaussian function is a even function, they don't have 
	// imaginary part or is zero we ommitted.
	//rIm.copy(im);
		
	lti::matrix<float> imgResult;
	ifft2d.apply(rRe,rIm,cordSys,imgResult);

        // shift back the result (in the space domain)
        for (int y=0;y<row;++y) {
          float s=lti::even(y) ? 1.f : -1.f;
          for (int x=0;x<col;++x) {
            imgResult.at(y,x)*=s;
            s*=-1.f;
          }
        }

    return imgResult;
      	
}

/******************************************************
 * 
 * Function to set the padding in a image
 * 
 * **********************************************
 * 
 * Inputs: 
 *	lti::matrix<float> imgToFilter: The image to filter 
 *	int kSize: The size of the kernel		
 * 
 * Outputs:
 * 	------------------	
 * 
 * Restrinctions:
 *	------------------
 * 			
 *****/
void  FilterImage::SetPadding(lti::matrix<float> imgToFilter, int kSize){

	// Set the padding
	const int n = lti::iround(lti::pow(2.0f,ceil(lti::log(2*lti::max(imgToFilter.rows(),
					imgToFilter.columns())+kSize-1)/lti::log(2.0f))));

	// The type  of padding is zero
	lti::eBoundaryType padding=lti::Zero;
	lti::boundaryExpansion::parameters bepar;
     	bepar.boundaryType = padding;

	// We set where the image is with respect the padding
     	bepar.topBorder = 0;
     	bepar.leftBorder = 0;
     	bepar.bottomBorder = n-imgToFilter.rows();
     	bepar.rightBorder = n-imgToFilter.columns();

	// Apply the boundary expansion
	lti::boundaryExpansion be(bepar);
      	be.apply(imgToFilter);

}

/******************************************************
 * 
 * Function to set the padding in a image
 * 
 * **********************************************
 * 
 * Inputs: 
 *	lti::matrix<float> imgToFilter: The image to filter 
 *	int kpSize: The size of the kernel + the padding		
 * 
 * Outputs:
 * 	------------------	
 * 
 * Restrinctions:
 *	------------------
 * 			
 *****/
void FilterImage::SetPaddingKernel(lti::kernel2D<float> kernelToFilter, int kpSize){

	// Set the padding
	lti::eBoundaryType padding=lti::Zero;
	lti::boundaryExpansion::parameters bepar;
     	bepar.boundaryType = padding;

	// We set where the filter is with respect the padding
     	bepar.topBorder = (kpSize - kernelToFilter.rows()-1)/2;
     	bepar.leftBorder = (kpSize - kernelToFilter.rows()-1)/2;
     	bepar.bottomBorder = (kpSize + kernelToFilter.rows()-3)/2;
     	bepar.rightBorder = (kpSize + kernelToFilter.rows()-3)/2;

	lti::boundaryExpansion be(bepar);
      	be.apply(kernelToFilter);
}

/******************************************************
 * 
 * Function to clone the matrix
 * 
 * **********************************************
 * 
 * Inputs: 
 *	lti::matrix<float> img: The image to clone
 *	int nSize: The columns amount of the image
 *	int mSize: The rows amount of the image
 * 
 * Outputs:
 * 	------------------	
 * 
 * Restrinctions:
 *	------------------
 * 			
 *****/
lti::matrix<float> FilterImage::CloneMatrix(lti::matrix<float> img, int nSize, int mSize){

	// We create a matrix with this
	lti::matrix<float> imgD(nSize,mSize);

	// We copy all the values
	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < nSize; j++)
		{
			imgD.at(j,i) = img.at(j,i);
		}
	}
	
	return imgD;

}

double FilterImage::GetSquareError(lti::matrix<float> imgSpace, lti::matrix<float> imgFreq, int nSize, int mSize){
	double count = 0;
	double diff = 0;

	for (int i = 0; i < mSize; i++)
	{
		for (int j = 0; j < nSize; j++)
		{
			diff = imgSpace.at(j,i) - imgFreq.at(j,i);
			count+=lti::sqr(diff);
			cout << "val freq en " << i << " " << j << " es " << imgFreq.at(j,i) << endl;
			//cout << "count es " << count << endl;
		}
	}

	count = count/(nSize*mSize);

	return count;

}