#ifndef INITIAL_PROPGEN_HPP_67378
#define INITIAL_PROPGEN_HPP_67378

#include <random>

struct initial_propgen_result{
	int sum;
	int propagate;
	int generate;
};

// a struct that determines probability of errors
// probability of error is 1 in <variable>
// 0 value means no chance of error
// propagate, generate and sum error rates is for when things go wrong in the specific circuits.
// variable_error is for noise errors that happens outside of circuits.
// stable_multiplier allows us to set the 0 and 3 (extreme values) in quaternary to be more stable.

// propagate and generate does not add errors in the propagate tree as that is "true" binary
// it is only for the initial propagate/generate from our quaternary/binary(ish) circuit
struct error_rates{
	int propagate_error;
	int generate_error;
	int sum_error;
	int input_error;
	int stable_multiplier;
};

class initial_propgen{
	private:
		error_rates ber,qer;


		std::default_random_engine generator;

		int q_add_variable_noise(int input, int error_prob);
		int b_add_variable_noise(int input, int error_prob);
		int add_propgen_noise(int input, int error_prob);

		int disabled = 0;


	public:
		initial_propgen(error_rates b_errors, error_rates q_errors);

		//base tells us if its binary or quaternary.
		initial_propgen_result compute(int input1, int input2, int base);
};

#endif /* end of include guard: INITIAL_PROPGEN_HPP_67378 */