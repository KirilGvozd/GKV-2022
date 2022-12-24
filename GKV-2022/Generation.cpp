#include "Generation.h"

#include <sstream>
#include <cstring>
#include <iosfwd>
#include <xstring>
using namespace std;
#define CODE_RDIV {codeAsm << "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx,\t0\n\tidiv ebx\n\tpush edx";}

namespace Gener
{
	static int conditionnum = 0;

	string itoS(int x) { stringstream r;  r << x;  return r.str(); }
	string genCallFuncCode(Lex::LEX& tables, Log::LOG& log, int i)
	{
		string str;

		IT::Entry e = ITENTRY(i); 
		stack <IT::Entry> temp;
		bool stnd = (e.iddatatype == IT::IDDATATYPE::VOI);

		for (int j = i + 1; LEXEMA(j) != LEX_RIGHTTHESIS; j++)
		{
			if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
				temp.push(ITENTRY(j)); 	
		}
		str += "\n";

		// раскручиваем стек
		while (!temp.empty())
		{
			if (temp.top().idtype == IT::IDTYPE::L && temp.top().iddatatype == IT::IDDATATYPE::STR)
				str = str + "push offset " + reinterpret_cast<char*>(temp.top().id) + "\n";
			else   str = str + "push " + reinterpret_cast<char*>(temp.top().id) + "\n";
			temp.pop();
		}

		str = str + "call " + reinterpret_cast<char*>((e.id)) + IN_CODE_ENDL;
		return str;
	}
	string genEqualCode(Lex::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e1 = ITENTRY(i - 1); 

		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::INT:
		{
			bool first = true;
			for (int j = i + 1; LEXEMA(j) != LEX_SEMICOLON; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (ITENTRY(j).idtype == IT::IDTYPE::F) 
					{
						str = str + genCallFuncCode(tables, log, j); 
						str = str + "push eax\n";				
						while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
						break;
					}
					else  str = str + "push " + reinterpret_cast<char*>(ITENTRY(j).id) + "\n";
					break;
				}
				case LEX_OPERATOR: {
					switch (ITENTRY(j).id[0])
					{
					case '+':
						str = str + "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
					case '-':
						str = str + "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
					case '*':
						str = str + "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
					case '/':
						str = str + "\n\tpop ebx\n\tpop eax\n\ttest ebx, ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx, \t0\n\tidiv ebx\n\tpush eax"; break;
					case '%':
						str = str + "\n\tpop ebx\n\tpop eax\n\ttest ebx, ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx, \t0\n\tidiv ebx\n\tpush edx"; break;
					default:
						break;
					}


				}

				}
			} 

