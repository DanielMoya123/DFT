#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <math.h>

#include <ltiGaussKernels.h>
#include <ltiOctagonalKernel.h>
#include <ltiChannel.h>
#include <ltiConvolution.h>
#include <ltiMatrix.h>
#include <ltiKernel2D.h>
#include <ltiIFFT.h>
#include <ltiRound.h>
#include <ltiBoundaryExpansion.h>

#include <ltiIOPNG.h>

using namespace std;

class FilterImage
{
	public:
		FilterImage();
		lti::kernel2D<float> GenerateSquareOddGaussianFilter(int kSize,int variance,bool octagonal);
		lti::matrix<float> CloneMatrix(lti::matrix<float> img, int nSize, int mSize,int beginN, int beginM);
		lti::matrix<float>  ConvolutionSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter);
		lti::matrix<float>  FreqSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter,int oRow,int oCol);
		void  SetPadding(lti::matrix<float> *imgToFilter, int kSize);
		void  SetPaddingKernel(lti::kernel2D<float> *kernelToFilter, int kpSize);
		double GetSquareError(lti::matrix<float> imgSpace, lti::matrix<float> imgFreq);
	private:
		
		
};
