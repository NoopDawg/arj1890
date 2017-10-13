#define _BSD_SOURCE
#include <stdlib.h>
#include <unistd.h> //getopt
#include <stdio.h> //fprintf
#include "display.h"


//Shortcut function to print usage information to stderr
void printusage(){
	fprintf (stderr,"usage: \nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str] [-v %%vac] [-e %%end] \n");
}

//Intakes arguments and checks for errors.
//prints to stderr 
void getargs(int argc, char* argv[], int* cycmax, int* delay, int* dim, double* vac, double* str, double* end){
	
	int c;
	int tmp;  
	while ((c = getopt(argc, argv, "ht:c:d:s:v:e:")) != -1)
		switch (c)
		{
		case 'h':
			fprintf(stderr,"usage: \nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s %%str]"
			" [-v %%vac] [-e %%end] \n");
			fprintf(stderr,"Option\t\tDefault\tExample\tDescription\n");
			fprintf(stderr,"'-h'\t\tNA\t-h\tprint this usage message.\n");
			fprintf(stderr,"'-t N'\t\t900000\t-t 5000\tmicroseconds cycle delay.\n");
			fprintf(stderr,"'-c N'\t\t-1\t-c4\tcount cycle maximum value.\n");
			fprintf(stderr,"'-d dim'\t15\t-d 7\twidth and height dimension.\n");
			fprintf(stderr,"'-s %%str'\t50\t-s 30\tstrength of preference.\n");
			fprintf(stderr,"'-v %%vac'\t20\t-v30\tpercent vacancies.\n");
			fprintf(stderr,"'-e %%end'\t60\t-e75\tpercent Endline braces. Others want Newline.\n");
			exit(EXIT_SUCCESS);
		case 't':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 0){
				*delay = tmp;
				break;
			}else{
				break;
			}
		case 'c':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 0){
				*cycmax = tmp;	
				break;
			}else{
				fprintf(stderr, "count (%d) must be a non-negative integer.\n", tmp);
				printusage();
				exit(1+EXIT_FAILURE);
			}
		case 'd':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 4 && tmp < 40){
				*dim = tmp;
				break;
			}else{
				fprintf(stderr, "dimension (%d) must be a value in [5...39]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case 's':
			tmp = (double)strtol(optarg, NULL, 10);
			if (tmp > 0 && tmp <100 ){
				*str = tmp;
				break;
			}else{
				fprintf(stderr, "preference strength (%d) must be a value in [1...99]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case 'v':
			tmp = (double)strtol(optarg, NULL, 10);
			if (tmp > 0 && tmp <100 ){
				*vac = tmp;
				break;
			}else{
				fprintf(stderr, "vacancy (%d) must be a value in [1...99]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case 'e':
			tmp = (double)strtol(optarg, NULL, 10);
			if (tmp > 0 && tmp <100 ){
				*end = tmp;
				break;
			}else{
				fprintf(stderr, "endline proportion (%d) must be a value in [1...99]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case '?':
			if (optopt == 't' || optopt == 'c'|| optopt == 'd' || optopt == 's' ||
		 	optopt == 'v' 	|| optopt == 'e'){
				printusage();
				exit(EXIT_FAILURE);
			}else{
				printusage();
				exit(EXIT_FAILURE);
			}
	}
}

//Initializes the 2D array with the specified parameters
//Uses random() function and modulo to generate.
void initsim(int dim, char room[dim][dim], double vac, double end){
	int places = dim*dim;
	int agentsn = (int) places -(places * (vac/100));
	int endnum = (int) agentsn * (end/100);
	int newnum = agentsn - endnum;

	int i = 0;
	int r;
	int c;
	int pos;

	for (int k = 0; k < dim; k++){
		for (int j = 0; j < dim; j++){
			room[k][j] = '.';
		}
	}
	while (i < endnum){
		pos = random() % places;
		r = pos / dim;
		c = pos % dim;
		if (room[r][c] == '.'){
			room[r][c] = 'e';
			i++;
		}else{
			continue;
		}
	}
	i = 0;
	while (i < newnum){
		pos = random() % places;
		r = pos / dim;
		c = pos % dim;
		if (room[r][c] == '.'){
			room[r][c] = 'n';
			i++;
		}else{
			continue;
		}
	}
}

// Checks neighbors and comes up with happiness value
double checkhappy(int dim, char room[dim][dim], int x, int y){
	double like = 0;
	double people = 0;
	
	for (int r = x-1; r <= x+1; r++){
		for (int c = y-1; c <= y+1; c++){
			if (!(c==y && x == r) && r >= 0 && c >= 0
			&& r < dim && c < dim){
				if ((room[r][c] == room[x][y]) && room[x][y] != '.'){
					like++;
					people++;
				}else if((room[r][c] == '.') || (room[r][c] == 'P')){
					continue;
				}else{
					people++;
				}				
			}			
		}
	}
	if (people == 0){
		return 1;
	}else{
		return like/people;
	}
}
//finds first empty location. converts coordinates to 
// int to be deciphered later.
int findvac(int dim, char room[dim][dim]){
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			if (room[i][j] == '.'){
				return dim*i + j;
			}
		}
	}
	return -1;
}


//prints 2D array
void display(int dim, char room[dim][dim]){
	for (int k = 0; k < dim; k++){
		for (int j = 0; j < dim; j++){
			printf("%c", room[k][j]);
		}
		printf("\n");
	}		
}

//Algorithm to move Agents
//Unhappy agents are moved to first open location
//Placeholders are left behind to keep track
//Placeholders are replaced with '.' at end of function.
int moveAgents(int dim, char room[dim][dim], double str){
	int loc = findvac(dim, room);
	int moves = 0;
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			if (loc == -1)
				break;
			if ((checkhappy(dim, room, i, j) < str/100) && (room[i][j] != '.')){
				room[loc/dim][loc%dim] = room [i][j];
				room[i][j] = 'P';
				loc = findvac(dim, room);
				moves++;
			}else{
				continue;
			}
		}
	}
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			if (room[i][j] == 'P'){
				room[i][j] = '.';
			}else{
				continue;
			}
		}
	}
	return moves;
}

