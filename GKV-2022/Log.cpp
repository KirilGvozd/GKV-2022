#include "Log.h"	
#include <time.h>

#pragma warning(disable:4996)
using namespace std;

#include <locale>	
#include <cwchar>
#include <iostream>
#include "Parm.h"
#include <fstream>
namespace Log {
	LOG getlog(wchar_t logfile[]) {
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
			throw ERROR_THROW(112);
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(ostream* log, const char* c, ...) {
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "")
			*log << ptr[i++];
		*log << endl;
	}

	void WriteLine(ostream* log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[100];
		int i = 0;
		while (ptr[i] != L"")
		{
			wcstombs(temp, ptr[i++], sizeof(temp));
			*log << temp;
		}
		*log << endl;
	}
	void WriteLog(ostream* log) {
		char temp[100];
		time_t tmr;
		time(&tmr);
		tm *tmf= localtime(&tmr);
		strftime(temp, sizeof(temp), "\n---------------Протокол-----------------\n ------%d.%m.%y %T-----\n  ", tmf);
		*log << temp;
	}
	void WriteIn(ostream* log, In::IN in) {
		*log
		<< "\n\nВсего символов: " << in.size
			<< "\n\nВсего строк: " << in.lines
			<< "\n\nПропущено: " << in.ignor << endl;
	}
	void WriteParm(ostream* log, Parm::PARM parm) {
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log << "\n ---- Параметры ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;
	}
	void WriteError(Log::LOG log, Error::ERROR error) {
		if (log.stream)
		{
			*log.stream << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
		}
		throw error;
	}
	void WriteErrors(Log::LOG log, Error::ERROR error) {
		if (log.stream)
		{
			*log.stream << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
		}
		cout << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
	}
	void Close(LOG log) {
		log.stream->close();
		delete log.stream;
	}
}