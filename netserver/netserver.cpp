// netserver.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "net.hpp"
#include "worker.hpp"
#include "jobqueue.hpp"

class RunServer
{
public:
	RunServer(int port)
	: _jobqueueptr(new jobqueue<message>)
	, _workerptr(new worker<message>((*_jobqueueptr.get())))
	, _serverptr(new server(port, (*_jobqueueptr.get())))
	{
 		_listenthreadptr.reset(new boost::thread(boost::bind(&RunServer::RunNetListen, this)));
 		_workthreadptr.reset(new boost::thread(boost::bind(&RunServer::RunWorks, this)));
	}
	
	~RunServer()
	{
		_workerptr->stop(); // ֹͣ����.
		_serverptr->stop(); // ֹͣ����.
		_jobqueueptr->notify_all(); // ֪ͨ����ȡ���ȴ�����.

		_listenthreadptr->join();
		_workthreadptr->join();
	}

	void RunNetListen()
	{
		if (_serverptr)
			_serverptr->run();
	}

	void RunWorks()
	{
		if (_workerptr)
			_workerptr->run();
	}

private:
	boost::shared_ptr<boost::thread> _listenthreadptr;
	boost::shared_ptr<boost::thread> _workthreadptr;
	boost::shared_ptr<jobqueue<message> > _jobqueueptr;	
	boost::shared_ptr<worker<message> > _workerptr;	
	boost::shared_ptr<server> _serverptr;
};

class __declspec(novtable) A
{
public:
	virtual void print()
	{
		printf("a\n");
	}
};

class  B : public A
{
public:
	virtual void print()
	{
		printf("b\n");
	}
};

class C : public B
{
public:
	/*virtual*/ void print()
	{
		printf("c\n");
	}
};
#include <io.h>
int main(int argc, char* argv[])
{
	try
	{
		open("c:\\s", 0);
		A * a = new B();
		a->print(); // �����a��Ȼ��ָ��A��ָ�룬���Ǳ����õĺ���(foo)ȴ��B��!

		A* p2 = (A*)new C;
		p2->print();

		if (argc != 2)
		{
			std::cerr << "Usage: netserver <port>\n";
			return 1;
		}

		RunServer runServer1(atoi(argv[1]));                // ��һ��server.
		RunServer runServer2(atoi(argv[1]) + 1);            // �ڶ���server.

		// �ȴ��˳�...
		std::string in;

		std::cout << "����\'exit\'�˳�.\nprompt # ";
		for (;;)
		{
			char c = getchar();
			if (c == '\n')
			{
				if (in == "exit")
					break;
				else if(in != "")
					std::cout << "Bad command ! \n";


				std::cout << "prompt # ";
				in = "";				
			}
			else
			{
				in += c;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

