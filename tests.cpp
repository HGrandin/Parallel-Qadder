#include "initial_propgen.hpp"
#include "propgen.hpp"
#include "kogge_stone.hpp"
#include "assert.h"
#include <stdio.h>
#include <random>

//only used in test_kogge_errors
#define Q_WIDTH				(8)
#define B_WIDTH				(16)
#define FILE_NAME "kogge_errors_16.csv"
#define JSON_NAME "kogge_errors_16.json"

//only used in make_error_model
#define ITERATIONS (100000000)

//used in both test_kogge_errors and make_error_model
#define Q_PROP_ERROR		(200000)
#define Q_GEN_ERROR			(200000)
#define Q_SUM_ERROR			(100000)
#define Q_INPUT_ERROR	 	(100000)
#define B_PROP_ERROR		(10000000)
#define B_GEN_ERROR			(10000000)
#define B_SUM_ERROR			(10000000)
#define B_INPUT_ERROR		(10000000)
#define Q_STABLE_MULT		(1)
#define Q_ONE_TWO_PROB		(4)
#define INPUT_SIZE			(31)



void test_initial_propgen(){
	initial_propgen_result result;
	error_rates ber = {};
    error_rates qer = {};
	initial_propgen *init_propgen = new initial_propgen(ber,qer);
	
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

void test_kogge_stone(int input1, int input2,int mode){
	std::default_random_engine generator(time(0));
    std::uniform_int_distribution<int> distribution(0, pow(2,INPUT_SIZE));
    unsigned int i1,i2,sum, minus;
    error_rates ber = {};
    error_rates qer = {};

	kogge_stone *adder = new kogge_stone(16,8,ber,qer);
	if(mode==0){
		for(int i=0; i<10000; i++){
			i1 = distribution(generator);
			i2 = distribution(generator);
			sum = adder->add(i1,i2);
			assert(sum == i1+i2);
			minus = adder->ones_complement(i1);
			if((int)minus != -i1)
				printf("minus: %d, -i1: %d\n", (int)minus, -i1);
			assert((int)minus == -i1);
		} 
		printf("Kogge Stone test completed succesfully (autotest)\n");
	}else{
		sum = adder->add(input1, input2);
		printf("sum %d \n", sum);
		assert(sum == input1+input2);
		printf("Kogge Stone test completed succesfully (manual test)\n");
	}
	delete(adder);
	
}

void test_kogge_error(){
	std::default_random_engine generator(time(0));
    std::uniform_int_distribution<int> distribution(0, pow(2,INPUT_SIZE));
    unsigned int i1,i2,sum,adds = 0,errors = 0;
    error_rates ber = {};
    error_rates qer = {};
    
    qer.propagate_error = Q_PROP_ERROR;
    qer.generate_error = Q_GEN_ERROR;
    qer.sum_error = Q_SUM_ERROR;
    qer.input_error = Q_INPUT_ERROR;
    qer.stable_multiplier = Q_STABLE_MULT;
    qer.one_two_prob = Q_ONE_TWO_PROB;

    ber.propagate_error = B_PROP_ERROR;
    ber.generate_error = B_GEN_ERROR;
    ber.sum_error = B_SUM_ERROR;
    ber.input_error = B_INPUT_ERROR;

	kogge_stone *adder = new kogge_stone(B_WIDTH,Q_WIDTH,ber,qer);
	int i = 0;
	while(true){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = adder->add(i1,i2);
		i++;
		if(i>=1000000){
			adder->print_stats(FILE_NAME, INPUT_SIZE, 2);
			adder->make_json_file(JSON_NAME);
			i=0;
		}
	} 
	
	
	delete(adder);
}

//100k iterations for each takes roughly 30-35 seconds
void make_error_model(){
	std::default_random_engine generator(time(0));
    std::uniform_int_distribution<int> distribution(0, pow(2,INPUT_SIZE));
    unsigned int i1,i2,sum,adds = 0,errors = 0;
    error_rates ber = {};
    error_rates qer = {};
    
    qer.propagate_error = Q_PROP_ERROR;
    qer.generate_error = Q_GEN_ERROR;
    qer.sum_error = Q_SUM_ERROR;
    qer.input_error = Q_INPUT_ERROR;
    qer.stable_multiplier = Q_STABLE_MULT;
    qer.one_two_prob = Q_ONE_TWO_PROB;

    ber.propagate_error = B_PROP_ERROR;
    ber.generate_error = B_GEN_ERROR;
    ber.sum_error = B_SUM_ERROR;
    ber.input_error = B_INPUT_ERROR;

	kogge_stone *b_adder = new kogge_stone(32,0,ber,qer);
	kogge_stone *q_adder = new kogge_stone(0,16,ber,qer);
	kogge_stone *q8_adder = new kogge_stone(24,4,ber,qer);
	kogge_stone *q16_adder = new kogge_stone(16,8,ber,qer);
	kogge_stone *q24_adder = new kogge_stone(8,12,ber,qer);
	kogge_stone *b_sub = new kogge_stone(32,0,ber,qer);
	kogge_stone *q_sub = new kogge_stone(0,16,ber,qer);
	kogge_stone *q8_sub = new kogge_stone(24,4,ber,qer);
	kogge_stone *q16_sub = new kogge_stone(16,8,ber,qer);
	kogge_stone *q24_sub = new kogge_stone(8,12,ber,qer);


	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = b_adder->add(i1,i2);
	}
	b_adder->print_stats("error_model/binary.csv", INPUT_SIZE, 2);
	b_adder->make_json_file("error_model/binary.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = q_adder->add(i1,i2);
	}
	q_adder->print_stats("error_model/quaternary.csv", INPUT_SIZE, 2);
	q_adder->make_json_file("error_model/quaternary.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = q8_adder->add(i1,i2);
	}
	q8_adder->print_stats("error_model/quaternary8.csv", INPUT_SIZE, 2);
	q8_adder->make_json_file("error_model/quaternary8.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = q16_adder->add(i1,i2);
	}
	q16_adder->print_stats("error_model/quaternary16.csv", INPUT_SIZE, 2);
	q16_adder->make_json_file("error_model/quaternary16.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		i2 = distribution(generator);
		sum = q24_adder->add(i1,i2);
	}
	q24_adder->print_stats("error_model/quaternary24.csv", INPUT_SIZE, 2);
	q24_adder->make_json_file("error_model/quaternary24.json");

	//ones complement stats.

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		sum = b_sub->ones_complement(i1);
	}
	b_sub->print_stats("error_model/binary_sub.csv", INPUT_SIZE, 2);
	b_sub->make_json_file("error_model/binary_sub.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		sum = q_sub->ones_complement(i1);
	}
	q_sub->print_stats("error_model/quaternary_sub.csv", INPUT_SIZE, 2);
	q_sub->make_json_file("error_model/quaternary_sub.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		sum = q8_sub->ones_complement(i1);
	}
	q8_sub->print_stats("error_model/quaternary8_sub.csv", INPUT_SIZE, 2);
	q8_sub->make_json_file("error_model/quaternary8_sub.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		sum = q16_sub->ones_complement(i1);
	}
	q16_sub->print_stats("error_model/quaternary16_sub.csv", INPUT_SIZE, 2);
	q16_sub->make_json_file("error_model/quaternary16_sub.json");

	for(int i=0;i<ITERATIONS;i++){
		i1 = distribution(generator);
		sum = q24_sub->ones_complement(i1);
	}
	q24_sub->print_stats("error_model/quaternary24_sub.csv", INPUT_SIZE, 2);
	q24_sub->make_json_file("error_model/quaternary24_sub.json");

	
	delete(b_adder);
	delete(q_adder);
	delete(q8_adder);
	delete(q16_adder);
	delete(q24_adder);
	delete(b_sub);
	delete(q_sub);
	delete(q8_sub);
	delete(q16_sub);
	delete(q24_sub);
}



int main(int argc, char *argv[]){
	test_initial_propgen();
	test_propgen();
	if(argc>2)
		test_kogge_stone(atoi(argv[1]), atoi(argv[2]), 1);
	else
		test_kogge_stone(0,0,0);
	// test_kogge_error();
	 make_error_model();



	//testings
	// int t1=1,t2=1;
	// t1 = t1 << 31;
	// unsigned int sum = 0;
	// // t2 = t2 << 31;
	// t2 = t2+123;
	// printf("t1: %d\n", t1);
	// printf("t2: %d\n", t2);
	// if(t1>t2)
	// 	sum =(unsigned int)t1 - (unsigned int)t2;
	// else
	// 	sum =(unsigned int)t2 - (unsigned int)t1;
	// printf("dif: %u\n", sum);
}