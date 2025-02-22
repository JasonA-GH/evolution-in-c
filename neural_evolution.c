#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define AGENT_STEPS 100
#define END_X 50
#define END_Y 50

#define LEVEL_MUTATE 0
#define LEVEL_BEST 1
#define LEVEL_RANDOM 2

#include "evo_nn.h"

const int agent_count = 100;

typedef struct agent_struct
{
  int x;
  int y;
  //int* dirs;
  int* input;
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

void update_inputs(agent* a)
{
  //Assumes the array is already malloced
  a->input[0] = a->x;
  a->input[1] = a->y;
  a->input[2] = END_X;
  a->input[3] = END_Y;
}

void do_agent(agent* a)
{
  //srand(a->seed);
  reset_agent(a);
  for(int i=0; i < AGENT_STEPS; i++)
    {
      update_inputs(a);
      int dir = get_ans(guess(a->n, a->input), neural_output);
      //a->dir = rand()%4;
      //printf("%d\n", dir);
      switch(dir)
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

//Copies from a into b
void copy(agent* a, agent* b)
{
  //b->x = a->x;
  //b->y = a->y;
  b->reward = a->reward;
  copy_neural_network(a->n, b->n);
  //copy_dirs_from(a, b);
  //b->seed = a->seed;
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
      //init_dirs(&(agents[i]));
      agents[i].input = malloc(sizeof(int)*neural_input);
      agents[i].reward = 0;
      //agents[i].seed = rand()
      agents[i].n = malloc(sizeof(*agents[i].n));
      init_network(agents[i].n, 4, 2, 4);
      
    }
}

void evolve(agent* base, agent* better, int level)
{
  if(level == LEVEL_MUTATE)
    {
      int index = rand()%AGENT_STEPS;

      if(rand()%2 == 0)//INPUT -> HIDDEN WEIGHTS
	{
      
	  for(int i=0; i < rand()%neural_input*neural_hidden; i++)
	    {
	      int randInput = rand()%neural_input;
	      int randHidden = rand()%neural_hidden;
	      base->n->in_to_hid[randInput][randHidden] = better->n->in_to_hid[randInput][randHidden];
	    }
	}
	  else // HIDDEN -> OUTPUT WEIGHTS
	    {
	      for(int i=0; i < rand()%neural_hidden*neural_output; i++)
		{
		  int randHidden = rand()%neural_hidden;
		  int randOutput = rand()%neural_output;
		  base->n->hid_to_out[randHidden][randOutput] = better->n->hid_to_out[randHidden][randOutput];
		}
	    }
    }
  else
    {
      randomize_weights(base->n);
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
	      temp.n = malloc(sizeof(*temp.n));
	      init_network(temp.n, neural_input, neural_hidden, neural_output);
	      //init_dirs(&temp);
	      copy(&(agents[i+1]), &temp);
	      copy(&(agents[i]), &(agents[i+1]));
	      copy(&temp, &(agents[i]));
	      //agents[i+1] = agents[i];
	      //agents[i] = temp;

	      free(temp.n);
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

      for(int i=agent_count/2; i < agent_count-(agent_count/10); i++)
	{
	  evolve(&(agents[i]), &(agents[0]), LEVEL_BEST);
	}

      for(int i=agent_count-(agent_count/10); i < agent_count; i++)
	{
	  evolve(&(agents[i]), &(agents[0]), LEVEL_RANDOM);
	}
      z++;
    }
  printf("%d | Best: %lf x: %d y: %d | Worst: %lf x: %d y: %d\n", z, agents[0].reward, agents[0].x, agents[0].y, agents[agent_count-1].reward, agents[agent_count-1].x, agents[agent_count-1].y);

    reset_agent(&(agents[0]));
    for(int i=0; i < AGENT_STEPS; i++)
    {
      update_inputs(&(agents[0]));
      int dir = get_ans(guess(agents[0].n, agents[0].input), neural_output);
      switch(dir)
	{
	case 0:
	  {
	    agents[0].x++;
	    break;
	  }
	case 1:
	  {
	    agents[0].x--;
	    break;
	  }
	case 2:
	  {
	    agents[0].y++;
	    break;
	  }
	case 3:
	  {
	    agents[0].y--;
	    break;
	  }
    }
      printf("X: %d Y: %d\n", agents[0].x, agents[0].y);
    }

  //Free all memory
  //Free each agents dirs
  //Free agents
  
  return 0;
}
 
