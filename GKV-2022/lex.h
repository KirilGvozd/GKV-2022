#pragma once
#include "stdafx.h"
#define maxsize 10000
#define maxword 10000
using  namespace std;
namespace Lex {

	struct LEX //in lex.h
	{
		IT::IdTable idtable;;
		LT::LexTable lextable;
	};

	LEX lexAnaliz(Log::LOG log, In::IN in);
	bool checkBrace(unsigned char** word, int k);
	int IntinInt16(unsigned char* word);
	int getIndexInLT(LT::LexTable& lextable, int itTableIndex);
}