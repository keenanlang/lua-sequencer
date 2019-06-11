#include <stdio.h>
#include <string>
#include <sstream>
#include <epicsExport.h>
#include "luaEpics.h"

static int l_addstates(lua_State* state)
{
	lua_pushnil(state);
	while (lua_next(state, 2) != 0)
	{
		const char* key = lua_tostring(state, -2);
		lua_pushvalue(state, -1);
		lua_setfield(state, 1, key);
		lua_pop(state, 1);
	}

	return 1;
}

static int l_runseq(lua_State* state)
{
	const char* start = lua_tostring(state, 2);

	lua_getfield(state, 1, start);
	lua_len(state, -1);
	int length = lua_tointeger(state, -1);
	lua_pop(state, 2);

	printf("Number of states: %d\n", length);

	/*
	lua_getfield(state, -1, "conditional");

	std::stringstream conditional;

	conditional << "return (";
	conditional << lua_tostring(state, -1);
	conditional << ")";

	lua_pop(state, 1);

	luaL_dostring(state, conditional.str().c_str());

	int check = lua_toboolean(state, -1);
	lua_pop(state, 1);

	if (check)
	{

	}



	lua_getfield(state, -1, "function");

	lua_CFunction tocall = lua_tocfunction(state, -1);
	*/

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

	luaL_newlibtable(state, program_funcs);

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
