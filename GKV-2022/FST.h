#pragma once
#include "Parm.h"
#include "FST_def.h"



namespace FST {
	struct RELATION {			
		unsigned char symbol;			
		short nnode;			//номер вершины

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
		short nstates;			// количество состояний автомата
		NODE* nodes;			
		short* rstates;			// возможные состояния автомата на данной позиции

		FST(unsigned char* s, short ns, NODE n, ...);
	};

	
	bool execute(FST& fst);

};
