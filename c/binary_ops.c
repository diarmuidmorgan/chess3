#include <stdint.h>
#include <strings.h>
#include <inttypes.h>
/* Base method, converts a 64 word binary array to a uint64_t number
* 
**/
uint64_t board_to_word(int * a){
	
	uint64_t number = 0LL;
	uint64_t base = 1LL;
	uint64_t expo = 2LL;
	for(int i = 0; i < 64; i++){	
		if ( a[i]) {
			number += base;
			
			
		}
		base = base * expo;
		
	}
	
	return number;
}
/*Very inefficient method of finding the index of the highest significant bit in uint64_t word.
 * The reverse of ffsll()
 */
uint64_t highest_significant_bit_index(uint64_t word) {

	uint64_t index = 0LL;
       	while (word != 0){
		uint64_t i = ffsll(word);
		index += i;
		word >> i;


	}	
	return index;

}
/* Helper method. Prints uint64_t word in 8*8 grid of zeros and ones.
*
*/
void binary_print_board(uint64_t word) {
	int index;
	int A[64];
	for(int i=0;i<64;i++) A[i] = 0;
	int incr = 0;
	while (word != 0LL) {
	index = ffsll(word);
	incr += index;
	A[incr - 1] = 1;
	word = word >> index;	
	}
	printf("\n");
	for (int i =0; i<8; i++){


		for (int j=0; j<8; j++) {

			printf("%d", A[i*8+j]);
		}
		printf("\n");
	}
}


