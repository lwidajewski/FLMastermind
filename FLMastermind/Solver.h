#ifndef SOLVER_H
#define SOLVER_H

#include "Tree.h"
#include "Sequence.h"

class Solver {
	private:
		Sequence guess;
		Sequence solveSeq;
	public:
		void solvePlay();
};

#endif