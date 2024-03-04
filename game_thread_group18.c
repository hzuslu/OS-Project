#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define repeat 3

int n;
int NUM_Players;
pthread_mutex_t mutex;
pthread_mutex_t turnMutex;
int turn = 0;
char **gameBoard;
bool **visited;
int count = 0;
int distance = -1;
int guess_count = 0;
pthread_barrier_t barrier;
int avarage_count = 0;
bool check = false;
int ***distance_array;
int *minPlayers;
struct location
{
    int row;
    int collumn;
};
struct location *loc_arr;
struct location *guess_arr;
int *average_distance;
int turn_location=0;

void initializeGameBoard()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            gameBoard[i][j] = '-';
            visited[i][j] = false;
        }
    }
}

void printGameBoard()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%c ", gameBoard[i][j]);
        }
        printf("\n");
    }
}

void findMin(int ***arr)
{
    int minValue = 99999;

    for (int x = 0; x < NUM_Players; x++)
        minPlayers[x] = -1;

    for (int i = 0; i < NUM_Players; i++)
    {
        for (int j = 0; j < NUM_Players; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (arr[i][j][k] < minValue && arr[i][j][k] != 0)
                    minValue = arr[i][j][k];
            }
        }
    }

    for (int i = 0; i < NUM_Players; i++)
    {
        for (int j = 0; j < NUM_Players; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (arr[i][j][k] == minValue)
                    minPlayers[i] = i;
            }
        }
    }

    printf("\nThe game ends!\n");
    printf("The winner with the closest guess of %d-distance:\n", minValue);

    for (int i = 0; i < NUM_Players; i++)
        if (minPlayers[i] != -1)
            printf("player%d, ", minPlayers[i] + 1);
}

