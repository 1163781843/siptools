#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include <logger.h>
#include <errcode.h>

int main(int argc, const char **argv)
{
	errorcode *errcode = NULL;

	errcode = new errorcode;

	std::cout << "error code: " << errorcode::get_errorcode(errorcode::SIPTOOLS_SUCCESS) << std::endl;

#if 0
	std::shared_ptr<logmsg> msg(new logmsg(
		std::shared_ptr<std::string>(new std::string("test")),
		std::shared_ptr<std::string>(new std::string("test")),
		__LINE__,
		getpid(),
		std::shared_ptr<std::string>(new std::string("test")),
		std::shared_ptr<std::string>(new std::string("test"))));

	log_print(log::log_debug, "msg[%s]\n", msg->get_logmsg_timestamp()->c_str());

#endif
	log_print(log::log_debug, "msg[%s]\n", __FUNCTION__);

	delete errcode;
	errcode = nullptr;

	return 0;
}
