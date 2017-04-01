/*****************************
 * 
 * Libraries
 * 
 * ****************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>

#include <ltiTimer.h>
#include <ltiImage.h>

#include "FilterImage.h"
#include "GenerateOctavePlot.h"


/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/
using namespace std;


/*****************************
 * 
 * Definitions
 * 
 * ****************************/
#define KMAX 1023
#define KMIN 3
#define N_MIN 64
#define N_MAX 1920
#define M_MIN 48
#define M_MAX 1080
 

/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
	
	cout << "usage: ./main \n" << endl;   
  
}

/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{
	
	if (argc > 1 && (std::string(argv[1])=="-h" || std::string(argv[1])=="--help")) {
		
		help();
		
	}else{
		
		// The paths to save the images from the processing
		string pathImg = "./imageToProcess.png";
		string pathFilter = "./Guassfilter.png";

		double **dataMatrix;
		dataMatrix = new double*[10];
		for(int i = 0; i <10; i++)

			dataMatrix[i] = new double[10];
		
		int u = 100, v = 100;
		int kSize = 3;
		
		lti::matrix<float> img(u,v);
		
		for (int i = 0; i < u; i++)
		{
			for (int j = 0; j < v; j++)
			{
				img.at(i,j) = (int)(((float)rand()/(float)RAND_MAX)*255);
			}
		}
		
		// We get the size in x or y of the filter and set the kSize
		double variance = pow((kSize+2)/6, 2);
		
		// *************************************  
		// We create the filter
		// ************************************* 
		FilterImage filterController = FilterImage();
		
		// We create the 2D kernel 
		lti::kernel2D<float> dKernel = filterController.GenerateSquareOddGaussianFilter(kSize,variance,false);
		// We create the octogonal kernel 
		lti::kernel2D<float> gKernel2D = filterController.GenerateSquareOddGaussianFilter(kSize,variance,true);
		
		int n = 0;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				
				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					lti::timer chron;
					chron.start();
		
					// Funciones para tomar el tiempo
					filterController.ConvolutionSquareFilter(dKernel,img);

					chron.stop();
					dataMatrix[i][j] =  (dataMatrix[i][j] + chron.getTime())/2;
				}
			}
		}
		
		GenerateOctavePlot meshPlot = GenerateOctavePlot();
		
		meshPlot.GenerateFileOfPlot("./filter.m",dataMatrix);

		return EXIT_SUCCESS;
	}
	
	return EXIT_FAILURE;
}
