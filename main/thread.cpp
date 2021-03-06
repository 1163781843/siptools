#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include <thread.h>

#include <errcode.h>
#include <logger.h>

lock::lock()
{
	int status = errorcode::SIPTOOLS_SUCCESS;

	pthread_mutexattr_t mattr;
	status = pthread_mutexattr_init(&mattr);
	if (status) {
		log_print(log::log_error, "pthread_mutexattr_init failure!\n");
	}

	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);

	mlock = new pthread_mutex_t;
	if (nullptr == mlock) {
		goto end;
	}

	status = pthread_mutex_init(mlock, &mattr);
	if (status) {
		log_print(log::log_error, "pthread_mutex_init failure!\n");
	}

end:
	pthread_mutexattr_destroy(&mattr);
}

lock::~lock()
{
	pthread_mutex_destroy(mlock);

	delete mlock;
	mlock = nullptr;
}

int lock::mutex_lock()
{
	return pthread_mutex_lock(mlock);
}

int lock::mutex_trylock()
{
	return pthread_mutex_trylock(mlock);
}

int lock::mutex_unlock()
{
	return pthread_mutex_unlock(mlock);
}

pthread_mutex_t *lock::mutex_get_lock() const
{
	return mlock;
}

cond::cond()
{
	thread_cond = new pthread_cond_t;

	pthread_cond_init(thread_cond, NULL);
}

cond::~cond()
{
	pthread_cond_destroy(thread_cond);

	delete thread_cond;
	thread_cond = nullptr;
}

int cond::cond_signal()
{
	return pthread_cond_signal(thread_cond);
}

int cond::cond_broadcast()
{
	return pthread_cond_broadcast(thread_cond);
}

int cond::cond_wait(lock & mutex)
{
	return pthread_cond_wait(thread_cond, mutex.mlock);
}

int cond::cond_timedwait(lock & mutex, int nsecond)
{
	struct timeval t;
	struct timespec ts;

	gettimeofday(&t, NULL);
	ts.tv_sec = t.tv_sec;
	ts.tv_nsec= t.tv_usec * 1000;

	ts.tv_sec += nsecond / 1000000000;
	ts.tv_nsec+= nsecond % 1000000000;
	if (ts.tv_nsec >= 1000000000) {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec += 1;
	}

	return pthread_cond_timedwait(thread_cond, mutex.mlock, &ts);
}

int cond::cond_timedwait(lock & mutex, const struct timespec *abstime)
{
	return pthread_cond_timedwait(thread_cond, mutex.mlock, abstime);
}

pthread_cond_t *cond::cond_get() const
{
	return thread_cond;
}

static void *start_routine(void *data)
{
	thread *thd = static_cast<thread *>(data);

	thd->run_thread(thd->get_thread_data());
}

int thread::start_thread()
{
	pthread_attr_t attr;
	int status = errorcode::SIPTOOLS_SUCCESS;

	this->data = nullptr;

	status = pthread_attr_init(&attr);
	if (status) {
		log_print(log::log_error, "pthread_attr_init failure!\n");
		return status;
	}

	status = pthread_create(&pthread, &attr, start_routine, this);
	if (status) {
		log_print(log::log_error, "pthread_create failure!\n");
		return status;
	}

	pthread_attr_destroy(&attr);

	return status;
}

int thread::start_thread(void *data)
{
	pthread_attr_t attr;
	int status = errorcode::SIPTOOLS_SUCCESS;

	if (data) {
		this->data = data;
	} else {
		this->data = nullptr;
	}

	status = pthread_attr_init(&attr);
	if (status) {
		log_print(log::log_error, "pthread_attr_init failure!\n");
		return status;
	}

	status = pthread_create(&pthread, &attr, start_routine, this);
	if (status) {
		log_print(log::log_error, "pthread_create failure!\n");
		return status;
	}

	pthread_attr_destroy(&attr);

	return status;
}

int thread::start_detached_thread()
{
	pthread_attr_t attr;
	int status = errorcode::SIPTOOLS_SUCCESS;

	this->data = NULL;

	status = pthread_attr_init(&attr);
	if (status) {
		log_print(log::log_error, "pthread_attr_init failure!\n");
		return status;
	}

	status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (status) {
		log_print(log::log_error, "pthread_attr_setdetachstate failure!\n");
		return status;
	}

	status = pthread_create(&pthread, &attr, start_routine, this);
	if (status) {
		log_print(log::log_error, "pthread_create failure!\n");
		return status;
	}

	pthread_attr_destroy(&attr);

	return status;
}

int thread::start_detached_thread(void *data)
{
	pthread_attr_t attr;
	int status = errorcode::SIPTOOLS_SUCCESS;

	if (data) {
		this->data = data;
	}

	status = pthread_attr_init(&attr);
	if (status) {
		log_print(log::log_error, "pthread_attr_init failure!\n");
		return status;
	}

	status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (status) {
		log_print(log::log_error, "pthread_attr_setdetachstate failure!\n");
		return status;
	}

	status = pthread_create(&pthread, &attr, start_routine, this);
	if (status) {
		log_print(log::log_error, "pthread_create failure!\n");
		return status;
	}

	pthread_attr_destroy(&attr);

	return status;
}

int thread::join_thread()
{
	return pthread_join(pthread, NULL);
}

void thread::run_thread(void *data)
{}

void *thread::get_thread_data()
{
	return data;
}
