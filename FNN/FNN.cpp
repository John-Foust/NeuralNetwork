// FNN.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>  
#include <time.h>  

using namespace std;
#define eta 0.01
#define numLayers 2
#define numNodes 2 // nodes per layer
#define numInputs 2
#define numOutputs 1 // if > numNodes, code needs reworked
#define RAND_MAX 101 // changing RandMax to 400 seems to make the values range within 0-100
#define weightFilePath "C:/Users/Me/Desktop/Current & Past Schooling & Work/Fall 2018 (Masters)/FNN practice (Allison Practice)/Practice Create FNN/weights.csv"
#define inputFilePath "C:/Users/Me/Desktop/Current & Past Schooling & Work/Fall 2018 (Masters)/FNN practice (Allison Practice)/Practice Create FNN/practice_data_torque.csv"
#define outputFilePath "C:/Users/Me/Desktop/Current & Past Schooling & Work/Fall 2018 (Masters)/FNN practice (Allison Practice)/FNN Outputs/output.csv"
#define zValOutputFilePath "C:/Users/Me/Desktop/Current & Past Schooling & Work/Fall 2018 (Masters)/FNN practice (Allison Practice)/FNN Outputs/zVals.csv"

class FNN
{
public:

	long double input[numInputs];
	long double W[4][numLayers][numNodes]; 
	long double z[numNodes];
	long double output[numOutputs];

/* W[0] - weights, W[0][layer][node]
   W[1] - output weights, W[output node][weights]
   W[2] - biases, W[2][0][biases]
   W[3] - output bias, W[3][0][biases]
*/

	// Constructors
	FNN()		// Assigns random weights and biases to the network
	{
		randWeights();
	}
	FNN(long double W1[4][numLayers][numNodes])	// Assigns weights and biases to network based on parameter sent. May not be necessary with new setup, here in case is needed
	{
		assignWeights(W1);
	}

	// Constructor helpers
	void randWeights() // assigns random decimal weights between 0 and 100
	{
		srand(time(NULL));
		for (int i = 0; i < 4; i++)	// for each matrix
			for (int j = 0; j < numLayers; j++)	// for each layer in a matrix
				for (int k = 0; k < numNodes; k++)	// for each node in a layer
				{
					W[i][j][k] = (double)(0 + rand() % RAND_MAX);  //create a random number for this location within the range of 0 - RAND_MAX(101), since 0-100 is 101 numbers
					cout << "Random Weight: " << W[i][j][k] << endl;
				}
	}
	void assignWeights(long double W1[4][numLayers][numNodes]) // assigns weights based on parameter 
	{
		for (int i = 0; i < 4; i++)	// for each matrix
			for (int j = 0; j < numLayers; j++)	// for each layer in a matrix
				for (int k = 0; k < numNodes; k++)	// for each node in a layer
				{
					W[i][j][k] = W1[i][j][k];
				}
	}

	// sigmoid function used in FNN and BP
	long double sigmoidFunc(long double x)
	{
		return 1 / (1 + exp(-1 * x));
	}
	long double partialDerivative(long double delta, long double output)
	{
		//can apply to hidden layer or output layer
		return delta * output; //partial derivative of E with respect to W of ij
	}
	long double totalError(long double actual, long double desired) {
		// combination of loss, absolute error, and square error
		return 0.5 * (abs(actual - desired) * abs(actual - desired));
	}

	// FNN calculation functions
	void calcz()
	{
		cout << endl << "calc z: " << endl;
		long double sum = 0;

		for (int i = 0; i < numNodes; i++)
		{
			sum = 0;

			for (int j = 0; j < numInputs; j++)
			{
				sum += W[0][j][i] * input[j];
				cout << "W[0][j][i] : " << W[0][j][i] << " input of j: " << input[j] << endl;

			}
			cout << endl << "z:    ";
			sum += W[2][0][i];  // bias of node i
			z[i] = sigmoidFunc(sum);

			cout << "Z: " << z[i] << endl;
		}
	}
	void calcy()
	{
		cout << endl << "calc y: " << endl;

		long double	 sum = 0;
		// this function needs work
		for (int i = 0; i < numOutputs; i++)
		{
			sum = 0;
			for (int j = 0; j < numNodes; j++)
			{
				sum += W[1][i][j] * z[j]; // output weights * z value
				cout << "W[1][i][j] " << W[1][i][j] << "   z[i] : " << z[i] << endl;
			}

			output[i] = sigmoidFunc(sum + W[3][0][i]); //changed 'weightData[3][0]' to 'W[4][0]'
			cout << "y = output [i] = " << output[i] << endl;
		}

	}

