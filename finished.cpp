#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>
using namespace std;

using State = unsigned int;
using Symbol = char;

struct MISNFA {
	std::set < State > m_States;
	std::set < Symbol > m_Alphabet;
	std::map < std::pair < State, Symbol >, std::set < State > > m_Transitions;
	std::set < State > m_InitialStates;
	std::set < State > m_FinalStates;
};

struct DFA {
	std::set < State > m_States;
	std::set < Symbol > m_Alphabet;
	std::map < std::pair < State, Symbol >, State > m_Transitions;
	State m_InitialState;
	std::set < State > m_FinalStates;

	bool operator== ( const DFA & other ) {
	return
		std::tie ( m_States, m_Alphabet, m_Transitions, m_InitialState, m_FinalStates ) ==
		std::tie ( other.m_States, other.m_Alphabet, other.m_Transitions, other.m_InitialState, other.m_FinalStates );
	}
};
#endif


// TODO: Implement these functions

set<State> nonDeterministicTransition(set<State>& initState, Symbol symbol, map < std::pair < State, Symbol >, std::set < State > > const & m_Transitions)
{
	set<State> result;
	for (State state : initState)
	{
		auto pair = make_pair(state, symbol);
		if (m_Transitions.find(pair) != m_Transitions.end())
		{
			set<State> nextStateSet = m_Transitions.at(pair);
			result.insert(nextStateSet.begin(), nextStateSet.end());
		}
	}
	return result;
}

State setStateToNumber(set<State> stateSet, deque<set<State>>& knownStates, map<set<State>, State>& stateNumbers)
{
	if (stateNumbers.find(stateSet) == stateNumbers.end())
	{
		knownStates.push_back(stateSet);
		stateNumbers.emplace(stateSet, stateNumbers.size());
	}
	return stateNumbers[stateSet];
}

State oldStateToNewState(State oldState, deque<State>& knownStates, map<State, State>& stateNumbers)
{
	if (stateNumbers.find(oldState) == stateNumbers.end())
	{
		knownStates.push_back(oldState);
		stateNumbers.emplace(oldState, stateNumbers.size());
	}
	return stateNumbers[oldState];
}

set<State> firstNIntegers(unsigned int n)
{
	std::set<State> indices;
	for (unsigned int i = 0; i < n; ++i)
	{
		indices.emplace_hint(indices.end(), i);
	}
	return indices;
}

bool isFinal(set<State> stateSet, set < State > const & m_FinalStates)
{
	for (State state : stateSet)
	{
		if (m_FinalStates.find(state) != m_FinalStates.end())
			return true;
	}
	return false;
}

DFA determinize ( const MISNFA & nfa ) {
	deque<set<State>> knownStates;
	map<set<State>, State> stateNumbers;
	setStateToNumber(nfa.m_InitialStates, knownStates, stateNumbers);
	std::map < std::pair < State, Symbol >, State > m_Transitions;
	set < State > m_FinalStates;
	unsigned int i = 0; // Poradove cislo zpracovavaneho stavu / cislo radku vysledne tabulky
	while (knownStates.size() > 0) // Dokud se neudelala tabulka pro kazdy stav
	{
		set<State> currentStateSet = knownStates.front();
		knownStates.pop_front();
		if (isFinal(currentStateSet, nfa.m_FinalStates))
			m_FinalStates.emplace(i);
		for (Symbol symbol : nfa.m_Alphabet)
		{
			set<State> newStateSet = nonDeterministicTransition(currentStateSet, symbol, nfa.m_Transitions);
			State newState = setStateToNumber(newStateSet, knownStates, stateNumbers);
			m_Transitions.emplace(make_pair(i, symbol), newState);
		}
		i++;
	}
	return DFA { 
		firstNIntegers(stateNumbers.size()),
		nfa.m_Alphabet,
		m_Transitions,
		0,
		m_FinalStates
	};
}

set<State> getUsefullStates(const DFA& dfa)
{
	map<State, set<State>> posiblePredecesors;
	for (auto& pair : dfa.m_Transitions)
	{
		State destination = pair.second;
		State from = pair.first.first;
		posiblePredecesors[destination].insert(from);
	}

	deque<State> openedStates;
	set<State> usefullStates = dfa.m_FinalStates;
	for (State finalState : dfa.m_FinalStates)
	{
		openedStates.push_back(finalState);
	}
	while (openedStates.size() > 0)
	{
		State state = openedStates.front();
		openedStates.pop_front();
		for (State predecesor : posiblePredecesors[state])
		{
			if (usefullStates.find(predecesor) == usefullStates.end())
			{
				usefullStates.emplace(predecesor);
				openedStates.push_back(predecesor);
			}
		}
	}
	return usefullStates;
}

