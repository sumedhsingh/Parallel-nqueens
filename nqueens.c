// from the Cilk manual: http://supertech.csail.mit.edu/cilk/manual-5.4.6.pdf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// struct for storing arguments passed to nqueens
typedef struct{
    char* config;
    int n;
    int i;
} thread_args;

// add a mutex for thread safety
pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;

// checks if placing queen at coordinates (i,j) is safe
// 1:safe 0:unsafe
int safe(char * config, int i, int j)
{
    int r, s;

    for (r = 0; r < i; r++)
    {
        s = config[r];
        if (j == s || i-r==j-s || i-r==s-j)
            return 0;
    }
    return 1;
}

int count = 0;

// recursively places queens on all columns of a row and proceeds to next row until n is reached
// returns number of valid configurations
void nqueens(char *config, int n, int i)
{
    char *new_config;
    int j;
    
    if (i==n)
    {
        pthread_mutex_lock(&mutext); //critical section begins
        count++;
        pthread_mutex_unlock(&mutext); //critical section ends
    }
    
    
    /* try each possible position for queen <i> */
    for (j=0; j<n; j++)
    {
        /* allocate a temporary array and copy the config into it */
        new_config = malloc((i+1)*sizeof(char)); //change to n or not?
        memcpy(new_config, config, i*sizeof(char));
        if (safe(new_config, i, j))
        {
            new_config[i] = j;
	        nqueens(new_config, n, i+1);
        }
        free(new_config);
    }
    return;
}

// function for calling nqueens with args and deallocating memory
void* arg_thread(void* arg)
{
    thread_args* args = (thread_args*) arg;  // set void* to thread_args*
    nqueens(args->config, args->n, args->i);
    free(args->config);
    free(args);
    return NULL;
}

//creates n threads
int create_thread (int nthreads)
{
    pthread_t threads[nthreads];

    for(int i=0; i<nthreads; i++){
        thread_args* args = malloc(sizeof(thread_args));
        if (args == NULL){
            fprintf(stderr, "Error to allocate memory for thread_args struct\n");
            return -1;
        }
        args->config = malloc(nthreads*sizeof(char));
        if(args->config == NULL){
            fprintf(stderr, "Error to allocate memory for config\n");
            free(args);
            return -1;
        }
        memset(args->config, 0, sizeof(char)*nthreads); //imp step
        args->n = nthreads;
        args->i = 1;                                    // row 0 has already been configured
        args->config[0] = i;

        if (pthread_create(&threads[i], NULL, arg_thread, args) != 0){
            fprintf(stderr, "Failed to create thread for column %d\n",i);
            free(args->config);
            free(args);
            return -1;
        }
    }

    for(int i=0; i<nthreads; i++){
        pthread_join(threads[i], NULL);             // collect all threads here
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int n;

    if (argc < 2)
    {
        printf("%s: number of queens required\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    printf("running queens %d\n", n);
    if(create_thread(n) != 0){
        fprintf(stderr, "Error creating threads\n");
        return -1;
    }
    
    printf("# solutions: %d\n", count);

    return 0;
}
