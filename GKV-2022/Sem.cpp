#include "Sem.h"

bool Sem::SemAnaliz(LT::LexTable lextable, IT::IdTable idtable, Log::LOG log)
{

	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		switch (lextable.table[i].lexema)
		{
		case LEX_EQUAL: 
		{
			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) 
			{
				IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				for (int k = i + 1; lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (k == lextable.size)
						break; 
					if (lextable.table[k].idxTI != LT_TI_NULLIDX) 
					{
						if (!ignore)
						{
							if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL) {
								IT::IDDATATYPE righttype = idtable.table[lextable.table[k].idxTI].iddatatype;
								if (righttype == IT::IDDATATYPE::INT16) righttype = IT::IDDATATYPE::INT;
								if (lefttype != righttype) 
								{
									Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
									sem_ok = false;
									break;
								}
							}
						}
						if (lextable.table[k + 1].lexema == LEX_LEFTTHESIS)
						{
							ignore = true;
							continue;
						}
						if (ignore && lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
						{
							ignore = false;
							continue;
						}
					}
					if (lefttype == IT::IDDATATYPE::STR) 
					{
						char l = lextable.table[k].lexema;
						if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR) 
						{
							Log::WriteErrors(log, Error::geterrorin(316, lextable.table[k].sn, 0));
							sem_ok = false;
							break;
						}
					}
				}
			}
			break;
		}
		case LEX_ID: 
		{
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			if (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION)
			{
				if (e.idtype == IT::IDTYPE::F && e.iddatatype != IT::IDDATATYPE::VOI) 
				{
					for (int k = i + 1; ; k++)
					{
						char l = lextable.table[k].lexema;
						if (l == LEX_RETURN)
						{
							if (lextable.table[k + 1].lexema == LEX_LITERAL || lextable.table[k + 1].lexema == LEX_ID) {
								int next = lextable.table[k + 1].idxTI; 
								if (next != LT_TI_NULLIDX)
								{
									if (idtable.table[next].iddatatype != e.iddatatype)
									{
										Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
										sem_ok = false;
										break;
									}
								}
								break; 
							}
							else {
								Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
								throw Error::geterrorin(315, lextable.table[k].sn, 0);
							}
						}
						if (k == lextable.size) break;
					}
				
				}
				else if (e.idtype == IT::IDTYPE::F && e.iddatatype == IT::IDDATATYPE::VOI) {
					for (int k = i + 1; ; k++)
					{
						char l = lextable.table[k].lexema;
						if (l == LEX_RETURN)
						{
							int next = lextable.table[k + 1].lexema; 
								if (next != LEX_SEMICOLON)
								{
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
									break;
								}
							break; 
						}
						if (k == lextable.size) break;
					}
				}
			}
			if (lextable.table[i + 1].lexema == LEX_LEFTTHESIS && lextable.table[i - 1].lexema != LEX_FUNCTION) 
			{
				if (e.idtype == IT::IDTYPE::F || e.iddatatype != IT::IDDATATYPE::VOI) 
				{
					int paramscount = NULL;
					for (int j = i + 1; lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
						{
							paramscount++;
							if (e.parm == NULL)
								break;
							IT::IDDATATYPE ctype = idtable.table[lextable.table[j].idxTI].iddatatype;
							if (ctype != idtable.table[lextable.table[i].idxTI + paramscount].iddatatype)
							{
								Log::WriteErrors(log, Error::geterrorin(309, lextable.table[i].sn, 0));
								sem_ok = false;
								break;
							}
						}
						if (j == lextable.size)
							break;
					}
					if (paramscount != e.parm)
					{
						Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
						sem_ok = false;
					}
					if (paramscount > 3)
					{
						Log::WriteErrors(log, Error::geterrorin(307, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
			break;
		}
		case LEX_LOGOPERATOR:
		{
			bool flag = true;
			if (i > 1 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
					flag = false;
			}
			if (lextable.table[i + 1].idxTI != LT_TI_NULLIDX)
			{
				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
					flag = false;
			}
			if (!flag)
			{
				Log::WriteErrors(log, Error::geterrorin(317, lextable.table[i].sn, 0));
				sem_ok = false;
			}
			break;
		}
		}
	}
	return sem_ok;
}
