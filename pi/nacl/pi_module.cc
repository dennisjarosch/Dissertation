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

#include <nacl/npupp.h>

// These functions are called when a module instance is first loaded, and when
// the module instance is finally deleted.  They must use C-style linkage.

extern "C" {
// Populates |plugin_funcs| by calling InitializePluginFunctions.
// Declaration: npupp.h
// Web Reference: N/A
NPError NP_GetEntryPoints(NPPluginFuncs* plugin_funcs)
{
	extern NPError InitializePluginFunctions(NPPluginFuncs* plugin_funcs);
	return InitializePluginFunctions(plugin_funcs);
}


// Some platforms, including Native Client, use the two-parameter version of
// NP_Initialize(), and do not call NP_GetEntryPoints().  Others (Mac, e.g.)
// use single-parameter version of NP_Initialize(), and then call
// NP_GetEntryPoints() to get the NPP functions.  Also, the NPN entry points
// are defined by the Native Client loader, but are not defined in the trusted
// plugin loader (and must be filled in in NP_Initialize()).

// Called when the first instance of this plugin is first allocated to
// initialize global state.  The browser is hereby telling the plugin its
// interface in |browser_functions| and expects the plugin to populate
// |plugin_functions| in return.  Memory allocated by this function may only
// be cleaned up by NP_Shutdown.
// returns an NPError if anything went wrong.
// Declaration: npupp.h
// Documentation URL: https://developer.mozilla.org/en/NP_Initialize
NPError NP_Initialize(NPNetscapeFuncs* browser_functions,
                      NPPluginFuncs* plugin_functions) 
{
	return NP_GetEntryPoints(plugin_functions);
}

// Called just before the plugin itself is completely unloaded from the
// browser.  Should clean up anything allocated by NP_Initialize.
// Declaration: npupp.h
// Documentation URL: https://developer.mozilla.org/en/NP_Shutdown
NPError NP_Shutdown() 
{
	return NPERR_NO_ERROR;
}

}  // extern "C"
