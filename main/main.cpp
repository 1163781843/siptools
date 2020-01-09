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

	std::unique_ptr<logmsg> msg(new logmsg(
		std::unique_ptr<std::string>(new std::string("test")),
		std::unique_ptr<std::string>(new std::string("test")),
		__LINE__,
		getpid(),
		std::unique_ptr<std::string>(new std::string("test")),
		std::unique_ptr<std::string>(new std::string("test"))));

	std::cout << msg << std::endl;
	log_print(log::log_debug, "msg[%p]\n", msg.get());

	delete errcode;
	errcode = nullptr;

	return 0;
}