//cursor function to display room on top left of termnial screen.
void infdisp(int dim, char room[dim][dim]){
	for (int k = 0; k < dim; k++){
		for (int j = 0; j < dim; j++){
			set_cur_pos(k, j);
			put(room[k][j]);
		}
	}		
}

//For each entry that is not empty the happiness is checked and summed
//Sum is divided by the total number of agents and returned as a double
double teamHappiness(int dim, char room[dim][dim]){
	double happysum = 1;
	double pplnum = 10;
		
	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){
			if (room[i][j] != '.'){
				happysum = happysum + checkhappy(dim,room,  i,j);
				pplnum++;			
			}
		}
	}
	return happysum/pplnum;	
}


//Main function. Initializes variables with defaults and calls other functions to 
//manipulate variables.
int main(int argc, char* argv[]){

	int cycmax = -1;
	int delay = 900000;
	int dim = 15;
	double vac = 20;
	double str = 50;
	double end = 60;

	getargs(argc, argv, &cycmax, &delay, &dim, &vac, &str, &end);
	
	char room[dim][dim];
	srandom(41);
	initsim(dim, room, vac, end);
	
	if (cycmax != -1){
		int moves = 0;
		for (int i = 0; i <= cycmax; i ++){
			display(dim, room);
			printf("cycle: %d \n", i);
			printf("moves this cycle: %d.  \n", moves);
			printf("teams' \"happiness\": %.4f\n", teamHappiness(dim, room));
			printf("dim: %d, %%strength of preference: %.1f%%,"
				" %%vacancy: %.1f%% %%end: %.1f%% \n",
				dim, str, vac, end);
			moves = moveAgents(dim, room, str);
		}
	}else{
		clear();
		int moves = 0;		
		int i = 0;
		while(1){
			infdisp(dim, room);
			printf("\ncycle: %d \n", i);
			printf("moves this cycle: %d.  \n", moves);
			printf("teams' \"happiness\": %.4f\n", teamHappiness(dim,room));
			printf("dim: %d, %%strength of preference: %.1f%%,"
				" %%vacancy: %.1f%% %%end: %.1f%% \n",
				dim, str, vac, end);
			moves = moveAgents(dim, room, str);
			i++;
			usleep(delay);
		}
	}		
}






