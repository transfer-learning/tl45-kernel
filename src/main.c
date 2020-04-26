#include <string.h>
#include "tiny-json/tiny-json.h"

int search(json_t const* json) {
	jsonType_t const type = json_getType( json );
	if ( type == JSON_OBJ ) {
		json_t const* yeet = json_getProperty( json, "boomer" );
		if ( yeet && JSON_INTEGER == json_getType( yeet ) ) {
			return (int) json_getInteger ( yeet );
		}
    }
    if ( type == JSON_OBJ || type == JSON_ARRAY ) {
	    for( json_t const* child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {
	    	int result = search(child);
	        if (result != 0xFFFF) {
	        	return result;
	        }
	    }
    }
	return 0xffff;
}

int main() {
	volatile char* str = 0x100000;
	json_t mem[32];
	json_t const* json = json_create( str, mem, sizeof mem / sizeof *mem );
	if (!json) return 0;
	int result = search(json);
	if (result == 0x81) {
		return 0x6969;
	}
	return 0xbad;
}