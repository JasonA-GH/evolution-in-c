#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int neural_input = 4;
int neural_hidden = 1;
int neural_output = 4;

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

void init_network(nn* n, int ni, int nh, int no)
{
  neural_input = ni;
  neural_hidden = nh;
  neural_output = no;
    //CALLING MORE THAN ONCE WILL LEAK MEMORY
  
    n->in_to_hid = malloc(sizeof(double*)*neural_input);
    n->hid_to_out = malloc(sizeof(double*)*neural_hidden);
    
    for(int i=0; i < neural_input; i++)
    {
        n->in_to_hid[i] = malloc(sizeof(double)*neural_hidden);
	
        for(int j=0; j < neural_hidden; j++)
        {
            n->in_to_hid[i][j] = random_weight();
        }
    }
    
    for(int i=0; i < neural_hidden; i++)
    {
        n->hid_to_out[i] = malloc(sizeof(double)*neural_output);
        
        for(int j=0; j < neural_output; j++)
        {
            n->hid_to_out[i][j] = random_weight();
        }
    }
    
}

double* guess(nn* n, int* ins)
{
    double* output = malloc(sizeof(double)*neural_output);
    
    double** i_weights = malloc(sizeof(double*)*neural_input);
    double** h_weights = malloc(sizeof(double*)*neural_hidden);
    
    for(int i=0; i < neural_input; i++)
    {
        i_weights[i] = malloc(sizeof(double)*neural_hidden);
    }
    
    for(int i=0; i < neural_hidden; i++)
    {
        h_weights[i] = malloc(sizeof(double)*neural_output);
    }
    
    //Multiply ins by input layer weights
    for(int i=0; i < neural_input; i++)
    {
        for(int j=0; j < neural_hidden; j++)
        {
            i_weights[i][j] = n->in_to_hid[i][j] * ins[i];
        }
    }
    
    //Set hidden neurons to sum of input layer weights times input
    double* hidden_layer = malloc(sizeof(double)*neural_hidden);
    for(int j=0; j < neural_hidden; j++)
        {
            hidden_layer[j] = 0;
            for(int i=0; i < neural_input; i++)
            {
	      hidden_layer[j] += i_weights[i][j];//add bias here
            }
	    hidden_layer[j] = sigmoid(hidden_layer[j]);
        }
    
    //Multiply hidden layer weights by input layer weights
    
    for(int i=0; i < neural_hidden; i++)
      {
	for(int j=0; j < neural_output; j++)
	  {
	    h_weights[i][j] = n->hid_to_out[i][j] * hidden_layer[i];
	  }
      }

    for(int i=0; i < neural_output; i++)
      {
	output[i] = 0;
	for(int j=0; j < neural_hidden; j++)
	  {
	    output[i] += h_weights[j][i];
	  }
	output[i] = sigmoid(output[i]);
        
      }
    //Free i_weights, h_weights, and hidden_layer
    for(int i=0; i < neural_input; i++)
      {
        free(i_weights[i]);
    }
    
    for(int i=0; i < neural_hidden; i++)
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
  for(int i=0; i < neural_input; i++)
    {
      for(int j=0; j < neural_hidden; j++)
        {
	  b->in_to_hid[i][j] = a->in_to_hid[i][j];
        }
    }
    
  for(int i=0; i < neural_hidden; i++)
    {
      for(int j=0; j < neural_output; j++)
        {
	  b->hid_to_out[i][j] = a->hid_to_out[i][j];
        }
    }
}

void randomize_weights(nn* n)
{
  for(int i=0; i < neural_input; i++)
    {
        for(int j=0; j < neural_hidden; j++)
        {
            n->in_to_hid[i][j] = random_weight();
        }
    }
    
    for(int i=0; i < neural_hidden; i++)
    {
        for(int j=0; j < neural_output; j++)
        {
            n->hid_to_out[i][j] = random_weight();
        }
    }
}

#endif
