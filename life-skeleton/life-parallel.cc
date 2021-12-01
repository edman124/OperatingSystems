#include "life.h"
#include <pthread.h>

struct Info
{
	int numThreads;
	LifeBoard *currState;
	LifeBoard *next_state;
	int maxSteps;
	int threadId;
	pthread_barrier_t *barrier;
	// pthread_barrier_t *barrier2;
};
void *sim_life(void *arg) {
	struct Info* mystruct = (struct Info*)arg;
	int n = mystruct->threadId;
    LifeBoard *cState = (mystruct->currState);
    LifeBoard *nState = (mystruct->next_state);
    for (int step = 0; step < mystruct->maxSteps; step += 1) {

        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
    	if (step%2 == 0)
    	{
	        for (int y = 1; y < cState->height() - 1; y += 1) {
	            for (int x = n+1; x < cState->width() - 1; x += mystruct->numThreads) {
	                
	                /* For each cell, examine a 3x3 "window" of cells around it,
	                 * and count the number of live (true) cells in the window. */
	                int live_in_window = 0;
	                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
	                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
	                        if (cState->at(x + x_offset, y + y_offset))
	                            live_in_window += 1;
	                
	                /* Cells with 3 live neighbors remain or become live.
	                   Live cells with 2 live neighbors remain live. */
	                nState->at(x, y) = (
	                    live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
	                    (live_in_window == 4 && cState->at(x, y)) /* live cell with 3 neighbors */
	                );
	            }
	        }
	        
	        //insert barrier here
	        pthread_barrier_wait ((mystruct->barrier));
	        /* now that we computed nState, make it the current state */
	        // if(n==0)
	        // {
	        // 	swap(*cState, *nState);
	        // 	//LB_display(cState,stdout);
	        // }
       	}
       	else
    	{
	        for (int y = 1; y < nState->height() - 1; y += 1) {
	            for (int x = n+1; x < nState->width() - 1; x += mystruct->numThreads) {
	                
	                /* For each cell, examine a 3x3 "window" of cells around it,
	                 * and count the number of live (true) cells in the window. */
	                int live_in_window = 0;
	                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
	                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
	                        if (nState->at(x + x_offset, y + y_offset))
	                            live_in_window += 1;
	                
	                /* Cells with 3 live neighbors remain or become live.
	                   Live cells with 2 live neighbors remain live. */
	                cState->at(x, y) = (
	                    live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
	                    (live_in_window == 4 && nState->at(x, y)) /* live cell with 3 neighbors */
	                );
	            }
	        }
	        
	        //insert barrier here
	        pthread_barrier_wait ((mystruct->barrier));
	        /* now that we computed nState, make it the current state */
       	}
        // pthread_barrier_wait ((mystruct->barrier2));
    }
    // if(n==0){
    // 	LB_del(nState);
    // }
    if(n==0 && mystruct->maxSteps%2==1)
    {
    	swap(*nState, *cState);
    	//LB_display(cState,stdout);
    }
    return NULL;
}
void simulate_life_parallel(int threads, LifeBoard &state, int steps) {
    /* YOUR CODE HERE */
    pthread_t *lifeThreads;
    lifeThreads = (pthread_t *)malloc(threads*sizeof(pthread_t));//free later
    LifeBoard initTemp{state.width(), state.height()};
    pthread_barrier_t *mybarrier = (pthread_barrier_t *)malloc(sizeof(pthread_barrier_t));
    // pthread_barrier_t *mybarrier2 = (pthread_barrier_t *)malloc(sizeof(pthread_barrier_t));
    pthread_barrier_init(mybarrier, NULL, threads);
    // pthread_barrier_init(mybarrier2, NULL, threads);
    struct Info *passInfo = (Info *)malloc(threads*sizeof(struct Info));
    for (int i=0; i<threads; i++) 
    {
    	passInfo[i].numThreads=threads;
    	passInfo[i].currState = &state;
    	passInfo[i].maxSteps = steps;
    	passInfo[i].next_state = &initTemp; 
    	passInfo[i].threadId = i;
    	passInfo[i].barrier=mybarrier;
    	// passInfo[i].barrier2=mybarrier2;
    	pthread_create(&(lifeThreads[i]), NULL, sim_life, (void *)(&passInfo[i]));
    }
    for (int i = 0; i<threads; i++)
    {
    	pthread_join(lifeThreads[i], NULL);
    }
    pthread_barrier_destroy(mybarrier);
    // pthread_barrier_destroy(mybarrier2);
    free(passInfo);
    free(lifeThreads);
    free(mybarrier);
    // free(mybarrier2);
}
