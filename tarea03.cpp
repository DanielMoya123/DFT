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

#include <ltiIOPNG.h>
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
 * Constants
 * 
 * ****************************/
const int K_MAX = 15;
const int K_MIN = 5;
const int N_MIN = 64;
const int N_MAX = 1920;
const int M_MIN = 48;
const int M_MAX = 1080;
const int K_STEP = 1;
const int N_STEP = 185;
const int M_STEP = 103;
const int NC_MIN = 1;
const int NC_MAX = 10;

const int SIZESQ = 10;
 

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

		

		// We initialize the matrix to save the data
		double dataMatrix[10][10];

		for (int i = 0; i < SIZESQ; i++)
		{
			for (int j = 0; j < SIZESQ; j++)
			{
				dataMatrix[i][j] = 0;
			}
		}
		
		// We init the image with random numbers
		lti::matrix<float> img(N_MAX,M_MAX);

		// Image to get the error
		lti::matrix<float> imgSpa;
		lti::matrix<float> imgFre;
		
		// We fill the matrix with random numbers
		for (int i = 0; i < M_MAX; i++)
		{
			for (int j = 0; j < N_MAX; j++)
			{
				img.at(j,i) = (int)(((float)rand()/(float)RAND_MAX)*255);
			}
		}
		
		// We get the size in x or y of the filter and set the kSize
		double variance = 0;
		
		
		// *************************************  
		// We create the filter controller
		// ************************************* 
		FilterImage filCont = FilterImage();
		// *************************************  
		// We create the plot controller
		// ************************************* 
		GenerateOctavePlot plotCont = GenerateOctavePlot();
		
		// Set the variables to use in the functions
		int kSize = K_MAX;
		int data[] = {K_MIN,N_MIN,M_MIN,K_STEP,N_STEP,M_STEP};		

		// Init the counters
		double n = 0;
		int n_actual = 0, m_actual = 0;
		
		// Save the main image
		lti::ioPNG saver;
 		saver.save ("./imgOri.png", img);
		lti::timer chron;

		cout << "Start" << endl;
		cout << "Start 2D Filter Loop" << endl;
		/******************************
		2D FILTER LOOP
		*******************************/
		for (int i = 0; i < SIZESQ; i++)
		{

			// Init the values of the image and values
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			lti::matrix<float> imgD = filCont.CloneMatrix(img,n_actual,m_actual,0,0);
			kSize = K_MAX + K_STEP;
			
			for (int j = 0; j < SIZESQ; j++)
			{
				
				kSize -= K_STEP;
				variance = lti::sqr((kSize+2)/6);
				float numberator = (float)(n_actual*m_actual + kSize*kSize);
				float denominator = (float)(N_MAX*M_MAX + K_MAX*K_MAX);
				n = (1-(numberator/denominator))*(NC_MAX-NC_MIN)+NC_MIN;
				
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filCont.GenerateSquareOddGaussianFilter(kSize,variance,false);

				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					// We take the time to execute this
					chron.start();
					imgSpa = filCont.ConvolutionSquareFilter(dKernel,imgD);
					chron.stop();
					dataMatrix[i][j] += (chron.getTime())/n;	

				}
			}
		}
		// Plot the first data
		plotCont.GenerateFileOfPlot("./filterD.m",dataMatrix,data);

		
		cout << "Start Octagonal Loop" << endl;
		/******************************
		OCTOGONAL LOOP
		*******************************/
		/******************************
		for (int i = 0; i < SIZESQ; i++)
		{

			// Init the values of the image and values
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			lti::matrix<float> imgG = filCont.CloneMatrix(img,n_actual,m_actual,0,0);
			kSize = K_MAX + K_STEP;

			for (int j = 0; j < SIZESQ; j++)
			{

				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				float numberator = (float)(n_actual*m_actual + kSize*kSize);
				float denominator = (float)(N_MAX*M_MAX + K_MAX*K_MAX);
				n = (1-(numberator/denominator)) * (NC_MAX-NC_MIN) + NC_MIN;

				// We create the 2D kernel 
				lti::kernel2D<float> gKernel2D = filCont.GenerateSquareOddGaussianFilter(kSize,variance,true);
				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					chron.start();
					filCont.ConvolutionSquareFilter(gKernel2D,imgG);
					chron.stop();
					dataMatrix[i][j] += (chron.getTime())/n;
				}
			}
		}
		// Plot the second data
		plotCont.GenerateFileOfPlot("./filterG.m",dataMatrix,data);
		*******************************/

		cout << "Start Frenquency Loop" << endl;
		/******************************
		FREQUENCY LOOP
		*******************************/
		for (int i = 0; i < SIZESQ; i++)
		{
			// Init the values of the image and values
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			kSize = K_MAX + K_STEP;
			lti::matrix<float> imgF = filCont.CloneMatrix(img,n_actual,m_actual,0,0);
			filCont.SetPadding(&imgF, kSize-(K_STEP*(i+1)));
			
			for (int j = 0; j < SIZESQ; j++)
			{
				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				float numberator = (float)(n_actual*m_actual + kSize*kSize);
				float denominator = (float)(N_MAX*M_MAX + K_MAX*K_MAX);
				n = (1-(numberator/denominator)) * (NC_MAX-NC_MIN) + NC_MIN;
				
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filCont.GenerateSquareOddGaussianFilter(kSize,variance,false);
				filCont.SetPaddingKernel(&dKernel, imgF.rows());
				
				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					chron.start();
					imgFre = filCont.FreqSquareFilter(dKernel,imgF,n_actual,m_actual);
					chron.stop();
					dataMatrix[i][j] += (chron.getTime())/n;
				}
			}
		}
		// Plot the third data
		plotCont.GenerateFileOfPlot("./filterF.m",dataMatrix,data);

		//Get the square difference error
		double sError;

		cout << "rows of space " << imgSpa.rows() << endl;
		cout << "cols of space " << imgSpa.columns() << endl;

		cout << "rows of freq " << imgFre.rows() << endl;
		cout << "cols of freq " << imgFre.columns() << endl;

		saver.save ("./imgSpa.png", imgSpa);
		saver.save ("./imgFre.png", imgFre);

		sError = filCont.GetSquareError(imgSpa, imgFre);

		cout << "Mean squared error: " << sError << endl;

		return EXIT_SUCCESS;
	}
	
	return EXIT_FAILURE;
}
