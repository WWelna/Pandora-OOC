/*
* Copyright (C) 2016 William H. Welna All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY William H. Welna ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL William H. Welna BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* -- Pandora Source Code --
 *
 * None of this source code released will compile without full understanding of
 * the code as well as some work. This is only a fraction of the code and these
 * select parts are for informational purposes only. This for teaching and showing
 * how to code in a OOC style to trip up disassemblers and antivirus software.
 *
 *
*/

#include <pandora.h>

/* Generic */
void *new_ctor(gen_ctor ctor) {
	generic_object *o = (generic_object *)myalloc(1, sizeof(generic_object));
	if(o) {
		o->fail = (void *)&gen_exception; // Set the Generic Exception Handler so having custom handler optional to parent object constructor
		o->fail_code = 0;
		ctor((void *)o); // Call Constructor
		return o; // return object
	}
	else
		return NULL; // Can't allocate
}

void gen_exception(generic_object *o, unsigned int code) { // Handles Generic Fails by printing an error and calling the objects destructor
	fprintf(stderr, ">>>[ Object %p Failed with code number %i ]<<<\n", o, code);
	o->fail_code = code;
}

void gen_exception_noop(generic_object *o, unsigned int code) { // Special exception handler that just sets the fail_code and exits
	o->fail_code = code;
}

void delete_dtor(generic_object *o) {
	if(o) {
		if(o->dtor) { // Does it even have a destructor?
			gen_dtor dtor = (gen_dtor)o->dtor;
			dtor((void *)o); // Call object's destructor
		}
		if(o->add)
			myfree(o->add);
		if(o->methods)
			myfree(o->methods);
		if(o->data)
			myfree(o->data);
		myfree(o); // Call free here so that if exception happens can retrive code before destroying object
	}
}
/* Generic */


