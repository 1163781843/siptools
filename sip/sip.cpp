#include <sip.h>

#include <errcode.h>

osip_t *sip::osip = nullptr;

int sip::sip_init_osip()
{
	int status = errorcode::SIPTOOLS_SUCCESS;

	if (nullptr == osip) {
		status = osip_init(&osip);
	}

	if (errorcode::SIPTOOLS_SUCCESS != status) {
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

sip::sip()
{
}

sip::~sip()
{
}