	// BP calculation functions
	long double deltaOutput(long double received, long double target) {
		return received * (1-received) * (received - target);
	}
	long double deltaHidden(int j, long double z, long double W1[4][numLayers][numNodes]) {
		int sum = 0;
		sum += W1[1][j][k];

		for (int j = 0; j < numLayers; j++) {
			for (int k = 0; k < numNodes; k++)
			{
				sum += (deltaOutput(W[1][j][k],9))*(W[1][j][k]); //multiply the output weight by the y value to get to the Hidden Layer
			}
		}
			
		return z * (1 - z)*sum;
	}

	// Serves as FNN driver
	void getOutput()
	{
		calcz();
		calcy();
	}
};

int parseWeightsAndBiases(long double WParameters[4][numLayers][numNodes]);
int FNNTest();

int main()
{
	FNN network;

	FNNTest();	// Tests FNN using data torque inputs and weights. Does not use the BP.

}

int  parseWeightsAndBiases(long double WParameters[4][numLayers][numNodes])
{
	string row;

	// open weights file
	ifstream weightsbias(weightFilePath);
	if (!weightsbias.is_open()) {
		cout << "Weight file could not be found." << endl;		//will return if the file cannot be found or opened
		return 0;
	}

	/* W[0] - weights, W[0][layer][node]
	   W[1] output weights, W[1][0][weights]
	   W[2] - biases, W[2][0][biases]
	   W[3] - output bias, W[3][0][biases]
	*/

	// initialize biases
	for (int i = 0; i < numNodes; i++)
	{
		getline(weightsbias, row, ',');

		WParameters[2][0][i] = stold(row);

	}

	// initialize weights
	for (int i = 0; i < numLayers; i++)
	{
		for (int j = 0; j < numNodes; j++)
		{

				getline(weightsbias, row, ',');

			WParameters[0][i][j] = stold(row);
		}
	}

	// initialize output biases
	for (int i = 0; i < numOutputs; i++)
	{
			getline(weightsbias, row, ',');

		WParameters[3][0][i] = stold(row);
	}

	// initialize output weights
	for(int i = 0; i < numOutputs; i++)
	{
		for (int j = 0; j < numNodes; j++)
		{
			getline(weightsbias, row, ',');
			WParameters[1][i][j] = stold(row);
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		cout << endl;
		for (int j = 0; j < numLayers; j++)
		{
			for (int k = 0; k < numNodes; k++)
				cout << WParameters[i][j][k] << "  |  ";
			cout << endl;
		}
	}
	cout << endl;
}

// returns 0 if file could not be opened, 1 if the test successfully runs.
int FNNTest()
{
	long double WParam[4][numLayers][numNodes] = { 0 };
	/*	W[0] - weights, W[0][layer][node]
		W[1] - output weights, W[output node][weights]
		W[2] - biases, W[2][0][biases]
		W[3] - output bias, W[3][0][biases]
	*/

	// Set up FNN to be called
	parseWeightsAndBiases(WParam);
	FNN network(WParam);

	// Open Files
	ifstream datatorque(inputFilePath);	
	ofstream outfile(outputFilePath);
	ofstream outfile2 (zValOutputFilePath);
	if (!datatorque.is_open() || !outfile.is_open() || !outfile2.is_open()) {
		cout << "File could not be found in FNNTest" << endl;
		return 0;
	}

	string row;	// used to store lines from input file
	getline(datatorque, row);		//this will SKIP the first row with the headers
	
	// Parsing through the input data
	while (getline(datatorque, row))
	{
		// Set up input array
		for (int i = 0; i < numInputs; i++)
		{
		//	cout << i << endl;
			if (i == numInputs - 1)
			{
				network.input[i] = stold(row);
			}
			else
			{
				auto spot = row.find(',');
				network.input[i] = stold(row.substr(0, spot));
				row = row.substr(spot+1, row.length());
			}
		//	cout << " row: " << row << endl;
		}
		
		network.getOutput();
			
		for (int o = 0; o < numOutputs; o++)
		{
			outfile << network.output[o];
		}

		for (int z = 0; z < numNodes; z++)
		{
			outfile2 << network.z[z];
		}

		outfile << endl;
		outfile2 << endl;
	}

	// Close files
	datatorque.close();
	outfile.close();
	outfile2.close();
}