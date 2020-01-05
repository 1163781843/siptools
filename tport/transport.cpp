#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#include <transport.h>
#include <errcode.h>

int tport::tport_open_socket(int domain, int type, int protocal)
{
	int ldomain;
	int ltype;

	switch (domain) {
	case TPORT_AF_LOCAL:
		ldomain = AF_UNIX;
		break;
	case TPORT_AF_INET:
		ldomain = AF_INET;
		break;
	case TPORT_AF_INET6:
		ldomain = AF_INET6;
		break;
	default:
		ldomain = AF_INET;
		break;
	}

	switch (type) {
	case TPROT_UDP:
		ltype = SOCK_DGRAM;
		break;
	default:
		ltype = SOCK_STREAM;
		break;
	}

	sockfd = ::socket(ldomain, ltype, protocal);
	if (sockfd > 0) {
		return errorcode::SIPTOOLS_SOCK_FAILURE;
	} else {
		return errorcode::SIPTOOLS_SUCCESS;
	}
}

int tport::tport_open_socket(int domain, int type)
{
	int ldomain;
	int ltype;

	switch (domain) {
	case TPORT_AF_LOCAL:
		ldomain = AF_UNIX;
		break;
	case TPORT_AF_INET:
		ldomain = AF_INET;
		break;
	case TPORT_AF_INET6:
		ldomain = AF_INET6;
		break;
	default:
		ldomain = AF_INET;
		break;
	}

	switch (type) {
	case TPROT_UDP:
		ltype = SOCK_DGRAM;
		break;
	default:
		ltype = SOCK_STREAM;
		break;
	}

	sockfd = ::socket(ldomain, ltype, 0);
	if (sockfd > 0) {
		return errorcode::SIPTOOLS_SOCK_FAILURE;
	} else {
		return errorcode::SIPTOOLS_SUCCESS;
	}
}

int tport::tport_close_socket()
{
	::close(sockfd);
}

void tport::tport_set_sockaddr(int domain)
{
	struct sockaddr_in sockaddr;
	struct sockaddr_in6 sockaddr6;

	switch (domain) {
	case TPORT_AF_LOCAL:
		break;
	case TPORT_AF_INET:
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr_len = sizeof(sockaddr);
		memcpy(&addr.addr_in, &sockaddr, addr_len);
		break;
	case TPORT_AF_INET6:
#if 0
		sockaddr6.sin6_family = AF_INET6;
		sockaddr6.sin6_addr.s_addr = htonl(INADDR_ANY);
		memcpy(&addr.addr_in, &sockaddr6, addr_len);
#endif
		break;
	}
}

void tport::tport_set_sockaddr(int domain, const char *host)
{
	struct sockaddr_in sockaddr;
	struct sockaddr_in6 sockaddr6;

	switch (domain) {
	case TPORT_AF_LOCAL:
		break;
	case TPORT_AF_INET:
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = inet_addr(host);
		addr_len = sizeof(sockaddr);
		memcpy(&addr.addr_in, &sockaddr, addr_len);
		break;
	case TPORT_AF_INET6:
#if 0
		sockaddr6.sin6_family = AF_INET6;
		sockaddr6.sin6_addr.s_addr = inet6_addr(host);
		memcpy(&addr.addr_in, &sockaddr6, addr_len);
#endif
		break;
	}
}

const struct sockaddr &tport::tport_get_sockaddr() const
{
	return addr.addr_in;
}

void tport::tport_set_port(short port, int domain)
{
	switch (domain) {
	case TPORT_AF_LOCAL:
		break;
	case TPORT_AF_INET:
		((struct sockaddr_in *)&addr.addr_in)->sin_port = htons(port);
		break;
	case TPORT_AF_INET6:
		((struct sockaddr_in6 *)&addr.addr_in)->sin6_port = htons(port);
		break;
	}
}

short tport::tport_get_port(int domain) const
{
	switch (domain) {
	case TPORT_AF_INET:
		return ((struct sockaddr_in *)&addr.addr_in)->sin_port;
	case TPORT_AF_INET6:
		break;
	defalut:
		break;
	}

	return -1;
}

void tport::tport_bind_addr()
{
	::bind(sockfd, &addr.addr_in, addr_len);
}

tport::tport() : sockfd(-1)
{}

tport::~tport()
{
	if (sockfd > 0) {
		this->tport_close_socket();
	}
}
