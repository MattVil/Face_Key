#include "mlp.h"

int main(int argc, char const *argv[]) {

  int vect[20] = {1};

  build_vect(0.25, vect, 20);

  int i;
  for(i=0; i<20; i++){
    printf("%d - ", vect[i]);
  }
  printf("\n");

  return 0;
}
