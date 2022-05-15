#ifndef WATCH_DOG_H
#define WATCH_DOG_H

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

typedef struct Watch_Dog Watch_Dog;
typedef struct Control_Unit{
    pthread_mutex_t unit_mutex;
    pthread_t thread;
} Control_Unit;

Watch_Dog* Watch_Dog_new(size_t number_of_puppies);
Watch_Dog* Watch_Dog_delete(Watch_Dog* w_d);
int_fast16_t Watch_Dog_add_puppy(Watch_Dog* w_d, Control_Unit* c_t);
int_fast16_t Watch_Dog_remove_puppy(Watch_Dog* c_t);
int_fast16_t Watch_Dog_start(Watch_Dog* w_d);
int_fast16_t Watch_Dog_finish(Watch_Dog* w_d);

#endif