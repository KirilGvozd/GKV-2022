#pragma once
#include "Parm.h"
#include "FST_def.h"



namespace FST {
	struct RELATION {			
		unsigned char symbol;			
		short nnode;			//����� �������

		RELATION(unsigned char c = 0x00, short ns = NULL);
	};

	struct NODE {				
		short n_relation;		
		RELATION* relations;	

		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST {				
		unsigned char* string;		
		short position;			
		short nstates;			// ���������� ��������� ��������
		NODE* nodes;			
		short* rstates;			// ��������� ��������� �������� �� ������ �������

		FST(unsigned char* s, short ns, NODE n, ...);
	};

	
	bool execute(FST& fst);

};
