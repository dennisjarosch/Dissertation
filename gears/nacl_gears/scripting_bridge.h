// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include <map>

#include <nacl/nacl_npapi.h>
#include <nacl/npruntime.h>

//_______________________________________________________________________

class ScriptingBridge : public NPObject
{
public:
	typedef bool (ScriptingBridge::*Method)(const NPVariant* args,
		                  uint32_t arg_count,
		                  NPVariant* result);
	typedef bool (ScriptingBridge::*Property)(NPVariant* result);

	explicit ScriptingBridge(NPP npp);
	virtual ~ScriptingBridge();

	// methods that implement the NPObject interface
	virtual void Invalidate(void);
	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name,
		              const NPVariant* args,
		              uint32_t arg_count,
		              NPVariant* result);
	virtual bool InvokeDefault(const NPVariant* args,
		                     uint32_t arg_count,
		                     NPVariant* result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant* result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant* value);
	virtual bool RemoveProperty(NPIdentifier name);

	// methods that are exposed to the browser
	bool KeyDown(const NPVariant* args,
					uint32_t arg_count,
					NPVariant* result);

	bool KeyUp(const NPVariant* args,
					uint32_t arg_count,
					NPVariant* result);


	// static
	static NPClass 		np_class;
	static NPIdentifier	idKeyDown;
	static NPIdentifier	idKeyUp;

	static void Init(void);

private:
	NPP 		_npp;
	NPObject* 	_npObj;

	static std::map<NPIdentifier, Method> 	methodTable;
	static std::map<NPIdentifier, Property>	propertyTable;
};

//_______________________________________________________________________
