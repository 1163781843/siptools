#include <stdarg.h>
#include<sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#include <iostream>
#include <memory>

#include <logger.h>
#include <thread.h>

std::shared_ptr<std::queue<std::shared_ptr<logmsg>>> log::logmsg_task;

std::shared_ptr<cond> log::log_cond;
std::shared_ptr<lock> log::log_lock;

int log::run = 0;

const std::shared_ptr<std::string> & logmsg::get_logmsg_timestamp() const
{
	return timestamp;
}

const std::shared_ptr<std::string> & logmsg::get_logmsg_file() const
{
	return file;
}

const std::shared_ptr<std::string> & logmsg::get_logmsg_level() const
{
	return level;
}

const std::shared_ptr<std::string> & logmsg::get_logmsg_data() const
{
	return data;
}

int logmsg::get_logmsg_line() const
{
	return line;
}

long logmsg::get_logmsg_tid() const
{
	return tid;
}

logmsg::logmsg(const std::shared_ptr<std::string> &timestamp, const std::shared_ptr<std::string> &file, int line, long tid,
	const std::shared_ptr<std::string> &level, const std::shared_ptr<std::string> &data)
	: timestamp(timestamp), file(file), line(line), tid(tid), level(level), data(data)
{
}

logmsg::~logmsg()
{
}

void log::log_push(const std::shared_ptr<logmsg> &msg_ptr)
{
	log_lock->mutex_lock();
	logmsg_task->push(msg_ptr);
	fprintf(stdout, "%s, msg_ptr.use_count()[%d], msg_ptr[%p]\n", msg_ptr->get_logmsg_data()->c_str(), msg_ptr.use_count(), msg_ptr.get());
	log_cond->cond_signal();
	log_lock->mutex_unlock();
}

void log::log_pop()
{
	struct timeval now;
	struct timespec abstime = {0};
	gettimeofday(&now, NULL);
	abstime.tv_sec = now.tv_sec + 1;
	abstime.tv_nsec = 0;

	log_lock->mutex_lock();
	log_cond->cond_timedwait(*log_lock.get(), &abstime);

#if 0
	for (int i = 0; i < logmsg_task->size(); i++) {
		std::cout << logmsg_task->front()->get_logmsg_level()->c_str() << std::endl;
	}
#endif

#if 1
	while (logmsg_task->size()) {
		//printf("logmsg_task->empty()[%d], logmsg_task->size()[%ld]\n", logmsg_task->empty(), logmsg_task->size());
		std::shared_ptr<logmsg> msg(logmsg_task->front());
		fprintf(stdout, "%s, msg[%p], logmsg_task->front().use_count[%d]\n", logmsg_task->front()->get_logmsg_data()->c_str(), msg.get(), logmsg_task->front().use_count());
		abort();
		logmsg_task->pop();
	}
#endif
	log_lock->mutex_unlock();
}

void log::run_thread(void *data)
{
	while (run) {
		log_pop();
	}
}

void log::set_run(int run)
{
	this->run = run;
}

log::log()
{
	if (nullptr == logmsg_task) {
		std::shared_ptr<std::queue<std::shared_ptr<logmsg>>> ptr(new std::queue<std::shared_ptr<logmsg>>);
		logmsg_task = ptr;
	}

	if (nullptr == log_lock) {
		std::shared_ptr<lock> loglock(new lock());
		log_lock = loglock;
	}

	if (nullptr == log_cond) {
		std::shared_ptr<cond> logcond(new cond());
		log_cond = logcond;
	}
}

log::~log()
{
}

void sip_log_print(int level, const char *file, int line, const char *format, ...)
{
	char *buffer = nullptr;
	int len = 0;
	const char *filename = nullptr;
	char date[128] = {0};
	time_t cur_tt;
	struct tm *cur_tm = NULL;
	struct timeval cur_sec;
	long tid = -1;

	cur_tt = time(NULL);
	cur_tm = localtime(&cur_tt);
	gettimeofday(&cur_sec, NULL);

	snprintf(date, sizeof(date) - 1, "%4d-%02d-%02d %02d:%02d:%02d:%03ld",
		cur_tm->tm_year + 1900, cur_tm->tm_mon + 1, cur_tm->tm_mday, cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec, cur_sec.tv_usec / 1000);

	tid = syscall(SYS_gettid);

	if (file) {
		filename = strrchr(file, '/');
	}

	va_list ap;
	va_start(ap, format);
	len = vasprintf(&buffer, format, ap);
	va_end(ap);

	log::log_push(std::shared_ptr<logmsg>(new logmsg(
		std::shared_ptr<std::string>(new std::string(date)),
		std::shared_ptr<std::string>(filename ? (new std::string(++filename)) : (new std::string(file))),
		line,
		tid,
		std::shared_ptr<std::string>(new std::string("\033[32;1mDEBUG\33[0m")),
		std::shared_ptr<std::string>(new std::string(buffer)))));

	//fprintf(stdout, "[%s] [%ld] %s:%d -- %s\n", date, tid, filename, line, buffer);

	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}
}
