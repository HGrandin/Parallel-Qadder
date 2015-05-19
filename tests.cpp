#include "initial_propgen.hpp"
#include "propgen.hpp"
#include "kogge_stone.hpp"
#include "assert.h"
#include <stdio.h>
#include <random>

void test_initial_propgen(){
	initial_propgen_result result;
	initial_propgen *init_propgen = new initial_propgen();
	
	result = init_propgen->compute(0,0,2);
	assert(result.sum == 0);
	assert(result.propagate == 0);
	assert(result.generate == 0);

	result = init_propgen->compute(0,2,2);
	assert(result.sum == 2);
	assert(result.propagate == 1);
	assert(result.generate == 0);

	result = init_propgen->compute(2,0,2);
	assert(result.sum == 2);
	assert(result.propagate == 1);
	assert(result.generate == 0);

	result = init_propgen->compute(2,2,2);
	assert(result.sum == 0);
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
	std::default_random_engine generator(time(0));
    std::uniform_int_distribution<int> distribution(0, pow(2,31));
    int i1,i2,sum;
	kogge_stone *adder = new kogge_stone(16,8);
	if(input1 == 0 and input2 == 0){
		for(int i=0; i<1000000; i++){
			i1 = distribution(generator);
			i2 = distribution(generator);
			sum = adder->add(i1,i2);
			assert(sum == i1+i2);
		} 
	}else{
		sum = adder->add(input1, input2);
		printf("sum %d \n", sum);
		assert(sum == input1+input2);
	}
	delete(adder);
	printf("Kogge Stone test completed succesfully\n");
}


int main(int argc, char *argv[]){
	test_initial_propgen();
	test_propgen();
	if(argc>2)
		test_kogge_stone(atoi(argv[1]), atoi(argv[2]));
	else
		test_kogge_stone(0,0);
}