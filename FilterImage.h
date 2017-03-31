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
#include <ltiBoundaryExpansion.h>
#include "ltiFFT.h"
#include "ltiIFFT.h"
#include <ltiImage.h>
#include <ltiChannel.h>
#include <ltiConvolution.h>
#include <ltiMatrix.h>
#include <ltiKernel2D.h>

using namespace std;

class FilterImage
{
	public:
		FilterImage();
		lti::kernel2D<float> GenerateSquareOddGaussianFilter(int kSize,int variance,bool octagonal);
		void  ConvolutionSquareFilter(lti::kernel2D<float> kernel,lti::matrix<float> imgToFilter);
		void  SpaceSquareFilter(lti::kernel2D<float> kernel,lti::image imgToFilter);
	private:
		
};
