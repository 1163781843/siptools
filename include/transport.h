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

		int tport_open_socket(int domain = TPORT_AF_INET, int type = TPROT_UDP, int protocal = 0);
		int tport_open_socket(int domain = TPORT_AF_INET, int type = TPROT_UDP);
		int tport_close_socket();
		void tport_set_sockaddr(int domain = TPORT_AF_INET);
		void tport_set_sockaddr(int domain, const char *host);
		const struct sockaddr &tport_get_sockaddr() const;
		void tport_set_port(short port, int domain = TPORT_AF_INET);
		short tport_get_port(int domain = TPORT_AF_INET) const;
		void tport_bind_addr();

		virtual int tport_send(const char *buffer, size_t size) = 0;
		virtual int tport_recv(char *buffer, size_t size) = 0;

	protected:
		tport();
	private:
		union {
			struct {
				struct sockaddr_un local;
				struct sockaddr_un remote;
			} addr_un;

			struct {
				struct sockaddr local;
				struct sockaddr remote;
			} addr_in;
		} addr;
		int addr_len;
		int sockfd;
};

#endif
