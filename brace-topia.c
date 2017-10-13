#include <stdlib.h>

#include <unistd.h> //getopt
#include <stdio.h> //fprintf
#define _BSD_SOURCE



void printusage(){
	fprintf (stderr,"usage: \nbrace_battle [-h] [-t N] [-c N] [-d dim] [-s s] [-v vac] [-e end] \n");
}

void getargs(int argc, char* argv[], int* cycmax, int* delay, int* dim, int* vac, int* str, int* end){
	
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
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 0 && tmp <100 ){
				*str = tmp;
				break;
			}else{
				fprintf(stderr, "preference strength (%d) must be a value in [1...99]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case 'v':
			tmp = (int)strtol(optarg, NULL, 10);
			if (tmp > 0 && tmp <100 ){
				*vac = tmp;
				break;
			}else{
				fprintf(stderr, "vacancy (%d) must be a value in [1...99]\n", tmp);
				printusage();
				exit(1 + EXIT_FAILURE);
			}
		case 'e':
			tmp = (int)strtol(optarg, NULL, 10);
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

int main(int argc, char* argv[]){

	int cycmax = -1;
	int delay = 900000;
	int dim = 15;
	int vac = 20;
	int str = 50;
	int end = 60;

	getargs(argc, argv, &cycmax, &delay, &dim, &vac, &str, &end);
	printf("The value are parameters are:\n delay: %d\n dim: %d\n str: %d\n"
		" vac: %d\n end: %d\n ",delay, dim, str, vac, end);
}






