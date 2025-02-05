#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define AGENT_STEPS 100
#define END_X 100
#define END_Y 0

const int agent_count = 100;

typedef struct agent_struct
{
  int x;
  int y;
  int* dirs;
  double reward;
  //int seed;
}agent;

agent* agents;


void do_agent(agent* a)
{
  //srand(a->seed);
  a->x = 0;
  a->y = 0;
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

void init_dirs(agent* a)
{
  a->dirs = malloc(sizeof(int)*AGENT_STEPS);
  for(int i=0; i < AGENT_STEPS; i++)
    {
      a->dirs[i] = rand()%4;
    }
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

void evolve(agent* base, agent* better)
{
  //srand(time(NULL));
  //a->seed++;


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
	  base->dirs[i] = rand()%4;
	}
    }
  
  //bottom half (minus the bottom 10)
  //Convert entire ai to Best
  //Mutate 10% of dirs

  //Last 10% just randomly generate
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
  a->reward = 10000.0/dist(a->x, a->y);
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
  for(z=0; z < 1000; z++)
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
      for(int i=1; i < agent_count; i++)
	{
	  evolve(&(agents[i]), &(agents[i-1]));
	}
    }
  printf("%d | Best: %lf x: %d y: %d | Worst: %lf x: %d y: %d\n", z, agents[0].reward, agents[0].x, agents[0].y, agents[agent_count-1].reward, agents[agent_count-1].x, agents[agent_count-1].y);

  
  
  return 0;
}
