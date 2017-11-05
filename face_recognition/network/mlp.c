#include "mlp.h"

#define DEBUG 1

/*
This function build a neural network depending of the #define on network.h
*/
Network build_neural_network(){

	if(NB_LAYOR < 2)
		exit(0);

	if(DEBUG){
		printf("\n---------------------------------------------------------------\n");
		printf("                  Start building neural network ...              \n");
	}
	//network creation
	Network network;
	network.nb_layor = NB_LAYOR;
	network.tab_layor = malloc(NB_LAYOR * sizeof(Layor));
	network.nb_weight_matrix = NB_LAYOR-1;
	network.tab_weight_matrix = malloc((NB_LAYOR-1) * sizeof(Weight_matrix));

	int k;
	//layors creation
	if(DEBUG)
  	printf("Layor creation :\n");

	for(k=0; k<NB_LAYOR; k++){
		//creation of the inputLayor
		if(k == 0){
			Layor inputLayor;
			inputLayor.nb_neuron = NB_NEURON_INPUT;
			inputLayor.tab_neuron = malloc(NB_NEURON_INPUT * sizeof(Neuron));
      inputLayor.dropout = DROPOUT;
      inputLayor.dropVect = malloc(NB_NEURON_INPUT * sizeof(int));

			int i;
			for(i=0; i<NB_NEURON_INPUT; i++){
				Neuron neuron;
				neuron.potential = 0;
				neuron.value = 0;
				neuron.error_average = 0;

				inputLayor.tab_neuron[i] = neuron;

        inputLayor.dropVect[i] = 1;
			}
			network.tab_layor[k] = inputLayor;
		}
		//creation of the ouputLayor
		else if(k == NB_LAYOR-1){
			Layor outputLayor;
			outputLayor.nb_neuron = NB_NEURON_OUTPUT;
		  outputLayor.tab_neuron = malloc(NB_NEURON_OUTPUT * sizeof(Neuron));
      outputLayor.dropout = 0;
      outputLayor.dropVect = malloc(NB_NEURON_OUTPUT * sizeof(int));

			int i;
			for(i=0; i<NB_NEURON_OUTPUT; i++){
				Neuron neuron;
				neuron.potential = 0;
				neuron.value = 0;
				neuron.error_average = 0;

				outputLayor.tab_neuron[i] = neuron;

        outputLayor.dropVect[i] = 1;
			}
			network.tab_layor[k] = outputLayor;
		}
		//creation of the hidden layor(s)
		else{
			Layor hiddenLayor;
			hiddenLayor.nb_neuron = NB_NEURON_HIDDEN;
			hiddenLayor.tab_neuron = malloc(NB_NEURON_HIDDEN * sizeof(Neuron));
      hiddenLayor.dropout = DROPOUT;
      hiddenLayor.dropVect = malloc(NB_NEURON_HIDDEN * sizeof(int));

			int i;
			for(i=0; i<NB_NEURON_HIDDEN; i++){
				Neuron neuron;
				neuron.potential = 0;
				neuron.value = 0;
				neuron.error_average = 0;

				hiddenLayor.tab_neuron[i] = neuron;

        hiddenLayor.dropVect[i] = 1;
			}
			network.tab_layor[k] = hiddenLayor;
		}
		if(DEBUG)
			printf("\tLayor %d created : %d\n", k, network.tab_layor[k].nb_neuron);
	}

	//creation of connexion matrix table
	if(DEBUG)
		printf("Weight matrix creation :\n");
	int j;
	srand(time(NULL));

	for(j=0; j<NB_LAYOR-1; j++){
		Weight_matrix matrix;
		matrix.input = &(network.tab_layor[j]);
		matrix.size_in = network.tab_layor[j].nb_neuron;
		matrix.output = &(network.tab_layor[j+1]);
		matrix.size_out = network.tab_layor[j+1].nb_neuron;

		matrix.weight = malloc(matrix.size_in*sizeof(double));
		if(matrix.weight == NULL)
			exit(1);

		int i;
		for(i=0; i<matrix.size_in; i++){
			matrix.weight[i]=malloc(matrix.size_out*sizeof(double));
		}
		int n, m;
		for(n=0; n<matrix.size_in; n++){
			for(m=0; m<matrix.size_out; m++){
				matrix.weight[n][m] = ((double)rand() /(double)RAND_MAX)*2 -1;
			}
		}

		network.tab_weight_matrix[j] = matrix;
		if(DEBUG)
			printf("\tWeight matrix %d created : %dx%d\n", j, network.tab_weight_matrix[j].size_in, network.tab_weight_matrix[j].size_out);
	}

	if(DEBUG){
		printf("\nNetwork creation DONE !\n");
		printf("---------------------------------------------------------------\n");
	}

	return network;
}


