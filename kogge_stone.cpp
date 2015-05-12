#include "kogge_stone.hpp"
#include <math.h>
#include <stdio.h>


kogge_stone::kogge_stone(int size){
	width = size;
	height = log2(size);
	inter_results = new propgen_result[(height+1)*width];
	init_propgen = new initial_propgen();
	propgen_node = new propgen();
	// printf("height: %d \n", height);
}

int kogge_stone::add(int input1, int input2){
	//size of the current bit we are adding
	int bit_size = 1;
	int i1_temp, i2_temp;
	propgen_result current,prev;
	int prev_index;
	int sum = 0;
	// The first step  (the rectangles in reference-pic)
	for(int i=0; i<width; i++){
		i1_temp = input1/bit_size;
		i1_temp = i1_temp % 2;

		i2_temp = input2/bit_size;
		i2_temp = i2_temp % 2;
		inter_results[i] = init_propgen->add(i1_temp,i2_temp);
		bit_size = bit_size*2;
		
	}
	
	// The intermediate steps (the propagate generate nodes)
	for(int c=0; c<height; c++){
		for(int i=0; i<width; i++){
			
			// The small circle case (watch reference-pic)
			//we will just forward previous result
			prev_index = i-(pow(2,c));
			if(i+1<pow(2,c+1)){
				inter_results[(c+1)*width+i] = inter_results[c*width+i];
				// printf("(c:%d, i:%d, Little circle)\n",c,i);
			}
			// the big circle case (watch reference-pic)
			else{
				current = inter_results[c*width+i];
				
				prev = inter_results[c*width+prev_index];
				inter_results[(c+1)*width + i] = propgen_node->compute(current,prev);
				// printf("(c:%d, i:%d, Big circle)\n",c,i);
			}
			// if(c==1 and i == 2){
			// 	printf("prev_index: %d\n", prev_index);
			// 	printf("current propagate: %d, generate: %d\n", current.propagate, current.generate);
			// 	printf("prev propagate: %d, generate: %d\n", prev.propagate, prev.generate);
			// 	printf("result propagate: %d, generate: %d\n", propgen_node->compute(current,prev).propagate,propgen_node->compute(current,prev).generate);
			// }
		}
	}
 	//The last step (triangles in reference-pic)
 	// Xor the initial propagate of same weight with the calculated generate.
	sum = sum + inter_results[height*width].propagate;
	bit_size = 2;
	for(int i=1; i<width; i++){
		int XOR = init_propgen->add(inter_results[i].propagate,
			inter_results[height*width+i-1].generate).propagate;
		// printf("Result %d, XOR: %d\n", i, XOR);
		sum = sum + bit_size * XOR;
		bit_size = bit_size*2;
	}


	// for(int c=0; c<height+1; c++){
	// 	printf("\nIteration %d:\n", c);
	// 	for(int i=0; i<width; i++){
	// 		printf("Inter results %d, propagate: %d, generate: %d\n", i, inter_results[c*width+i].propagate, inter_results[c*width+i].generate);
	// 	}
	// }
	return sum;
}








