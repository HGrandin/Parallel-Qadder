#ifndef INITIAL_PROPGEN_HPP_67378
#define INITIAL_PROPGEN_HPP_67378

struct initial_propgen_result{
	int sum;
	int propagate;
	int generate;
};

class initial_propgen{
	private:
	

	public:
		initial_propgen();

		//base tells us if its binary or quaternary.
		initial_propgen_result compute(int input1, int input2, int base);
};

#endif /* end of include guard: INITIAL_PROPGEN_HPP_67378 */