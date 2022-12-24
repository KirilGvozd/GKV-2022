#define _CRT_SECURE_NO_WARNINGS

#define IN_CODE_DELIMETR '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='
#define MAX_INTEGER 2147483647
#define MIN_INTEGER -2147483647
#define MAX_STROKA 256
#include "FST_def.h"
#include "stdafx.h"
namespace Lex {
	LEX lexAnaliz(Log::LOG log, In::IN in) 
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		int i = 0;
		int line = 1;
		int indexLex = 0;
		int indexID = 0;
		int countLit = 1;
		int position = 0;

		IT::Entry entryIT;
		IT::Entry bufentry;
		unsigned char emptystr[] = "";
		bool findFunc = false;
		bool findParm = false;
		bool findSameID = false;
		bool findReturn = false;
		bool endif = false;
		int Idx_Func_IT = 0;
		int Parm_count_IT = 0;
		int count_main = 0;
		bool newindf = false;
		bool errorssem = false;

		unsigned char* RegionPrefix = new unsigned char[10]{ "" };
		unsigned char* buferRegionPrefix = new unsigned char[10]{ "" };
		unsigned char* pastRegionPrefix = new unsigned char[10]{ "" }; 
		unsigned char* L = new unsigned char[2]{ "L" };
		unsigned char* source1 = new unsigned char[8]{ "source1" };
		unsigned char* source2 = new unsigned char[8]{ "source2" };
		unsigned char* source = new unsigned char[8]{ "source" };
		unsigned char* bufL = new unsigned char[TI_STR_MAXSIZE];
		char* charCountLit = new char[10]{ "" };
		unsigned char* nameLiteral = new unsigned char[TI_STR_MAXSIZE] { "" };
		
