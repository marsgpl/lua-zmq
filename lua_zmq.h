// zmq

#include <stdlib.h>
#include <zmq.h>
#include <math.h>

#include "lua_pd.h"

//

#define LUA_MT_ZMQ_CONTEXT "zmq.context"
#define LUA_MT_ZMQ_SOCKET "zmq.socket"

#define LUA_ZMQ_CURVE_KEY_MAXLENGTH 41
#define LUA_ZMQ_RECV_BUFF_SIZE 4096

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define lua_zmq_fail( L, function_name ) { \
    lua_pushnil(L); \
    lua_pushfstring(L, "%s failed: %s", function_name, zmq_strerror(zmq_errno())); \
    lua_pushinteger(L, zmq_errno()); \
    return 3; \
}

//

typedef struct lua_ud_zmq_context {
    void *context;
} lua_ud_zmq_context;

typedef struct lua_ud_zmq_socket {
    void *socket;
} lua_ud_zmq_socket;

//

LUAMOD_API int luaopen_zmq( lua_State *L );

static int lua_zmq_context( lua_State *L );
static int lua_zmq_context_get( lua_State *L );
static int lua_zmq_context_set( lua_State *L );
static int lua_zmq_context_gc( lua_State *L );
static int lua_zmq_context_shutdown( lua_State *L );

static int lua_zmq_socket( lua_State *L );
static int lua_zmq_socket_bind( lua_State *L );
static int lua_zmq_socket_unbind( lua_State *L );
static int lua_zmq_socket_connect( lua_State *L );
static int lua_zmq_socket_disconnect( lua_State *L );
static int lua_zmq_socket_gc( lua_State *L );
static int lua_zmq_socket_get( lua_State *L );
static int lua_zmq_socket_set( lua_State *L );
static int lua_zmq_socket_send( lua_State *L );
static int lua_zmq_socket_recv( lua_State *L );

static int lua_zmq_z85_encode( lua_State *L );
static int lua_zmq_z85_decode( lua_State *L );
static int lua_zmq_z85_keypair( lua_State *L );

//

static const luaL_Reg __index[] = {
    {"context", lua_zmq_context},
    {NULL, NULL}
};

static const luaL_Reg __context_index[] = {
    {"get", lua_zmq_context_get},
    {"set", lua_zmq_context_set},
    {"socket", lua_zmq_socket},
    {"term", lua_zmq_context_gc},
    {"shutdown", lua_zmq_context_shutdown},
    {NULL, NULL}
};

static const luaL_Reg __socket_index[] = {
    {"bind", lua_zmq_socket_bind},
    {"unbind", lua_zmq_socket_unbind},
    {"connect", lua_zmq_socket_connect},
    {"disconnect", lua_zmq_socket_disconnect},
    {"close", lua_zmq_socket_gc},
    {"get", lua_zmq_socket_get},
    {"set", lua_zmq_socket_set},
    {"send", lua_zmq_socket_send},
    {"recv", lua_zmq_socket_recv},
    {NULL, NULL}
};

static const luaL_Reg __z85_index[] = {
    {"encode", lua_zmq_z85_encode},
    {"decode", lua_zmq_z85_decode},
    {"keypair", lua_zmq_z85_keypair},
    {NULL, NULL}
};
