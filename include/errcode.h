#ifndef __ERRCODE_H__
#define __ERRCODE_H__

#include <algorithm>
#include <map>
#include<memory>

class errorcode {
	public:
		errorcode();
		~errorcode();
		enum {
			SIPTOOLS_SOCK_FAILURE	= -2,
			SIPTOOLS_ERROR			= -1,
			SIPTOOLS_SUCCESS		= 0,
			SIPTOOLS_TOTAL,
		};

		static const char *get_errorcode(int code = SIPTOOLS_TOTAL);

	private:
		static std::map<int, std::unique_ptr<std::string>> *errmap;
};

#endif