		unsigned char** word = in.word;
		for (i = 0; word[i][0] != NULL; indexLex++, i++)
		{


			bool findSameID = false;
			FST::FST fstTypeInteger(word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				newindf = true;
				entryIT.iddatatype = IT::INT;
				continue;
			}
			FST::FST fstTypeVoid(word[i], FST_VOID);
			if (FST::execute(fstTypeVoid))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_VOID, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				newindf = true;
				entryIT.iddatatype = IT::VOI;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_STROKA);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				newindf = true;
				entryIT.iddatatype = IT::STR;
				_mbscpy(entryIT.value.vstr.str, emptystr);
				continue;
			}
			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.idtype = IT::F;
				findFunc = true;
				findParm = true;
				Parm_count_IT = 0;
				Idx_Func_IT = 0;
				findReturn = false;
				continue;
			}
			FST::FST fstReturn(word[i], FST_RETURN);
			if (FST::execute(fstReturn))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findReturn = true;
				continue;
			}
			FST::FST fstIf(word[i], FST_IF);
			if (FST::execute(fstIf))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				endif = true;
				continue;
			}
			FST::FST fstRunOut(word[i], FST_OUTPUT);
			if (FST::execute(fstRunOut))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RUNOUT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRunin(word[i], FST_RUNIN);
			if (FST::execute(fstRunin))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RUNIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRoof(word[i], FST_ROOF);
			if (FST::execute(fstRoof))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_ROOF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			//FST::FST fstFOR(word[i], FST_FOR);
			//if (FST::execute(fstFOR))
			//{
			//	LT::Entry entryLT = writeEntry(entryLT, LEX_FOR, LT_TI_NULLIDX, line);
			//	LT::Add(lextable, entryLT);
			//	endif = true;
			//	continue;
			//}

			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				count_main++;
				findReturn = false;
				_mbscpy(pastRegionPrefix, RegionPrefix);
				_mbscpy(RegionPrefix, emptystr);
				continue;
			}

			FST::FST fstLen(word[i], FST_LEN);
			if (FST::execute(fstLen))
			{
				int idx = TI_NULLIDX;
				idx = IT::IsId(idtable, word[i]);
				if (idx != TI_NULLIDX)
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
					LT::Add(lextable, entryLT);
					continue;
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);

				entryIT.idtype = IT::F;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = TI_INT_DEFAULT;
				entryIT.parm = 1;
				entryIT.idxfirstLE = indexLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT.idtype = IT::P;
				entryIT.iddatatype = IT::STR;
				entryIT.value.vstr.len = 0;
				memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
				entryIT.idxfirstLE = -1;
				_mbscpy(entryIT.id, source);
				IT::Add(idtable, entryIT);
				indexID = indexID + 1;
				entryIT = bufentry;
				continue;
			}
			FST::FST fstCmp(word[i], FST_CMP);
			if (FST::execute(fstCmp))
			{
				int idx = TI_NULLIDX;
				idx = IT::IsId(idtable, word[i]);
				if (idx != TI_NULLIDX)
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
					LT::Add(lextable, entryLT);
					continue;
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);

				entryIT.idtype = IT::F;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = TI_INT_DEFAULT;
				entryIT.parm = 2;

				entryIT.idxfirstLE = indexLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT.idtype = IT::P;
				entryIT.iddatatype = IT::STR;
				entryIT.value.vstr.len = 0;
				memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
				entryIT.idxfirstLE = -1;
				_mbscpy(entryIT.id, source1);
				IT::Add(idtable, entryIT);
				entryIT.idtype = IT::P;
				entryIT.iddatatype = IT::STR;
				entryIT.value.vstr.len = 0;
				memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
				entryIT.idxfirstLE = -1;
				_mbscpy(entryIT.id, source2);
				IT::Add(idtable, entryIT);
				indexID = indexID + 2;
				entryIT = bufentry;
				continue;
			}
			FST::FST fstCOMP(word[i], FST_COMP);
			if (FST::execute(fstCOMP))
			{
				int idx = TI_NULLIDX;
				idx = IT::IsId(idtable, word[i]);
				if (idx != TI_NULLIDX)
				{
					LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
					LT::Add(lextable, entryLT);
					continue;
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);

				entryIT.idtype = IT::F;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = TI_INT_DEFAULT;
				entryIT.parm = 2;

				entryIT.idxfirstLE = indexLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT.idtype = IT::P;
				entryIT.iddatatype = IT::STR;
				entryIT.value.vstr.len = 0;
				memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
				entryIT.idxfirstLE = -1;
				_mbscpy(entryIT.id, source1);
				IT::Add(idtable, entryIT);
				entryIT.idtype = IT::P;
				entryIT.iddatatype = IT::STR;
				entryIT.value.vstr.len = 0;
				memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
				entryIT.idxfirstLE = -1;
				_mbscpy(entryIT.id, source2);
				IT::Add(idtable, entryIT);
				indexID = indexID + 2;
				entryIT = bufentry;
				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif)) {
				int length = _mbslen(word[i]);
				if (length > 5) {
					Log::WriteError(log, Error::geterrorin(202, line, position));
				}
				if (findFunc) {
					int idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						if(entryIT.iddatatype != IT::IDDATATYPE::NUL) {
							errorssem = true;
							Log::WriteErrors(log, Error::geterrorin(305, line, position));
						}
						LT::Entry entryLT = LT::writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
				}
				else {
					int idx = IT::IsId(idtable, word[i]);

					if (idx != TI_NULLIDX) {
						if (entryIT.iddatatype != IT::IDDATATYPE::NUL) {
							errorssem = true;
							Log::WriteErrors(log, Error::geterrorin(305, line, position));
						}
						LT::Entry entryLT = LT::writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					_mbscpy(buferRegionPrefix, RegionPrefix); 
					word[i] = _mbscat(buferRegionPrefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX)
					{
						if (entryIT.iddatatype != IT::IDDATATYPE::NUL) {
							errorssem = true;
							Log::WriteErrors(log, Error::geterrorin(305, line, position));
						}
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
					if (!newindf) Log::WriteError(log, Error::geterrorin(200, line, position));
				}
				LT::Entry entryLT = LT::writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);
				if (findParm && !findFunc) {
					entryIT.idtype = IT::P;
					Parm_count_IT++;
				}
				else if (!findFunc) {
					entryIT.idtype = IT::V;
					if (entryIT.iddatatype == IT::NUL) {
						errorssem = true;
						Log::WriteErrors(log, Error::geterrorin(303, line, position));
					}
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					else
						if (entryIT.iddatatype == IT::STR) {
							entryIT.value.vstr.len = 0;
							memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
						}
				}
				else {
					_mbscpy(pastRegionPrefix, RegionPrefix);
					_mbscpy(RegionPrefix, word[i]);
				}
				entryIT.idxfirstLE = indexLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = bufentry;
				if(findFunc)
				Idx_Func_IT = IT::IsId(idtable, word[i]);
				newindf = false;
				findFunc = false;
				continue;
			}

			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt)) {
				int value = atoi((char*)word[i]);
				if (value > MAX_INTEGER) {
					Log::WriteError(log, Error::geterrorin(202, line, position)); }
				if (value < MIN_INTEGER) { 
					Log::WriteError(log, Error::geterrorin(202, line, position)); }
				for (int k = 0; k < idtable.size; k++) {
					if (idtable.table[k].iddatatype == IT::IDDATATYPE::INT && idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L) {
						LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID) continue;
				if (lextable.table[lextable.size - 1].lexema == LEX_OPERATOR && lextable.table[lextable.size - 2].lexema == LEX_EQUAL && idtable.table[lextable.table[lextable.size - 1].idxTI].id[0] == '-') {
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID-1, line);
					LT::Add(lextable, entryLT, lextable.size - 1);
				}
				else {
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
				}
				entryIT.iddatatype = IT::INT;
				entryIT.idtype = IT::L;
				if (lextable.table[lextable.size - 2].lexema == LEX_EQUAL && word[i-1][0] == '-')
				{
					entryIT.value.vint = -value;
				}else
					entryIT.value.vint = value; 
				entryIT.idxfirstLE = indexLex;
				_itoa_s(countLit++, charCountLit, sizeof(char) * 10, 10);
				_mbscpy(bufL, L);
				word[i] = _mbscat(bufL, (unsigned char*)charCountLit);
				_mbscpy(entryIT.id, word[i]);
				if (lextable.table[lextable.size - 2].lexema == LEX_EQUAL && word[i - 1][0] == '-')
				{
					IT::Add(idtable, entryIT, idtable.size-1);
				}
				else
					IT::Add(idtable, entryIT);
					entryIT = bufentry;
				continue;
			}
			FST::FST fstLiteralInt16(word[i], FST_INT16LIT);
			if (FST::execute(fstLiteralInt16)) {
				int length = _mbslen(word[i]);
				if (length > 14) { 
					Log::WriteError(log, Error::geterrorin(202, line, position));}
				unsigned char* value = word[i];
				for (int k = 0; k < idtable.size; k++) {
					if (idtable.table[k].iddatatype == IT::IDDATATYPE::INT16 && idtable.table[k].value.vstr.str == value && idtable.table[k].idtype == IT::L) {
						LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID) continue;
				if (lextable.table[lextable.size - 1].lexema == LEX_OPERATOR && lextable.table[lextable.size - 2].lexema == LEX_EQUAL && idtable.table[lextable.table[lextable.size - 1].idxTI].id[0] == '-') {
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID - 1, line);
					LT::Add(lextable, entryLT, lextable.size - 1);
				}
				else {
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
				}
				entryIT.iddatatype = IT::INT16;
				entryIT.nums = 1;
				entryIT.idtype = IT::L;
				_mbscpy(entryIT.value.vstr.str, word[i]); 
				entryIT.value.vint = IntinInt16(word[i]);

				if (lextable.table[lextable.size - 2].lexema == LEX_EQUAL && word[i - 1][0] == '-')
				{
					entryIT.value.vint = -entryIT.value.vint;
				}
				else
					entryIT.value.vint = entryIT.value.vint;
				entryIT.idxfirstLE = indexLex;
				_itoa_s(countLit++, charCountLit, sizeof(char) * 10, 10);
				_mbscpy(bufL, L);
				word[i] = _mbscat(bufL, (unsigned char*)charCountLit);
				_mbscpy(entryIT.id, word[i]);
				if (lextable.table[lextable.size - 2].lexema == LEX_EQUAL && word[i - 1][0] == '-')
				{
					IT::Add(idtable, entryIT, idtable.size - 1);
				}
				else
					IT::Add(idtable, entryIT);
				entryIT = bufentry;
				continue;
			}

			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString)) {

				int length = _mbslen(word[i]);
				if (length == 2) { errorssem = true; Log::WriteErrors(log, Error::geterrorin(310, line, position)); }
				if (length > MAX_STROKA) {
					Log::WriteError(log, Error::geterrorin(202, line, position));
					throw  ERROR_THROW_IN(202, line, position); }
				for (int k = 0; k < length; k++)
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;

				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].iddatatype == IT::IDDATATYPE::STR && !(_mbscmp(idtable.table[k].value.vstr.str, word[i])))
					{
						findSameID = true;
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}

				if (findSameID) continue;
				LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.iddatatype = IT::STR;
				entryIT.idtype = IT::L;

				entryIT.value.vstr.len = length - 2;
				_mbscpy(entryIT.value.vstr.str, word[i]); 
				entryIT.idxfirstLE = indexLex;
				_itoa_s(countLit++, charCountLit, sizeof(char) * 10, 10);
				_mbscpy(bufL, L);
				nameLiteral = _mbscat(bufL, (unsigned char*)charCountLit);
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				entryIT = bufentry;
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);

				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indexLex;
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				entryIT = bufentry;
				continue;
			}

			FST::FST fstlogOperator(word[i], FST_LOGOPERATOR);
			if (FST::execute(fstlogOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LOGOPERATOR, indexID++, line);

				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indexLex;
				entryIT.idtype = IT::LO;
				IT::Add(idtable, entryIT);
				entryIT = bufentry;
				continue;
			}
			FST::FST fstSemicolon(word[i], FST_SEMICOLON);
			if (FST::execute(fstSemicolon))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);

				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE && !checkBrace(word, i + 1))
				{
					_mbscpy(RegionPrefix, pastRegionPrefix);
				}
				if(findParm)
				idtable.table[Idx_Func_IT].parm = Parm_count_IT;
				findParm = false;
				findParm = false;
				Parm_count_IT = 0;
				Idx_Func_IT = 0;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstAnd(word[i], FST_AND);
			if (FST::execute(fstAnd))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_AND, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTilda(word[i], FST_TILDA);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_TILDA, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTwoPoint(word[i], FST_TWOPOINT);
			if (FST::execute(fstTwoPoint))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_TWOPOINT, LT_TI_NULLIDX, line);
					LT::Add(lextable, entryLT);
					continue;
			}
			FST::FST fstLitStr_1(word[i], FST_LITERALSTRING_1);
			if (FST::execute(fstLitStr_1))
			{
				LT::Entry entryLT = writeEntry(entryLT, word[i][0], LT_TI_NULLIDX, line);
					LT::Add(lextable, entryLT);
					errorssem = true;
					Log::WriteErrors(log, Error::geterrorin(311, line, position));
					continue;
			}



			position += _mbslen(word[i]);
			if (word[i][0] == IN_CODE_DELIMETR && endif) {
				LT::Entry entryLT = writeEntry(entryLT, LEX_ENDIF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				endif = false;
			}
				if (word[i][0] == IN_CODE_DELIMETR) {
					line++;
					position = 0;
					indexLex--;
					continue;
				}
				Log::WriteError(log, Error::geterrorin(201, line, position));
		}
		lex.idtable = idtable;
		lex.lextable = lextable;
		if (count_main > 1) { errorssem = true; Log::WriteErrors(log, Error::geterror(302));  }
		if (count_main == 0) { errorssem = true; Log::WriteErrors(log, Error::geterror(301));  }
		if (count_main == 1 && findReturn) { throw Error::geterror(601); }
		if (errorssem) throw Error::geterror(113);
		return lex;
	}


	bool checkBrace(unsigned char** word, int k)
	{
		while (word[k][0] == IN_CODE_DELIMETR)
		{
			k++;
		}
		if (word[k][0] == LEX_LEFTBRACE)
			return 1;
		else
			return 0;
	}
	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)					
	{
		if (itTableIndex == TI_NULLIDX)		
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return TI_NULLIDX;
	}

	int IntinInt16(unsigned char* word)
	{
		int length = _mbslen(word);
		for (int k = 0; k < length-2; k++)
			word[k] = word[k + 3];
		int id = 0;
		int sum = 0;;
		for (int k = length - 4; k >= 0; k--) {
			int x = 0;
			if (word[k] >= 'A' && word[k] <= 'F') {
				x = abs('A' - word[k]) + 10;
			}
			else x = abs('0'- word[k]);
			sum = sum + x * pow(16, id++);
		}
		
		return sum;
	}
	
}
