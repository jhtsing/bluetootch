#include "stdafx.h"
#include "session.h"


session::session(boost::asio::io_service& in_io_service):
io_service_(in_io_service),
socket_(in_io_service)
{

}
session::~session(void)
{

}
bool session::start()
{
	try
	{
		boost::asio::async_read()
	}
	catch (std::exception& e)
	{

	}

}
void session::stop()
{

}