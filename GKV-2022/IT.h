#pragma once
#define ID_MAXSIZE		10			// максимальное количество символов в идентификаторе
#define TI_MAXSIZE		4096		// максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000	// значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT	0x00		// значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff	// нет элкмента таблицы идентификаторов
#define TI_STR_MAXSIZE	255			// 
#include "Parm.h"
#include "Log.h"
namespace IT			// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, INT16 = 1, NUL = 6, VOI = 4, CHR = 5 };			// типы данных идентификаторов: integer, string, int16bit, без типпа данных, процедура, символ
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5, LO = 6 };	// типы идентификаторов: переменна€, функци€, параметр, литерал, оператор

	struct Entry	// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		unsigned char	id[ID_MAXSIZE];		// индентификатор (автоматически усекаетс€ до ID_MAXSIZE)
		IDDATATYPE	iddatatype = NUL;			// тип данных
		IDTYPE		idtype;				// тип идентификатора
		int parm = 0;
		int nums = 0; //0 - 10 —— 1 - 16 ——
		union
		{
			int vint;					// значение integer
			struct
			{
				int len;						// количество символов в string
				unsigned char str[TI_STR_MAXSIZE - 1];	// символы string
			} vstr;				// значение string
		} value;		// значение идентификатора
	};

	struct IdTable				// экземпл€р таблицы идентификаторов
	{
		int max_size;			// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;				// текущий размер таблицы идентификаторов < maxsize
		Entry* table;			// массив строк таблицы идентификаторов
	};

	IdTable Create(				// создать таблицу идентификаторов
		int size				// емкость таблицы идентификаторов < TI_MAXSIZE
	);
	void Add(				// добавить строку в таблицу идентификаторов
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		Entry entry			// строка таблицы идентификаторов
	);
	void Add(IdTable& idtable, Entry entry, int i);

	Entry GetEntry(			// получить строку таблицы идентификаторов
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		int n				// номер получаемой строки
	);

	int IsId(				// возврат: номер строки (если есть), TI_NULLIDX (если нет)
		IdTable& idtable,	// экземпл€р таблицы идентификаторов
		unsigned char id[ID_MAXSIZE]	// идентификатор
	);

	void Delete(IdTable& idtable);	// удалить таблицу лексем (освободить пам€ть)

	void showITable(IdTable& table, ostream * log);	// вывод таблицы лексем
};