DFA trim ( const DFA & dfa ) {
	set<State> usefullStates = getUsefullStates(dfa);

	deque<State> knownStates; // Dosazitelne stavy z puvodniho automatu
	map<State, State> stateNumbers; // Precislovani z puvodniho do noveho automatu
	oldStateToNewState(dfa.m_InitialState, knownStates, stateNumbers);
	std::map < std::pair < State, Symbol >, State > m_Transitions;
	set < State > m_FinalStates;
	unsigned int i = 0; // Poradove cislo zpracovavaneho stavu / cislo radku vysledne tabulky
	while (knownStates.size() > 0) // Dokud se neudelala tabulka pro kazdy stav
	{
		State currentOldState = knownStates.front();
		knownStates.pop_front();
		if (dfa.m_FinalStates.find(currentOldState) != dfa.m_FinalStates.end())
			m_FinalStates.emplace(i);
		for (Symbol symbol : dfa.m_Alphabet)
		{
			State nextOldState = dfa.m_Transitions.at(make_pair(currentOldState, symbol));
			if (usefullStates.find(nextOldState) != usefullStates.end())
			{
				State nextState = oldStateToNewState(nextOldState, knownStates, stateNumbers);
				m_Transitions.emplace(make_pair(i, symbol), nextState);
			}
		}
		i++;
	}
	return DFA{
		firstNIntegers(stateNumbers.size()),
		dfa.m_Alphabet,
		m_Transitions,
		0,
		m_FinalStates
	};
	return dfa;
}

map<Symbol, State> getTableRow(const DFA& dfa, State state)
{
	map<Symbol, State> result;
	for (Symbol symbol : dfa.m_Alphabet)
	{
		State nextState;
		auto pair = make_pair(state, symbol);
		if (dfa.m_Transitions.find(pair) != dfa.m_Transitions.end())
			nextState = dfa.m_Transitions.at(pair);
		else
			nextState = dfa.m_States.size();
		result.emplace(symbol, nextState);
	}
	return result;
}

State maxKey(map<State, State> row)
{
	State max = 0;
	for (auto& pair : row)
	{
		if (pair.first > max)
			max = pair.first;
	}
	return max;
}

map<Symbol, State> changeNumbering(map<Symbol, State> row, map<State, State> stateMap)
{
	map<Symbol, State> result;
	for (auto& pair : row)
	{
		State converted;
		if (stateMap.find(pair.second) != stateMap.end())
			converted = stateMap.at(pair.second);
		else
			converted = maxKey(stateMap) + 1;//stateMap.size(); // TODO zmensit na pocet klicu
		result.emplace(pair.first, converted);
	}
	return result;
}

void writeRow(map<Symbol, State> row)
{
	for (auto& pair : row)
	{
		cout << pair.second << " ";
	}
	cout << endl;
}

set<State> changeNumberingOfSet(set<State> stateSet, map<State, State> stateMap)
{
	set<State> result;
	for (State state : stateSet)
	{
		result.emplace(stateMap.at(state));
	}
	return result;
}

map<pair<State, Symbol>, State> changeTransitionsNumbering(map<pair<State, Symbol>, State> transitions, map<State, State>& stateMap)
{
	map<pair<State, Symbol>, State> result;
	for (auto& pair : transitions)
	{
		State from = stateMap.at(pair.first.first);
		Symbol symbol = pair.first.second;
		result.emplace(make_pair(from, symbol), stateMap.at(pair.second));
	}
	return result;
}

DFA minimize ( const DFA & dfa ) {
	map<State, State> stateMap;
	map<State, State> newStateMap;
	for (State state : dfa.m_States)
	{
		if (dfa.m_FinalStates.find(state) != dfa.m_FinalStates.end())
			newStateMap.emplace(state, 1);
		else
			newStateMap.emplace(state, 0);
	}
	map<pair<State, map<Symbol, State>>, State> newNumbering;
	int prevNumberOfStates;
	int nextNumberOfStates = 0;// 2;
	do
	{
		newNumbering.clear();
		stateMap = newStateMap;
		newStateMap.clear();
		prevNumberOfStates = nextNumberOfStates;
		for (State state : dfa.m_States)
		{
			auto row = getTableRow(dfa, state);
			auto newRow = changeNumbering(row, stateMap);
			//cout << stateMap.at(state) << " <- ";
			//writeRow(newRow);
			auto pair = make_pair(stateMap.at(state), newRow);
			if (newNumbering.find(pair) == newNumbering.end())
			{
				newNumbering.emplace(pair, newNumbering.size());
			}
			newStateMap.emplace(state, newNumbering.at(pair));
		}
		cout << endl;
		nextNumberOfStates = newNumbering.size();
	} while (prevNumberOfStates != nextNumberOfStates);

	return DFA{
		firstNIntegers(newNumbering.size()),
		dfa.m_Alphabet,
		changeTransitionsNumbering(dfa.m_Transitions, newStateMap),
		0,
		changeNumberingOfSet(dfa.m_FinalStates, newStateMap)
	};
}


