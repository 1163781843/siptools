#ifndef __SIP_H__
#define __SIP_H__

#include <iostream>
#include <queue>
#include <time.h>
#include <memory>

#include <thread.h>
#include <udp_transport.h>

#include <osip2/osip.h>

class sip : public thread, public udp_tport
{
public:
	sip();
	virtual ~sip();

	static int sip_init_osip();
	static int sip_release_osip();
	void run_thread(void *data);
	void set_run(int run);
protected:
private:
	static osip_t *osip;
	static std::queue<std::shared_ptr<osip_event_t>> task;
	static int run;
};

#endif
