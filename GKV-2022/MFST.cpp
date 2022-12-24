﻿#include "stdafx.h"
#include "MFST.h"

namespace MFST {
#pragma region CONSTRUCTORS
	MfstState::MfstState() {
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) {
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	}

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) {
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis() {
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) {
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	}

	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; }
	Mfst::Mfst(LT::LexTable& plex, GRB::Greibach pgreibach) {
		greibach = pgreibach;
		lex = plex;
		lenta = new short[lenta_size = lex.size];
		for (int k = 0; k < lenta_size; k++)
			lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema);
		lenta_position = 0;
		st.push(greibach.stbottomT);
		st.push(greibach.startN);
		nrulechain = -1;
	}

#pragma endregion
	Mfst::RC_STEP Mfst::step() {
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size) {
			if (GRB::Rule::Chain::isN(st.top())) {
				GRB::Rule rule;
				if ((nrule = greibach.getRule(st.top(), rule)) >= 0) {
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) {
						MFST_TRACE1
							if(more) 
								MFST_TRACE1_M

							savestate(); st.pop(); push_chain(chain); rc = NS_OK;
						MFST_TRACE2
							if (more)
								MFST_TRACE2_M

					}
					else {
						MFST_TRACE4("NS_NRCHAIN/NS_NR")
							if (more)
								MFST_TRACE4_M("NS_NRCHAIN/NS_NR")

							saveddiagnosis(NS_NORULECHAIN); rc = resetstate() ? NS_NORULECHAIN : NS_NORULE;
					}
				}
				else rc = NS_ERROR;
			}
			else if (st.top() == lenta[lenta_position]) {
				lenta_position++; st.pop(); nrulechain = -1; rc = TS_OK;
				MFST_TRACE3
					if (more)
						MFST_TRACE3_M

			}
			else {
				MFST_TRACE4(TS_NOK / NS_NORULECHAIN) rc = resetstate() ? TS_NOK : NS_NORULECHAIN;
				if (more) MFST_TRACE4_M(TS_NOK / NS_NORULECHAIN)

			}
		}
		else {
			rc = LENTA_END;
			MFST_TRACE4(LENTA_END);
			if (more)
				MFST_TRACE4_M(LENTA_END);
		}

		return rc;
	}

	bool Mfst::push_chain(GRB::Rule::Chain chain) {
		for (int k = chain.size - 1; k >= 0; k--)
			st.push(chain.nt[k]);
		return true;
	}

	bool Mfst::savestate() {
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6("SAVESTATE:", storestate.size());
		if (more)
			MFST_TRACE6_M("SAVESTATE:", storestate.size());

		return true;
	}

	bool Mfst::resetstate() {
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0)) {
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2
				if (more) {
					MFST_TRACE5_M("RESSTATE")
						MFST_TRACE2_M
				}

		}

		return rc;
	}

	bool Mfst::saveddiagnosis(RC_STEP prc_step) {
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER)) {
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lenta_position = -1;
		}

		return rc;
	}

	bool Mfst::start() {
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE]{};
		MFST_TRACE_START_0
		MFST_TRACE_START
			if (more) {
				MFST_TRACE_START_0_M
					MFST_TRACE_START_M
			}
		rc_step = step();
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step();

		switch (rc_step) {
		case LENTA_END:
			MFST_TRACE4("------>LENTA_END")
				if (more) {
					MFST_TRACE4_M("------>LENTA_END")
						std::cout << "------------------------------------------------------------------------------------------" << std::endl;
				}
					*log.stream << "------------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: Всего строк %d, синтаксический анализ выполнен без ошибок", 0, lex.table[lex.size - 1].sn);
			std::cout << "\tСинтаксический анализ обработал строк " << lex.table[lex.size - 1].sn << "," << std::endl;
			rc = true;
			break;
		case NS_NORULE:
			MFST_TRACE4("------>NS_NORULE")
				if (more) {
					MFST_TRACE4_M("------>NS_NORULE")
						std::cout << "------------------------------------------------------------------------------------------" << std::endl;
				}

				*log.stream << "------------------------------------------------------------------------------------------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN:
			MFST_TRACE4("------>NS_NORULECHAIN")
				if (more)
					MFST_TRACE4_M("------>NS_NORULECHAIN")
				break;
		case NS_ERROR:
			MFST_TRACE4("------>NS_ERROR") 
				if (more)
					MFST_TRACE4_M("------>NS_ERROR")
				break;
		case SURPRISE:
			MFST_TRACE4("------>NS_SURPRISE") 
				if (more)
					MFST_TRACE4_M("------>NS_SURPRISE")

				break;
		}

		return rc;
	}

	char* Mfst::getCSt(char* buf) {
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k) {
			p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = '\0';
		return buf;
	}

	char* Mfst::getCLenta(char* buf, short pos, short n) {
		short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

		for (int i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

		buf[i - pos] = '\0';
		return buf;
	}

	char* Mfst::getDiagnosis(short n, char* buf) {
		char* rc = new char[200]{};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0) {
			errid = greibach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		}

		return rc;
	}

	void Mfst::printrules() {
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++) {
			state = storestate.c[i];
			rule = greibach.getRule(state.nrule);
			MFST_TRACE7
				if (more)
					MFST_TRACE7_M

		}
	}

	bool Mfst::savededucation() {
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++) {
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}

		return true;
	}
}