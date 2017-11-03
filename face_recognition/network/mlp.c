#include "mlp.h"


/*
This function build the dropout vector wich will be rebuild after each backprop
*/
void build_dropout_vect(Network* net){
  int i, j;
  srand(time(NULL));
  for(i=0; i<net->nb_layor; i++){
    //if output layer, no dropout
    if(i==(net->nb_layor-1)){
      for(j=0; j<net->tab_layor[i].nb_neuron; j++){
        net->tab_layor[i].dropVect[j] = 1;
      }
    }
    //if a previous layer
    else{
      int nb_drop = (int)(net->tab_layor[i].nb_neuron * net->tab_layor[i].dropout);
      for(j=0; j<net->tab_layor[i].nb_neuron; j++)
        net->tab_layor[i].dropVect[j] = 1;

      for(j=0; j<nb_drop; j++){
        int ind = rand()%net->tab_layor[i].nb_neuron;
        net->tab_layor[i].dropVect[ind] = 0;
      }
    }
  }
}
