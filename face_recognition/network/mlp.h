#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct neuron{
	double potential;
	double value;
	double error_average;
	double error_signal;
}Neuron;

typedef struct layor{
  int nb_neuron;
  Neuron* tab_neuron;
  Neuron bias;

  float dropout;
  int* dropVect;
}Layor;

typedef struct weight_matrix{
	int size_in;
  Layor* input;
  int size_out;
  Layor* output;

	double** weight;
}Weight_matrix;

typedef struct network
{
	Layor* tab_layor;
	int nb_layor;
	Weight_matrix* tab_weight_matrix;
	int nb_weight_matrix;
}Network;