/*
This function build the dropout vector wich will be rebuild after each backprop
*/
void build_dropout_vect(Network* net){
  int i, j;
  srand(time(NULL));
  //for each layor we build the dropVect according to the dropout
  for(i=0; i<net->nb_layor; i++){
    int nb_drop = (int)(net->tab_layor[i].nb_neuron * net->tab_layor[i].dropout);
    for(j=0; j<net->tab_layor[i].nb_neuron; j++)
      net->tab_layor[i].dropVect[j] = 1;

    for(j=0; j<nb_drop; j++){
      int ind = rand()%net->tab_layor[i].nb_neuron;
      net->tab_layor[i].dropVect[ind] = 0;
    }
  }
}


double* softMax(double* score){
	double* prob = malloc(NB_NEURON_OUTPUT * sizeof(double));

	int i, j;
	double sum = 0;
	for(j=0; j<NB_NEURON_OUTPUT; j++)
		sum += exp(score[j]);

	for(i=0; i<NB_NEURON_OUTPUT; i++){
		prob[i] = exp(score[i])/sum;
	}

	return prob;
}

double train_network(Network* network, Image* img){

	put_img_in_input(network, img);

	build_dropout_vect(network);

	compute_output(network);
	//double error = calcul_error(network, *img);
	double error = compute_error(network, *img);
	backpropagation(network);
	return error;
}
/*
This function put a image in input of the neural network
*/
void put_img_in_input(Network* network, Image* img){
	int i;
	for(i=0; i<NB_NEURON_INPUT; i++){
		//we put each pixels normalized on the input layor
		network->tab_layor[0].tab_neuron[i].potential = img->imgbuf[i]/255.0;
		network->tab_layor[0].tab_neuron[i].value = img->imgbuf[i]/255.0;
	}
}

/*
This function compute the values of the output layor using the weight matrix between the
diffrent layor
*/
void compute_output(Network* network){

	int k;

	//for each layor except the first
	for(k=1; k<NB_LAYOR; k++){
		int nb_neuron = network->tab_layor[k].nb_neuron;
		int i, j;
		//we calcul the value of each neuron
		for(i=0; i<nb_neuron; i++){
			double summe = 0;
			//we look at the previous layor and the weight matrix before to calcul the summe
			for(j=0; j<network->tab_layor[k-1].nb_neuron; j++){
				if(network->tab_layor[k-1].dropVect[j] == 1 && network->tab_layor[k].dropVect[i] == 1){
					double value_previous_neuron = network->tab_layor[k-1].tab_neuron[j].value;
					double weight = network->tab_weight_matrix[k-1].weight[j][i];

					summe += value_previous_neuron * weight;
				}
			}
			if(network->tab_layor[k-1].dropVect[j] == 1 && network->tab_layor[k].dropVect[i] == 1)
				network->tab_layor[k].tab_neuron[i].potential = summe;

			//we apply the logistic function f(x)=1/(1+e(-x)) to compute the value
			network->tab_layor[k].tab_neuron[i].value = 1/(1+exp(-summe));
		}
	}
}

