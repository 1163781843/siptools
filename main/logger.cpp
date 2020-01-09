#include <stdarg.h>
#include<sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#include <iostream>
#include <memory>

#include <logger.h>
#include <thread.h>

std::unique_ptr<std::queue<logmsg>> logmsg_task = 0;
lock log::log_lock;
cond log::log_cond;
int log::run = 0;

logmsg::logmsg(const std::unique_ptr<std::string> &timestamp, const std::unique_ptr<std::string> &file, int line, long tid,
		const std::unique_ptr<std::string> &level, const std::unique_ptr<std::string> &data)
	: timestamp(timestamp), file(file), line(line), tid(tid), level(level), data(data)
{}

logmsg::~logmsg()
{
}

void log::log_push(const std::unique_ptr<logmsg> &msg_ptr)
{
	log_lock.mutex_lock();
	//logmsg_task->push(msg_ptr);
	log_cond.cond_signal();
	log_lock.mutex_unlock();
}

void log::log_pop()
{
	const struct timespec abstime = {0};

	log_lock.mutex_lock();
	log_cond.cond_timedwait(log_lock, &abstime);
	log_lock.mutex_unlock();
}

void log::run_thread(void *data)
{
	while (run) {
		log_pop();
	}
}

void log::set_run(int run)
{
	run = run;
}

log::log()
{
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

	log::log_push(std::unique_ptr<logmsg>(new logmsg(
		std::unique_ptr<std::string>(new std::string(date)),
		std::unique_ptr<std::string>(new std::string(filename)),
		line,
		tid,
		std::unique_ptr<std::string>(new std::string("")),
		std::unique_ptr<std::string>(new std::string(buffer)))));

	fprintf(stdout, "[%s] [%ld] %s:%d -- %s\n", date, tid, file, line, buffer);

	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}
}
