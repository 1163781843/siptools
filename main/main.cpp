#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <memory.h>

#include <thread.h>
#include <logger.h>
#include <errcode.h>

int main(int argc, const char **argv)
{
	std::unique_ptr<errorcode> status(new errorcode);

#if 0
	errorcode *errcode = NULL;

	errcode = new errorcode;
	std::cout << "error code: " << errorcode::get_errorcode(errorcode::SIPTOOLS_SUCCESS) << std::endl;


	std::shared_ptr<logmsg> msg(new logmsg(
		std::shared_ptr<std::string>(new std::string("test")),
		std::shared_ptr<std::string>(new std::string("test")),
		__LINE__,
		getpid(),
		std::shared_ptr<std::string>(new std::string("test")),
		std::shared_ptr<std::string>(new std::string("test"))));

	log_print(log::log_debug, "msg[%s]\n", msg->get_logmsg_timestamp()->c_str());


	delete errcode;
	errcode = nullptr;
#endif

	std::shared_ptr<std::string> msg(new std::string());
	std::shared_ptr<log> logobject(new log);
#if 0

	logobject->set_run(1);
	logobject->start_thread(logobject.get());

	log_print(log::log_debug, "msg[%s]\n", __FUNCTION__);

	sleep(60);

	logobject->set_run(0);
	logobject->join_thread();
#endif

#if 0
	delete logobject;
	logobject = nullptr;
#endif

	return 0;
}
