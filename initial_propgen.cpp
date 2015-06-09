#include "initial_propgen.hpp"
#include "assert.h"
#include <stdio.h>

initial_propgen::initial_propgen(error_rates b_errors, error_rates q_errors){
	ber = b_errors;
	qer = q_errors;
	generator.seed(time(0));
}

initial_propgen_result initial_propgen::compute(int input1, int input2, int base){
	initial_propgen_result result;
	result.sum = 0;
	result.propagate = 0;
	result.generate = 0;
	if(base == 2){
		//input errors
		if(ber.input_error != disabled){
			input1 = b_add_variable_noise(input1, ber.input_error);
			input2 = b_add_variable_noise(input2, ber.input_error);
		}
		
		//Results
		result.sum = (input1 + input2) % 4;

		if((input1 + input2) == 2){
			result.propagate = 1;
		}
		if((input1 + input2) > 3){
			result.generate = 1;
		}

		//circuit errors
		if(ber.sum_error != disabled){
			result.sum = b_add_variable_noise(result.sum, ber.sum_error);
		}
		if(ber.propagate_error != disabled){
			result.propagate = add_propgen_noise(result.propagate, ber.propagate_error);
		}
		if(ber.generate_error != disabled){
			result.generate = add_propgen_noise(result.generate, ber.generate_error);
		}
	}
	else if(base == 4){
		//input errors
		if(qer.input_error != disabled){
			input1 = q_add_variable_noise(input1, qer.input_error);
			input2 = q_add_variable_noise(input2, qer.input_error);
		}

		//Results
		result.sum = (input1 + input2) % 4;
		if((input1 + input2) == 3){
			result.propagate = 1;
		}
		if((input1 + input2) > 3){
			result.generate = 1;
		}

		//circuit errors
		if(qer.sum_error != disabled){
			result.sum = q_add_variable_noise(result.sum, qer.sum_error);
		}
		if(qer.propagate_error != disabled){
			result.propagate = add_propgen_noise(result.propagate, qer.propagate_error);
		}
		if(qer.generate_error != disabled){
			result.generate = add_propgen_noise(result.generate, qer.generate_error);
		}

	}
	else{
		printf("Error: initial_propgen::compute: invalid value on base\n");
		assert(false);
	}

	return result;
}

int initial_propgen::q_add_variable_noise(int input, int error_prob){
	std::uniform_int_distribution<int> error_distribution(1, error_prob);
	std::uniform_int_distribution<int> stable_distribution(1, qer.stable_multiplier);
	std::uniform_int_distribution<int> fifty_fifty(1, 2);

	if(error_distribution(generator) == 1){
		switch(input){
			case 0 : 
				if(stable_distribution(generator) == 1){
					input++;
				}
				break;
			case 3 : 
				if(stable_distribution(generator) == 1){
					input--;
				}
				break;
			default :
				if(input != 1 and input != 2){
					printf("Error: In initial_propgen::q_add_variable_noise, case that should never happen.\n");
					assert(false);
				}
				if(fifty_fifty(generator) == 1){
					input++;
				}
				else{
					input--;
				}
				break;
		}
	}
	return input;
}

int initial_propgen::b_add_variable_noise(int input, int error_prob){
	std::uniform_int_distribution<int> error_distribution(1, error_prob);
	if(error_distribution(generator) == 1){
		if(input == 0){
			input = 2;
		}
		else if(input == 2){
			input = 0;
		}
		else{
			printf("Error: In initial_propgen::b_add_variable_noise, case that should never happen.\n");
			assert(false);
		}
	}
	return input;
}

int initial_propgen::add_propgen_noise(int input, int error_prob){
	std::uniform_int_distribution<int> error_distribution(1, error_prob);
	if(error_distribution(generator) == 1){
		if(input == 1){
			input = 0;
		}
		else if(input == 0){
			input = 1;
		}
		else{
			printf("Error in initial_propgen::add_propgen_noise\n");
			assert(false);
		}
	}
	return input;
}
