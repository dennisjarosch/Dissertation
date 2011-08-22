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

#include <assert.h>
#include <nacl/npapi_extensions.h>
#include <nacl/npupp.h>

// This file implements functions that are used by the plugin to call into the
// browser.  Note that the NPN_* functions defined by the NPAPI are provided
// by the Native Client runtime.

// Pepper extensions.  These get instantiated when NPN_New is called and
// populated with pepper extensions in the NPN_GetValue call below.
static NPExtensions* kPepperExtensions = NULL;

// Chrome specific, this function calls GetValue to get Pepper extensions
// made available to the plugin, |instance|.
// Declaration of NPNVPepperExtensions: npapi_extensions.h
void InitializePepperExtensions(NPP instance) {
  // Grab the Pepper extensions.
  NPN_GetValue(instance, NPNVPepperExtensions,
               reinterpret_cast<void*>(&kPepperExtensions));
  assert(NULL != kPepperExtensions);
}

// These are Pepper extensions.  Gets a higher-level device from
// the pepper interface.  For example audio, 2d and 3d devices.
// Returns NULL if device could not be acquired.
// Decaration of kPepperExtensions::acquireDevice: npapi_extensions.h
NPDevice* NPN_AcquireDevice(NPP instance, NPDeviceID device) {
  return kPepperExtensions ?
      kPepperExtensions->acquireDevice(instance, device) : NULL;
}
