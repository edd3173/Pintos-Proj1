#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>


int main(int argc, char** argv){

	int first=atoi(argv[0]);
	int second=atoi(argv[1]);
	int third=atoi(argv[2]);
	int fourth=atoi(argv[3]);
	int fifth=atoi(argv[4]);

	//printf("argvs : %d %d %d %d %d\n",first,second,third,fourth,fifth);

	int fib=0; int max=0;

	//printf("Performing fibo(%d)\n",second);
	fib=fibonacci(second);

	//printf("Performing max(%d,%d,%d,%d)\n",second,third,fourth,fifth);
	max=max_of_four_int(second,third,fourth,fifth);

	printf("%d %d\n",fib,max);
	
	return EXIT_SUCCESS;
}
