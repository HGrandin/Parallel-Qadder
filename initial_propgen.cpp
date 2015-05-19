#include "initial_propgen.hpp"
#include "assert.h"
#include <stdio.h>

initial_propgen::initial_propgen(){
}

initial_propgen_result initial_propgen::compute(int input1, int input2, int base){
	initial_propgen_result result;
	result.sum = 0;
	result.propagate = 0;
	result.generate = 0;
	if(base == 2){
		result.sum = (input1 + input2) % 4;
		if((input1 + input2) == 2){
			result.propagate = 1;
		}
		if((input1 + input2) > 3){
			result.generate = 1;
		}
	}
	else if(base == 4){
		result.sum = (input1 + input2) % 4;
		if((input1 + input2) == 3){
			result.propagate = 1;
		}
		if((input1 + input2) > 3){
			result.generate = 1;
		}
	}
	else{
		printf("Error: invalid value on base\n");
		assert(false);
	}

	return result;
}