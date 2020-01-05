#include <iostream>

#include <errcode.h>

int main(int argc, const char **argv)
{
	errorcode *errcode = NULL;

	errcode = new errorcode;

	std::cout << "error code: " << errorcode::get_errorcode(errorcode::SIPTOOLS_SUCCESS) << std::endl;

	delete errcode;
	errcode = nullptr;

	return 0;
}
