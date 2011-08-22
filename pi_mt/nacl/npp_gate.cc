/*
  Copyright 2010 The Native Client SDK Authors. All rights reserved.

  Copyright 2010, Google Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

	* Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following disclaimer
      in the documentation and/or other materials provided with the
      distribution.
	* Neither the name of Google Inc. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nacl/nacl_npapi.h>
#include <nacl/npupp.h>

struct Pi
{
	NPP npp;
	NPObject *npobject;
};

// This file implements functions that the plugin is expected to implement so
// that the browser can call them.  All of them are required to be implemented
// regardless of whether this is a trusted or untrusted build of the module.


// Called after NP_Initialize with a Plugin Instance Pointer and context
// information for the plugin instance that is being allocated.
// Declaration: npapi.h
// Documentation URL: https://developer.mozilla.org/en/NPP_New
NPError NPP_New(NPMIMEType mime_type,
                NPP instance,
                uint16_t mode,
                int16_t argc,
                char* argn[],
                char* argv[],
                NPSavedData* saved) 
{
	if (instance == NULL)
		return NPERR_INVALID_INSTANCE_ERROR;

	struct Pi *pi = NULL;
	pi = new Pi;

	pi->npp 	 = instance;
	pi->npobject = NULL;

	instance->pdata = pi;
	return NPERR_NO_ERROR;
}

// Called when a Plugin |instance| is being deleted by the browser.  This
// function should clean up any information allocated by NPP_New but not
// NP_Initialize.  Use |save| to store any information that should persist but
// note that browser may choose to throw it away.
// In the NaCl module, NPP_Destroy is called from NaClNP_MainLoop().
// Declaration: npapi.h
// Documentation URL: https://developer.mozilla.org/en/NPP_Destroy
NPError NPP_Destroy(NPP instance, NPSavedData** save) 
{
	if (NULL == instance)
		return NPERR_INVALID_INSTANCE_ERROR;

	// free plugin
	if (NULL != instance->pdata) 
	{
	    Pi* pi = static_cast<Pi*>(instance->pdata);
		delete pi;
		instance->pdata = NULL;
	}

	return NPERR_NO_ERROR;
}

// NPP_GetScriptableInstance returns the NPObject pointer that corresponds to
// NPPVpluginScriptableNPObject queried by NPP_GetValue() from the browser.
// Helper function for NPP_GetValue to create this plugin's NPObject.
// |instance| is this plugin's representation in the browser.  Returns the new
// NPObject or NULL.
// Declaration: local
// Documentation URL: N/A (not actually an NPAPI function)
NPObject *NPP_GetScriptableInstance(NPP instance) 
{
	struct Pi* pi;
	extern NPClass *GetNPSimpleClass();

	if (NULL == instance) 
	    return NULL;

	pi = static_cast<Pi*>(instance->pdata);
	
	if (NULL == pi->npobject) 
		pi->npobject = NPN_CreateObject(instance, GetNPSimpleClass());

	if (NULL != pi->npobject)
		NPN_RetainObject(pi->npobject);

	return pi->npobject;
}

// Implemented so the browser can get a scriptable instance from this plugin.
// Declaration: npapi.h
// Documentation URL: https://developer.mozilla.org/en/NPP_GetValue
NPError NPP_GetValue(NPP instance, NPPVariable variable, void* ret_value) 
{
	if (NPPVpluginScriptableNPObject == variable) 
	{
		void** v = reinterpret_cast<void**>(ret_value);
		*v = NPP_GetScriptableInstance(instance);

		return NPERR_NO_ERROR;
	}
	else
		return NPERR_GENERIC_ERROR;
}

// |window| contains the current state of the window in the browser.  If this
// is called, that state has probably changed recently.
// Declaration: npapi.h
// Documentation URL: https://developer.mozilla.org/en/NPP_SetWindow
NPError NPP_SetWindow(NPP instance, NPWindow* window) 
{
	return NPERR_NO_ERROR;
}

extern "C" 
{
// When the browser calls NP_Initialize the plugin needs to return a list
// of functions that have been implemented so that the browser can
// communicate with the plugin.  This function populates that list,
// |plugin_funcs|, with pointers to the functions.
NPError InitializePluginFunctions(NPPluginFuncs* plugin_funcs) 
{
	memset(plugin_funcs, 0, sizeof(*plugin_funcs));
	plugin_funcs->version = NPVERS_HAS_PLUGIN_THREAD_ASYNC_CALL;
	plugin_funcs->size = sizeof(*plugin_funcs);
	plugin_funcs->newp = NPP_New;
	plugin_funcs->destroy = NPP_Destroy;
	plugin_funcs->setwindow = NPP_SetWindow;
	plugin_funcs->getvalue = NPP_GetValue;
	
	return NPERR_NO_ERROR;
}

}  // extern "C"
