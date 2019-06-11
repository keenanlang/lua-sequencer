#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <epicsExport.h>
#include "luaEpics.h"

static int l_addstates(lua_State* state)
{
	//Put the incoming table into the seq program
	lua_setfield(state, 1, "states");
	lua_pushvalue(state, 1);

	return 1;
}

static int l_runseq(lua_State* state)
{	
	const char* running_state = lua_tostring(state, 2);

	// seqprog["states"]
	lua_getfield(state, 1, "states");
	
	do
	{
		// seqprog["states"]["<start_state>"]
		lua_getfield(state, -1, running_state);
		
		// Iterate over all the transitions
		lua_len(state, -1);
		int length = lua_tonumber(state, -1);
		lua_pop(state, 1);
		
		for(int index = 1; index <= length; index += 1)
		{
			/*
			* Each transition is just a table with:
			*    ["conditional"] -> String, condition to check if true
			*    ["function"]    -> Function, code to call if the condition evaluates to true
			*    ["next_state"]  -> String, Name of the next state to run
			*
			* For each transition we'll pull out the conditional...
			*/
			
			lua_geti(state, -1, index);
			lua_getfield(state, -1, "conditional");
		
			/*
			* Turn it into a function by putting it into
			*   return (<CODE>)
			*/
			std::stringstream conditional;
		
			conditional << "return (";
			conditional << lua_tostring(state, -1);
			conditional << ")";
		
			// And evaluate it as a boolean
			luaL_dostring(state, conditional.str().c_str());
		
			int check = lua_toboolean(state, -1);
			lua_pop(state, 2);
		
			if (check)
			{
				lua_getfield(state, -1, "function");
				lua_call(state, 0, 0);
				
				lua_getfield(state, -1, "next_state");
				running_state = lua_tostring(state, -1);
				lua_pop(state, 2);
				break;
			}
			
			lua_pop(state, 1);
		}
		
		lua_pop(state, 1);
	} while(strcmp(running_state, "exit") != 0);

	lua_pop(state, 1);
	
	return 0;
}

static int l_seqprogram(lua_State* state)
{
	const char* program_name = lua_tostring(state, 1);

	static const luaL_Reg program_meta[] = {
		{"__call", l_addstates},
		{NULL, NULL}
	};

	static const luaL_Reg program_funcs[] = {
		{"run", l_runseq},
		{NULL, NULL}
	};

	luaL_newmetatable(state, "program_meta");
	luaL_setfuncs(state, program_meta, 0);
	lua_pop(state, 1);

	lua_newtable(state);
	luaL_setfuncs(state, program_funcs, 0);

	lua_pushstring(state, program_name);
	lua_setfield(state, -2, "program_name");

	luaL_setmetatable(state, "program_meta");

	return 1;
}

static int l_seqtransition(lua_State* state)
{
	lua_newtable(state);
	lua_pushvalue(state, 1);
	lua_setfield(state, -2, "conditional");

	lua_pushvalue(state, 2);
	lua_setfield(state, -2, "function");

	lua_pushvalue(state, 3);
	lua_setfield(state, -2, "next_state");

	return 1;
}

int luaopen_seq (lua_State *L)
{
	static const luaL_Reg mylib[] = {
		{"program",    l_seqprogram},
		{"transition", l_seqtransition},
		{NULL, NULL}  /* sentinel */
	};

	luaL_newlib(L, mylib);
	return 1;
}

static void libseqRegister(void)    { luaRegisterLibrary("seq", luaopen_seq); }

extern "C"
{
	epicsExportRegistrar(libseqRegister);
}
