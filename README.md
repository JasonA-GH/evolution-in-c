# evolution-in-c
Simple evolution algorithm in c

The agent is trying to reach END_X, END_Y (100, 0) The agent picks a series of random directions, and then moves along that series. At the end of the generation the agents are adjusted based on their performance. After being sorted, the top half of all candidates either change a random number of directions in their first half or their second half. The number of directions is randomly selected. There is also a 10% chance to mutate a small amount of random directions. From half to 10 under the max, their directions are set to the best preforming agent and then some of their directions are mutated. The bottom 10 just receive completely random directions.
