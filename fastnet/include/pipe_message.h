#ifndef __PIPE_MESSAGE_H__
#define __PIPE_MESSAGE_H__ 
#include <cstdint>
#include <vector>
namespace named_pipe
{
	class pipe_message
	{
		typedef std::int32_t message_length_t;
		typedef std::int32_t message_id_t;
		struct Header
		{
			message_length_t m_len;
			message_id_t m_id;
		};
		enum{ kHEADER_LENGTH = sizeof(Header) };//[message_length][message_id]
		friend class pipe_channel;
		friend class channel;
		typedef pipe_message this_type;
	public:
		pipe_message(int msgid, const void* _data, size_t _len)
		{
			data_.resize(kHEADER_LENGTH+_len);
			Header* h = (Header*)&data_[0];
			h->m_len = (int)_len;
			h->m_id = msgid;
			if (_data&&_len)
			{
				memcpy((char*)data(), _data, _len);
			} 
		}
		pipe_message()
		{
		} 
		const char* data()const
		{
			if (!&data_[0] || !size())
				return nullptr;
			return &data_[0] + kHEADER_LENGTH;
		}
		size_t size()const
		{
			const Header* h = header();
			if (h) return h->m_len;
			return 0;
		}
		int id()const
		{
			const Header* h = header();
			if (h) return h->m_id;
			return 0;
		}

		size_t buffer_size()const
		{
			if (&data_[0])
				return size() + kHEADER_LENGTH;
			return 0;
		}
		void* buffer()
		{
			if (&data_[0])
				return &data_[0];
			return nullptr;
		}
		const void* buffer()const
		{
			return const_cast<this_type*>(this)->buffer();
		}
		Header* header()
		{
			return reinterpret_cast<Header*>(buffer());
		}
		const Header* header()const
		{
			return reinterpret_cast<const Header*>(buffer());
		}
		void malloc_when_recvd_header(const Header& h)
		{
			data_.resize(kHEADER_LENGTH+h.m_len);
			*header() = h;
		}
	private:
		std::vector<char> data_;
	};

}
#endif