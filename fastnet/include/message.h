#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <vector>
#include <assert.h>
#include <cstdint>
namespace network
{
class message
{
public:
	struct msgheader
	{
		std::uint32_t msg_len;
		std::uint32_t msg_id;
	};
	enum { KMSGHEADER_LENGTH = sizeof(msgheader) }; 
	message():
	data_(KMSGHEADER_LENGTH)
	{
		msgheader* _header = reinterpret_cast<msgheader *>(buffer());
		assert(_header != nullptr);
		_header->msg_len = 0;
		_header->msg_id = 0;
	}
	message(const char* buf ,std::uint32_t len):
	data_(KMSGHEADER_LENGTH+len)
	{
	}
	char *buffer()
	{
		return (char *)(&data_[0]);
	}
	const char* buffer() const
	{
		return const_cast<message*>(this)->buffer();
	}
	std::uint32_t size()
	{
		return (KMSGHEADER_LENGTH + data_size());
	}
	const std::uint32_t size() const
	{
		return const_cast<message*>(this)->size();
	}
	char *data()
	{
		return (char *)(&data_[0]);
	}
	const char* data() const 
	{
		return const_cast<message *>(this)->data();
	}
	std::uint32_t data_size()
	{
		msgheader* _header = reinterpret_cast<msgheader *>(buffer());
		assert(_header != nullptr);
		return _header->msg_len;
	}
	const std::uint32_t data_size() const
	{
		return const_cast<message *>(this)->data_size();
	}
	std::uint32_t msg_id()
	{
		msgheader* _header = reinterpret_cast<msgheader *>(buffer());
		assert(_header != nullptr);
		return _header->msg_id;
	}
	const std::uint32_t msg_id() const 
	{
		return const_cast<message *>(this)->msg_id();
	}
	void alloc_with_header()
	{
		msgheader _header = *(reinterpret_cast<msgheader *>(buffer()));
		data_.resize(size());
		*(reinterpret_cast<msgheader *>(buffer())) = _header;
	}
private:
	std::vector<char> data_;
};
}
#endif 