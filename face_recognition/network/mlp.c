#include "mlp.h"


/*
This function build a neural network depending of the #define on network.h
*/
Network build_neural_network(){

	if(NB_LAYOR < 2)
		exit(0);

	//printf("\n---------------------------------------------------------------\n");
	//printf("                  Start building neural network ...              \n");

	//network creation
	Network network;
	network.nb_layor = NB_LAYOR;
	network.tab_layor = malloc(NB_LAYOR * sizeof(Layor));
	network.nb_weight_matrix = NB_LAYOR-1;
	network.tab_weight_matrix = malloc((NB_LAYOR-1) * sizeof(Weight_matrix));

	int k;
	//layors creation
  #ifdef DEBUG
  printf("Layor creation :\n");
  #endif

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
		//printf("\tLayor %d created : %d\n", k, network.tab_layor[k].nb_neuron);
	}

	//creation of connexion matrix table
	//printf("Weight matrix creation :\n");
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
		//printf("\tWeight matrix %d created : %dx%d\n", j, network.tab_weight_matrix[j].size_in, network.tab_weight_matrix[j].size_out);
	}

	//printf("\nNetwork creation DONE !\n");
	//printf("---------------------------------------------------------------\n");

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
