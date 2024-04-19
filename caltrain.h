#ifndef CALTRAIN_H_
#define CALTRAIN_H_

#include <pthread.h>

struct station {
	pthread_mutex_t mutex;
	pthread_cond_t train_arrived;
	pthread_cond_t train_loaded;
	int passenger_waiting;
	int available_seats;
};


void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);

#endif
