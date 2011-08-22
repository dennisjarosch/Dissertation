// Copyright (c) 2011 Dennis Jarosch. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <nacl/nacl_npapi.h>

#define POW_NEG1(x) (1 - ((((int)(x)) & 1) << 1))

// Defines the JS method name
static const char* kPi = "calc";

// Calculates pi according to the Leibnitz formula.
static bool Calc(const NPVariant* args,
                 uint32_t arg_count,
				 NPVariant *result) 
{
	if (result) 
	{
		double pi = 0;
		unsigned int n; 
		unsigned int iterations = NPVARIANT_TO_INT32(args[0]);

		for (n = 0; n < iterations; n++)
//			pi = pi + pow(-1, n) / ((2 * n) + 1);
			pi = pi + (double)POW_NEG1(n) / ((2 * n) + 1);

		pi = pi * 4;

		DOUBLE_TO_NPVARIANT(pi, *result);
	}

	return true;
}


// Creates the plugin-side instance of NPObject.
// Called by NPN_CreateObject, declared in npruntime.h
// Documentation URL: https://developer.mozilla.org/en/NPClass
static NPObject* Allocate(NPP npp, NPClass* npclass) 
{
	return new NPObject;
}

// Cleans up the plugin-side instance of an NPObject.
// Called by NPN_ReleaseObject, declared in npruntime.h
// Documentation URL: https://developer.mozilla.org/en/NPClass
static void Deallocate(NPObject* object) 
{
	delete object;
}

// Returns |true| if |method_name| is a recognized method.
// Called by NPN_HasMethod, declared in npruntime.h
// Documentation URL: https://developer.mozilla.org/en/NPClass
static bool HasMethod(NPObject* obj, NPIdentifier method_name) 
{
	char *name = NPN_UTF8FromIdentifier(method_name);
	bool is_method = false;

	if (!strcmp((const char *)name, kPi)) 
	{
		is_method = true;
	}

	NPN_MemFree(name);
	return is_method;
}

// Called by the browser to invoke the default method on an NPObject.
// Returns null.
// Apparently the plugin won't load properly if we simply
// tell the browser we don't have this method.
// Called by NPN_InvokeDefault, declared in npruntime.h
// Documentation URL: https://developer.mozilla.org/en/NPClass
static bool InvokeDefault(NPObject *obj, const NPVariant *args,
                          uint32_t argCount, NPVariant *result) 
{
	if (result) {
	    NULL_TO_NPVARIANT(*result);
	}

	return true;
}

// Called by the browser to invoke a function object whose name
// is |method_name|.
// Called by NPN_Invoke, declared in npruntime.h
// Documentation URL: https://developer.mozilla.org/en/NPClass
static bool Invoke(NPObject* obj,
                   NPIdentifier method_name,
                   const NPVariant *args,
                   uint32_t arg_count,
                   NPVariant *result) 
{
	NULL_TO_NPVARIANT(*result);
	char *name = NPN_UTF8FromIdentifier(method_name);
	if (name == NULL)
	    return false;

	bool rval = false;

	// Map the method name to a function call.  |result| is filled in by the
	// called function, then gets returned to the browser when Invoke() returns.
	if (!strcmp((const char *)name, kPi)) 
	    rval = Calc(args, arg_count, result);

	// Since name was allocated above by NPN_UTF8FromIdentifier,
	// it needs to be freed here.
	NPN_MemFree(name);
	return rval;
}

// Represents a class's interface, so that the browser knows what functions it
// can call on this plugin object.  The browser can use the methods in this
// class to discover the rest of the plugin's interface.
// Documentation URL: https://developer.mozilla.org/en/NPClass
static NPClass kPiClass = {
	NP_CLASS_STRUCT_VERSION,
	Allocate,
	Deallocate,
	NULL,  // Invalidate is not implemented
	HasMethod,
	Invoke,
	InvokeDefault,
	NULL,  // HasProperty is not implemented
	NULL,  // GetProperty is not implemented
	NULL,  // SetProperty is not implemented
};

// Called by NPP_GetScriptableInstance to get the scripting interface for
// this plugin.
NPClass *GetNPSimpleClass() {
	return &kPiClass;
}
