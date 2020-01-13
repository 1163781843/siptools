#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <signal.h>

class cond;

class lock
{
public:
	lock();
	~lock();
	int mutex_lock();
	int mutex_trylock();
	int mutex_unlock();
	pthread_mutex_t *mutex_get_lock() const;
protected:
private:
	friend class cond;
	pthread_mutex_t *mlock;
};

class cond
{
public:
	cond();
	~cond();

	int cond_signal();
	int cond_broadcast();
	int cond_wait(lock &mutex);
	int cond_timedwait(lock &mutex, const struct timespec *abstime);
	int cond_timedwait(lock &mutex, int nsecond);
	pthread_cond_t *cond_get() const;
protected:
private:
	pthread_cond_t *thread_cond;
};

class thread
{
public:
	int start_thread();
	int start_thread(void *data);
	int start_detached_thread();
	int start_detached_thread(void *data);
	int join_thread();
	virtual void run_thread(void *data);
	void *get_thread_data();
protected:
private:
	pthread_t pthread;
	void *data;
};

#endif
