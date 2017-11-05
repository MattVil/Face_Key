#include "mlp.h"

int main(int argc, char const *argv[]) {

  //training file
	int nb_img;
	nb_img = open_training_files();

	//test file
	int nb_img_test;
	nb_img_test = open_test_files();

  Network network = build_neural_network();

  int x;
  for(x=0; x<network.nb_layor; x++){
    printf("%d : %lf\n", x, network.tab_layor[x].dropout);
  }

  printf("\n########### TRAINING... ############\n");
  int i, j, z, p;
  for(j=0; j<2; j++){
    for(i=0; i<60000; i++){
      Image img;
      read_input_number(i, &img);



      double error = train_network(&network, &img);

			/*printf("%d %d %d\n", network.tab_layor[1].nb_neuron, network.tab_layor[2].nb_neuron, network.tab_layor[3].nb_neuron);
			for(z=1; z<NB_LAYOR; z++){
				for(p=0; p<network.tab_layor[z].nb_neuron; p++){
					printf("%d", network.tab_layor[z].dropVect[p]);
				}
				printf("\t");
			}
			printf("\n");*/

      if(i%1 == 0) printf("%d :\t%lf\n", i, error);
      //if(i%1 == 0) printf("%d : ", i);

    }
  }
  printf("Done.\n\n############# TEST... #############\n");

  int good_result = 0, bad_result = 0;

  for(j=0; j<10000; j++){
    int nb = rand()%(10000-1) +1;
    Image img2;
    read_input_number_test(nb, &img2);

    put_img_in_input(&network, &img2);
    compute_output(&network);

    //printf("Image n°%d : %d\t", j, img2.label);
    //affiche_img(&img2);

    int network_value = convert_result(network);

    if(network_value == img2.label)
      good_result++;
    else
      bad_result++;

    //printf("Network : %d\t", network_value);
    //printf("at %.2f%%\n", network.tab_layor[NB_LAYOR-1].tab_neuron[network_value].value * 100);
  }

  printf("#####  Good result : %.2f %% ######\n", ((double)good_result/10000)*100.0);
















  /*double tab[10] = {3.0, 1.0, 2.0, -2.0, 0.326, 1.25, 0.0001, 0.326, 1.25, 0.0001};
  double* prob = softMax(tab);
  int i;
  for(i=0; i<NB_NEURON_OUTPUT; i++)
    printf("% lf ", prob[i]);
  printf("\n");*/


  return 0;
}
