#include "propgen.hpp"

propgen::propgen(){

}

propgen_result propgen::compute(propgen_result current, propgen_result prev){
	int propagate = 0, generate = 0;
	propgen_result result;
	
	if(current.generate or (current.propagate and prev.generate))
		generate = 1;

	if(current.propagate and prev.propagate)
		propagate = 1;

	result.propagate = propagate;
	result.generate = generate;
	return result;
}