			str = str + "\npop ebx\nmov " + reinterpret_cast<char*>(e1.id) + ", ebx\n";		
			break;
		}
		case IT::IDDATATYPE::STR:
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F)) 
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + reinterpret_cast<char*>(e1.id) + ", eax";
			}
			else if (lex == LEX_LITERAL) 
			{
				str = str + "mov " + reinterpret_cast<char*>(e1.id) + ", offset " + reinterpret_cast<char*>(e2.id);
			}
			else 
			{
				str = str + "mov ecx, " + reinterpret_cast<char*>(e2.id) + "\nmov " + reinterpret_cast<char*>(e1.id) + ", ecx";
			}
		}
		}

		return str;
	}
	string genIFCode(Lex::LEX& tables, Log::LOG& log, int i, string napr)
	{
		string str;
		bool first = true;
		for (int j = i + 1; LEXEMA(j) != LEX_LOGOPERATOR && LEXEMA(j) != LEX_ENDIF && LEXEMA(j) != LEX_AND; j++)
		{
			switch (LEXEMA(j))
			{
			case LEX_LITERAL:
			case LEX_ID:
			{
				if (ITENTRY(j).idtype == IT::IDTYPE::F) 
				{
					str = str + genCallFuncCode(tables, log, j); 
					str = str + "push eax\n";				
					while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
					break;
				}
				else  str = str + "push " + reinterpret_cast<char*>(ITENTRY(j).id) + "\n";
				break;
			}
			case LEX_OPERATOR: {
				switch (ITENTRY(j).id[0])
				{
				case '+':
					str = str + "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
				case '-':
					str = str + "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
				case '*':
					str = str + "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
				case '/':
					str = str + "\n\tpop ebx\n\tpop eax\n\ttest ebx, ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx, \t0\n\tidiv ebx\n\tpush eax"; break;
				case '%':
					str = str + "\n\tpop ebx\n\tpop eax\n\ttest ebx, ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tmov\tedx, \t0\n\tidiv ebx\n\tpush edx"; break;
				default:
					break;
				}


			}

			}
		} 
		str = str + "\npop ebx\nmov " + napr + ", ebx\n";			


		return str;
	}
	string genFunctionCode(Lex::LEX& tables, int i, string funcname, int pcount)
	{
		string str;
		IT::Entry e = ITENTRY(i + 1);
		IT::IDDATATYPE type = e.iddatatype;

		str = SEPSTR(funcname) + reinterpret_cast<char*>(e.id) + string(" PROC,\n\t");
		int j = i + 3; 
		while (LEXEMA(j) != LEX_RIGHTTHESIS) 
		{
			if (LEXEMA(j) == LEX_ID) 
				str = str + reinterpret_cast<char*>(ITENTRY(j).id) + (ITENTRY(j).iddatatype == IT::IDDATATYPE::INT || ITENTRY(j).iddatatype == IT::IDDATATYPE::INT16 ? " : sdword, " : " : dword, ");
			j++;
		}
		int f = str.rfind(',');
		if (f > 0)
			str[f] = ' ';

		str += "\n; --- save registers ---\npush ebx\npush edx\n; ----------------------";

		return str;
	}
	string genExitCode(Lex::LEX& tables, int i, string funcname, int pcount)
	{
		string str = "; --- restore registers ---\npop edx\npop ebx\n; -------------------------\n";
		if (LEXEMA(i + 1) != LEX_SEMICOLON)	
		{
			str = str + "mov eax, " + reinterpret_cast<char*>(ITENTRY(i + 1).id) + "\n";
		}
		str += "ret\n";
		str += funcname + " ENDP" + SEPSTREMP;
		return str;
	}
	string genConditionCode(Lex::LEX& tables, int i, string& cyclecode)
	{
		string str;
		string buf;
		conditionnum++;
		cyclecode.clear();
		bool w = false, r = false, c = false;
		string wstr, rstr;
		int u = 0;
		for (int j = i; LEXEMA(j) != ':'; j--) { u = j; };
		if (LEXEMA(u - 2) == LEX_FOR) c = true;
		str = str + "mov edx, " + "left" + "\ncmp edx, " + "rig" + "\n";
		switch (ITENTRY(i).id[0])
		{
		case '>':  rstr = "jg";  wstr = "jl";  break;
		case '<':   rstr = "jl";  wstr = "jg";  break;
		case '~':    rstr = "jz";  wstr = "jnz";  break;
		case '!':   rstr = "jnz";  wstr = "jz";  break;
		}

		if (!c) str = str + "\n" + rstr + " right" + itoS(conditionnum);
		if (!c) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);
		if (!c) str = str + "\n" " right" + itoS(conditionnum)+ ":";
		if (c)
		{
			str = str + "\n" + rstr + " cycle" + itoS(conditionnum);
			cyclecode = str;
			str = str + "\njmp cyclenext" + itoS(conditionnum);
			str = str + "\ncycle" + itoS(conditionnum)+ ":";
		}
		return str;
	}
	vector <string> startFillVector(Lex::LEX& tables)
	{
		vector <string> v;
		v.push_back(BEGIN);
		v.push_back(EXTERN);

		vector <string> vlt;  vlt.push_back(CONST);
		vector <string> vid;  vid.push_back(DATA);

		for (int i = 0; i < tables.idtable.size; i++)
		{
			IT::Entry e = tables.idtable.table[i];
			string str = reinterpret_cast<char*>(e.id);

			if (tables.idtable.table[i].idtype == IT::IDTYPE::L)	
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:  str = str + " sdword " + itoS(e.value.vint);  break;
				case IT::IDDATATYPE::STR:  str = str + " byte '" + reinterpret_cast<char*>(e.value.vstr.str) + "', 0";  break;
				}
				vlt.push_back(str);
			}
			else if (tables.idtable.table[i].idtype == IT::IDTYPE::V)
			{
				switch (e.iddatatype)
				{
					case IT::IDDATATYPE::INT: {

						str = str + " sdword 0";  break; }
				case IT::IDDATATYPE::STR: str = str + " dword ?";  break;
				}
				vid.push_back(str);
			}
		}

		string str = "left dword ?\nrig dword ?\nresult sdword ?\nresult_str byte 4 dup(0)";
		vid.push_back(str);
		v.insert(v.end(), vlt.begin(), vlt.end());
		v.insert(v.end(), vid.begin(), vid.end());
		v.push_back(CODE);
		return v;
	}
	void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log)
	{
		vector <string> v = startFillVector(tables);
		ofstream ofile;
		ofile.open("..\\ASM\\Programm.asm");
		if (!ofile.is_open())throw ERROR_THROW(114);
		string funcname;	
		string cyclecode;	
		int pcount;			
		string str;
		string buf;
		bool r = false, l = false, w = false;
		bool commbr = false;
		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_MAIN:
			{
				str = str + SEPSTR("MAIN") + "main PROC";
				break;
			}
			case LEX_FUNCTION:
			{
				funcname = reinterpret_cast<char*>(ITENTRY(i + 1).id);
				pcount = ITENTRY(i + 1).parm;
				str = genFunctionCode(tables, i, funcname, pcount);
				break;
			}
			case LEX_RETURN:
			{
				if(funcname != "main")
				str = genExitCode(tables, i, funcname, pcount);
				funcname = "main";
				buf.clear();
				break;
			}
			case LEX_ID: 
			{
				if (LEXEMA(i + 1) == LEX_LEFTTHESIS && LEXEMA(i - 1) != LEX_FUNCTION) 
					str = genCallFuncCode(tables, log, i);
				break;
			}
			case ':': 
			{
				str = genIFCode(tables, log, i, "left");
				if (!str.empty())
					v.push_back(str);
				buf = buf + str;
				str.clear();
				while (LEXEMA(++i) != LEX_LOGOPERATOR);
				int lop = i;
				str = genIFCode(tables, log, i, "rig");
				if (!str.empty())
					v.push_back(str);
				buf = buf + str;
				str.clear();
				str = genConditionCode(tables, lop, cyclecode);
				while (LEXEMA(++i) != LEX_LOGOPERATOR && LEXEMA(i-1) != LEX_ENDIF);

				break;
			}

			case '&': 
			{
				str = genIFCode(tables, log, i, "left");
				if (!str.empty())
					v.push_back(str);
				str.clear();
				while (LEXEMA(++i) != LEX_LOGOPERATOR);
				int lop = i;
				str = genIFCode(tables, log, i, "ri");
				if (!str.empty())
					v.push_back(str);
				str.clear();
				str = genConditionCode(tables, lop, cyclecode);
				while (LEXEMA(++i) != LEX_LOGOPERATOR || LEXEMA(++i) != LEX_ENDIF);

				break;
			}
			case LEX_BRACELET:	
			{
				if (l) {
					str += "next" + itoS(conditionnum) + ":";
					l = false;
				}
				if (w) {
					

					str = buf + cyclecode + "\ncyclenext" + itoS(conditionnum) + ":";
					w = false;
					r = true;
				}
				break;
			case LEX_ENDIF: 
			{
				if (!r) {
					str = str + "right" + itoS(conditionnum) + ":";
					r = false;
				}
				break;
			}
			case LEX_ROOF: 
			{
				str = str + "\njmp next" + itoS(conditionnum) + "\n";

				str = str + "wrong" + itoS(conditionnum) + ":";
				l = true;
				break;
			}
			case LEX_FOR: 
			{
				str = str + "cycle" + itoS(conditionnum) + ":";
				w = true;
				r = true;
				break;
			}
			case LEX_EQUAL: 
			{
				str = genEqualCode(tables, log, i);
				while (LEXEMA(++i) != LEX_SEMICOLON);	
				break;
			}
			case LEX_RUNOUT: 
			{
				IT::Entry e = ITENTRY(i + 1);
				if(e.idtype != IT::IDTYPE::F)
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT: {
					str = str + "mov eax, " + reinterpret_cast<char*>(e.id) + "\nmov result, eax	\nINVOKE int_to_char, offset result_str, result\n";
					str = str + "INVOKE outw, offset result_str\n";
					i++;
					break;
				}
				case IT::IDDATATYPE::STR:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\nINVOKE outw, offset " + reinterpret_cast<char*>(e.id) + "\n";
					else  str = str + "\nINVOKE outw, " + reinterpret_cast<char*>(e.id) + "\n";
					i++;
					break;
				}
				else {
					if (LEXEMA(i + 2) == LEX_LEFTTHESIS && LEXEMA(i) != LEX_FUNCTION) 
						str = genCallFuncCode(tables, log, i+1);
						str = str + "push eax\n";
					switch (e.iddatatype)
					{
					case IT::IDDATATYPE::INT: {
						str = str + "\nmov result, eax	\nINVOKE int_to_char, offset result_str, result\n";
						str = str + "\nmov result, eax\n";
						str = str + "INVOKE outn, result\n";
						break;
					}
					case IT::IDDATATYPE::STR:
						str = str + "\npush offset " + reinterpret_cast<char*>(e.id);
						str = str + "\nINVOKE outw, " + reinterpret_cast<char*>(e.id) + "\n";
						break;
					}
					i++;
					break;
				}
			}

			}

			}

			if (!str.empty())
				v.push_back(str);
			str.clear();
		}
		v.push_back(END);
		for (auto x : v)
			ofile << x << endl;
		ofile.close();
	};
}