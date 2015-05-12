#ifndef HALF_ADDER_HPP_67378
#define HALF_ADDER_HPP_67378

struct propgen_result{
	int propagate;
	int generate;
};

class half_adder{
	private:
	

	public:
		half_adder();


		propgen_result add(int input1, int input2);
};

#endif /* end of include guard: HALF_ADDER_HPP_67378 */