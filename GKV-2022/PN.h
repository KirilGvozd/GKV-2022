#pragma once
#include "stdafx.h"
#include <vector>

typedef std::vector <LT::Entry> ltvec;
typedef std::vector <int> intvec;

namespace Polish
{
	bool PolishNotation(Lex::LEX& lex, Log::LOG& log);
	// � ������ ��������� ������� ������ ���� ��������� � �������� ������
	intvec getExprPositions(Lex::LEX& lex);
	// ��������� ������ ��������� � ��������� �������
	int  fillVector(int lextable_pos, LT::LexTable& lextable, ltvec& v);
	// ������ ������� �������� ������� ��������������� � �������� �������
	bool setPolishNotation(IT::IdTable& idtable, Log::LOG& log, int lextable_pos, ltvec& v);
	// ��������� ������ � ����� ������� ������
	void addToTable(LT::LexTable& new_table, IT::IdTable& idtable, ltvec& v);
};