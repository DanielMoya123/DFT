/*****************************
 * 
 * Libraries
 * 
 * ****************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <math.h>

#include <boost/program_options.hpp>

#include <ltiTimer.h>



/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/
using namespace std;
 namespace po = boost::program_options;
 

/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
	
	cout << "usage: main [option] [N] [option] [N] ... \n" << endl;  
	cout << "       -pi|--pathimage <Path>   the path of the image" << endl;
	cout << "       -pf|--pathfilter <Path>  the path of the filter" << endl;  
  
}



/******************************************************
 * 
 * The main function
 * 
 * **********************************************/
int main(int argc, char *argv[])
{

	// 64×48 pixeles y 1920×1080 pixeles
	// 10 valores intermedios
	
	// 3×3 pixeles y 1023×1023 pixeles
	// 10 valores intermedios
	
	// Generar una tabla con 100 campos del cruze de esos datos
	// Tomar n muestras de cada funcion

	// Example of how to use boost program options
	if (argc > 1) {
		
		// If the user is asked about info
		if (std::string(argv[1])=="-h" || std::string(argv[1])=="--help") {
			help();
			return EXIT_SUCCESS;
		}

		po::options_description desc("Usage");
		desc.add_options()
		("pathimage", po::value<string>()->default_value("./Lenna.png"), 
		"Where is the path of the image?");
		desc.add_options()
		("pathfilter", po::value<string>()->default_value("./filter.png"), 
		"Where is the path of the filter?");

		po::variables_map opts;
		po::store(po::parse_command_line(argc, argv, desc), opts);

		try {
			po::notify(opts);
		} catch (std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
			return EXIT_FAILURE;
		}

		string pathImg = opts["pathimage"].as<string>();
		string pathFilter = opts["pathfilter"].as<string>();

		// End example
		lti::timer chron;
		chron.start();

		/* A GRAY IMAGE IS GOING TO BE GENERATED
		lti::loadImage loader; 
		lti::image img;

		// Load PNG image
		loader.load("Lenna.png",img); 

		lti::channel  imgGray;
		imgGray.castFrom(img);  */
		
		// *************************************  
		// We create the filter
		// *************************************  


		// We get the size in x or y of the filter and set the kSize
		double variance = pow((kSize+2)/6, 2);

		//Guassian kernel
		lti::gaussKernel2D<lti::channel::value_type> gKernel(kSize,variance); 
		lti::convolution gFilter;                        // convolution operator
		lti::convolution::parameters gParam;             // parameters
		gParam.setKernel(gKernel);                        // use the gauss kernel
		gFilter.setParameters(gParam);                    // set parameters
		gFilter.apply(imgGray);

		//Octogonal kernel
		lti::octagonalKernel<lti::channel::value_type> oKernel(kSize,variance); 
		lti::convolution oFilter;                        // convolution operator
		lti::convolution::parameters oParam;             // parameters
		oParam.setKernel(oKernel);                        // use the ortogonal kernel
		oFilter.setParameters(oParam);                    // set parameters
		oFilter.apply(imgGray);

		//In frequency domain

		// NEED TO APPLY BOUNDARY EXPANSION

		lti::fft fft2d;     // for 2-dimensional FFT
		lti::ifft ifft2d;   // for 2-dimensional inverse FFT


		/* NOT SURE WHY */
		par2d.mode = lti::fft::Polar;
		ifft2d.setParameters(par2d);
		fft2d.setParameters(par2d);
		/**************************/

		lti::channel back;
		lti::channel re,im;

		fft2d.apply(R, re, im);       // the actual FFT, Parameters: source, real result part, imaginary result part
		ifft2d.apply(re, im, back);   // inverse FF, Parameters: real part, imaginary part, destination


		// Multiply 2 channels
		lti::channel  result;

		result = lti::matrix< T >::emultiply(a,b )//not sure if so for channel instead of matrix

		chron.stop();


		cout << "Duration: " << chron.getTime() << " microseconds\n";

		return EXIT_SUCCESS;
	}
	
	return EXIT_FAILURE;
}
