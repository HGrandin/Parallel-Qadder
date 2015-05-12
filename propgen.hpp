#ifndef PROPGEN_HPP_86384
#define PROPGEN_HPP_86384

// struct propgen_result{
// 	int propagate;
// 	int generate;
// };

class propgen{
	private:
	

	public:
		propgen();


		propgen_result compute(propgen_result current, propgen_result prev);
};

#endif /* end of include guard: PROPGEN_HPP_86384 */