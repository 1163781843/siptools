#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<netinet/in.h>

class tport {
	public:
		enum {
			TPORT_LOCAL,
			TPROT_UDP,
			TPORT_TCP,
			TPORT_TLS,
		} tport_type;

		enum {
			TPORT_AF_LOCAL,
			TPORT_AF_INET,
			TPORT_AF_INET6,
		} tport_domain;

		virtual ~tport();

		virtual int tport_get_sockfd() const = 0;

		virtual int tport_open_socket() = 0;
		virtual int tport_close_socket() = 0;

		virtual int tport_connect_socket() = 0;

		virtual void tport_set_local_addr(const char *host) = 0;
		virtual void tport_set_local_port(unsigned short port) = 0;

		virtual void tport_set_remote_addr(const char *host) = 0;
		virtual void tport_set_remote_port(unsigned short port) = 0;

		virtual const struct sockaddr & tport_get_local_sockddr() const = 0;
		virtual const struct sockaddr & tport_get_remote_sockddr() const = 0;

		virtual const char *tport_get_local_sockaddr() const = 0;
		virtual const char *tport_get_remote_sockaddr() const = 0;

		virtual unsigned short tport_get_local_port() const = 0;
		virtual unsigned short tport_get_remote_port() const = 0;

		virtual int tport_bind_addr() = 0;
	protected:
		tport();
};

#endif
