#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station)
{
    station->passenger_waiting = 0;
    station->available_seats = 0;    
    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->train_arrived, NULL);
    pthread_cond_init(&station->train_loaded, NULL);
}

void station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->mutex);
    //test
    
    // Notify passengers about available seats
    station->available_seats = count*2;
    pthread_cond_signal(&station->train_arrived);
    
    // Wait for passengers to board if there are available seats
    while (station->available_seats > 0 && station->passenger_waiting > 0)
    {
        pthread_cond_wait(&station->train_loaded, &station->mutex);
    }
    station->available_seats = 0; 
    pthread_mutex_unlock(&station->mutex);
}


void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    
    station->passenger_waiting++;
    
    while (station->available_seats == 0||station->available_seats % 2 == 1)
    {
        pthread_cond_wait(&station->train_arrived, &station->mutex);
    }
    
    
    station->available_seats--;
    
    pthread_mutex_unlock(&station->mutex);
}

void station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);
    //seat taken and passenger sat down 
    station->available_seats--;
    station->passenger_waiting--;
    
    if (station->available_seats == 0 || station->passenger_waiting == 0)
    {
        pthread_cond_signal(&station->train_loaded);
    }
    //for next train 
    pthread_cond_signal(&station->train_arrived);
    pthread_mutex_unlock(&station->mutex);
}





