#include "Parm.h"
#include "Error.h"
#include "Log.h"
#include <locale>	
#include <cwchar>
#include <iostream>
#include <fstream>
namespace Parm {
	PARM getparm(int argc, wchar_t* argv[]) {
		PARM parm;
		bool in = 0, o = 0, l = 0, h = 0;
		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);
			if (wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				in = 1;
			}
			if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(parm.out, argv[i] + wcslen(PARM_OUT));
				o = 1;
			}
			if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
				l = 1;
			}
			if (wcsstr(argv[i], PARM_MORE))
			{
				parm.more = true;
			}

			if (wcsstr(argv[i], PARM_LENTA))
			{
				parm.lenta = true;
			}
			if (wcsstr(argv[i], PARM_IT))
			{
				parm.it = true;
			}
			if (wcsstr(argv[i], PARM_LT))
			{
				parm.lt = true;
			}
			if (wcsstr(argv[i], PARM_LOUT))
			{
				parm.Lout = true;
			}
			if (wcsstr(argv[i], PARM_HELP))
			{
				parm.help = true;

			}
		}
		if (parm.help) {
			std::cout << "						------HELP------\n";
			std::cout << "	-in:[Имя файла]    - Файл исходного кода(Обязательный параметр)\n";
			std::cout << "	-out:[Имя файла]   - Файл вывода преобразовонного кода\n";
			std::cout << "	-log:[Имя файла]   - Файл лога\n";
			std::cout << "	-more              - Вывод всей доступной информации\n";
			std::cout << "	-lenta             - Вывод ленты разбора ситаксического анализа\n";
			std::cout << "	-IT                - Вывод таблицы идетификаторов\n";
			std::cout << "	-LT                - Вывод таблицы лексем\n";
			std::cout << "	-Lout              - Вывод преобразовонного исходного кода\n";
		}
		if (!in) throw  ERROR_THROW(100);
		if (!o)
		{
			wcscpy_s(parm.out, parm.in);
			wcscat_s(parm.out, PARM_OUT_DEFAULT_EXIT);
		}
		if (!l)
		{
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXIT);
		}
		return parm;
	}
}