#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <iostream>
#include <memory.h>

#include <thread.h>
#include <logger.h>
#include <errcode.h>
#include <sip.h>

class log;
class sip;

int main(int argc, const char **argv)
{
	std::unique_ptr<errorcode> status(new errorcode);
	std::unique_ptr<class log> logobject(new class log);

	logobject->set_run(1);
	logobject->start_thread();

	std::unique_ptr<sip> instance(new sip());
	sip::sip_init_osip();
	instance->tport_open_socket();
	instance->tport_set_local_addr("0.0.0.0");
	instance->tport_set_local_port(6666);
	instance->tport_bind_addr();
	instance->set_run(1);
	instance->start_thread();

	while (1) {
		sleep(1);
	}

	instance->set_run(0);
	instance->join_thread();
	sip::sip_release_osip();

	logobject->set_run(0);
	logobject->join_thread();

	return 0;
}
