#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int guess(int pre_guess[], int area, int current_guess[], int guess_arr[], int index_arr)
{
  int i, loop, j;
  int index = 0;
  int guess_index = 0;
  int flag = 0;
  int arr[1000];
  int pre_arr[1000];
  int size_arr = 0;

  srand(time(NULL) + (getpid()*2+19));
  if (pre_guess[0] == -1)
  {                                   // ilk tahmin
    current_guess[0] = rand() % area; // x
    current_guess[1] = rand() % area; // y
  }
  else
  {
    for (loop = 1; loop <= pre_guess[2] + 1; loop++)
    {
      arr[(loop * 8) - 8] = pre_guess[0] - (loop - 1);
      arr[(loop * 8) - 7] = pre_guess[1] - (pre_guess[2] - (loop - 1));

      arr[(loop * 8) - 6] = pre_guess[0] + (loop - 1);
      arr[(loop * 8) - 5] = pre_guess[1] + (pre_guess[2] - (loop - 1));

	  arr[(loop * 8) - 4] = pre_guess[0] + (loop - 1);
      arr[(loop * 8) - 3] = pre_guess[1] - (pre_guess[2] - (loop - 1));

	  arr[(loop * 8) - 2] = pre_guess[0] - (loop - 1);
      arr[(loop * 8) - 1] = pre_guess[1] + (pre_guess[2] - (loop - 1));

      size_arr += 8;
    }

    for (i = 1; i < size_arr; i += 2)
    {
      if (arr[i - 1] <= area && arr[i] <= area && arr[i] >= 0 && arr[i - 1] >= 0)
      {	
        pre_arr[index] = arr[i - 1];
        pre_arr[index + 1] = arr[i];
	index += 2;
      }
    }

      if (guess_arr[0] == -1)
      {
	for (i = 1; i < index; i += 2)
	{
        guess_arr[i - 1] = pre_arr[i - 1];
        guess_arr[i] = pre_arr[i];
        index_arr += 2;
	}
      }
     else
     {
      for (i = 1; i < size_arr; i += 2)
      {
        for (j = 1; j < index - 1; j += 2)
        {
          if (pre_arr[i - 1] == guess_arr[j - 1] && pre_arr[i] == guess_arr[j]
		   && pre_arr[i - 1] != guess_arr[ guess_index -2 ] && pre_arr[i] != guess_arr[guess_index -1 ])
          { 
            guess_arr[guess_index] = pre_arr[i - 1];
            guess_arr[guess_index + 1] = pre_arr[i];
            guess_index += 2;
            flag++;
          }
        }
      }
      
      if (flag == 0)
      {
        for (i = 1; i < index - 1; i += 2)
        {
          guess_arr[index_arr - 1] = pre_arr[i - 1];
          guess_arr[index_arr] = pre_arr[i];
          index_arr += 2;
        }
      }
      
      else
      {
        index_arr = guess_index;
      }
    }

    srand(time(NULL) + getpid());
    int random = rand() % index_arr;
    
    if (random % 2 == 0)
    {
      current_guess[0] = guess_arr[random];
      current_guess[1] = guess_arr[random + 1];
    }
    else
    {
      current_guess[0] = guess_arr[random - 1];
      current_guess[1] = guess_arr[random];
    }
  }
  for (i = 0; i < index_arr - 1; i += 2)
  return index_arr;
}


void randomCoordinate(int arr[], int mapSize) {
	srand(time(NULL) + getpid());
    	arr[0] = rand() % mapSize;
	arr[1] = rand() % mapSize;
}


int calculateManhattan(int player[], int guess[]) {
return abs(player[0]- guess[0]) + abs(player[1]-guess[1]);
}


