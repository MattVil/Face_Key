#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "load_mnist.h"

#define EPS 0.2

#define NB_LAYOR 4

#define DROPOUT 0.2

#define NB_NEURON_INPUT 784
#define NB_NEURON_HIDDEN 800
#define NB_NEURON_OUTPUT 10


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

typedef struct network{
	Layor* tab_layor;
	int nb_layor;
	Weight_matrix* tab_weight_matrix;
	int nb_weight_matrix;
}Network;

Network build_neural_network();
void build_dropout_vect(Network* net);
double* softMax(double* score);
double train_network(Network* network, Image* img);
void put_img_in_input(Network* network, Image* img);
void compute_output(Network* network);
double compute_error(Network* network, Image img);
void backpropagation(Network* network);
double* convert_label(Image img);
int convert_result(Network network);
