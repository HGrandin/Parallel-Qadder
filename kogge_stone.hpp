#ifndef KOGGE_STONE_HPP_38432
#define KOGGE_STONE_HPP_38432

#include "initial_propgen.hpp"
#include "propgen.hpp"

// struct propgen_result{
// 	int propagate;
// 	int generate;
// };

class kogge_stone{
	private:
		int width;
		int height;
		propgen_result *inter_results;
		initial_propgen *init_propgen;
		propgen *propgen_node;
	

	public:
		// size is the number of half adder nodes the kogge stone should have
		// size must be of size 2^x where x is an int
		kogge_stone(int size);
		
		int add(int input1, int input2);
};

#endif /* end of include guard: KOGGE_STONE_HPP_38432 */