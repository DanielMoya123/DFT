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
 * Definitions
 * 
 * ****************************/
const int K_MAX = 15;
const int K_MIN = 3;
const int N_MIN = 64;
const int N_MAX = 1920;
const int M_MIN = 48;
const int M_MAX = 1080;
const int K_STEP = 1;
const int N_STEP = 185;
const int M_STEP = 103;
const int NC_MIN = 1;
const int NC_MAX = 10;
 

/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
	
	cout << "usage: ./main \n" << endl;   
  
}

lti::matrix<float> cloneMatrix(lti::matrix<float> img, int nSize, int mSize){

	lti::matrix<float> imgD(nSize,mSize);

	for (int i = 0; i < mSize; i++)
		{
			for (int j = 0; j < nSize; j++)
			{
				imgD.at(j,i) = img.at(j,i);
			}
		}
	
	return imgD;

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
				img.at(j,i) = (int)(((float)rand()/(float)RAND_MAX)*255);
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

		lti::matrix<float> imgD;
		lti::ioPNG saver;

 		saver.save ("./imgD.png", img);
		lti::timer chron;
		cout << "inicio" << endl;
		/******************************
		2D FILTER LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			//cout << "ciclo i " << i << endl;
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			imgD = cloneMatrix(img,n_actual,m_actual);
			kSize = K_MAX + K_STEP;
			for (int j = 0; j < 10; j++)
			{
				//cout << "ciclo j " << j << endl;
				kSize -= K_STEP;
				
				variance = lti::sqr((kSize+2)/6);

				//cout << "variance " << variance << endl;
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filterController.GenerateSquareOddGaussianFilter(kSize,variance,false);
				n = (1- ( (float)(n_actual*m_actual + kSize*kSize)/(float)(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				
				cout << "n es " << n << endl;

				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{
					//cout << "ciclo k " << k << endl;

					//cout << "Kernel cols, rows " << dKernel.columns() << " " << dKernel.rows() << endl;
					//cout << "Img cols, rows " << imgD.columns() << " " << imgD.rows() << endl;

					chron.start();
		
					// Funciones para tomar el tiempo
					filterController.ConvolutionSquareFilter(dKernel,imgD);

					chron.stop();
					dataMatrixD[i][j] += (chron.getTime())/n;	

				}
			}
		}

		cout << "int size R ori " << imgD.rows() << endl;
		cout << "int size C ori " << imgD.columns() << endl;

		cout << "int size R ori " << img.rows() << endl;
		cout << "int size C ori " << img.columns() << endl;

		lti::matrix<float> imgG(N_MAX,M_MAX);
		kSize = K_MAX;

		/******************************
		OCTOGONAL LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			imgG = cloneMatrix(img,n_actual,m_actual);
			kSize = K_MAX;
			for (int j = 0; j < 10; j++)
			{
				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				// We create the octogonal kernel 
				lti::kernel2D<float> gKernel2D = filterController.GenerateSquareOddGaussianFilter(kSize,variance,true);

				n = (1- ( (float)(n_actual*m_actual + kSize*kSize)/(float)(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				
				cout << "n es " << n << endl;
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


		lti::matrix<float> imgF(N_MAX,M_MAX);
		kSize = K_MAX;

		/******************************
		FREQUENCY LOOP
		*******************************/
		for (int i = 0; i < 10; i++)
		{
			n_actual = N_MAX-(i*N_STEP);
			m_actual = M_MAX-(i*M_STEP);
			imgF = cloneMatrix(img,n_actual,m_actual);
			kSize = K_MAX;
			filterController.SetPadding(imgF, kSize-(K_STEP*(i+1)));
			

			
			for (int j = 0; j < 10; j++)
			{
				kSize -= K_STEP;
				variance = pow((kSize+2)/6, 2);
				
				// We create the 2D kernel 
				lti::kernel2D<float> dKernel = filterController.GenerateSquareOddGaussianFilter(kSize,variance,false);
				filterController.SetPaddingKernel(dKernel, imgF.rows());


				n = (1- ( (float)(n_actual*m_actual + kSize*kSize)/(float)(N_MAX*M_MAX + K_MAX*K_MAX) ) ) * (NC_MAX - NC_MIN) + NC_MIN;

				
				cout << "n es " << n << endl;
				// Calculate the new image and filter to get the n data
				for (int k = 0; k < n; k++)
				{

					chron.start();
		
					filterController.FreqSquareFilter(dKernel,imgF);

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
