#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <math.h>

#include <ltiGaussKernels.h>

#include <ltiBoundaryExpansion.h>
#include "ltiFFT.h"
#include "ltiIFFT.h"

using namespace std;
using namespace lti;

class FilterImage
{
	public:
		FilterImage();
		image GenerateSquareOddGaussianFilter(int size,string path);
		void  ConvolutionSquareFilter2D(int size,image Gaussian,image imgToFilter);
		void  ConvolutionSquareFilterOctogonal(int size,image Gaussian,image imgToFilter);
		void  SpaceSquareFilter(int size,string pathGaussian,image Gaussian,image imgToFilter);
	private:
		
};
