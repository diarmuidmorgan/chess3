#include <stdint.h>
#include <strings.h>
#include <inttypes.h>

//This is all up for deletion. Though the binary_print_board function is good for printdebugging.

/* Base method, converts a 64 word binary array to a uint64_t number
* Destroys the provided pointer 'a'.
**/

//used when building the inital game state and move masks. Not for much else.
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
	// oh wait - those arrays do get freed afterall
	//free(a);
	
	return number;
}
// eh useful because you can't seem to pass the ffsll() function as a int (* func) () parameter,
// for some reason?

int bitscanforward(uint64_t msk){
	return __builtin_ffsll(msk);
}
int bitscanreverse(uint64_t msk){

	return 64 - (__builtin_clzll(msk));

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
	index = __builtin_ffsll(word);
	
	
	incr += index;
	A[incr - 1] = 1;
	if (index == 64) break;
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


