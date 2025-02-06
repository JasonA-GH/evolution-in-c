#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define AGENT_STEPS 100
#define END_X 100
#define END_Y 0

#define LEVEL_MUTATE 0
#define LEVEL_BEST 1
#define LEVEL_RANDOM 2

#define NEURAL_INPUT 2
#define NEURAL_HIDDEN 8
#define NEURAL_OUTPUT 1

const int agent_count = 100;

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
    
    double* in_to_hid;
    double* hid_to_out;
    
}nn;

/*NEURAL NETWORK SECTION START*/



double random_weight(/*double* n*/)
{
    //*n = (double)rand()*2/(double)RAND_MAX;
    return (double)rand()*2/(double)RAND_MAX;
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
    
    n->in_to_hid = malloc(sizeof(double)*NEURAL_INPUT*NEURAL_HIDDEN);
    n->hid_to_out = malloc(sizeof(double)*NEURAL_HIDDEN*NEURAL_OUTPUT);
    
    for(int i=0; i < NEURAL_INPUT*NEURAL_HIDDEN; i++)
    {
        n->in_to_hid[i] = random_weight();
    }
    
    for(int i=0; i < NEURAL_HIDDEN*NEURAL_OUTPUT; i++)
    {
        n->hid_to_out[i] = random_weight();
    }
}

int guess(nn* n, int* ins)
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
    
    for(int i=0; i < NEURAL_INPUT*NEURAL_HIDDEN; i++)
    {
        n->in_to_hid[i] 
    }
    
    for(int i=0; i < NEURAL_HIDDEN*NEURAL_OUTPUT; i++)
    {
        n->hid_to_out[i] = random_weight();
    }
}

void train(nn* n, int in_x, int in_y)
{
    //Train
}

/*NEURAL NETWORK SECTION END*/

typedef struct agent_struct
{
  int x;
  int y;
  //int* dirs; 
  double reward;
  nn* n;
  //int seed;
}agent;

agent* agents;

void reset_agent(agent* a)
{
    a->x = 0;
    a->y = 0;
    a->reward = 0;
}

void do_agent(agent* a)
{
  //srand(a->seed);
  reset_agent(a);
  for(int i=0; i < AGENT_STEPS; i++)
    {
      //a->dir = rand()%4;
      switch(a->dirs[i])
	{
	case 0:
	  {
	    a->x++;
	    break;
	  }
	case 1:
	  {
	    a->x--;
	    break;
	  }
	case 2:
	  {
	    a->y++;
	    break;
	  }
	case 3:
	  {
	    a->y--;
	    break;
	  }
      
	}
	if(a->x == END_X && a->y == END_Y)
	{
	    a->reward = 10000.0/i;
	    return;
	}
	else
	    a->reward--;
    }
}

void copy_dirs_from(agent* base, agent* end)
{
  for(int i=0; i < AGENT_STEPS; i++)
    {
      end->dirs[i] = base->dirs[i];
    }
}

void copy_dirs_from_start_end(agent* base, agent* target, int start, int end)
{
  for(int i=start; i < end; i++)
    {
      target->dirs[i] = base->dirs[i];
    }
}

void copy(agent* a, agent* b)
{
  //b->x = a->x;
  //b->y = a->y;
  b->reward = a->reward;
  copy_dirs_from(a, b);
  //b->seed = a->seed;
}

void randomize_dirs(agent* a)
{
  for(int i=0; i < AGENT_STEPS; i++)
    {
      a->dirs[i] = rand()%4;
    }
}

void init_dirs(agent* a)
{
  //IF THIS IS CALLED MORE THAN ONCE IT WILL LEAK MEMORY
  a->dirs = malloc(sizeof(int)*AGENT_STEPS);
  randomize_dirs(a);
}

void init_agents(int count)
{
  //IF THIS IS CALLED MORE THAN ONCE IT WILL LEAK MEMORY
  srand(time(NULL));
  agents = malloc(sizeof(agent)*count);
  for(int i=0; i < count; i++)
    {
      agents[i].x = 0;
      agents[i].y = 0;
      //agents[i].dir = 0;
      init_dirs(&(agents[i]));
      agents[i].reward = 0;
      //agents[i].seed = rand();
    }
}

