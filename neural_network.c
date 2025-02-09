#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NEURAL_INPUT 2
#define NEURAL_HIDDEN 8
#define NEURAL_OUTPUT 1

typedef struct neural_network
{
    //Make neural_network
    //inputs
    //hidden
    //output
    
    //int input_size;
    //int hidden_size;
    //int output_size;
    
    //double* input_neurons;
    //double* hidden_neurons;
    //double* output_neurons;
    
    double** in_to_hid;
    double** hid_to_out;
    
}nn;

double random_weight(/*double* n*/)
{
    //*n = (double)rand()*2/(double)RAND_MAX;
  return ((double)rand()/(double)RAND_MAX)*2-1;
}

double sigmoid(double x)
{
  return 1.0/(1.0+exp(-x));
}

void init_network(nn* n)
{
    //CALLING MORE THAN ONCE WILL LEAK MEMORY
    //n->input_size = NEURAL_INPUT;
    //n->hidden_size = NEURAL_HIDDEN;
    //n->output_size = NEURAL_OUTPUT;
    
    /*n->input_neurons = malloc(sizeof(double)*NEURAL_INPUT);
    n->hidden_neurons = malloc(sizeof(double)*NEURAL_HIDDEN);
    n->output_neurons = malloc(sizeof(double)*NEURAL_OUTPUT);
    
    for(int i=0; i < NEURAL_INPUT; i++)
    {
        init_neuron(&(n->input_neurons[i]));
    }
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
    {
        init_neuron(&(n->hidden_neurons[i]));
    }
    
    for(int i=0; i < NEURAL_OUTPUT; i++)
    {
        init_neuron(&(n->output_neurons[i]));
    }*/

  //n = malloc(sizeof(*n));
  
    n->in_to_hid = malloc(sizeof(double*)*NEURAL_INPUT);
    n->hid_to_out = malloc(sizeof(double*)*NEURAL_HIDDEN);
    for(int i=0; i < NEURAL_INPUT; i++)
    {
        n->in_to_hid[i] = malloc(sizeof(double)*NEURAL_HIDDEN);
	
        for(int j=0; j < NEURAL_HIDDEN; j++)
        {
            n->in_to_hid[i][j] = random_weight();
        }
    }
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
    {
        n->hid_to_out[i] = malloc(sizeof(double)*NEURAL_OUTPUT);
        
        for(int j=0; j < NEURAL_OUTPUT; j++)
        {
            n->hid_to_out[i][j] = random_weight();
        }
    }
    
}

double* guess(nn* n, int* ins)
{
    //Multiply input by ins
    //Multiply input layer's output by hidden
    //Multiply hidden layer's output by output
    
    //double* in_to_hidden = malloc(sizeof(double)*NEURAL_HIDDEN*NEURAL_INPUT);
    //double* hid_to_out = malloc(sizeof(double)*NEURAL_OUTPUT*NEURAL_HIDDEN);
    
    //input[0] = hidden[0-8] //0-7
    //input[1] = hidden[0-8] // 8-15
    
    //16 weights
    
    //ins * each input to hidden weight
    //ins[0] * input[0] to hidden
    //ins[1] * input[1] to hidden
    
    //ins[0%8]
    
    double* output = malloc(sizeof(double)*NEURAL_OUTPUT);
    
    double** i_weights = malloc(sizeof(double*)*NEURAL_INPUT);
    double** h_weights = malloc(sizeof(double*)*NEURAL_HIDDEN);
    
    for(int i=0; i < NEURAL_INPUT; i++)
    {
        i_weights[i] = malloc(sizeof(double)*NEURAL_HIDDEN);
    }
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
    {
        h_weights[i] = malloc(sizeof(double)*NEURAL_OUTPUT);
    }
    
    //Multiply ins by input layer weights
    for(int i=0; i < NEURAL_INPUT; i++)
    {
        for(int j=0; j < NEURAL_HIDDEN; j++)
        {
            i_weights[i][j] = n->in_to_hid[i][j] * ins[i];
        }
    }
    
    //Set hidden neurons to sum of input layer weights times input
    double* hidden_layer = malloc(sizeof(double)*NEURAL_HIDDEN);
    for(int j=0; j < NEURAL_HIDDEN; j++)
        {
            hidden_layer[j] = 0;
            for(int i=0; i < NEURAL_INPUT; i++)
            {
	      hidden_layer[j] += i_weights[i][j];//add bias here
            }
	    hidden_layer[j] = sigmoid(hidden_layer[j]);
        }
    
    //Multiply hidden layer weights by input layer weights
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
      {
	for(int j=0; j < NEURAL_OUTPUT; j++)
	  {
	    h_weights[i][j] = n->hid_to_out[i][j] * hidden_layer[i];
	  }
      }

    for(int i=0; i < NEURAL_OUTPUT; i++)
      {
	output[i] = 0;
	for(int j=0; j < NEURAL_HIDDEN; j++)
	  {
	    output[i] += h_weights[j][i];
	  }
	output[i] = sigmoid(output[i]);
        
      }
    //Free i_weights, h_weights, and hidden_layer
    for(int i=0; i < NEURAL_INPUT; i++)
      {
        free(i_weights[i]);
    }
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
    {
        free(h_weights[i]);
    }
    free(i_weights);
    free(h_weights);
    free(hidden_layer);

    return output;
}

void train(nn* n, int in_x, int in_y)
{
    //Train
  //Do guess
  //Get error
  //Backprop the changes
}

int main()
{
  srand(time(NULL));
  nn n;
  init_network(&n);
  int* ins = malloc(sizeof(int)*NEURAL_INPUT);
  for(int i=0; i < NEURAL_INPUT; i++)
    {
      ins[i] = i;
    }
  
  double* g = guess(&n, ins);

  printf("%lf\n", g[0]);
  
  return 0;
}

#endif
