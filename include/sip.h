#ifndef __SIP_H__
#define __SIP_H__

#include <iostream>
#include <queue>
#include <time.h>

#include <osip2/osip.h>

class sip
{
public:
	virtual ~sip();

	static int sip_init_osip();
	static int sip_release_osip();
protected:
	sip();
private:
	static osip_t *osip;
	static std::queue<osip_event_t *> task;
};

#endif