void evolve(agent* base, agent* better, int l)
{
  if(l == LEVEL_MUTATE)
    {
      //Randomly choose index
      //Randomly choose whether to change first or second half
      //10% chance to mutate 10% of the dirs
      
      int index = rand()%AGENT_STEPS;
      if(rand()%2 == 0)//FIRST HALF
	{
	  //end = AGENT_STEPS;
	  copy_dirs_from_start_end(better, base, 0, index);
	}
      else//SECOND HALF
	{
	  copy_dirs_from_start_end(better, base, index, AGENT_STEPS);
	}

      if(rand()%10 == 0)
	{
	  int count = rand()%(AGENT_STEPS/10);
	  for(int i=0; i < count; i++)
	    {
	      base->dirs[rand()%AGENT_STEPS] = rand()%4;
	      //randomize_dirs(base);
	    }
	}
    }
  else if(l == LEVEL_BEST)
    {
      //bottom half (minus the bottom 10)
      //Convert entire ai to Best
      //Mutate 10% of dirs
      copy_dirs_from(better, base);
      int count = rand()%(AGENT_STEPS/5);
      for(int i=0; i < count; i++)
	{
	  base->dirs[i] = rand()%4;
	}
      
    }
  else if(l == LEVEL_RANDOM)
    {
      //Last 10% just randomly generate
      randomize_dirs(base);
    }
}

void sort()
{
  int done = 0;
  while(!done)
    {
      done = 1;
      for(int i=0; i < agent_count-1; i++)
	{
	  if(agents[i+1].reward > agents[i].reward)
	    {
	      agent temp;
	      init_dirs(&temp);
	      copy(&(agents[i+1]), &temp);
	      copy(&(agents[i]), &(agents[i+1]));
	      copy(&temp, &(agents[i]));
	      //agents[i+1] = agents[i];
	      //agents[i] = temp;
	      done = 0;
	    }
	}
    }
}

double dist(int x, int y)
{
  return sqrt(pow(x-END_X, 2) + pow(y-END_Y, 2));
}

void get_reward(agent* a)
{
    if(a->reward < 0)
    a->reward *= dist(a->x, a->y);
  //return a->reward;
}

int main()
{
  //do task
  //get reward
  //change randomly

  //double rewards[agent_count];
  
  init_agents(agent_count);
  int z=0;
  //while(1)
  for(z=0; z < 2500; z++)
    {
  
      for(int i=0; i < agent_count; i++)
	{
	  do_agent(&(agents[i]));
	  get_reward(&(agents[i]));
	}


      sort();

      if(z%100 == 0)
	printf("%d | Best: %lf x: %d y: %d | Worst: %lf x: %d y: %d\n", z, agents[0].reward, agents[0].x, agents[0].y, agents[agent_count-1].reward, agents[agent_count-1].x, agents[agent_count-1].y);

      //printf("best seed %d\n", agents[0].seed);
      for(int i=1; i < agent_count/2; i++)
	{
	  evolve(&(agents[i]), &(agents[i-1]), LEVEL_MUTATE);
	}

      for(int i=agent_count/2; i < agent_count-10; i++)
	{
	  evolve(&(agents[i]), &(agents[0]), LEVEL_BEST);
	}

      for(int i=agent_count-10; i < agent_count; i++)
	{
	  evolve(&(agents[i]), &(agents[0]), LEVEL_RANDOM);
	}
      z++;
    }
  //printf("%d | Best: %lf x: %d y: %d | Worst: %lf x: %d y: %d\n", z, agents[0].reward, agents[0].x, agents[0].y, agents[agent_count-1].reward, agents[agent_count-1].x, agents[agent_count-1].y);

    int x = 0;
    int y = 0;
    for(int i=0; i < AGENT_STEPS; i++)
    {
        switch(agents[0].dirs[i])
	{
	case 0:
	  {
	    x++;
	    break;
	  }
	case 1:
	  {
	    x--;
	    break;
	  }
	case 2:
	  {
	    y++;
	    break;
	  }
	case 3:
	  {
	    y--;
	    break;
	  }
    }
    printf("dirs[%d]: %d X: %d Y: %d\n", i, agents[0].dirs[i], x, y);
    }

  //Free all memory
  //Free each agents dirs
  //Free agents
  
  return 0;
}
 
