#include "Parm.h"
#include "Error.h"
#include "In.h"
#include <locale>	
#include <cwchar>
#include <iostream>
#include <fstream>
using namespace std;
namespace In
{
	unsigned char** word;
	IN getin(wchar_t infile[])
	{
		IN in;
		in.size = 0; in.lines = 0; in.ignor = 0;
		int col = 0;
		word = new unsigned char*[1000];
		for (int i = 0; i < 1000; i++)
			word[i] = new unsigned char[1000] {NULL};
		int st = 0;
		int cl = 0;
		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		unsigned char* textout = new unsigned char[IN_MAX_LEN_TEXT];
		ifstream fin(infile);
		bool fkov = false;
		if (fin.fail()) throw ERROR_THROW(110);
		if (sizeof(fin)>IN_MAX_LEN_TEXT) throw ERROR_THROW(113);
		while (in.size < IN_MAX_LEN_TEXT)
		{
			
			char c; fin.get(c);
			unsigned char x = c;
			if (fin.eof())
			{
				text[in.size] = '\0';
				in.lines++;
				break;
			}
			if (x == IN_CODE_ENDL)
			{
				in.lines++;
				col = 0;
			}
			if (in.code[x] == in.T)
			{
				text[in.size] = x;
				word[st][cl++] = x;
				in.size++;
				col++;
			}
			else if (in.code[x] == in.I)
			{
				in.ignor++;
			}
			else if (in.code[x] == in.F)
			{
				throw ERROR_THROW_IN(111, in.lines, col);
			}
			else if (in.code[x] == in.P) {
				fkov = not(fkov);
				text[in.size] = x;
				in.size++;
				if (fkov == true){
				if (word[st][0] != NULL) {
					st++;
					cl = 0;
				}
					word[st][cl++] = x;
				}
				else {
					word[st][cl++] = x;
					st++;
					cl = 0;
				}
			}
			else if (in.code[x] == in.S) {
				if (fkov) {
					text[in.size] = x;
					word[st][cl++] = x;
					in.size++;
					col++;
					continue;
				}else
				if (((in.size == 0) || in.code[text[in.size - 1]] == in.S) &&  (x == ' '))
				{
					continue;
				}else
				if (text[in.size - 1] == ' ' && x == ' ') {
					in.size = in.size - 1;	
				}
				if (x != ' ') {
					if (word[st][0] != NULL) {
						st++;
						cl = 0;
					}
					word[st][cl++] = x;
					st++;
					cl = 0;
				}
				else {
					if (word[st][0] != NULL) {
						st++;
						cl = 0;
					}
				}
				text[in.size] = x;
				in.size++;
				col++;
			}
			else
			{
				if (x == '\n') fkov = false;
			if (word[st][0] != NULL) {
				st++;
				cl = 0;
			}
				word[st][cl++] = in.code[x];
				st++;
				cl = 0;
				text[in.size] = in.code[x];
				in.size++;
				col++;
			}
			
		}

		text[in.size] = '\0';
		in.text = text;
		in.word = word;
		return in;
	}
}