void *player_threads(void *arg)
{
    long tid = (long)arg;
     while (1)
        {
            pthread_mutex_lock(&mutex);
            if (turn_location == tid - 1)
            {
                break;
            }
            pthread_mutex_unlock(&mutex);
        }

    int random_row, random_col;

    do
    {
        random_row = rand() % n;
        random_col = rand() % n;
        struct location store;
        store.row = random_row;
        store.collumn = random_col;
        loc_arr[tid - 1] = store;
    } while (visited[random_row][random_col]);
      
      turn_location = (turn_location + 1) % NUM_Players;
    
    visited[random_row][random_col] = true;
    gameBoard[random_row][random_col] = '0' + tid;

    printf("player %ld location: [%d, %d]\n ", tid, random_row, random_col);
    pthread_mutex_unlock(&mutex);
   pthread_barrier_wait(&barrier);
       

    for (int k = 0; k < repeat; k++)
    {
        while (1)
        {
            pthread_mutex_lock(&turnMutex);
            if (turn == tid - 1)
            {
                break;
            }
            pthread_mutex_unlock(&turnMutex);
        }

        if (check == false)
        {
            if (k == 0 && tid == 1)
            {
                  
              printf("\n");
              printf("***********************\n");
              printf("location of player table\n");
              printf("************************\n");
              printGameBoard();
              printf("\n\n");
                printf("Game launches -->\n");
            }
            if (tid == 1)
            {
                printf("---------- Round-%d ----------\n\n", k + 1);
            }

            int guess_row, guess_col;
            struct location guess_store;

            if (k == 0)
            {
                guess_col = rand() % n;
                guess_row = rand() % n;
                guess_store.row = guess_row;
                guess_store.collumn = guess_col;
                guess_arr[guess_count];
                guess_count++;
            }

            if (k == 1)
            {
                do
                {
                    guess_col = guess_arr[tid * k - 1].collumn + (rand() % (2 * average_distance[tid - 1] + 1) - average_distance[tid - 1]);
                    guess_row = guess_arr[tid * k - 1].row + (rand() % (2 * average_distance[tid - 1] + 1) - average_distance[tid - 1]);
                } while (guess_row < 0 || guess_row > 9 || guess_col < 0 || guess_col > 9);
                guess_store.row = guess_row;
                guess_store.collumn = guess_col;
                guess_arr[guess_count];
                guess_count++;
            }

            if (k == 2)
            {
                do
                {
                    guess_col = guess_arr[(tid - 1) + NUM_Players].collumn + (rand() % ((2 * average_distance[(tid - 1) + NUM_Players] + 1) - average_distance[(tid - 1) + NUM_Players]));
                    guess_row = guess_arr[(tid - 1) + NUM_Players].row + (rand() % ((2 * average_distance[(tid - 1) + NUM_Players] + 1) - average_distance[(tid - 1) + NUM_Players]));
                } while (guess_row < 0 || guess_row > 9 || guess_col < 0 || guess_col > 9);

                guess_store.row = guess_row;
                guess_store.collumn = guess_col;
                guess_arr[guess_count];
                guess_count++;
            }

            printf("%d. guess of player%ld: [%d,%d]\n", k + 1, tid, guess_row, guess_col);
            int distance_sum = 0;
            int distance_avarage = 0;

            for (int i = 0; i < NUM_Players; i++)
            {

                if (tid - 1 != i)
                {

                    distance = abs(loc_arr[i].row - guess_row) + abs(loc_arr[i].collumn - guess_col);

                    printf("the distance with player%d : %d\n", i + 1, distance);

                    distance_array[tid - 1][i][k] = distance;
                    distance_sum += distance;
                    if (distance == 0)
                    {
                        check = true;

                        printf("*******************\n");
                        printf("player %ld won the game\n", tid);
                        printf("*******************\n");
                        break;
                    }

                    count++;
                }
            }

            distance_avarage = (int)distance_sum / (NUM_Players - 1);
            average_distance[avarage_count] = distance_avarage;
            avarage_count++;
        }

        printf("\n");

        turn = (turn + 1) % NUM_Players;
        pthread_mutex_unlock(&turnMutex);
        if (tid == NUM_Players && k == 2 && check == false)
        {
            findMin(distance_array);
        }
        pthread_barrier_wait(&barrier);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   
    if (argc > 2)
    {
        n = atoi(argv[1]);
        NUM_Players=atoi(argv[2]);
    }
    else
    {
      printf("You did not provide the number of players and the size of the matrix; the game is initialized with default values\n");
      n=8;
      NUM_Players=4;
    }
     printf("%dx%d map is created\n", n, n);
    printf("%d threads are created\n", NUM_Players);
    printf("Coordinates of the players are chosen randomly\n");

    gameBoard = (char **)malloc(n * sizeof(char *));
    visited = (bool **)malloc(n * sizeof(bool *));
    for (int i = 0; i < n; i++)
    {
        gameBoard[i] = (char *)malloc(n * sizeof(char));
        visited[i] = (bool *)malloc(n * sizeof(bool));
    }

    loc_arr = (struct location *)malloc(NUM_Players * sizeof(struct location));
    guess_arr = (struct location *)malloc(NUM_Players * repeat * sizeof(struct location));
    average_distance = (int *)malloc(NUM_Players * repeat * sizeof(int));
    minPlayers = (int *)malloc(NUM_Players * sizeof(int));

    distance_array = (int ***)malloc(NUM_Players * sizeof(int **));
    for (int i = 0; i < NUM_Players; i++)
    {
        distance_array[i] = (int **)malloc(NUM_Players * sizeof(int *));
        for (int j = 0; j < NUM_Players; j++)
        {
            distance_array[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    srand((unsigned int)time(NULL));
    initializeGameBoard();
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, NUM_Players);
    pthread_mutex_init(&turnMutex, NULL);

    pthread_t threads[NUM_Players];
    int rc;
    long t;

    for (t = 0; t < NUM_Players; t++)
    {
        rc = pthread_create(&threads[t], NULL, player_threads, (void *)(t + 1));
        if (rc)
        {
            printf("error: pthread_create() failure %d\n", rc);
            return -1;
        }
    }

    for (t = 0; t < NUM_Players; t++)
    {
        pthread_join(threads[t], NULL);
    }

    for (int i = 0; i < n; i++)
    {
        free(gameBoard[i]);
        free(visited[i]);
    }
    free(gameBoard);
    free(visited);
    free(loc_arr);
    free(guess_arr);
    free(average_distance);
    free(minPlayers);
    for (int i = 0; i < NUM_Players; i++)
    {
        for (int j = 0; j < NUM_Players; j++)
        {
            free(distance_array[i][j]);
        }
        free(distance_array[i]);
    }
    free(distance_array);

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&turnMutex);

    return 0;
}
