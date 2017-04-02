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
#define K_MAX 1023
#define K_MIN 3
#define N_MIN 64
#define N_MAX 1920
#define M_MIN 48
#define M_MAX 1080
#define K_STEP 102
#define N_STEP 185
#define M_STEP 103
#define NC_MIN 10
#define NC_MAX 100
 

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

		double **dataMatrixD;
		double **dataMatrixG;
		double **dataMatrixF;
		dataMatrixD = new double*[10];
		dataMatrixG = new double*[10];
		dataMatrixF = new double*[10];
		for(int i = 0; i <10; i++)
		{
			dataMatrixD[i] = new double[10];
			dataMatrixG[i] = new double[10];
			dataMatrixF[i] = new double[10];
		}
		
		lti::matrix<float> img(N_MAX,M_MAX);
		
		for (int i = 0; i < M_MAX; i++)
		{
			for (int j = 0; j < N_MAX; j++)
			{
				img.at(i,j) = (int)(((float)rand()/(float)RAND_MAX)*255);
			}
		}
		
		// We get the size in x or y of the filter and set the kSize
		double variance = 0;
		
		
		// *************************************  
		// We create the filter
		// ************************************* 
		FilterImage filterController = FilterImage();
		
		int kSize = K_MAX;
		
		double n = 0;
		int n_actual = 0;
		int m_actual = 0;

		lti::matrix<float> imgD(N_MAX,M_MAX);

		lti::timer chron;

		/******************************
		2D FILTER LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			imgD = img.copy(imgD, 0, n_actual, 0, m_actual);
			kSize = K_MAX;
			for (int j = 0; j < 10; j++)
			{
				kSize -= K_STEP;
				
				variance = pow((kSize+2)/6, 2);
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filterController.GenerateSquareOddGaussianFilter(kSize,variance,false);

				n = ( 1-( (n_actual*m_actual + kSize*kSize)/(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					
					chron.start();
		
					// Funciones para tomar el tiempo
					filterController.ConvolutionSquareFilter(dKernel,imgD);

					chron.stop();
					dataMatrixD[i][j] += (chron.getTime())/n;	

				}
			}
		}



		lti::matrix<float> imgG(N_MAX,M_MAX);
		kSize = K_MAX;

		/******************************
		OCTOGONAL LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			n_actual = N_MAX-i*N_STEP;
			m_actual = M_MAX-i*M_STEP;
			imgG = img.copy(imgG, 0, n_actual, 0, m_actual);
			kSize = K_MAX;
			for (int j = 0; j < 10; j++)
			{
				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				// We create the octogonal kernel 
				lti::kernel2D<float> gKernel2D = filterController.GenerateSquareOddGaussianFilter(kSize,variance,true);

				n = ( 1-( (n_actual*m_actual + kSize*kSize)/(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{

					chron.start();
		
					// Funciones para tomar el tiempo
					filterController.ConvolutionSquareFilter(gKernel2D,imgG);

					chron.stop();
					dataMatrixG[i][j] += (chron.getTime())/n;
				}
			}
		}


		lti::fft fft2d;     // for 2-dimensional FFT
		lti::matrix<float> imgF(N_MAX,M_MAX);
		lti::channel reK, imK; // real and imaginary part of kernel
		lti::channel reI, imI; // real and imaginary part of image
		kSize = K_MAX;

		/******************************
		FREQUENCY LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			n_actual = N_MAX-i*N_STEP;
			m_actual = M_MAX-i*M_STEP;
			imgG = img.copy(imgF, 0, n_actual, 0, m_actual);
			kSize = K_MAX;	
			for (int j = 0; j < 10; j++)
			{
				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				
				lti::matrix<float> imgF = filterController.GetPadded(imgF, kSize);
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filterController.GenerateSquareOddGaussianFilter(kSize,variance,false);

				//Transform to frequency domain
				fft2d.apply(imgF, reI, imI);
				fft2d.apply(dKernel, reK, imK);

				n = ( 1-( (n_actual*m_actual + kSize*kSize)/(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{

					chron.start();
		
					//CORRECT MATRIX MULTIPLICATION MISSING !

					chron.stop();
					dataMatrixF[i][j] += (chron.getTime())/n;
				}
			}
		}
	
		
		int data[] = {K_MIN,N_MIN,M_MIN,K_STEP,N_STEP,M_STEP};
		
		GenerateOctavePlot meshPlot = GenerateOctavePlot();
		
		meshPlot.GenerateFileOfPlot("./filterD.m",dataMatrixD,data);
		meshPlot.GenerateFileOfPlot("./filterG.m",dataMatrixG,data);
		meshPlot.GenerateFileOfPlot("./filterF.m",dataMatrixF,data);

		return EXIT_SUCCESS;
	}
	
	return EXIT_FAILURE;
}
