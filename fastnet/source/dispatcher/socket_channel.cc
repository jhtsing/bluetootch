#include "include\socket_channel.h"

namespace network
{
	bool socket_channel::connect(const end_point& remote_point)
	{
		if (sock_)
		{
			return sock_->connect(remote_point.addr_, remote_point.port_);
		}
		else
		{
			return false;
		}
	}
	void socket_channel::close()
	{
		if (sock_)
		{
			sock_->disconnect(SD_BOTH);
		}
	}
	void socket_channel::async_connect(const end_point& remote_point, connect_handler_type handler)
	{
		if (sock_)
		{
			sock_->async_connect(remote_point, handler);
		}
		else
		{
			handler(std::error_code());
		}
	}
	void socket_channel::async_write(const message msg, write_handler_type handler)
	{
		if (sock_)
		{
			const_buffer_t _send_buf(msg.buffer(), msg.size());
			sock_->async_write(_send_buf, handler); 
		}
		else
		{
			handler(std::error_code(), 0);
		}
	}
	void socket_channel::async_read(read_handler_type handler)
	{
		if (sock_)
		{
			std::shared_ptr<message> msg(new message());
			mutable_buffer_t read_buf(msg->buffer(), msg->size());
			sock_->async_read(read_buf,
				std::bind(&socket_channel::handle_read_header,
				shared_from_this(),
				handler,
				msg,
				std::placeholders::_1,
				std::placeholders::_2
				));
		}
		else
		{
			handler(message(), std::error_code());		
		}
	}
	void socket_channel::handle_read_header(
		read_handler_type handler,
		std::shared_ptr<message> msg, 
		std::error_code ec, 
		std::uint32_t size)
	{
		if (ec)
		{
			handler(message(), ec);
			return;
		}
		std::uint32_t data_size = msg->data_size();
		if (data_size > 0&&sock_)
		{
			assert(size == message::KMSGHEADER_LENGTH);
			msg->alloc_with_header();
			mutable_buffer_t body_buf(msg->data(), msg->data_size());
			sock_->async_read(body_buf, 
				std::bind(&socket_channel::handle_read_body,
				shared_from_this(),
				handler,
				msg,
				std::placeholders::_1,
				std::placeholders::_2
				));
		}
		else
		{
			handler(message(), ec);
		}
	}
	void socket_channel::handle_read_body(
		read_handler_type handler,
		std::shared_ptr<message> msg,
		std::error_code ec, 
		std::uint32_t size)
	{
		if (ec)
		{
			handler(message(), ec);
			return;
		}
		assert(size == msg->data_size());
		handler(*msg.get(), ec);
	}
	channel_ptr create_channel_ptr(service::iocp_impl & io_service)
	{
		return channel_ptr(new socket_channel(io_service));
	}
	channel_ptr create_channel_ptr(service::iocp_impl & io_service, socket_handle_ptr ptr)
	{
		return channel_ptr(new socket_channel(io_service, ptr));
	}
}
