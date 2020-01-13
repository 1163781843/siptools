#ifndef __UDP_TRANSPORT_H__
#define __UDP_TRANSPORT_H__

#include <transport.h>

class udp_tport : public tport
{
public:
	udp_tport();
	~udp_tport();

	int tport_get_sockfd() const;

	int tport_open_socket();
	int tport_close_socket();

	int tport_connect_socket();

	void tport_set_local_addr(const char *host);
	void tport_set_local_port(unsigned short port);

	void tport_set_remote_addr(const char *host);
	void tport_set_remote_port(unsigned short port);

	const struct sockaddr & tport_get_local_sockddr() const;
	const struct sockaddr & tport_get_remote_sockddr() const;

	const char *tport_get_local_sockaddr() const;
	const char *tport_get_remote_sockaddr() const;

	unsigned short tport_get_local_port() const;
	unsigned short tport_get_remote_port() const;

	int tport_bind_addr();

	unsigned int tport_recvfrom(char *buffer, size_t len);
	unsigned int tport_sendto(const char *buffer, size_t len);
protected:
private:
	int sockfd;
	struct sockaddr local_addr;
	struct sockaddr remote_addr;
	socklen_t local_addrlen;
	socklen_t remote_addrlen;
};

#endif

