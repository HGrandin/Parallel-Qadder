#include "kogge_stone.hpp"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


kogge_stone::kogge_stone(int b_size, int q_size, error_rates b_errors, error_rates q_errors){
	ber = b_errors;
	qer = q_errors;
	q_width = q_size;
	b_width = b_size;
	width = b_width+q_width;
	height = ceil(log2(width));
	sums = new int[width];
	inter_results = new propgen_result[(height+1)*width];

	init_propgen = new initial_propgen(ber,qer);
	propgen_node = new propgen();

	//statistics
	quaternary_size = pow(4,q_width);
	bit_error_array = new long long int[32];
	for(int i=0; i<32;i++){
		bit_error_array[i]=0;
	}
	quart_error_array = new long long int[q_width];
	for(int i=0; i<q_width;i++){
		quart_error_array[i]=0;
	}
	error_size_array = new long long int[32];
	for(int i=0; i<32;i++){
		error_size_array[i]=0;
	}
	bit_error_size_array = new long long int[32];
	for(int i=0; i<32;i++){
		bit_error_size_array[i]=0;
	}

	quaternary_errors=0;
	binary_errors=0;
	nr_of_adds=0;
	incorrect_adds=0;
}

void kogge_stone::initial_step(unsigned int input1, unsigned int input2, int start_index, int iterations, int base){
	//size of the current bit we are adding
	int bit_size = pow(4,start_index);
	unsigned int i1_temp, i2_temp;
	initial_propgen_result init_propgen_result;
	
	for(int i=start_index; i<iterations+start_index; i++){
		i1_temp = input1/bit_size;
		i1_temp = i1_temp % base;

		i2_temp = input2/bit_size;
		i2_temp = i2_temp % base;

		if(base == 2){
			i1_temp = i1_temp*2;
			i2_temp = i2_temp*2;
		}
		init_propgen_result = init_propgen->compute(i1_temp,i2_temp,base);
		
		sums[i] = init_propgen_result.sum;
		inter_results[i].propagate = init_propgen_result.propagate;
		inter_results[i].generate = init_propgen_result.generate;
		bit_size = bit_size*base;
	}
}

int kogge_stone::summarizing_step(int start_index, int iterations,int base){
	unsigned int partial_sum, sum, bit_size;
	//special case, first node cant look at the lower bit neighboor for generate
	if(start_index == 0 and iterations > 0){
		if(base == 2){
			sums[0] = sums[0]/2;
		}
		sum = sums[0];
		bit_size = base;
		start_index = 1;
		iterations = iterations - 1;//fulhack
	}else{
		sum = 0;
		bit_size = pow(4,start_index);
	}
	for(int i=start_index; i<iterations+start_index; i++){
		 int gen = inter_results[height*width+i-1].generate;
		 if(base == 2){
		 	gen = gen*2;
		 }
		partial_sum = init_propgen->compute(sums[i], gen, base).sum;

		if(base == 2){
			partial_sum = partial_sum/2;
		}
		sum = sum + bit_size * partial_sum;
		bit_size = bit_size*base;
	}
	return sum;
}

unsigned int kogge_stone::add(unsigned int input1, unsigned int input2){
	propgen_result current,prev;
	int prev_index;
	unsigned int sum = 0;
	// The first step  (the rectangles in reference-pic)
	initial_step(input1,input2,0,q_width,4);
	initial_step(input1,input2,q_width,b_width,2);

	// The intermediate steps (the propagate generate nodes)
	for(int c=0; c<height; c++){
		for(int i=0; i<width; i++){
			
			// The small circle case (watch reference-pic)
			//we will just forward previous result
			prev_index = i-(pow(2,c));
			if(i<pow(2,c)){
				inter_results[(c+1)*width+i] = inter_results[c*width+i];
			}
			// the big circle case (watch reference-pic)
			else{
				current = inter_results[c*width+i];
				
				prev = inter_results[c*width+prev_index];
				inter_results[(c+1)*width + i] = propgen_node->compute(current,prev);
			}
		}
	}
 	

 	// Xor the initial propagate of same weight with the calculated generate.
	unsigned int qpart,bpart;
	qpart = summarizing_step(0,q_width,4);
	bpart = summarizing_step(q_width,b_width,2);
	sum = qpart + bpart;

	//DEBUGGING
	// if(sum != input1+input2){
	// 	printf("qpart: %d\n", qpart);
	// 	printf("bpart: %d\n\n", bpart);
	// 	printf("            %d\n", input1);
	// 	printf("           +%d\n", input2);
	// 	printf("Real answer:%d\n",input1+input2);
	// 	printf("got         %ld\n",sum);
	// 	printf("difference  %ld\n", sum-(input1+input2));
	// 	printf("dimension   2^%d\n",(int)log(input1+input2));
	// }

	// int row=3;
	// printf("printing row %d\n", row);
	// for(int i=width-1; i>-1;i--){
	// 	printf("cell %d: g=%d, p=%d \n", i, inter_results[row*width+i].generate, inter_results[row*width+i].propagate);
	// }
	// DEBUGGING

	compare_results(sum, input1+input2);
	return sum;
}