/*
Version 2 of the fonction calcul_error with a earlier propagation of the error
*/
double compute_error(Network* network, Image img){

	double* expected_result = convert_label(img);
	double neuron_value = 0;
	double error = 0;
	double summe_error = 0;

	int i, j;

	double* score = malloc(NB_NEURON_OUTPUT * sizeof(double));
	double* prob = malloc(NB_NEURON_OUTPUT * sizeof(double));

	for(i=0; i<NB_NEURON_OUTPUT; i++)
		score[i] = network->tab_layor[NB_LAYOR-1].tab_neuron[i].value;

	prob = softMax(score);

	//calcul of the error of each neuron of the output layor
	for(i=0; i<NB_NEURON_OUTPUT; i++){
		//calcul of the error
		neuron_value = network->tab_layor[NB_LAYOR-1].tab_neuron[i].value;
		error = expected_result[i] - prob[i];

		summe_error += fabs(error);
		network->tab_layor[NB_LAYOR-1].tab_neuron[i].error_average = error;

		//calcul of the error signal
		network->tab_layor[NB_LAYOR-1].tab_neuron[i].error_signal = neuron_value * (1 - neuron_value) * error;
	}

	double error_signal = 0;
	double summe = 0;

	//propagation of the error to the layor n-1
	for(i=0; i<NB_NEURON_HIDDEN; i++){
		summe = 0;
		for(j=0; j<NB_NEURON_OUTPUT; j++){
			if(network->tab_layor[NB_LAYOR-2].dropVect[i] == 1 && network->tab_layor[NB_LAYOR-1].dropVect[1] == 1){
				summe += network->tab_layor[NB_LAYOR-1].tab_neuron[j].error_signal * network->tab_weight_matrix[NB_LAYOR-2].weight[i][j];
			}
		}
		if(network->tab_layor[NB_LAYOR-2].dropVect[i] == 1 && network->tab_layor[NB_LAYOR-1].dropVect[1] == 1)
			network->tab_layor[NB_LAYOR-2].tab_neuron[i].error_average = summe;
	}

	return fabs(summe_error);
}


/*
This function backpropagate the error to switch the weight matrix
*/
void backpropagation(Network* network){

	int i, j, k;

	double previous_weight;
	double in_signal;
	double error_signal;
	double neuron_value;
	double summe = 0;

	//for each layor, beginning at the end
	for(k=NB_LAYOR-2; k>=0; k--){

		//we calcul the new weight matrix
		for(i=0; i<network->tab_weight_matrix[k].size_in; i++){
			for(j=0; j<network->tab_weight_matrix[k].size_out; j++){
				if(network->tab_layor[k].dropVect[i] == 1 && network->tab_layor[k+1].dropVect[j] == 1){
					previous_weight = network->tab_weight_matrix[k].weight[i][j];
					in_signal = network->tab_layor[k].tab_neuron[i].value;
					error_signal = network->tab_layor[k+1].tab_neuron[j].error_signal;

					network->tab_weight_matrix[k].weight[i][j] = previous_weight + (EPS * in_signal * error_signal);
				}
			}
		}

		//calcul of the error signal
		for(i=0; i<network->tab_layor[k].nb_neuron; i++){
			if(network->tab_layor[k].dropVect[i] == 1){
				neuron_value = network->tab_layor[k].tab_neuron[i].value;
				network->tab_layor[k].tab_neuron[i].error_signal = neuron_value * (1 - neuron_value) * network->tab_layor[k].tab_neuron[i].error_average;
			}
		}

		//propagation of the error
		if(k>0){ //if it's not the firdt layor
			for(i=0; i<network->tab_layor[k-1].nb_neuron; i++){
				summe = 0;
				for(j=0; j<network->tab_layor[k].nb_neuron; j++){
					if(network->tab_layor[k].dropVect[j] == 1 && network->tab_layor[k-1].dropVect[i] == 1){
						summe += network->tab_layor[k].tab_neuron[j].error_signal * network->tab_weight_matrix[k-1].weight[i][j];
					}
				}
				if(network->tab_layor[k].dropVect[j] == 1 && network->tab_layor[k-1].dropVect[i] == 1)
					network->tab_layor[k-1].tab_neuron[i].error_average = summe;
			}
		}
	}
}

/*
This function convert the label of the image on a table of expected result for the network
*/
double* convert_label(Image img){

	double* result;
	result = malloc(NB_NEURON_OUTPUT*sizeof(double));

	int i;

	for(i=0; i<NB_NEURON_OUTPUT; i++){
		if(i==img.label)
			result[i] = 1.00;
		else
			result[i] = 0.00;
	}

	return result;
}

/*
This function convert the table in output of the network in a int result
*/
int convert_result(Network network){

	int i, max = -1;
	double max_value = -10000000; //long min

	for(i=0; i<network.tab_layor[NB_LAYOR-1].nb_neuron; i++){
		if(network.tab_layor[NB_LAYOR-1].tab_neuron[i].value > max_value){
			max_value = network.tab_layor[NB_LAYOR-1].tab_neuron[i].value;
			max = i;
		}
	}

	return max;
}
