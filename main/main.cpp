#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <iostream>
#include <memory.h>

#include <thread.h>
#include <logger.h>
#include <errcode.h>

int main(int argc, const char **argv)
{
	std::unique_ptr<errorcode> status(new errorcode);

	int count = 10000;
	std::shared_ptr<class log> logobject(new class log);
	logobject->set_run(1);
	logobject->start_thread(logobject.get());

	sleep(1);

	do {
		log_print(log::log_debug, "This is a main function!\n");
		sleep(1);
	} while (count-- > 0);

	logobject->set_run(0);
	logobject->join_thread();

	return 0;
}
