#include "stdafx.h"
#include "acceptor.h"

using namespace boost::asio::ip;

acceptor::acceptor(boost::asio::io_service& _io_service):
io_service_(_io_service),
acceptor_(_io_service,tcp::endpoint(tcp::v4(), 1000))
{

}
acceptor::~acceptor(void)
{

}
bool acceptor::start()
{
	
}
void acceptor::stop()
{

}