#include "stats.h"

// Exercise 3: fix concurrency bugs by Monitor

// FIXME:
// These statistics should be implemented as a Monitor,
// which keeps track of kvserver's status

int n_writes = 0;   // number of writes
int n_reads = 0;    // number of reads
int n_deletes = 0;  // number of deletes
int n_increases = 0;// number of increases
int UPDATING = 0;

// TODO: define your synchronization variables here
// (hint: don't forget to initialize them)

pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t statsBusy = PTHREAD_COND_INITIALIZER;

// FIXME: implementation below is not thread-safe.
// Fix this by implementing them as a Monitor.

void inc_write() {
	pthread_mutex_lock(&stats_mutex);
	UPDATING = 1;
    n_writes++;
    UPDATING = 0;
    pthread_cond_signal(&statsBusy);
    pthread_mutex_unlock(&stats_mutex);
}

void inc_read() {
	pthread_mutex_lock(&stats_mutex);
	UPDATING = 1;
    n_reads++;
    UPDATING = 0;
    pthread_cond_signal(&statsBusy);
    pthread_mutex_unlock(&stats_mutex);
}

void inc_delete() {
	pthread_mutex_lock(&stats_mutex);
	UPDATING = 1;
    n_deletes++;
    UPDATING = 0;
    pthread_cond_signal(&statsBusy);
    pthread_mutex_unlock(&stats_mutex);
}

void inc_increase() {
	pthread_mutex_lock(&stats_mutex);
	UPDATING = 1;
    n_increases++;
    UPDATING = 0;
    pthread_cond_signal(&statsBusy);
    pthread_mutex_unlock(&stats_mutex);
}


int get_writes() {
	pthread_mutex_lock(&stats_mutex);
	while(UPDATING){
		pthread_cond_wait(&statsBusy, &stats_mutex);
	}
	pthread_mutex_unlock(&stats_mutex);
    return n_writes;
}

int get_reads() {
	pthread_mutex_lock(&stats_mutex);
	while(UPDATING){
		pthread_cond_wait(&statsBusy, &stats_mutex);
	}
	pthread_mutex_unlock(&stats_mutex);
    return n_reads;
}

int get_deletes() {
	pthread_mutex_lock(&stats_mutex);
	while(UPDATING){
		pthread_cond_wait(&statsBusy, &stats_mutex);
	}
	pthread_mutex_unlock(&stats_mutex);
    return n_deletes;
}

int get_increases() {
	pthread_mutex_lock(&stats_mutex);
	while(UPDATING){
		pthread_cond_wait(&statsBusy, &stats_mutex);
	}
	pthread_mutex_unlock(&stats_mutex);
    return n_increases;
}
