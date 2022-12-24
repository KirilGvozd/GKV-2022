#pragma once
#define LEXEMA_FIXSIZE	1			// ������������� ������ �������
#define LT_MAXSIZE		4096		// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX	0xfffffff	// ��� �������� ������� ���������������
#define LEX_INTEGER		't'	// ������� ��� integer
#define LEX_STRING		't'	// ������� ��� string
#define LEX_ID			'i'	// ������� ��� ��������������
#define LEX_LITERAL		'l'	// ������� ��� ��������
#define LEX_FUNCTION	'f'	// ������� ��� function
#define LEX_RETURN		'r'	// ������� ��� return
#define LEX_RUNOUT		'p'	// ������� ��� runout
#define LEX_RUNIN		'p'	// ������� ��� runin
#define LEX_FOR			'o'	// ������� ��� for
#define LEX_MAIN		'm'	// ������� ��� main
#define LEX_SEMICOLON	';'	// ������� ��� ;
#define LEX_TWOPOINT	':'	// ������� ��� :
#define LEX_COMMA		','	// ������� ��� ,
#define LEX_LEFTBRACE	'{'	// ������� ��� {
#define LEX_BRACELET	'}'	// ������� ��� }
#define LEX_LEFTTHESIS	'('	// ������� ��� (
#define LEX_RIGHTTHESIS	')'	// ������� ��� )
#define LEX_PLUS		'v'	// ������� ��� +
#define LEX_MINUS		'v'	// ������� ��� -
#define LEX_STAR		'v'	// ������� ��� *
#define LEX_DIRSLASH	'v'	// ������� ��� /
#define LEX_OPERATOR	'v'	// ������� ��� ����������
#define LEX_LOGOPERATOR 'q' // ������� ��� ���������� ����������
#define LEX_SMALL		'q' // <
#define LEX_BIG			'q' // >
#define LEX_AND			'&' // &
#define LEX_EXL			'q' // !
#define LEX_TILDA		'q' // ~
#define LEX_EQUAL		'='
#define LEX_ROOF		'^'
#define LEX_IF			'w' // ������� ������
#define LEX_VOID		'n' // ������� void
#define LEX_ENDIF		'|' // ����� �������
#define TI_NULLIDX		0xffffffff
namespace LT		// ������� ������
{
	struct Entry	// ������ ������� ������
	{
		unsigned char lexema;	// �������
		int sn;					// ����� ������ � �������� ������
		int idxTI = TI_NULLIDX;				// ������ � ������� ��������������� ��� LT_TI_NULLIDX
	};

	struct LexTable				// ��������� ������� ������
	{
		int max_size;			// ������� ������� ������ < LT_MAXSIZE
		int size;				// ������� ������ ������� ������ < max_size
		Entry* table;			// ������ ����� ������� ������
	};

	LexTable Create(		// ������� ������� ������
		int size			// ������� ������� ������ < LT_MAXSIZE
	);

	void Add(				// �������� ������ � ������� ������
		LexTable& lextable,	// ��������� ������� ������
		Entry entry			// ������ ������� ������
	);
	void Add(LexTable& lextable, Entry entry, int i);

	Entry GetEntry(			// �������� ������ ������� ������
		LexTable& lextable,	// ��������� ������� ������
		int n				// ����� ���������� ������
	);

	void Delete(LexTable& lextable);	// ������� ������� ������ (���������� ������)

	Entry writeEntry(					// ��������� ������ ������� ������
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line
	);
	void writeLexTable(std::ostream* stream, LT::LexTable& lextable);
	void showTable(LexTable lextable, std::ostream *fout);	// ����� ������� ������

};