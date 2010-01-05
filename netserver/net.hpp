/*
 *
 * Copyright (C) 2009 jack.wgm, microcai.
 * For conditions of distribution and use, see copyright notice 
 * in (http://code.google.com/p/netsever/source/browse/trunk/COPYING)
 *
 * Author: jack.wgm
 * Email:  jack.wgm@gmail.com
 */

#ifndef NET_H__
#define NET_H__

#if _MSC_VER
#pragma once
#endif

#include "jobqueue.hpp"

using boost::asio::ip::tcp;

class server;
class message;
class session;
class io_service_pool;

typedef boost::shared_ptr<session> session_ptr;
typedef boost::shared_ptr<message> message_ptr;

#ifdef SOCKET_SSL
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
#endif // SOCKET_SSL

/*/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////*/

class message
{
public:
	// ����ṹ��ʹ��1�ֽڶ���.
	#pragma pack(push, 1)

	typedef struct _tagPacketHeader 
	{
		unsigned int type;							// ���ݰ�����.
		unsigned int checksum;						// У���,Ϊ��������֮��. e.c type + rand + packsize.
		unsigned int rand;							// �����.
		unsigned int packsize;						// ���ݰ���С.
	} header, *headerPtr;

	// �ָ��ṹ���ֽڶ���.
	#pragma pack(pop)

#define HEADER_LENGTH sizeof(message::header)	// ͷ��С.
	#define DEFAULT_BODY_LENGTH 10240			// Ĭ���ڴ��С10k.

public:
	message() 
		: body_length_(0)
		, msg_(NULL)
	{
		// Ĭ�Ϸ����ڴ���С.
		data_ = new char[DEFAULT_BODY_LENGTH + HEADER_LENGTH];
		wptr_ = rptr_ = data_;
		data_size_ = DEFAULT_BODY_LENGTH + HEADER_LENGTH;
	}

	message(const message& msg)
	{		
		// ԭ�ڴ��̫С,���·����ڴ�.
		if (data_size_ < msg.data_size_) 
		{
			if (data_ != NULL)
				delete data_;
			data_size_ = msg.data_size_;
			data_ = new char[data_size_];
			wptr_ = rptr_ = data_;
		}
		
		// �����ڴ�.
		memcpy(data_, msg.data_, msg.data_size_);
		body_length_ = msg.body_length_;
		msg_ = (headerPtr)data_;
		session_ = msg.session_;
	}

	message& operator =(message &msg)
	{
		if (data_size_ < msg.data_size_) // ԭ�ڴ��̫С,���·����ڴ�.
		{
			if (data_ != NULL)
				delete data_;
			data_size_ = msg.data_size_;
			data_ = new char[msg.data_size_];
			wptr_ = rptr_ = data_;
		}

		// �����ڴ�.
		memcpy(data_, msg.data_, msg.data_size_);
		body_length_ = msg.body_length_;
		msg_ = (headerPtr)data_;
		session_ = msg.session_;

		return (*this);
	}

	~message()
	{
		if (data_)
			delete data_;
		wptr_ = rptr_ = data_ = NULL;
		msg_ = NULL;
		data_size_ = 0;
		body_length_ = 0;
	}

	// ��������ָ��.
	char* data()
	{
		return data_;
	}

	// �������ݰ��ĳ���.
	size_t length() const
	{
		return HEADER_LENGTH + body_length_;
	}

	// ����sessionָ��.
	void setsession(const session_ptr& _session)
	{
		session_ = _session;
	}

	// �õ�sessionָ��.
	void getsession(session_ptr& _session)
	{
		_session = session_.lock();
	}

	// ����body,����header������.
	char* body()
	{
		return wptr_;
	}

	// ����header�ĳ���.
	size_t header_length() const
	{
		return HEADER_LENGTH;
	}

	// ����body�ĳ���.
	size_t body_length() const
	{
		return body_length_;
	}

	// ����body�ĳ���.
	void body_length(size_t length)
	{
		if (length < 0)
			body_length_ = 0;
		body_length_ = length;
	}

	// ����header��ͷָ��.
	message::headerPtr head()
	{
		return msg_;
	}

