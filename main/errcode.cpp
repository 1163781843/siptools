#include <iterator>
#include <string>
#include <utility>
#include <iostream>

#include <errcode.h>

std::map<int, std::unique_ptr<std::string>> * errorcode::errmap = new std::map<int, std::unique_ptr<std::string>>();

const char *errorcode::get_errorcode(int code)
{
	auto result = errorcode::errmap->find(code);
	if (result != errorcode::errmap->end()) {
		return result->second->c_str();
	}

	return "unknow";
}

errorcode::errorcode()
{
	if (errmap) {
		errorcode::errmap->insert(std::make_pair(SIPTOOLS_SOCK_FAILURE, std::unique_ptr<std::string>(new std::string("create socket failure"))));
		errorcode::errmap->insert(std::make_pair(SIPTOOLS_ERROR, std::unique_ptr<std::string>(new std::string("execute operate failure"))));
		errorcode::errmap->insert(std::make_pair(SIPTOOLS_SUCCESS, std::unique_ptr<std::string>(new std::string("execute operate success"))));
		errorcode::errmap->insert(std::make_pair(SIPTOOLS_TOTAL, std::unique_ptr<std::string>(new std::string("unknow error"))));
	}
}

errorcode::~errorcode()
{
	if (errmap) {
		auto iter = errmap->erase(errmap->begin());

		delete errmap;
	}
}
