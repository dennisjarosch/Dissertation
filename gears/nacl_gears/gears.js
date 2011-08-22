/*
  Copyright 2010 Dennis Jarosch.
  Use of this source code is governed by a BSD-style license that can
  be found in the LICENSE file.
*/

gears.Application = function() {
}

gears.Application.prototype.module = null;

gears.Application.prototype.moduleDidLoad = function(naclModule) {
	this.module = naclModule;
}

gears.Application.prototype.assert = function(cond, message) {
	if (!cond) {
		message = "Assertion failed: " + message;
		alert(message);
		throw new Error(message);
	}
}

gears.Application.prototype.run = function(divName) {
  contentDivName = divName || gears.Application.DEFAULT_DIV_NAME;
  var contentDiv = document.getElementById(contentDivName);
  this.assert(contentDiv, "Missing DOM element '" + contentDivName + "'");
  // Conditionally load the develop or publish version of the module.
  if (window.location.hash == '#develop') {
    contentDiv.innerHTML = '<embed id="'
                           + gears.Application.MODULE_NAME + '" '
                           + 'type="pepper-application/gears" '
                           + 'width="400" height="400" '
                           + 'dimensions="3" />'
    moduleDidLoad();
  } else {
    // Load the published .nexe.  This includes the 'nexes' attribute which
    // shows how to load multi-architecture modules.  Each entry in the
    // table is a key-value pair: the key is the runtime ('x86-32',
    // 'x86-64', etc.); the value is a URL for the desired NaCl module.
    var nexes = 'x86-32: gears_x86_32.nexe\n'
                + 'x86-64: gears_x86_64.nexe\n'
                + 'ARM: gears_arm.nexe ';
    contentDiv.innerHTML = '<embed id="'
                           + gears.Application.MODULE_NAME + '" '
                        // + 'nexes="' + nexes + '" '
                           + 'type="application/x-nacl-srpc" '
                           + 'width="400" height="400" '
                           + 'dimensions="3" '
                           + 'onload="moduleDidLoad()" />'
    // Note: this code is here to work around a bug in Chromium build
    // #47357.  See also
    // http://code.google.com/p/nativeclient/issues/detail?id=500
    document.getElementById('gears').nexes = nexes;
  }
}

// Module name and default div name
gears.Application.MODULE_NAME = 'gears';
gears.Application.DEFAULT_DIV_NAME = 'gears_content';
