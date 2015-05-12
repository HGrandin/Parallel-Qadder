#include "half_adder.hpp"

half_adder::half_adder(){
}

propgen_result half_adder::add(int input1, int input2){
	int sum = 0, carry = 0;
	propgen_result result;
	if(input1 and input2)
		carry = 1;
	if((input1 and !input2) or (!input1 and input2))
		sum = 1;
	result.propagate = sum;
	result.generate = carry;
	return result;
}