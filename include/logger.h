#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <memory>
#include <iostream>
#include <queue>

#include <thread.h>

class logmsg
{
public:
	logmsg(const std::shared_ptr<std::string> &timestamp, const std::shared_ptr<std::string> &file, int line, long tid,
		const std::shared_ptr<std::string> &level, const std::shared_ptr<std::string> &data);
	~logmsg();

	const std::shared_ptr<std::string> & get_logmsg_timestamp() const;
	const std::shared_ptr<std::string> & get_logmsg_file() const;
	const std::shared_ptr<std::string> & get_logmsg_level() const;
	const std::shared_ptr<std::string> & get_logmsg_data() const;
	int get_logmsg_line() const;
	long get_logmsg_tid() const;
protected:
private:
	const std::shared_ptr<std::string> & timestamp;
	const std::shared_ptr<std::string> & file;
	const std::shared_ptr<std::string> & level;
	const std::shared_ptr<std::string> & data;
	int line;
	long tid;
};

class log : public thread
{
public:
	enum {
		log_error,
		log_warning,
		log_noitce,
		log_debug,
		log_verb,
	};

	log();
	~log();

	static void log_push(const std::shared_ptr<logmsg> &msg_ptr);
	void log_pop();
	void run_thread(void *data);
	void set_run(int run);
protected:
private:
	static std::shared_ptr<std::queue<std::shared_ptr<logmsg>>> logmsg_task;
	static std::shared_ptr<cond> log_cond;
	static std::shared_ptr<lock> log_lock;
	static int run;
};

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void sip_log_print(int level, const char *file, int line, const char *format, ...);

#define log_print(level, ...) do {								\
	if (level) {												\
		sip_log_print(level, __FILE__, __LINE__, __VA_ARGS__);	\
	}															\
} while (0)

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
#endif
