#define MAIN_3
#ifdef MAIN_3

#include "sched.h"

int main(){

	/* Init the Schedular*/
	SCHED_Init();

	/* Start sched */
    SCHED_start();


	while (1);
	return 0;
}









#endif /*MAIN_3*/