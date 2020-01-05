#include <udp_transport.h>

int udp_tport::tport_recv(char *buffer, size_t size)
{
	int recvnum = 0;

	recvnum = ::recvfrom(sockfd, buffer, size, 0, &addr.addr_in.local, &addr_len);
	if (recvnum > 0) {
		buffer[recvnum] = '\0';
	}

	return recvnum;
}

int udp_tport::tport_send(const char *buffer, size_t size)
{
	int sendtonum = 0;

	sendtonum = ::sendto(sockfd, buffer, size, 0, &addr.addr_in.remote, &addr_len);
}

udp_tport::udp_tport()
{}

udp_tport::~udp_tport()
{}
