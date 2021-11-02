#include "clockgame.h"

void static clockgame_run(clockgame_t *clockgame){
	/* Va aumentado el contado time cada 0,1 segundo */

	struct timespec req;
   struct timespec rem;

	req.tv_sec = 0;
   req.tv_nsec = 100000000;

	while(true){
		if(clockgame->state == CLOCK_RUN)
			clockgame->time++;
		nanosleep(&req,&rem);
	}
}

void clockgame_init(clockgame_t *clockgame){
	pthread_create(&(clockgame->th), NULL, (void*)&clockgame_run, clockgame);
}

void clockgame_restore(clockgame_t *clockgame){
	clockgame->time = 0;
}

void clockgame_start(clockgame_t *clockgame){
	clockgame->state = CLOCK_RUN;
}

void clockgame_stop(clockgame_t *clockgame){
	clockgame->state = CLOCK_STOP;
}

/* retorna el tiempo actual */
uint32_t clockgame_time(clockgame_t *clockgame){
	return clockgame->time;
}
