/*****************************
 * 
 * Libraries
 * 
 * ****************************/
#include <boost/program_options.hpp>

#include <ltiTimer.h>


/*****************************
 * 
 * NameSpaces
 * 
 * ****************************/
 namespace po = boost::program_options;
 

/******************************************************
 * 
 * Help Function
 * 
 * **********************************************/
void help()  {
	cout << "usage: \n" << endl;   
  
	cout << "usage: Campbell-Robson [option] [N] [option] [N] ... \n" << endl;  
	cout << "       -r|--rows <M>     the number of rows" 			<< endl;
	cout << "       -c|--columns <N>  the number of columns" 			<< endl;  
	cout << "       -l|--lineal       if the use what linear scale" 	<< endl;  
	cout << "       -x|--rowShow <N>  the number of row to show" 		<< endl;  
	cout << "       -h|--help        show this help\n"	<< endl; 
  
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
		("robots", po::value<int>()->default_value(3), 
		"How many robots do you want to send on a murderous rampage?");

		po::variables_map opts;
		po::store(po::parse_command_line(argc, argv, desc), opts);

		try {
			po::notify(opts);
		} catch (std::exception& e) {
			std::cerr << "Error: " << e.what() << "\n";
			return 1;
		}

		int nRobots = opts["robots"].as<int>(); 
		// automatically assigns default when option not supplied by user!!

		std::cout << nRobots << " robots have begun the silicon revolution" << std::endl;

		// End example

		lti::timer chron;

		chron.start();

		lti::loadImage loader; // functor to load images
		lti::image img;        // the image loaded will be left here.

		loader.load("Lenna.png",img);  // load PNG image

		lti::channel  imgGray;
		imgGray.castFrom(img);    

		double variance;

		// kSize NEEDS TO BE DEFINED AS AN ENTRY PARAMETER
		variance = pow((kSize+2)/6, 2)

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
