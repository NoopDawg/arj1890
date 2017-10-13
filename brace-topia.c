#define _BSD_SOURCE
#include <stdlib.h>
#include <unistd.h> //getopt
#include <stdio.h> //fprintf



void printusage(){
	fprintf (stderr,"usage: \nbrace_battle [-h] [-t N] [-c N] [-d dim] [-s s] [-v vac] [-e end] \n");
}

void getargs(int argc, char* argv[], int* cycmax, int* delay, int* dim, double* vac, double* str, double* end){
	
	int c;
	int tmp;  
	while ((c = getopt(argc, argv, "ht:c:d:s:v:e:")) != -1)
		switch (c)
		{
		case 'h':
			printf("usage: \nbrace-topia [-h] [-t N] [-c N] [-d dim] [-s s] [-v vac] [-e end] \n");
			printf("Option\t\tDefault\tExample\tDescription\n");
			printf("'-h'\t\tNA\t-h\tprint this usage message.\n");
			printf("'-t N'\t\t90000\t-t 5000\tmicroseconds cycle delay.\n");
			printf("'-c N'\t\t-1\t-c4\tcount cycle maximum value.\n");
			printf("'-d dim'\t15\t-d 7\twidth and height dimension.\n");
			printf("'-s %%str'\t50\t-s 30\tstrength of preference.\n");
			printf("'-v %%vac'\t20\t-v30\tpercent vacancies.\n");
			printf("'-e %%end'\t60\t-e75\tpercent Endline braces. Others want Newline.\n");
			break;
		case 't':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 0){
				*delay = tmp;
				break;
			}else if (tmp < 0) {
				fprintf(stderr, "Cycle delay time (%d) must be a positive integer\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}else{
				break;
			}
		case 'c':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp >= 0){
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
			if (optopt == 't' || optopt == 'c'|| optopt == 'd' || optopt == 's' || optopt == 'v' 					|| optopt == 'e')
			fprintf (stderr, "brace-topia: invalid option -- '%c' \n", optopt);
			printusage();
	}
}


void initsim(int dim, char room[dim][dim], double vac, double end){
	int places = dim*dim;
	int agentsn = (int) places -(places * (vac/100));
	int endnum = (int) agentsn * (end/100);
	int newnum = agentsn - endnum;

	printf("places: %d, Agents: %d, Endlines: %d, Newlines: %d\n", places,
		 agentsn, endnum, newnum);
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
	for (int k = 0; k < dim; k++){
		for (int j = 0; j < dim; j++){
			printf("%c", room[k][j]);
		}
		printf("\n");
	}
}


int main(int argc, char* argv[]){

	int cycmax = -1;
	int delay = 900000;
	int dim = 15;
	double vac = 20;
	double str = 50;
	double end = 60;

	getargs(argc, argv, &cycmax, &delay, &dim, &vac, &str, &end);
		printf("The value are parameters are:\n delay: %d\n dim: %d\n str: %.1f\n"
		" vac: %.1f\n end: %.1f\n ",delay, dim, str, vac, end);
	
	char room[dim][dim];
	srandom(41);
	initsim(dim, room, vac, end);
}






