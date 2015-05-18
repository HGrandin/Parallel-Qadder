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
		int q_width;
		int b_width;
		int height;
		int *sums;
		propgen_result *inter_results;

		initial_propgen *init_propgen;
		propgen *propgen_node;
		
		//The initial addition/propagation/generate step (the rectangles in reference-pic)
		void initial_step(int input1, int input2, int start_index, int iterations,int base);
		
		//The last step (triangles in reference-pic)
		int summarizing_step(int start_index, int iterations,int base);

	public:
		// binary_size is the number of binary adder nodes 
		// quaternary size is the number of adders that is using base 4
		kogge_stone(int binary_size, int quaternary_size);
		
		int add(int input1, int input2);
};

#endif /* end of include guard: KOGGE_STONE_HPP_38432 */