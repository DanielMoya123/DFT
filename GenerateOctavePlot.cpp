#include "GenerateOctavePlot.h"

/*****************************
 * NameSpaces
 * ****************************/
using namespace std;

/*****************************
 * Constants
 * ****************************/
const int SIZESQ = 10;


/******************************************************
 * 
 * Constructor of the class GenerateOctavePlot 
 * 
 * ************************************************/
GenerateOctavePlot::GenerateOctavePlot(void) {}


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
void GenerateOctavePlot::GenerateFileOfPlot(const char* plot,double **matrix,int values[]){
	
	ofstream myfile;
	myfile.open(plot);

	// The order of values {K_MIN,N_MIN,M_MIN,K_STEP,N_STEP,M_STEP};

	int kPix = values[0]*values[0];
	int kStep = values[3]*values[3];
	int iPix = values[1]*values[2];
	int iStep= values[4]*values[5];

	myfile << "xx = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = SIZESQ; j > 0; myfile << kPix+(kStep*i)+iPix+(iStep*(j--)) << ",");
		myfile << ";";
	}
	
	myfile << "]" << endl << "yy = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = SIZESQ; j > 0; myfile << kPix+(kStep*(j--))+iPix+(iStep*i) << ",");
		myfile << ";";
	}
	
	myfile << "]" << endl << "zz = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = 0; j < SIZESQ; myfile << matrix[i][j++] << ",");
		myfile << ";";
	}
	myfile << "]" << endl;
	
	myfile << "mesh(xx,yy,zz)";	
	myfile.close();
	
}
