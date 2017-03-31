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
void GenerateOctavePlot::GenerateFileOfPlot(const char* plot,double **matrix){
	
	ofstream myfile;
	myfile.open(plot);
	
	myfile << "xx = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = 0; j < SIZESQ; myfile << matrix[i][j++] << ",");
		myfile << ";";
	}
	
	myfile << "]" << endl << "yy = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = 0; j < SIZESQ; myfile << (i + j++) << ",");
		myfile << ";";
	}
	
	myfile << "]" << endl << "zz = [";
	for (int i = 0; i < SIZESQ; i++)
	{
		for (int j = 0; j < SIZESQ; myfile << (i + j++) << ",");
		myfile << ";";
	}
	myfile << "]" << endl;
	
	myfile << "mesh(xx,yy,zz)";	
	myfile.close();
	
}
