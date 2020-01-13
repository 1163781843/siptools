#include <unistd.h>
#include <poll.h>

#include <sip.h>

#include <errcode.h>
#include <logger.h>

std::queue<std::shared_ptr<osip_event_t>> sip::task;
osip_t *sip::osip = nullptr;
int sip::run = 0;

int sip::sip_init_osip()
{
	int status = errorcode::SIPTOOLS_SUCCESS;

	if (nullptr == osip) {
		status = osip_init(&osip);
	}

	if (errorcode::SIPTOOLS_SUCCESS != status) {
		log_print(log::log_error, "sip init osip failure!\n");
		return status;
	}

	return status;
}

int sip::sip_release_osip()
{
	if (osip) {
		osip_release(osip);
		osip = nullptr;
	}

	return 0;
}

void sip::run_thread(void *data)
{
	int retval = 0;
	static char buffer[8192] = {0};

	while (run) {
		struct pollfd pfd;
		pfd.fd = tport_get_sockfd();
		pfd.events = POLLIN | POLLPRI;

		retval = ::poll(&pfd, 1, 1000);
		if (retval > 0 && (pfd.revents & POLLIN)) {
			memset(buffer, 0, sizeof(buffer) - 1);
			tport_recvfrom(buffer, sizeof(buffer) - 1);
			log_print(log::log_debug, "SIP thread recvfrom message from '%s:%d': \n%s\n", tport_get_local_sockaddr(), tport_get_local_port(), buffer);
		}
	}
}

void sip::set_run(int run)
{
	this->run = run;
}

sip::sip()
{
}

sip::~sip()
{
}
