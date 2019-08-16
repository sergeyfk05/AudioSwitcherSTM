#include "MyMath.h"


namespace Models
{
	
	// Наименьшее общее кратное
	uint32_t Math::LCM(uint32_t n1, uint32_t n2) 
	{ 
		uint32_t max = n1 > n2 ? n1 : n2;
		for (uint32_t i = max; i > 0; i++) {
    
			if ((i % n1 == 0) && (i % n2 == 0)) {
				return i;
			}
		}
	}
}