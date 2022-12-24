#pragma once


#include "In.h"	
#include "Error.h"	
#include "Parm.h"
#include <iostream>
#include <fstream>
using namespace std;
namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream* stream;
		int errors_cout;
	};

	static const LOG INITLOG = { L"", NULL };
	LOG getlog(wchar_t logfile[]);
	void WriteLine(ostream* log, const char* c, ...);
	void WriteLine(ostream* log, const wchar_t* c, ...);
	void WriteLog(ostream* log);
	void WriteParm(ostream* log, Parm::PARM parm);
	void WriteIn(ostream* log, In::IN in);
	void WriteError(Log::LOG log, Error::ERROR error);
	void WriteErrors(Log::LOG log, Error::ERROR error);
	void Close(LOG log);
	void writelex(In::IN in);
};