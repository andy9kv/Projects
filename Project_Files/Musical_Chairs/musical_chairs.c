
/***********************|
| Author: Andrew Morato |
| March 30, 2018        |
| musical_chairs.c      |
|***********************/

#include "musical_chairs.h"


/*If no command line argument is given that is 2 or greater,
the default number of players is set to 10.
Sample run: ./Musical_Chairs 5     results in 5 players*/
int main(int argc, char const *argv[]){

	int numberOfPlayers;
	/*Reads in the command line argument (if any), and sets
	the global count of remaining chairs accordingly*/
	if (argv[1] == NULL){ //no argument passed
		numberOfPlayers = DEFAULT_PLAYERS;
	}
	else if(atoi(argv[1]) < 2){ //invalid number of players
		numberOfPlayers = DEFAULT_PLAYERS;
	}
	else{ //valid number of players
		int N = atoi(argv[1]);
		numberOfPlayers = N;
	}

	/*Initializes the array of "chair objects" to be acquired by
	players (threads) and the linkd list that keeps track of players
	(threads). These lists keep track of players that are left
	and the chairs they sat in. As the number of remaining chairs
	shrinks, so will the chair array indexing, making "less" chairs*/
	remaining_chairs = numberOfPlayers - 1;
	chairs = malloc(sizeof(struct chair *) * remaining_chairs);
	players = init_list();
	int i = 0;
	round_over = 0;
	music = 1;
	for(i = 0; i < remaining_chairs; i++){
		/*Initializes each chair "object"*/
		chairs[i] = malloc(sizeof(struct chair));
		chairs[i]->chairID = i + 1; //sets the ID
		chairs[i]->threadID = 0;
		chairs[i]->taken = CHAIR_FREE; //sets the chair to free
		pthread_mutex_init (&chairs[i]->lock, NULL);
		/*adds node to keep track of each player to the player
		list. The player (thread) ID is given by the iterator*/
		append_node(players, (i+1));
	}
	/*adds a tracking spot for another player (1 more P than C)*/
	append_node(players, (i+1));

	/*launches all the threads*/
	for(i = 0; i < remaining_chairs + 1; i++){
		pthread_t tid;
		long long ID = (long long) (i + 1);
    	pthread_create(&tid, NULL, play_game, (void *) ID);
	}

	/*calls the emcee function which plays, and regulates, the game*/
	emcee();

	/*frees allocated memory and destroys locks*/
	free_memory(numberOfPlayers - 1);

	/*end of the program*/
	return 0;
}


/*function loops for the number of chairs, regulating the game played
by each thread (makes use of several flags).*/
void emcee(){

	/*announcement that the game has begun!*/
	printf("BEGIN %d players\n\n", remaining_chairs + 1);

	int i, j;
	/*Each iteration of the loop is a round*/
	for(j = 1; remaining_chairs > 0; j++) {

		/*prints the current round*/
		printf("round %d\n", j);

		round_over = 0; //sets the round as non over
		music = 0; //turns the music OFF!
		printf("music off\n");

		/*waits until the players (threads) finish grabbing chairs
		and one thread is left without a chair*/
		while(!round_over){}

		/*removes the player from the player list*/
		int loser = remove_completed(players);

		/*turns the music ON, then waits until the remaining players
		have gotten out of their chairs (released the locks), then
		begins the next round with one chair less*/
		music = 1;

		/*waits until all the players have gotten up from their
		chairs, i.e. all threads released thier chair objects*/
		while(chair_status() != ALL_CHAIRS_FREE){}

		/*prints the results of each round*/
		for(i = 0; i < remaining_chairs; i++){
			struct chair * chair = chairs[i];
			printf("P%d sat in C%d\n", chair->threadID, 
			                           chair->chairID);
		}

		/*removes a chair*/
		remaining_chairs = remaining_chairs - 1;

		printf("P%d lost\n\n", loser);
	}

	/*prints the winner and ends the game!*/
	printf("P%d wins!\n", players->next->threadID);
	printf("END\n");

}


/*Threads (players) begin here. This function is their attempt to
grab a chair before the other players. The argument, although of
type (void *), holds an integer marking the threadID*/
void* play_game(void *vptr){
	long long param = (long long) vptr;
	int threadID = (int) param;
	/*loops while the game is on*/
	while(1){
		/*waits until the music is turned off, then races to
		grab a chair against the other players (threads)*/
		while(music){}
		/*checks if all the other chairs have been taken. If they
		have, the thread (player) returns (player has lost)*/
		if(chair_status() == ALL_CHAIRS_TAKEN) {
			set_completed(players, threadID);
			round_over = 1;
			return 0; //leaves the game
		}
		/*randomly finds a chair*/
		int random_index = rand() % remaining_chairs;
		struct chair * chair = chairs[random_index];
		/*attempts to grab (sit in) the chair if availabe*/
		if(chair->taken == CHAIR_FREE){
			int failure = pthread_mutex_trylock(&chair->lock);
			/*if the lock was successfully grabbed (i.e. no other
			thread (player) grabbed the chair before this one did,
			then set the chair as taken and wait until the music
			stops. Once the round is over, the chair (lock) is
			released and this thread reruns the loop*/
			if(!failure){
				chair->taken = CHAIR_TAKEN;
				chair->threadID = threadID;
				/*waits until the round is over (music is back on)*/
				while(!music){}
				/*gets up from its chair for the next round!*/
				pthread_mutex_unlock(&chair->lock);
				chair->taken = CHAIR_FREE;
			} //else, keep looping, trying random chairs
		}
	}
}


/*-----------------------------helpers-----------------------------*/

/*Iterates through the global array of chair objects, and,
  returns 0 if all chairs have been taken
  returns 1 if all chairs are free
  returns 2 if neither*/
int chair_status(){
	int i, sum = 0;
	/*counts the number of taken chairs*/
	for(int i = 0; i < remaining_chairs; i++){
		sum += chairs[i]->taken;
	}
	/*no chair has been taken*/
	if(sum == 0) {return 1;} //all chairs are free
	if(sum == remaining_chairs) {return 0;} //all chairs are taken
	return 2; //some chairs are taken and some remain free
}

/*frees the allocated memory and destorys the locks created. The int
parameter refers to the original amount of chairs*/
void free_memory(int original_size){
	/*frees the entire thread-tracking linked list*/
	destroy_list(players);
	/*destroys each lock and frees each chair "object"*/
	int i;
	for(i = 0; i < original_size; i++){
		pthread_mutex_destroy(&(chairs[i]->lock));
		free(chairs[i]);
	}
	/*frees the pointer to the chair array*/
	free(chairs);
}

/*prints the current state of all variables in memory*/
void print_info(){
	print(players);
	printf("Remaining Chairs: %d\n", remaining_chairs);
	printf("Music: %d\n\n", music);
	printf("Chairs Status: %d\n", chair_status());
	printf("Round Over: %d\n", round_over);
	int i;
	printf("Chair List\n");
	printf("Chair ID:  ");
	for(i = 0; i < remaining_chairs; i++){
		printf("%d ", chairs[i]->chairID);
	}
	printf("\nThread ID: ");
	for(i = 0; i < remaining_chairs; i++){
		printf("%d ", chairs[i]->threadID);
	}
	printf("\nTaken:     ");
	for(i = 0; i < remaining_chairs; i++){
		printf("%d ", chairs[i]->taken);
	}
	printf("\n\n");
}