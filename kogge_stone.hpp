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
		error_rates ber,qer;
		int width;
		int q_width;
		int b_width;
		int height;
		int *sums;
		propgen_result *inter_results;
		initial_propgen *init_propgen;
		propgen *propgen_node;
		
		//statistics
		long long int *quart_error_array;
		long long int *bit_error_array; //still looks at all 32 bits even if some might be quaternary
		long long int *error_size_array;

		//looks at the error size, each active bit of the error zise increases by one
		// if error size is for, bit3 will be increased by one.
		long long int *bit_error_size_array;
		int quaternary_size;
		long long int quaternary_errors;
		long long int binary_errors;
		long long int incorrect_adds;
		long long int nr_of_adds;

		//The initial addition/propagation/generate step (the rectangles in reference-pic)
		void initial_step(unsigned int input1, unsigned int input2, int start_index, int iterations,int base);
		
		//The last step (triangles in reference-pic)
		int summarizing_step(int start_index, int iterations,int base);

		//compare results of the kogge stone with actual addition, store statistics.
		void compare_results(unsigned int input1,unsigned int input2);

	public:
		// binary_size is the number of binary adder nodes 
		// quaternary size is the number of adders that is using base 4
		// The span of quaternary+binary bits may not be bigger than 2^32
		kogge_stone(int binary_size, int quaternary_size, error_rates b_errors, error_rates q_errors);
		
		unsigned int add(unsigned int input1, unsigned int input2);

		unsigned int ones_complement(unsigned int input);

		//prints statistics of the adder,
		//depending on mode, stats will be written to either terminal, file or both
		// if mode == 0 only terminal mode, 1 only file, 2 both
		void print_stats(std::string file, int input_size, int mode);

		//makes a json file with the name as the input string file
		//The contents of the file is the probability that each bit
		//in a error variable is 1
		void make_json_file(std::string file);
};

#endif /* end of include guard: KOGGE_STONE_HPP_38432 */