unsigned int kogge_stone::ones_complement(unsigned int input){
	//size of the current bit we are adding
	int bit_size = 1;
	unsigned int i_temp;
	int result = 0, partial_result = 0;
	
	for(int i=0; i<q_width; i++){
		i_temp = input/bit_size;
		i_temp = i_temp % 4;

		result = result + bit_size*init_propgen->q_add_variable_noise(i_temp,qer.ones_complement_error,qer.one_two_prob);
		bit_size = bit_size << 2;
	}
	for(int i=0;i<b_width;i++){
		i_temp = input/bit_size;
		i_temp = i_temp % 2;
		 i_temp = i_temp<<1;

		partial_result = init_propgen->b_add_variable_noise(i_temp,ber.ones_complement_error);
		partial_result = partial_result>>1;
		partial_result = bit_size*partial_result;
		result = result + partial_result;
		bit_size = bit_size << 1;
	}
	result = ~result+1;
	input = ~input+1;
	compare_results(result,input);
	return result;
}

void kogge_stone::compare_results(unsigned int input1, unsigned int input2){
	int i1_temp, i2_temp, starting_bit=1;
	unsigned long error_size = 0;
	nr_of_adds++;
	if(input1 != input2){
		incorrect_adds++;
		
		// size of error counter
		if(input1>input2)
			error_size = (unsigned int)input1-(unsigned int)input2;
		else
			error_size = (unsigned int)input2-(unsigned int)input1;

		for(int i = 0; i < 32; i++){
			if(error_size >= pow(2,i) and error_size < pow(2,i+1))
				error_size_array[i]++;
		}
		//quaternary error counter
		if(input1 % quaternary_size != input2 % quaternary_size){
			quaternary_errors++;
		}
		//binary error counter
		if(input1 / quaternary_size != input2 / quaternary_size){
			binary_errors++;
		}
		//bitwise error counter
		for(int i = 0; i <32;i++){
			i1_temp = input1/starting_bit;
			i1_temp = i1_temp % 2;

			i2_temp = input2/starting_bit;
			i2_temp = i2_temp % 2;

			if(i1_temp != i2_temp){
				bit_error_array[i]++;
			}
			starting_bit = starting_bit*2;
		}
		//quaternary cells error counter
		starting_bit = 1;
		for(int i = 0; i<q_width; i++){
			i1_temp = input1/starting_bit;
			i1_temp = i1_temp % 4;

			i2_temp = input2/starting_bit;
			i2_temp = i2_temp % 4;

			if(i1_temp != i2_temp){
				quart_error_array[i]++;
			}
			starting_bit = starting_bit*4;
		}
		//bitwise error size counter
		starting_bit = 1;
		int error_temp;
		for(int i = 0; i <32;i++){
			error_temp = error_size/starting_bit;
			error_temp = error_temp % 2;

			if(error_temp == 1){
				bit_error_size_array[i]++;
			}
			starting_bit = starting_bit*2;
		}

	}
}