	// ר���ڽ������յ�header����.
	bool decode_header()
	{
		msg_ = (headerPtr)data_;
		header msg = *msg_;
		// ���У���.
		if (msg.checksum != (msg.type + msg.rand + msg.packsize))			
		{
			body_length_ = 0;
			return false;
		}
		// ����ʵ�����ݳ���. ��ʽ: body_length_ = ���ݰ���С - header_length.
		body_length_ = msg.packsize - HEADER_LENGTH;
		// ����ڴ治��,���·�����ڴ�.
		if ((body_length_ + HEADER_LENGTH) > data_size_)
		{
			if (data_ != NULL)
				delete data_;

			data_size_ = body_length_ + HEADER_LENGTH;
			data_ = new char[data_size_];
			msg_ = (headerPtr)data_;
			*msg_ = msg;			
		}
		else if (((body_length_ + HEADER_LENGTH) < DEFAULT_BODY_LENGTH) && data_size_ > DEFAULT_BODY_LENGTH)
		{	// �����ϴη����ڴ����,���·����ڴ�.
			if (data_ != NULL)
				delete data_;

			data_size_ = DEFAULT_BODY_LENGTH + HEADER_LENGTH;
			data_ = new char[data_size_];
			msg_ = (headerPtr)data_;
			*msg_ = msg;
		}

		// �ƶ������ݿ�ʼλ��.
		wptr_ = rptr_ = data_;
		wptr_ += HEADER_LENGTH;

		return true;
	}

	// ר���ڼ�����body����.
	bool check_body(size_t bytes_transferred)
	{
		msg_ = (headerPtr)data_;
		header msg = *msg_;
		if (bytes_transferred < body_length_) 
		{
			body_length_ -= bytes_transferred;
			wptr_ += bytes_transferred;
			return false; // δ���������߳���.
		}
		
		// �ָ�дָ��.
		wptr_ = data_ + HEADER_LENGTH;
		return true;
	}

private:
	headerPtr msg_;			// ָ������ͷָ��.
	char* data_;			// ����ָ��.
	char* wptr_;			// д����ָ��.
	char* rptr_;			// ������ָ��.
	size_t data_size_;		// �ڴ���С.
	size_t body_length_;	// Ϊʵ�����ݳ���.
	boost::weak_ptr<session> session_;
};

//////////////////////////////////////////////////////////////////////////
class io_service_pool
	: private boost::noncopyable
{
public:
	explicit io_service_pool(std::size_t pool_size);

	void run();
	void stop();

	boost::asio::io_service& get_io_service();

private:
	typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
	typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

	std::vector<io_service_ptr> io_services_;
	std::vector<work_ptr> work_;
	std::size_t next_io_service_;
};

//////////////////////////////////////////////////////////////////////////
#if defined(SOCKET_SSL) // ʹ��ssl����.

class session
	: public boost::enable_shared_from_this<session>
{
public:
	session(boost::asio::io_service& io_service, 
			jobqueue<message>& jobwork, 
			boost::object_pool<message>& message_pool, 
			boost::asio::ssl::context& context);
	~session();

	ssl_socket::lowest_layer_type& socket();

	void handle_handshake(const boost::system::error_code& error);

	void start();
	void write();

	void handle_read_head(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read_body(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

private:
	ssl_socket socket_;
	jobqueue<message>& jobwork_;
	message* message_;

#if defined(USE_SYNC)
	boost::asio::io_service::strand strand_;
#endif // USE_SYNC

	boost::object_pool<message>& message_pool_;
};

//////////////////////////////////////////////////////////////////////////
class server
{
public:
	server(short port, jobqueue<message>& jobwork, std::size_t io_service_pool_size = 4);

	void run();
	void stop();
	void handle_accept(session_ptr new_session,
		const boost::system::error_code& error);

	std::string get_password() const;

private:
	jobqueue<message>& jobwork_;
	io_service_pool io_service_pool_;
	tcp::acceptor acceptor_;

	boost::asio::ssl::context context_;
	boost::object_pool<message> message_pool_;
};

#else // δʹ��ssl����.

class session
	: public boost::enable_shared_from_this<session>
{
public:
	session(boost::asio::io_service& io_service, jobqueue<message>& jobwork, boost::object_pool<message>& message_pool);
	~session();
	tcp::socket& socket();

	void start();
	void write();

	void handle_read_head(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read_body(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

private:
	tcp::socket socket_;
	jobqueue<message>& jobwork_;
	message* message_;	

#if defined(USE_SYNC)
	boost::asio::io_service::strand strand_;
#endif // USE_SYNC

	boost::object_pool<message>& message_pool_;
};

//////////////////////////////////////////////////////////////////////////
class server
{
public:
	server(short port, jobqueue<message>& jobwork, std::size_t io_service_pool_size = 4);

	void run();
	void stop();
	void handle_accept(session_ptr new_session,
		const boost::system::error_code& error);

private:
	jobqueue<message>& jobwork_;
	io_service_pool io_service_pool_;
	tcp::acceptor acceptor_;

	boost::object_pool<message> message_pool_;
};

#endif // SOCKET_SSL



#endif // NET_H__