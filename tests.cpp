#include "initial_propgen.hpp"
#include "propgen.hpp"
#include "kogge_stone.hpp"
#include "assert.h"
#include <stdio.h>

void test_initial_propgen(){
	propgen_result result;
	initial_propgen *init_propgen = new initial_propgen();
	
	result = init_propgen->add(0,0);
	assert(result.propagate == 0);
	assert(result.generate == 0);

	result = init_propgen->add(0,1);
	assert(result.propagate == 1);
	assert(result.generate == 0);

	result = init_propgen->add(1,0);
	assert(result.propagate == 1);
	assert(result.generate == 0);

	result = init_propgen->add(1,1);
	assert(result.propagate == 0);
	assert(result.generate == 1);

	delete(init_propgen);
	printf("Half adder tests completed succesfully.\n");
}

void test_propgen(){
	propgen_result result, current, prev;
	propgen *node = new propgen();

	for(int gi=0; gi<2; gi++){
		for(int pi=0; pi<2; pi++){
			for(int gprev=0; gprev<2; gprev++){
				for(int pprev=0; pprev<2; pprev++){
					current.propagate = pi;
					current.generate = gi;
					prev.propagate = pprev;
					prev.generate = gprev;
					result = node->compute(current,prev);
					assert(result.propagate == (pi and pprev));
					assert(result.generate == (gi or (pi and gprev)));
				}
			}
		}
	}
	delete(node);
	printf("Propgen tests completed succesfully.\n");
}

void test_kogge_stone(int input1, int input2){
	kogge_stone *adder = new kogge_stone(32);
	int sum = adder->add(input1,input2);
	assert(sum == input1+input2);
	// printf("%d + %d = %d\n",input1, input2, sum);
	delete(adder);
}


int main(int argc, char *argv[]){
	test_initial_propgen();
	test_propgen();
	test_kogge_stone(atoi(argv[1]), atoi(argv[2]));
}