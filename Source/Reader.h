#ifndef __reader_client_h__
#define __reader_client_h__

#include "ReaderWindow.h"

class ReaderClient{
public:

	ReaderClient();
	~ReaderClient();

	void Run();

private:
	std::wstring res_file_name;
};

#endif