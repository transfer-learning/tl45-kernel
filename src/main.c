#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

volatile uint8_t* str;
volatile size_t input_size;

/*
#include "tiny-json/tiny-json.h"

int search(json_t const* json, const char* key) {
	jsonType_t const type = json_getType( json );
	if ( type == JSON_OBJ ) {
		json_t const* yeet = json_getProperty( json, key );
		if ( yeet && JSON_INTEGER == json_getType( yeet ) ) {
			return (int) json_getInteger ( yeet );
		}
    }
    if ( type == JSON_OBJ || type == JSON_ARRAY ) {
	    for( json_t const* child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {
	    	int result = search(child, key);
	        if (result != 0xFFFF) {
	        	return result;
	        }
	    }
    }
	return 0xffff;
}

int fuzz_json() {
	json_t mem[32];
	json_t const* json = json_create( str, mem, sizeof mem / sizeof *mem );
	if (!json) return 0;
	int value = search(json, "memes");
	if (value != 0x81) {
		return 0xbad1;
	}
	if (value + search(json, "yeet") != 228) {
		return 0xbad2;
	}
	return 0x1337;
}
*/

#include "upng.h"
int fuzz_png() {
	int result = 0;
	upng_t* png = upng_new_from_bytes(str, input_size);
	if (png != NULL) {
		result = 5;
		upng_decode(png);
		if (upng_get_error(png) == UPNG_EOK) {
			result = 1;
	    }
		upng_free(png);
	} else {
		result = 0xbad;
	}
	return result;
}

#ifdef _NATIVE
void init_input() {
	uintptr_t cap = 1024;
	uint8_t* buf = malloc(cap);
	size_t n = 0, nread = 0;
	while (nread = fread(buf+n, 1, cap-n, stdin)) {
		n += nread;
		if (n == cap) {
			cap *= 2;
			buf = realloc(buf, cap);
		}
	}
	str = (volatile uint8_t*) buf;
	input_size = n;
	// printf("%s %d\n", str, input_size);
}
#else
void init_input() {
	add_malloc_block(0x300000, 0x100000);
	str = (volatile uint8_t*) 0x100000;
	input_size = *(volatile size_t*) 0xFFFFC;
}
#endif

int main() {
	init_input();

	// fuzz_entry(str);

	return fuzz_png();
}