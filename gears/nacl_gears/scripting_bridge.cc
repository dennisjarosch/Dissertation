// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "scripting_bridge.h"

#include <assert.h>

#include "nacl_gears.h"

using namespace std;

// static
map<NPIdentifier, ScriptingBridge::Method> 		ScriptingBridge::methodTable;
map<NPIdentifier, ScriptingBridge::Property>	ScriptingBridge::propertyTable;

NPIdentifier ScriptingBridge::idKeyDown;
NPIdentifier ScriptingBridge::idKeyUp;
//_______________________________________________________________________

ScriptingBridge::ScriptingBridge(NPP npp)
	:	_npp(npp),
		_npObj(NULL)
{
	// get windows object
	NPError error = NPN_GetValue(_npp, NPNVWindowNPObject, &_npObj);
	assert(NPERR_NO_ERROR == error);
}
//_______________________________________________________________________

ScriptingBridge::~ScriptingBridge()
{
	if (_npObj)
		NPN_ReleaseObject(_npObj);
}
//_______________________________________________________________________

void ScriptingBridge::Invalidate(void)
{
}
//_______________________________________________________________________

bool ScriptingBridge::HasMethod(NPIdentifier name)
{
	map<NPIdentifier, Method>::iterator iMeth;
	iMeth = methodTable.find(name);

	return iMeth != methodTable.end();
}
//_______________________________________________________________________

bool ScriptingBridge::Invoke(NPIdentifier name,
	              const NPVariant* args,
	              uint32_t arg_count,
	              NPVariant* result)
{
	map<NPIdentifier, Method>::iterator iMeth;
	iMeth = methodTable.find(name);

	if (iMeth != methodTable.end())
		return (this->*(iMeth->second))(args, arg_count, result);

	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::InvokeDefault(const NPVariant* args,
	                     uint32_t arg_count,
	                     NPVariant* result)
{
	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::HasProperty(NPIdentifier name)
{
	map<NPIdentifier, Property>::iterator iProp;
	iProp = propertyTable.find(name);

	return iProp != propertyTable.end();
}
//_______________________________________________________________________

bool ScriptingBridge::GetProperty(NPIdentifier name, NPVariant* result)
{
	VOID_TO_NPVARIANT(*result);

	map<NPIdentifier, Property>::iterator iProp;
	iProp = propertyTable.find(name);

	if (iProp != propertyTable.end()) 
		return (this->*(iProp->second))(result);

	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::SetProperty(NPIdentifier name, const NPVariant* value)
{
	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::RemoveProperty(NPIdentifier name)
{
	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::KeyDown(const NPVariant* args,
								uint32_t arg_count,
								NPVariant* result)
{
	if (arg_count != 1 || !NPVARIANT_IS_INT32(args[0]))
		return false;

	int key = NPVARIANT_TO_INT32(args[0]);

	NaclGears* gears = static_cast<NaclGears*>(_npp->pdata);
	if (gears)
		return gears->KeyDown(key);

	return false;
}
//_______________________________________________________________________

bool ScriptingBridge::KeyUp(const NPVariant* args,
								uint32_t arg_count,
								NPVariant* result)
{
	if (arg_count != 1 || !NPVARIANT_IS_INT32(args[0]))
		return false;

	int key = NPVARIANT_TO_INT32(args[0]);


	NaclGears* gears = static_cast<NaclGears*>(_npp->pdata);
	if (gears)
		return gears->KeyUp(key);

	return false;
}
//_______________________________________________________________________

void ScriptingBridge::Init(void)
{
	idKeyDown = NPN_GetStringIdentifier("keyDown");
	idKeyUp   = NPN_GetStringIdentifier("keyUp");

	methodTable.insert(pair<NPIdentifier, Method>(idKeyDown, &ScriptingBridge::KeyDown));
	methodTable.insert(pair<NPIdentifier, Method>(idKeyUp, &ScriptingBridge::KeyUp));
}

//_______________________________________________________________________
//
// Boilerplate code to connect the browser interface to our 
// ScriptingBridge class
//_______________________________________________________________________



namespace gears {

void Invalidate(NPObject* object) 
{
	return static_cast<ScriptingBridge*>(object)->Invalidate();
}
//_______________________________________________________________________

bool HasMethod(NPObject* object, NPIdentifier name) 
{
	return static_cast<ScriptingBridge*>(object)->HasMethod(name);
}
//_______________________________________________________________________

bool Invoke(NPObject* object, 
			NPIdentifier name,
            const NPVariant* args, 
			uint32_t arg_count,
            NPVariant* result) 
{
	return static_cast<ScriptingBridge*>(object)->Invoke(name, args, arg_count, result);
}
//_______________________________________________________________________

bool InvokeDefault(NPObject* object, 
				   const NPVariant* args, 
				   uint32_t arg_count,
                   NPVariant* result) 
{
	return static_cast<ScriptingBridge*>(object)->InvokeDefault(args, arg_count, result);
}
//_______________________________________________________________________

bool HasProperty(NPObject* object, NPIdentifier name) 
{
	return static_cast<ScriptingBridge*>(object)->HasProperty(name);
}
//_______________________________________________________________________

bool GetProperty(NPObject* object, NPIdentifier name, NPVariant* result) 
{
	return static_cast<ScriptingBridge*>(object)->GetProperty(name, result);
}
//_______________________________________________________________________

bool SetProperty(NPObject* object, NPIdentifier name, const NPVariant* value) 
{
	return static_cast<ScriptingBridge*>(object)->SetProperty(name, value);
}
//_______________________________________________________________________

bool RemoveProperty(NPObject* object, NPIdentifier name) 
{
	return static_cast<ScriptingBridge*>(object)->RemoveProperty(name);
}
//_______________________________________________________________________

NPObject* Allocate(NPP npp, NPClass* npclass) 
{
	return new ScriptingBridge(npp);
}
//_______________________________________________________________________

void Deallocate(NPObject* object) 
{
	delete static_cast<ScriptingBridge*>(object);
}

} // namespace gears
//_______________________________________________________________________

// functions available to the browser
NPClass ScriptingBridge::np_class = {
	NP_CLASS_STRUCT_VERSION,
	gears::Allocate,
	gears::Deallocate,
	gears::Invalidate,
	gears::HasMethod,
	gears::Invoke,
	gears::InvokeDefault,
	gears::HasProperty,
	gears::GetProperty,
	gears::SetProperty,
	gears::RemoveProperty
};
//_______________________________________________________________________
