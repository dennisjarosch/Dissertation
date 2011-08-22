/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

function pi(from, to) {
	var result = 0;

	// implements the Leibnitz formula
	for (var n=from; n<to; n++) 
		result +=  (1 - ((((n)) & 1) << 1)) / ((2 * n) + 1);

	return result;
} 

self.addEventListener("message", function(event) {
	postMessage( pi(event.data.from, event.data.to) );
}, false);

