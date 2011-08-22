/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

function pi(iterations) {
	var result = 0;

	// implements the Leibnitz formula
	for (var n=0; n<iterations; n++) 
		result +=  (1 - ((((n)) & 1) << 1)) / ((2 * n) + 1);

	result = 4 *result;
	return result;
} 


self.addEventListener("message", function(event) {
	postMessage( pi(event.data) );
}, false);