#ifndef __PROGTEST__

#include "sample.h"

int main ( ) {
	/* IMPORTANT NOTE:
	 *
	 * Do not forget that automata equivalence (i.e., the regular language equivalence) is algorithmically decidable by
	 * checking for the isomorphism of two minimal DFAs.
	 *
	 * Your determinization algorithm *may* give you a result that is different from the test output used in the asserts below.
	 * This *may* not be wrong. If the automaton still accepts the same language, it will be accepted by Progtest. Progtest
	 * will minimize the automaton you returned from determinize() and/or trim() functions and compare it with the reference solution.
	 *
	 * Also note that the naming of the states does not play a role.
	 * The solutions (outD/outT/outM) for the simple "assert" tests are based upon one of our reference solutions.
	 * It is very much possible that your solutions uses a different naming scheme.
	 * Progtest accepts automata that use a different naming scheme.
	 *
	 * If your are unsure about the correct result of any algorithm on any input, you are welcome to use https://alt.fit.cvut.cz/webui/ tool.
	 */

	// determinize
	assert ( determinize ( in0 ) == outD0 );
	assert ( determinize ( in1 ) == outD1 );
	assert ( determinize ( in2 ) == outD2 );
	assert ( determinize ( in3 ) == outD3 );
	assert ( determinize ( in4 ) == outD4 );
	assert ( determinize ( in5 ) == outD5 );
	assert ( determinize ( in6 ) == outD6 );
	assert ( determinize ( in7 ) == outD7 );
	assert ( determinize ( in8 ) == outD8 );
	assert ( determinize ( in9 ) == outD9 );
	assert ( determinize ( in10 ) == outD10 );
	assert ( determinize ( in11 ) == outD11 );
	assert ( determinize ( in12 ) == outD12 );
	assert ( determinize ( in13 ) == outD13 );
	// trim
	assert (trim(determinize(in0)) == outT0 );
	assert ( trim ( determinize ( in1 ) ) == outT1 );
	assert ( trim ( determinize ( in2 ) ) == outT2 );
	assert ( trim ( determinize ( in3 ) ) == outT3 );
	assert ( trim ( determinize ( in4 ) ) == outT4 );
	assert ( trim ( determinize ( in5 ) ) == outT5 );
	assert ( trim ( determinize ( in6 ) ) == outT6 );
	assert ( trim ( determinize ( in7 ) ) == outT7 );
	assert ( trim ( determinize ( in8 ) ) == outT8 );
	assert ( trim ( determinize ( in9 ) ) == outT9 );
	assert ( trim ( determinize ( in10 ) ) == outT10 );
	assert ( trim ( determinize ( in11 ) ) == outT11 );
	assert ( trim ( determinize ( in12 ) ) == outT12 );
	assert ( trim ( determinize ( in13 ) ) == outT13 );

	auto tmp = minimize(trim(determinize(in0)));
	// minimize
	assert ( tmp == outM0 );
	assert ( minimize ( trim ( determinize ( in1 ) ) ) == outM1 );
	assert ( minimize ( trim ( determinize ( in2 ) ) ) == outM2 );
	assert ( minimize ( trim ( determinize ( in3 ) ) ) == outM3 );
	assert ( minimize ( trim ( determinize ( in4 ) ) ) == outM4 );
	assert ( minimize ( trim ( determinize ( in5 ) ) ) == outM5 );
	assert ( minimize ( trim ( determinize ( in6 ) ) ) == outM6 );
	assert ( minimize ( trim ( determinize ( in7 ) ) ) == outM7 );
	assert ( minimize ( trim ( determinize ( in8 ) ) ) == outM8 );
	auto tmp2 = minimize(trim(determinize(in9)));
	assert ( tmp2 == outM9 );
	assert ( minimize ( trim ( determinize ( in10 ) ) ) == outM10 );
	assert ( minimize ( trim ( determinize ( in11 ) ) ) == outM11 );
	assert ( minimize ( trim ( determinize ( in12 ) ) ) == outM12 );
	assert ( minimize ( trim ( determinize ( in13 ) ) ) == outM13 );

	return 0;
}
#endif
