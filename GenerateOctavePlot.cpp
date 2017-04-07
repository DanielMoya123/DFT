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
 * Generate the file to plot in octave
 * 
 * **********************************************
 * 
 * Inputs: 
 * 		const char* path: The path of the file to plot
 *		double **matrix: The matrix with the values in time
 *		int values[]: The data of the kernel and image
 *			 The order of values {K_MIN,N_MIN,M_MIN,K_STEP,N_STEP,M_STEP};
 * 
 * Outputs:
 * 		-------
 * 
 * Restrinctions:
 * 		-------
 *****/
void GenerateOctavePlot::GenerateFileOfPlot(const char* path,double matrix[][10],int values[]){
	
	// We open the file
	ofstream myfile;
	myfile.open(path);

	// Get the square values 
	int kPix = values[0]*values[0];
	int kStep = values[3]*values[3];
	int iPix = values[1]*values[2];
	int iStep= values[4]*values[5];
	
	// We write the x values
	myfile << "xx = [";
	for (int i = SIZESQ; i > 0; i--)
	{
		for (int j = 0; j < SIZESQ; j++){
			myfile << kPix+(kStep*i) << ",";
		}
		myfile << ";";
	}
	
	// We write the y values
	myfile << "]" << endl << "yy = [";
	for (int i = 0; i  < SIZESQ; i++)
	{
		for (int j = SIZESQ; j > 0; myfile << iPix+(iStep*(j--)) << ",");
		myfile << ";";
	}
	
	// We write the z values
	myfile << "]" << endl << "zz = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = 0; j < SIZESQ; myfile << matrix[i][j++] << ",");
		myfile << ";";
	}
	myfile << "]" << endl;
	
	// We write the mesh function in the file and close it
	myfile << "mesh(xx,yy,zz)";	
	myfile.close();
	
}
