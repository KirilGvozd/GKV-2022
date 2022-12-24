#pragma once
#define ID_MAXSIZE		10			// ������������ ���������� �������� � ��������������
#define TI_MAXSIZE		4096		// ������������ ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000	// �������� �� ��������� ��� ���� integer
#define TI_STR_DEFAULT	0x00		// �������� �� ��������� ��� ���� string
#define TI_NULLIDX		0xffffffff	// ��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255			// 
#include "Parm.h"
#include "Log.h"
namespace IT			// ������� ���������������
{
	enum IDDATATYPE { INT = 1, STR = 2, INT16 = 1, NUL = 6, VOI = 4, CHR = 5 };			// ���� ������ ���������������: integer, string, int16bit, ��� ����� ������, ���������, ������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5, LO = 6 };	// ���� ���������������: ����������, �������, ��������, �������, ��������

	struct Entry	// ������ ������� ���������������
	{
		int			idxfirstLE;			// ������ ������ ������ � ������� ������
		unsigned char	id[ID_MAXSIZE];		// �������������� (������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE	iddatatype = NUL;			// ��� ������
		IDTYPE		idtype;				// ��� ��������������
		int parm = 0;
		int nums = 0; //0 - 10 �� 1 - 16 ��
		union
		{
			int vint;					// �������� integer
			struct
			{
				int len;						// ���������� �������� � string
				unsigned char str[TI_STR_MAXSIZE - 1];	// ������� string
			} vstr;				// �������� string
		} value;		// �������� ��������������
	};

	struct IdTable				// ��������� ������� ���������������
	{
		int max_size;			// ������� ������� ��������������� < TI_MAXSIZE
		int size;				// ������� ������ ������� ��������������� < maxsize
		Entry* table;			// ������ ����� ������� ���������������
	};

	IdTable Create(				// ������� ������� ���������������
		int size				// ������� ������� ��������������� < TI_MAXSIZE
	);
	void Add(				// �������� ������ � ������� ���������������
		IdTable& idtable,	// ��������� ������� ���������������
		Entry entry			// ������ ������� ���������������
	);
	void Add(IdTable& idtable, Entry entry, int i);

	Entry GetEntry(			// �������� ������ ������� ���������������
		IdTable& idtable,	// ��������� ������� ���������������
		int n				// ����� ���������� ������
	);

	int IsId(				// �������: ����� ������ (���� ����), TI_NULLIDX (���� ���)
		IdTable& idtable,	// ��������� ������� ���������������
		unsigned char id[ID_MAXSIZE]	// �������������
	);

	void Delete(IdTable& idtable);	// ������� ������� ������ (���������� ������)

	void showITable(IdTable& table, ostream * log);	// ����� ������� ������
};