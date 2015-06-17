#ifndef __PIPE_ACCEPTOR_H__
#define __PIPE_ACCEPTOR_H__ 
#include <memory>
#include <string>
#include "p_channel.h"
#include "include\iocp_impl.h" 

namespace named_pipe
{
	class pipe_acceptor :
		public p_acceptor,
		public std::enable_shared_from_this<pipe_acceptor>
	{
	public:
		pipe_acceptor(service::iocp_impl& io_service);
		~pipe_acceptor();
		virtual bool open(const std::string& pipe_name);
		virtual bool close() ;
		virtual void async_accept(p_accept_handler_type handler);
	private:
		HANDLE create_pipe(std::error_code& ec);
		void handle_accept(p_channel::ptr chl, 
			p_accept_handler_type handler,
			const std::error_code& ec , 
			std::uint32_t size);
		void do_accept(p_accept_handler_type handler);
		service::iocp_impl& io_service_;
		size_t buffer_size_; 
		std::string pipe_name_;
	};
}
#endif 

