#include <stdint.h>
#include <inttypes.h>
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
