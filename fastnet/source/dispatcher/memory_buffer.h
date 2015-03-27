#ifndef __MEMORY_BUFFER_H__
#define __MEMORY_BUFFER_H__

namespace network
{
	// ---------------------------------------------------------
	// class mutable_buffer
	// 提供安全的修改，由外界提供缓冲区
	class mutable_buffer_t
	{
	public:
		typedef char					value_type;
		typedef char *					pointer;
		typedef const char*				const_pointer;
		typedef value_type*				iterator;
		typedef const value_type*		const_iterator;
	public:
		value_type *data_;
		size_t size_;
	public:
		mutable_buffer_t()
			: data_(0)
			, size_(0)
		{}
		mutable_buffer_t(value_type *data, size_t size)
			: data_(data)
			, size_(size)
		{}
		mutable_buffer_t(const mutable_buffer_t &rhs)
			: data_(rhs.data_)
			, size_(rhs.size_)
		{
		}
		~mutable_buffer_t()
		{
		}
	private:
		mutable_buffer_t &operator=(const mutable_buffer_t &);
	public:
		pointer data()
		{
			return data_;
		}
		size_t size() const
		{
			return size_;
		}
		iterator begin()
		{
			return data_;
		}
		iterator end()
		{
			return data_ + size_;
		}
		const_iterator begin() const
		{
			return data_;
		}
		const_iterator end() const
		{
			return data_ + size_;
		}
	};
	inline mutable_buffer_t operator+(const mutable_buffer_t &buffer, size_t sz)
	{
		assert(sz <= buffer.size_);
		if (sz > buffer.size_)
			printf("sz > buffer.size_");

		mutable_buffer_t::pointer newData = buffer.data_ + sz;
		size_t newSize = buffer.size_ - sz;

		return mutable_buffer_t(newData, newSize);
	}
	inline mutable_buffer_t operator+(size_t sz, const mutable_buffer_t &buffer)
	{
		return buffer + sz;
	}
	// ---------------------------------------------------------
	// class ConstBuffer

	// 不能修改缓冲区，由外界提供缓冲区
	class const_buffer_t
	{
	public:
		typedef char					value_type;
		typedef char *					pointer;
		typedef const char*				const_pointer;
		typedef value_type*				iterator;
		typedef const value_type*		const_iterator;

	public:
		const value_type *data_;
		size_t size_;

	public:
		const_buffer_t()
			: data_(0)
			, size_(0)
		{}
		const_buffer_t(const value_type *data, size_t size)
			: data_(data)
			, size_(size)
		{}
		const_buffer_t(const const_buffer_t &rhs)
			: data_(rhs.data_)
			, size_(rhs.size_)
		{
		}
		const_buffer_t &operator=(const const_buffer_t &rhs)
		{
			if (&rhs != this)
			{
				data_ = rhs.data_;
				size_ = rhs.size_;
			}

			return *this;
		}

	public:
		const_pointer data() const
		{
			return data_;
		}

		size_t size() const
		{
			return size_;
		}
		const_iterator begin() const
		{
			return data_;
		}
		const_iterator end() const
		{
			return data_ + size_;
		}
	};
	inline const_buffer_t operator+(const const_buffer_t &buffer, size_t sz)
	{
		assert(sz <= buffer.size_);
		if (sz > buffer.size_)
			throw printf("sz > buffer.size_");

		const_buffer_t::const_pointer newData = buffer.data_ + sz;
		size_t newSize = buffer.size_ - sz;

		return const_buffer_t(newData, newSize);
	}
	inline const_buffer_t operator+(size_t sz, const const_buffer_t &buffer)
	{
		return buffer + sz;
	}
}
#endif 