void kogge_stone::print_stats(std::string file, int input_size, int mode){
	if(mode == 1 or mode == 2){
		std::ofstream myfile;
		myfile.open (file);
		myfile << "Input size, Q_width, B_width, Q Propagate Error, Q Generate Error, Q Sum Error, ";
		myfile << "Q Input Error, Q Stable Mult, B Propagate Error, B Generate Error, ";
		myfile << "B Sum Error, B Input Error, Nr of adds, Errors, Q-side errors, B-side errors, ";
		for(int i = 1; i<q_width+1;i++){
			myfile << "Q-Cell " << i << ", ";
		}
		myfile << "Bit1, Bit2, Bit3, Bit4, Bit5, Bit6, Bit7, Bit8, Bit9, Bit10, Bit11, ";
		myfile << "Bit12, Bit13, Bit14, Bit15, Bit16, Bit17, Bit18, Bit19, Bit20, Bit21, Bit22, ";
		myfile << "Bit23, Bit24, Bit25, Bit26, Bit27, Bit28, Bit29, Bit30, Bit31, Bit32, ";
		myfile << "Error >= 2^0 and < 2^1, Error >= 2^1 and < 2^2, Error >= 2^2 and < 2^3, Error >= 2^3 and < 2^4, ";
		myfile << "Error >= 2^4 and < 2^5, Error >= 2^5 and < 2^6, Error >= 2^6 and < 2^7, ";
		myfile << "Error >= 2^7 and < 2^8, Error >= 2^8 and < 2^9, Error >= 2^9 and < 2^10, ";
		myfile << "Error >= 2^10 and < 2^11, Error >= 2^11 and < 2^12, Error >= 2^12 and < 2^13, ";
		myfile << "Error >= 2^13 and < 2^14, Error >= 2^14 and < 2^15, Error >= 2^15 and < 2^16, ";
		myfile << "Error >= 2^16 and < 2^17, Error >= 2^17 and < 2^18, Error >= 2^18 and < 2^19, ";
		myfile << "Error >= 2^19 and < 2^20, Error >= 2^20 and < 2^21, Error >= 2^21 and < 2^22, ";
		myfile << "Error >= 2^22 and < 2^23, Error >= 2^23 and < 2^24, Error >= 2^24 and < 2^25, ";
		myfile << "Error >= 2^25 and < 2^26, Error >= 2^26 and < 2^27, Error >= 2^27 and < 2^28, ";
		myfile << "Error >= 2^28 and < 2^29, Error >= 2^29 and < 2^30, Error >= 2^30 and < 2^31, ";
		myfile << "Error >= 2^31 and < 2^32\n";
		
		myfile << input_size << ", " <<q_width << ", " << b_width << ", " << qer.propagate_error << ", ";
		myfile << qer.generate_error << ", " << qer.sum_error << ", " << qer.input_error << ", ";
		myfile << qer.stable_multiplier << ", " << ber.propagate_error << ", ";
		myfile << ber.generate_error << ", " << ber.sum_error << ", " << ber.input_error << ", ";
		myfile << nr_of_adds << ", ";
		myfile << incorrect_adds << " ," << quaternary_errors << ", " << binary_errors;
		
		for(int i = 0; i < q_width; i++){
			myfile << ", " << quart_error_array[i];
		}
		for(int i = 0; i < 32; i++){
			myfile << ", " << bit_error_array[i];
		}
		for(int i = 0; i < 32; i++){
			myfile << ", " << error_size_array[i];
		}
		myfile << "\n";
		
		myfile.close();
	}
	if(mode == 0 or mode == 2){
		printf("Input size: %d\n", input_size);
		printf("Q_width: %d\n", q_width);
		printf("B_width: %d\n", b_width);
		
		printf("Q Propagate Error: 1 in %d \n", qer.propagate_error);
		printf("Q Generate Error: 1 in %d \n", qer.generate_error);
		printf("Q Sum Error: 1 in %d \n", qer.sum_error);
		printf("Q Input Error: 1 in %d \n", qer.input_error);
		printf("Q Stable multiplier: %d \n", qer.stable_multiplier);
		
		printf("B Propagate Error: 1 in %d \n", ber.propagate_error);
		printf("B Generate Error: 1 in %d \n", ber.generate_error);
		printf("B Sum Error: 1 in %d \n", ber.sum_error);
		printf("B Input Error: 1 in %d \n", ber.input_error);
		
		printf("Total number of adds %lld \n", nr_of_adds);
		printf("Number of incorrect adds %lld \n", incorrect_adds);
		printf("Errors on the quaternary side: %lld\n", quaternary_errors);
		printf("Errors on the binary side: %lld\n", binary_errors);
		
		for(int i = 0; i < q_width; i++){
			printf("Errors on Q-Cell %d: %lld\n", i+1, quart_error_array[i]);
		}
		for(int i = 0; i < 32; i++){
			printf("Errors on bit %d: %lld\n", i+1, bit_error_array[i]);
		}
		for(int i = 0; i < 32; i++){
			printf("Errors bigger or equal to 2^%d and less than 2^%d: %lld\n",i, i+1, error_size_array[i]);
		}
	}
}

void kogge_stone::make_json_file(std::string file){
	std::ofstream myfile;
	myfile.open (file);
	myfile << "{\"ADDITION_TOTAL\":" << nr_of_adds;
	for(int i=0; i<32; i++){
		myfile << ", \"BIT" << i+1 << "\":" << bit_error_size_array[i];
	}
	myfile << "}";
	myfile.close();

}
