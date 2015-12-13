//

#ifndef lua_pd_h
#define lua_pd_h

#include <string.h>

#include "lauxlib.h"

#define lua_fail( L, errstr, errno ) { \
    lua_pushnil(L); \
    lua_pushstring(L, errstr); \
    lua_pushinteger(L, errno); \
    return 3; \
}

#define lua_errno( L ) { \
    lua_fail(L, strerror(errno), errno); \
}

#define lua_newmt( L, name, __index, __gc ) { \
    luaL_newmetatable(L, name); \
    \
    luaL_newlib(L, __index); \
    lua_setfield(L, -2, "__index"); \
    \
    lua_pushcfunction(L, __gc); \
    lua_setfield(L, -2, "__gc"); \
    \
    lua_pop(L, 1); \
}

#define lua_add_int_const( L, name ) { \
    lua_pushinteger(L, name); \
    lua_setfield(L, -2, #name); \
}

#define lua_trace_stack( L ) { \
    int n = lua_gettop(L); \
    printf("stack size: %d\n", n); \
    int t; \
    for ( int i=1; i<=n; ++i ) { \
        t = lua_type(L,i); \
        printf("\t%d: %s: ", i, lua_typename(L,t)); \
        if ( t==LUA_TNIL||t==LUA_TNONE ) printf("nil"); \
        else if ( t==LUA_TNUMBER ) printf(LUA_NUMBER_FMT, lua_tonumber(L,i)); \
        else if ( t==LUA_TSTRING ) printf("%s", lua_tostring(L,i)); \
        else if ( t==LUA_TBOOLEAN ) printf(lua_toboolean(L,i) ? "true" : "false"); \
        else printf("%p", lua_topointer(L,i)); \
        printf("\n"); \
    } \
}

#endif