void displayMap(int player1[], int player2[], int guess_arr[], int index_arr,int mapSize) {
    for (int i = -1; i <= mapSize; i++) {
        for (int j = -1; j <= mapSize; j++) {
            if (i == -1 || i == mapSize) {
                printf("- ");
            } else if (j == -1 || j == mapSize) {
                printf("| ");
            } else {
                int isPlayer1 = (player1[0] == i && player1[1] == j);
                int isPlayer2 = (player2[0] == i && player2[1] == j);
                int isGuess = 0;

                for (int k = 0; k < index_arr; k += 2) {
                    if (guess_arr[k] == i && guess_arr[k + 1] == j) {
                        isGuess = 1;
                        break;
                    }
                }
                if (isPlayer1) {
                    printf("1 ");
                } else if (isPlayer2) {
                    printf("2 ");
                } else if (isGuess) {
                    printf("X ");
                } else {
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}


int main (int argc, char* argv[]){

srand(time(NULL) + getpid());

int mapSize = atoi(argv[1]);

int fd[2][2]; 
int i;

for (i=0; i<2;i++){
 if (pipe(fd[i])<0){
  printf("Pipe could not be initialized");
  return 1;
 }
}

int pid = fork();

if (pid <0 ){
 printf("Fork could not be executed");
 return 2;
}


if (pid > 0 ){
	int p1Distances[3];
	int p2MinDistance; 
	int player1[2];
	int player2Coor[2];

	int pre_guess[3] = {-1, -1, 1};
  	int guess_arr[1000];
  	guess_arr[0] = -1;
  	int index_arr = 0;
	
	randomCoordinate(player1,mapSize);
	
	
	printf("%dx%d map is created\n", mapSize, mapSize);
	printf("A child process created\n");
	printf("Coordinates of the players are chosen randomly\n");
	
	
	if (read(fd[1][0], &player2Coor,sizeof(int)*2) < 0){ return 5;}
	printf("player1: [%d,%d] , player2: [%d,%d]\n", player1[0],player1[1], player2Coor[0],player2Coor[1]);
	
	displayMap(player1, player2Coor, guess_arr, index_arr,mapSize);
	printf("Game launches -->");
for(i=1; i<4; i++){	
  
  printf("\n---------- Round-%d ----------\n", i);
  	
	int pGuess[2];
	
	int distance;
	
	index_arr = guess(pre_guess, mapSize, pGuess, guess_arr, index_arr);
	
	close(fd[0][0]);   
	close(fd[1][1]);   
	
	
	
	if (write(fd[0][1], pGuess, sizeof(int) *2 )<0) { return 5; }
	
	if (read(fd[1][0], &distance,sizeof(int)) < 0){ return 5;}
	
	p1Distances[i-1]=distance;
	
	if (distance == 0) {
	displayMap(player1, player2Coor, guess_arr, index_arr,mapSize);
    	printf("\n******************************************\n");
	printf("player1 won the game!!!\n");
	printf("******************************************\n");
    	kill(pid, SIGTERM);
    	exit(0);
} 
	else{
	pre_guess[2] = distance;
	pre_guess[0] = pGuess[0];
	pre_guess[1] = pGuess[1];
	}
	
	
	if (read(fd[1][0], pGuess, sizeof(int) *2) < 0){ return 5;}
	printf("%d.guess of player2: [%d,%d]\n", i,pGuess[0], pGuess[1]);
	
 	distance = calculateManhattan(player1, pGuess);
	printf("the distance with player1: %d\n", distance);
	
 	if (distance == 0) {
 	displayMap(player1, player2Coor, guess_arr, index_arr,mapSize);
    	printf("******************************************\n");
	printf("player2 won the game!!!\n");
	printf("******************************************\n");
    	kill(pid, SIGTERM);
    	exit(0);
}
	
	if(write(fd[0][1], &distance, sizeof(int) ) <0 ) { return 5; }
	 displayMap(player1, player2Coor, guess_arr, index_arr,mapSize);
	
}


if (read(fd[1][0], &p2MinDistance, sizeof(int) *1) < 0){ return 5;}

int p1MinDistance=p1Distances[0];

for(int i=0;i<3;i++) {
  if(p1Distances[i]<p1MinDistance) {
  p1MinDistance=p1Distances[i];
  }
}

if(p1MinDistance < p2MinDistance) {
printf("\n******************************************\n");
printf("player1 minimum distance: %d , player2 minimum distance: %d\n",p1MinDistance, p2MinDistance);
printf("player1 won the game!!!\n");
printf("******************************************\n");

}
else if(p1MinDistance > p2MinDistance){
printf("******************************************\n");
printf("player1 distance: %d , player2 distance: %d\n",p1MinDistance, p2MinDistance);
printf("player2 won the game!!!\n");
printf("******************************************\n");
}
else{
printf("\n******************************************\n");
printf("player1 distance: %d , player2 distance: %d\n",p1MinDistance, p2MinDistance);
printf("game is a draw!!!\n");
printf("******************************************\n");
}


}


else if (pid == 0 ){
	int distanceArr[3];
	int player2[2];
	int minDistance;

	int pre_guess[3] = {-1, -1, 1};
  	int guess_arr[200];
  	guess_arr[0] = -1;
  	int index_arr = 0;

	randomCoordinate(player2 , mapSize);
	
	if(write(fd[1][1], &player2, sizeof(int)*2) <0 ) { return 5; }
	for(i=1; i<4; i++) {	
	int cGuess[2];
	int distance;
	
	close(fd[0][1]);
	close(fd[1][0]);
	
	
	if (read(fd[0][0], cGuess, sizeof(int) *2) < 0){ return 5;}
	
	printf("%d.guess of player1: [%d,%d]\n",i, cGuess[0], cGuess[1]);
	
	distance = calculateManhattan(player2 , cGuess );
 	printf("the distance with player2:%d\n", distance);
 	
	if(write(fd[1][1], &distance, sizeof(int)) <0 ) { return 5; }
	
	index_arr = guess(pre_guess, mapSize, cGuess, guess_arr, index_arr); 
	
	if(write(fd[1][1], cGuess, sizeof(int) *2 )<0) { return 5; }

	if (read(fd[0][0], &distance,sizeof(int)) < 0){; return 5;}

	distanceArr[i-1]=distance;
	
	minDistance=distanceArr[0];
	for(int a=0;a<3;a++){
	if(distanceArr[a]< minDistance)
	{
	minDistance=distanceArr[a];
	}
	
	pre_guess[2] = distance;
	pre_guess[0] = cGuess[0];
	pre_guess[1] = cGuess[1];
	
}
}
if(write(fd[1][1], &minDistance, sizeof(int) *1 )<0) { return 5; }
}
}
