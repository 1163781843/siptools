#ifndef __UDP_TRANSPORT_H__
#define __UDP_TRANSPORT_H__

#include <transport.h>

class udp_tport : tport {
	public:
		udp_tport();
		~udp_tport();
		int tport_recv(char *buffer, size_t size);
		int tport_send(const char *buffer, size_t size);
	protected:
	private:
};

#endif

