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
#include <ltiImage.h>

using namespace std;

class FilterImage
{
	public:
		FilterImage();
		lti::image GenerateSquareOddGaussianFilter(int size,string path);
		void  ConvolutionSquareFilter2D(int size,lti::image Gaussian,lti::image imgToFilter);
		void  ConvolutionSquareFilterOctogonal(int size,lti::image Gaussian,lti::image imgToFilter);
		void  SpaceSquareFilter(int size,string pathGaussian,lti::image Gaussian,lti::image imgToFilter);
		lti::channel GenerateRandomImage(int sizeX, int sizeY);
	private:
		
};
