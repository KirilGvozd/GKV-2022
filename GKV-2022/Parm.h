#pragma once
#define PARM_IN		L"-in:"
#define	PARM_OUT	L"-out:"
#define	PARM_LOG	L"-log:"
#define	PARM_MORE	L"-more"
#define	PARM_LENTA	L"-lenta"
#define	PARM_IT		L"-IT"
#define	PARM_LT		L"-LT"
#define	PARM_LOUT	L"-Lout"
#define	PARM_HELP	L"-h"
#define	PARM_MAX_SIZE	300
#define	PARM_OUT_DEFAULT_EXIT	L".out"
#define	PARM_LOG_DEFAULT_EXIT	L".log"


namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		bool more = false;
		bool lenta = false;
		bool it = false;
		bool lt = false;
		bool Lout = false;
		bool help = false;
	};
	PARM getparm(int argc, wchar_t* argv[]);
}