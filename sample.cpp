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

DFA determinize ( const MISNFA & nfa ) { return DFA { }; }

DFA trim ( const DFA & dfa ) { return dfa; }

DFA minimize ( const DFA & dfa ) { return dfa; }


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
	assert ( trim ( determinize ( in0 ) ) == outT0 );
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


	// minimize
	assert ( minimize ( trim ( determinize ( in0 ) ) ) == outM0 );
	assert ( minimize ( trim ( determinize ( in1 ) ) ) == outM1 );
	assert ( minimize ( trim ( determinize ( in2 ) ) ) == outM2 );
	assert ( minimize ( trim ( determinize ( in3 ) ) ) == outM3 );
	assert ( minimize ( trim ( determinize ( in4 ) ) ) == outM4 );
	assert ( minimize ( trim ( determinize ( in5 ) ) ) == outM5 );
	assert ( minimize ( trim ( determinize ( in6 ) ) ) == outM6 );
	assert ( minimize ( trim ( determinize ( in7 ) ) ) == outM7 );
	assert ( minimize ( trim ( determinize ( in8 ) ) ) == outM8 );
	assert ( minimize ( trim ( determinize ( in9 ) ) ) == outM9 );
	assert ( minimize ( trim ( determinize ( in10 ) ) ) == outM10 );
	assert ( minimize ( trim ( determinize ( in11 ) ) ) == outM11 );
	assert ( minimize ( trim ( determinize ( in12 ) ) ) == outM12 );
	assert ( minimize ( trim ( determinize ( in13 ) ) ) == outM13 );

	return 0;
}
#endif
