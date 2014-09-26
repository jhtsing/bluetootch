#ifndef __MESSAGE_H__
#define __MESSAGE_H__ 
#include <vector>
#include <boost/cstdint.hpp>
#define  HEADER_LENGTH 8

using namespace boost;
class message
{
public:
	message():
	buf_(HEADER_LENGTH)
	{
		set_msg_id(-1);
		set_data_len(0);
	}
	message(int msg_id, const char* buf , int len):
	buf_(HEADER_LENGTH+len)
	{
		set_msg_id(msg_id);
		set_data_len(len);
		memcpy(data() ,buf,len);
	}
	char *buffer()
	{
		return &buf_[0];
	}
	char *data()
	{
		return &buf_[HEADER_LENGTH];
	}
	int header_len()
	{
		return HEADER_LENGTH;
	}
	int size()
	{
		return header_len()+data_len();
	}
	int data_len()
	{
		MsgHeader msg_headr = *((MsgHeader*)&buf_[0]);
		return msg_headr.msg_len;
	}
	int msgid()
	{
		MsgHeader msg_headr = *((MsgHeader*)&buf_[0]);
		return msg_headr.msg_id;
	}
	void set_data_len(int len)
	{
		MsgHeader msg_headr = *((MsgHeader*)&buf_[0]);
		msg_headr.msg_len = len;
		*((MsgHeader*)&buf_[0]) = msg_headr;
	}
	void set_msg_id(int msg_id)
	{
		MsgHeader msg_headr = *((MsgHeader*)&buf_[0]);
		msg_headr.msg_id = msg_id;
		*((MsgHeader*)&buf_[0]) = msg_headr; 
	}
	void alloc(int len)
	{
		int msg_id = msgid();
		buf_.resize(HEADER_LENGTH+len);
		set_data_len(len);
		set_msg_id(msg_id);
	}
	typedef struct _MsgHeader 
	{
		int32_t msg_id;
		int32_t msg_len;
	}MsgHeader; 
public:
	std::vector<char> buf_;
};
#endif