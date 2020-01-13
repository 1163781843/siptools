#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <udp_transport.h>

int udp_tport::tport_get_sockfd() const
{
	return sockfd;
}

int udp_tport::tport_open_socket()
{
	sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
}

int udp_tport::tport_close_socket()
{
	if (sockfd > 0) {
		close(sockfd);
		sockfd = -1;
	}
}

int udp_tport::tport_connect_socket()
{}

void udp_tport::tport_set_local_addr(const char *host)
{
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(host);
	local_addrlen = sizeof(sockaddr);

	memcpy(&local_addr, &sockaddr, local_addrlen);
}


void udp_tport::tport_set_local_port(unsigned short port)
{
	((struct sockaddr_in *)&local_addr)->sin_port = htons(port);
}

void udp_tport::tport_set_remote_addr(const char *host)
{
	struct sockaddr_in sockaddr;

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(host);

	remote_addrlen = sizeof(sockaddr);

	memcpy(&remote_addr, &sockaddr, remote_addrlen);
}

void udp_tport::tport_set_remote_port(unsigned short port)
{
	((struct sockaddr_in *)&remote_addr)->sin_port = htons(port);
}

const struct sockaddr & udp_tport::tport_get_local_sockddr() const
{
	return local_addr;
}

const struct sockaddr & udp_tport::tport_get_remote_sockddr() const
{
	return remote_addr;
}

const char *udp_tport::tport_get_local_sockaddr() const
{
	return inet_ntoa(((struct sockaddr_in *)&local_addr)->sin_addr);
}

const char *udp_tport::tport_get_remote_sockaddr() const
{
	return inet_ntoa(((struct sockaddr_in *)&remote_addr)->sin_addr);
}

unsigned short udp_tport::tport_get_local_port() const
{
	return ntohs(((struct sockaddr_in *)&local_addr)->sin_port);
}

unsigned short udp_tport::tport_get_remote_port() const
{
	return ntohs(((struct sockaddr_in *)&remote_addr)->sin_port);
}


int udp_tport::tport_bind_addr()
{
	return ::bind(sockfd, &local_addr, local_addrlen);
}

unsigned int udp_tport::tport_recvfrom(char *buffer, size_t len)
{
	unsigned int retval = 0;

	retval = ::recvfrom(sockfd, buffer, len, 0, &local_addr, &local_addrlen);
	if (retval > 0) {
		buffer[retval] = '\0';
	}

	return retval;
}

unsigned int udp_tport::tport_sendto(const char *buffer, size_t len)
{}

udp_tport::udp_tport() : sockfd(-1)
{}

udp_tport::~udp_tport()
{}
