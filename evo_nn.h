#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NEURAL_INPUT 4
#define NEURAL_HIDDEN 1
#define NEURAL_OUTPUT 4

typedef struct neural_network
{
    
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

void train(nn* n, int* ins)
{
  
}

int get_ans(double* g, int size)
{
  double x = 0;
  int index = 0;
  for(int i=0; i < size; i++)
    {
      if(g[i] > x)
	{
	  x = g[i];
	  index = i;
	}
    }
  return index;
}

void copy_neural_network(nn* a, nn* b)
{
  for(int i=0; i < NEURAL_INPUT; i++)
    {
      for(int j=0; j < NEURAL_HIDDEN; j++)
        {
	  b->in_to_hid[i][j] = a->in_to_hid[i][j];
        }
    }
    
  for(int i=0; i < NEURAL_HIDDEN; i++)
    {
      for(int j=0; j < NEURAL_OUTPUT; j++)
        {
	  b->hid_to_out[i][j] = a->hid_to_out[i][j];
        }
    }
}

void randomize_weights(nn* n)
{
  for(int i=0; i < NEURAL_INPUT; i++)
    {
        for(int j=0; j < NEURAL_HIDDEN; j++)
        {
            n->in_to_hid[i][j] = random_weight();
        }
    }
    
    for(int i=0; i < NEURAL_HIDDEN; i++)
    {
        for(int j=0; j < NEURAL_OUTPUT; j++)
        {
            n->hid_to_out[i][j] = random_weight();
        }
    }
}

#endif
