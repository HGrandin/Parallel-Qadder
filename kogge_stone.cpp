#include "kogge_stone.hpp"
#include <math.h>
#include <stdio.h>


kogge_stone::kogge_stone(int binary_size, int quaternary_size){
	q_width = quaternary_size;
	b_width = binary_size;
	width = b_width+q_width;
	height = ceil(log2(width));
	// printf("height: %d\n", height);
	sums = new int[width];
	inter_results = new propgen_result[(height+1)*width];

	init_propgen = new initial_propgen();
	propgen_node = new propgen();
	// printf("height: %d \n", height);
}

void kogge_stone::initial_step(int input1, int input2, int start_index, int iterations, int base){
	//size of the current bit we are adding
	int bit_size = pow(4,start_index);
	int i1_temp, i2_temp;
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
	int partial_sum, sum, bit_size;
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


int kogge_stone::add(int input1, int input2){
	propgen_result current,prev;
	int prev_index;
	long sum = 0;
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
	int qpart,bpart;
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
	return sum;
}








