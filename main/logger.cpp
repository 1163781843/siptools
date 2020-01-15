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

static int elim_spec_string(char *data, char delim, char **array, int arraylen);
#define arraylen(array) (sizeof(array) / sizeof(array[0]))
#define elim_string(data, delim, array) elim_spec_string((data), (delim), (array), arraylen(array))
 
static int string_char_delim(char *data, char delim, char **array, int arraylen)
{
	int count = 0;
	char *ptr = data;

	enum tokenizer_state {
		START,
		FIND_DELIM
	} state = START;

	while (*ptr && (count < arraylen)) {
		switch (state) {
		case START:
			array[count++] = ptr;
			state = FIND_DELIM;
			break;
		case FIND_DELIM:
			if (delim == *ptr) {
				*ptr = '\0';
				state = START;
			}
			++ptr;
			break;
		}
	}

	return count;
}

static int elim_spec_string(char *data, char delim, char **array, int arraylen)
{
	if (!data || !array || !arraylen) {
		return 0;
	}

	memset(array, 0, arraylen * sizeof(*array));

	return string_char_delim(data, delim, array, arraylen);
}

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
	log_cond->cond_signal();
	log_lock->mutex_unlock();
}

void log::log_pop()
{
	int argc = 0, i;
	char *dupstr = NULL;
	char *lines[1024] = {0};
	char strbuffer[65535] = {0};
	int bufferlen = 0;

	log_lock->mutex_lock();
	log_cond->cond_timedwait(*log_lock.get(), 1000000000);

	while (logmsg_task->size()) {
		std::shared_ptr<logmsg> msg(logmsg_task->front());

		argc = elim_string((dupstr = strdup(msg->get_logmsg_data()->c_str())), '\n', lines);
		for (i = 0; i < argc; i++) {
			if (strlen(lines[i])) {
				bufferlen += snprintf(strbuffer + bufferlen, sizeof(strbuffer) - bufferlen, "%s\n", lines[i]);
			}
		}

		fprintf(stdout, "[%s] %s[%ld]: %s:%d  %s",
			msg->get_logmsg_timestamp()->c_str(),
			msg->get_logmsg_level()->c_str(),
			msg->get_logmsg_tid(),
			msg->get_logmsg_file()->c_str(),
			msg->get_logmsg_line(),
			strbuffer);
		logmsg_task->pop();
	}

	log_lock->mutex_unlock();
}

void log::run_thread(void *data)
{
	while (run) {
		log_pop();
	}
}

const char *log::get_level(int level)
{
	switch (level) {
	case log_error:
		return "\033[31;1mERROR\33[0m";
	case log_warning:
		return "\033[32;31;1mWARRING\33[0m";
	case log_noitce:
		return "\033[33;1mNOTICE\33[0m";
	case log_debug:
		return "\033[32;1mDEBUG\33[0m";
	case log_verb:
		return "\033[32mVERBOSE\33[0m";
	};

	return "\033[32;1mDEBUG\33[0m";
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

	std::shared_ptr<std::string> cur_date(new std::string(date));

	log::log_push(std::shared_ptr<logmsg>(new logmsg(
		cur_date,
		std::shared_ptr<std::string>(filename ? (new std::string(++filename)) : (new std::string(file))),
		line,
		tid,
		std::shared_ptr<std::string>(new std::string(log::get_level(level))),
		std::shared_ptr<std::string>(new std::string(buffer)))));

	//fprintf(stdout, "[%s] [%ld] %s:%d -- %s\n", date, tid, filename, line, buffer);

	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}
}
