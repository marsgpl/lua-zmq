// zmq

#include "lua_zmq.h"

LUAMOD_API int luaopen_zmq( lua_State *L ) {
    lua_newmt(L, LUA_MT_ZMQ_CONTEXT, __context_index, lua_zmq_context_gc);
    lua_newmt(L, LUA_MT_ZMQ_SOCKET, __socket_index, lua_zmq_socket_gc);

    luaL_newlib(L, __index);

    luaL_newlib(L, __z85_index);
    lua_setfield(L, -2, "z85");

    lua_newtable(L);
        #include "lua_zmq_flags.c"
    lua_setfield(L, -2, "f");

    return 1;
}

// context

static int lua_zmq__get_ctx_mf( lua_State *L ) {
    lua_pushstring(L, LUA_ZMQ_CONTEXT_METAFIELD);
    return 1;
}

static int lua_zmq_context( lua_State *L ) {
    //luaL_checktype(L, 1, LUA_TTABLE);
    if ( !lua_istable(L, 1) ) {
        lua_settop(L, 0);
        lua_newtable(L);
    }

    lua_ud_zmq_context *ctx = (lua_ud_zmq_context *)lua_newuserdata(L, sizeof(lua_ud_zmq_context));

    if ( !ctx ) {
        lua_fail(L, "lua_ud_zmq_context alloc failed", 0);
    }

    //
    lua_getfield(L, LUA_REGISTRYINDEX, LUA_ZMQ_CONTEXT_METAFIELD);

    if ( lua_isnil(L, -1) ) { // create new
        ctx->context = zmq_ctx_new();
        if ( !ctx->context ) {
            lua_zmq_fail(L, "zmq_ctx_new");
        } else {
            lua_pop(L, 1);
            lua_pushlightuserdata(L, ctx->context);
            lua_setfield(L, LUA_REGISTRYINDEX, LUA_ZMQ_CONTEXT_METAFIELD);
        }
    } else { // use existing
        ctx->context = lua_touserdata(L, -1);
        lua_pop(L, 1);
    }
    //

    luaL_setmetatable(L, LUA_MT_ZMQ_CONTEXT);

    // set opts from arg#1 table
    lua_pushnil(L);
    while ( lua_next(L, 1) != 0 ) {
        zmq_ctx_set(ctx->context, (int)lua_tointeger(L, -2), (int)lua_tointeger(L, -1));
        lua_pop(L, 1);
    }

    return 1;
}

static int lua_zmq_context_debug( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);

    if ( !ctx->context ) {
        lua_fail(L, "zmq_context is dead", 0);
    }

    lua_pushfstring(L, "%p", ctx->context);
    return 1;
}

static int lua_zmq_context_get( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);
    int option = luaL_checknumber(L, 2);

    if ( !ctx->context ) {
        lua_fail(L, "zmq_context is dead", 0);
    }

    lua_pushinteger(L, zmq_ctx_get(ctx->context, option));
    return 1;
}

static int lua_zmq_context_set( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);
    int option = luaL_checknumber(L, 2);
    int optval = luaL_optnumber(L, 3, 0);

    if ( !ctx->context ) {
        lua_fail(L, "zmq_context is dead", 0);
    }

    int r = zmq_ctx_set(ctx->context, option, optval);

    if ( r != 0 ) {
        lua_zmq_fail(L, "zmq_ctx_set");
    } else {
        lua_pushinteger(L, zmq_ctx_get(ctx->context, option));
        return 1;
    }
}

static int lua_zmq_context_gc( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);

    if ( ctx->context ) {
        zmq_ctx_term(ctx->context);
        ctx->context = NULL;
    }

    return 0;
}

static int lua_zmq_context_shutdown( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);

    if ( ctx->context ) {
        zmq_ctx_shutdown(ctx->context);
    }

    return 0;
}

// socket

static int lua_zmq_socket( lua_State *L ) {
    lua_ud_zmq_context *ctx = luaL_checkudata(L, 1, LUA_MT_ZMQ_CONTEXT);
    int socket_type = luaL_checknumber(L, 2);

    if ( !ctx->context ) {
        lua_fail(L, "zmq_context is dead", 0);
    }

    lua_ud_zmq_socket *sock = (lua_ud_zmq_socket *)lua_newuserdata(L, sizeof(lua_ud_zmq_socket));

    if ( !sock ) {
        lua_fail(L, "lua_ud_zmq_socket alloc failed", 0);
    }

    sock->socket = zmq_socket(ctx->context, socket_type);

    if ( !sock->socket ) {
        lua_zmq_fail(L, "zmq_socket");
    }

    luaL_setmetatable(L, LUA_MT_ZMQ_SOCKET);

    return 1;
}

static int lua_zmq_socket_bind( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    if ( zmq_bind(sock->socket, addr) == -1 ) {
        lua_zmq_fail(L, "zmq_bind");
    } else {
        lua_settop(L, 1);
        return 1;
    }
}

static int lua_zmq_socket_unbind( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    if ( zmq_unbind(sock->socket, addr) == -1 ) {
        lua_zmq_fail(L, "zmq_unbind");
    } else {
        lua_settop(L, 1);
        return 1;
    }
}

static int lua_zmq_socket_connect( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    if ( zmq_connect(sock->socket, addr) == -1 ) {
        lua_zmq_fail(L, "zmq_connect");
    } else {
        lua_settop(L, 1);
        return 1;
    }
}

static int lua_zmq_socket_disconnect( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    if ( zmq_disconnect(sock->socket, addr) == -1 ) {
        lua_zmq_fail(L, "zmq_disconnect");
    } else {
        lua_settop(L, 1);
        return 1;
    }
}

static int lua_zmq_socket_gc( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);

    if ( sock->socket ) {
        zmq_close(sock->socket);
        sock->socket = NULL;
    }

    return 0;
}

static int lua_zmq_socket_get( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    int option = luaL_checknumber(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    size_t vl;
    void *v;
    int r;

    switch ( option ) {
        case ZMQ_MAXMSGSIZE:
            vl = sizeof(int64_t);
            v = (int64_t *)malloc(vl);
            r = zmq_getsockopt(sock->socket, option, v, &vl);

            if ( r == 0 ) {
                lua_pushnumber(L, *(int64_t *)v);
                free(v);
                return 1;
            }
            break;
        case ZMQ_AFFINITY:
            vl = sizeof(uint64_t);
            v = (uint64_t *)malloc(vl);
            r = zmq_getsockopt(sock->socket, option, v, &vl);

            if ( r == 0 ) {
                lua_pushnumber(L, *(uint64_t *)v);
                free(v);
                return 1;
            }
            break;
        case ZMQ_CURVE_SERVER:
        case ZMQ_BACKLOG:
        case ZMQ_EVENTS:
        case ZMQ_FD:
        case ZMQ_GSSAPI_PLAINTEXT:
        case ZMQ_GSSAPI_SERVER:
        case ZMQ_HANDSHAKE_IVL:
        case ZMQ_IMMEDIATE:
        case ZMQ_IPV6:
        case ZMQ_LINGER:
        case ZMQ_MECHANISM:
        case ZMQ_MULTICAST_HOPS:
        case ZMQ_PLAIN_SERVER:
        case ZMQ_RATE:
        case ZMQ_RCVBUF:
        case ZMQ_RCVHWM:
        case ZMQ_RCVMORE:
        case ZMQ_RCVTIMEO:
        case ZMQ_RECONNECT_IVL:
        case ZMQ_RECONNECT_IVL_MAX:
        case ZMQ_RECOVERY_IVL:
        case ZMQ_SNDBUF:
        case ZMQ_SNDHWM:
        case ZMQ_SNDTIMEO:
        case ZMQ_TCP_KEEPALIVE:
        case ZMQ_TCP_KEEPALIVE_CNT:
        case ZMQ_TCP_KEEPALIVE_IDLE:
        case ZMQ_TCP_KEEPALIVE_INTVL:
        case ZMQ_TOS:
        case ZMQ_TYPE:
            vl = sizeof(int);
            v = (int *)malloc(vl);
            r = zmq_getsockopt(sock->socket, option, v, &vl);

            if ( r == 0 ) {
                lua_pushinteger(L, *(int *)v);
                free(v);
                return 1;
            }
            break;
        case ZMQ_CURVE_PUBLICKEY:
        case ZMQ_CURVE_SECRETKEY:
        case ZMQ_CURVE_SERVERKEY:
            vl = LUA_ZMQ_CURVE_KEY_MAXLENGTH;
            v = (char *)malloc(sizeof(char) * vl);
            r = zmq_getsockopt(sock->socket, option, v, &vl);

            if ( r == 0 ) {
                if ( vl && ((char *)v)[vl-1] == '\0' ) {
                    vl--; // null-byte terminator is redundant here
                }
                lua_pushlstring(L, (char *)v, vl);
                free(v);
                return 1;
            }
            break;
        case ZMQ_GSSAPI_PRINCIPAL:
        case ZMQ_GSSAPI_SERVICE_PRINCIPAL:
        case ZMQ_IDENTITY:
        case ZMQ_LAST_ENDPOINT:
        case ZMQ_PLAIN_PASSWORD:
        case ZMQ_PLAIN_USERNAME:
        case ZMQ_ZAP_DOMAIN:
            vl = 256;
            v = (char *)malloc(sizeof(char) * vl);
            r = zmq_getsockopt(sock->socket, option, v, &vl);

            if ( r == 0 ) {
                if ( vl && ((char *)v)[vl-1] == '\0' ) {
                    vl--; // null-byte terminator is redundant here
                }
                lua_pushlstring(L, (char *)v, vl);
                free(v);
                return 1;
            }
            break;
        default:
            lua_fail(L, "zmq_getsockopt: unknown option", 0);
    }

    free(v);

    lua_zmq_fail(L, "zmq_getsockopt");
}

static int lua_zmq_socket_set( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);
    int option = luaL_checknumber(L, 2);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    size_t vl;
    int v_int;
    int64_t v_int64_t;
    uint64_t v_uint64_t;
    const char *v_str;
    int r;

    switch ( option ) {
        case ZMQ_BACKLOG:
        case ZMQ_CONFLATE:
        case ZMQ_CURVE_SERVER:
        case ZMQ_GSSAPI_PLAINTEXT:
        case ZMQ_GSSAPI_SERVER:
        case ZMQ_HANDSHAKE_IVL:
        case ZMQ_IMMEDIATE:
        case ZMQ_IPV6:
        case ZMQ_LINGER:
        case ZMQ_MULTICAST_HOPS:
        case ZMQ_PLAIN_SERVER:
        case ZMQ_PROBE_ROUTER:
        case ZMQ_RATE:
        case ZMQ_RCVBUF:
        case ZMQ_RCVHWM:
        case ZMQ_RCVTIMEO:
        case ZMQ_RECONNECT_IVL:
        case ZMQ_RECONNECT_IVL_MAX:
        case ZMQ_RECOVERY_IVL:
        case ZMQ_REQ_CORRELATE:
        case ZMQ_REQ_RELAXED:
        case ZMQ_ROUTER_HANDOVER:
        case ZMQ_ROUTER_MANDATORY:
        case ZMQ_SNDBUF:
        case ZMQ_SNDHWM:
        case ZMQ_SNDTIMEO:
        case ZMQ_TCP_KEEPALIVE:
        case ZMQ_TCP_KEEPALIVE_CNT:
        case ZMQ_TCP_KEEPALIVE_IDLE:
        case ZMQ_TCP_KEEPALIVE_INTVL:
        case ZMQ_TOS:
        case ZMQ_XPUB_VERBOSE:
            vl = sizeof(int);
            v_int = lua_tointeger(L, 3);
            r = zmq_setsockopt(sock->socket, option, &v_int, vl);
            break;
        case ZMQ_MAXMSGSIZE:
            vl = sizeof(int64_t);
            v_int64_t = lua_tonumber(L, 3);
            r = zmq_setsockopt(sock->socket, option, &v_int64_t, vl);
            break;
        case ZMQ_AFFINITY:
            vl = sizeof(uint64_t);
            v_uint64_t = lua_tonumber(L, 3);
            r = zmq_setsockopt(sock->socket, option, &v_uint64_t, vl);
            break;
        case ZMQ_CONNECT_RID:
            v_str = luaL_checklstring(L, 3, &vl);
            if ( vl && v_str[0] == '\0' ) {
                lua_fail(L, "zmq_setsockopt failed: ZMQ_CONNECT_RID string must not start with binary zero", 0);
            }
            r = zmq_setsockopt(sock->socket, option, v_str, vl);
            break;
        case ZMQ_CURVE_PUBLICKEY:
        case ZMQ_CURVE_SECRETKEY:
        case ZMQ_CURVE_SERVERKEY:
        case ZMQ_GSSAPI_PRINCIPAL:
        case ZMQ_GSSAPI_SERVICE_PRINCIPAL:
        case ZMQ_PLAIN_PASSWORD:
        case ZMQ_PLAIN_USERNAME:
        case ZMQ_ZAP_DOMAIN:
            v_str = luaL_checkstring(L, 3);
            vl = strlen(v_str) + 1;
            r = zmq_setsockopt(sock->socket, option, v_str, vl);
            break;
        case ZMQ_IDENTITY:
        case ZMQ_SUBSCRIBE:
        case ZMQ_UNSUBSCRIBE:
            v_str = luaL_checklstring(L, 3, &vl);
            r = zmq_setsockopt(sock->socket, option, v_str, vl);
            break;
        default:
            lua_fail(L, "zmq_setsockopt: unknown option", 0);
    }

    if ( r == 0 ) {
        lua_settop(L, 1);
        return 1;
    } else {
        lua_zmq_fail(L, "zmq_setsockopt");
    }
}

static int lua_zmq_socket_send( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    size_t len;
    const char *buf = luaL_checklstring(L, 2, &len);

    int dontwait = luaL_optinteger(L, 3, 0) ? ZMQ_DONTWAIT : 0;
    int sndmore = luaL_optinteger(L, 4, 0) ? ZMQ_SNDMORE : 0;

    int r = zmq_send(sock->socket, buf, len, dontwait | sndmore);

    if ( r < 0 ) {
        lua_zmq_fail(L, "zmq_send");
    } else {
        lua_settop(L, 1);
        return 1;
    }
}

static int lua_zmq_socket_recv( lua_State *L ) {
    lua_ud_zmq_socket *sock = luaL_checkudata(L, 1, LUA_MT_ZMQ_SOCKET);

    if ( !sock->socket ) {
        lua_fail(L, "zmq_socket is dead", 0);
    }

    int dontwait = luaL_optinteger(L, 2, 0) ? ZMQ_DONTWAIT : 0;

    char buf[LUA_ZMQ_RECV_BUFF_SIZE];

    int r = zmq_recv(sock->socket, buf, LUA_ZMQ_RECV_BUFF_SIZE, dontwait);

    if ( r < 0 ) {
        lua_zmq_fail(L, "zmq_recv");
    } else {
        lua_pushlstring(L, buf, min(LUA_ZMQ_RECV_BUFF_SIZE, r)); // data (truncated to buf len)
        lua_pushnumber(L, r); // real len received
        return 2;
    }
}

// z85

static int lua_zmq_z85_encode( lua_State *L ) {
    size_t len;
    const uint8_t *str = (uint8_t *)luaL_checklstring(L, 1, &len);

    if ( !len || len%4 ) {
        lua_fail(L, "arg #1 is empty or its length is not dividable by 4", 0);
    }

    size_t dest_len = (size_t)(ceil(len * 1.25) + 1);
    char dest[dest_len];
    memset(dest, 0, dest_len);

    if ( zmq_z85_encode(dest, str, len) ) {
        lua_pushstring(L, dest);
        return 1;
    } else {
        lua_zmq_fail(L, "zmq_z85_encode");
    }
}

static int lua_zmq_z85_decode( lua_State *L ) {
    const char *str = luaL_checkstring(L, 1);
    size_t len = strlen(str);

    if ( !len || len%5 ) {
        lua_fail(L, "arg #1 is empty or its length is not dividable by 5", 0);
    }

    size_t dest_len = (size_t)(ceil(len * 0.8) + 1);
    uint8_t dest[dest_len];

    if ( zmq_z85_decode(dest, str) ) {
        lua_pushlstring(L, (char *)dest, dest_len - 1);
        return 1;
    } else {
        lua_zmq_fail(L, "zmq_z85_decode");
    }
}

static int lua_zmq_z85_keypair( lua_State *L ) {
    char public_key[LUA_ZMQ_CURVE_KEY_MAXLENGTH];
    char secret_key[LUA_ZMQ_CURVE_KEY_MAXLENGTH];

    int r = zmq_curve_keypair(public_key, secret_key);

    if ( r == 0 ) {
        lua_newtable(L);

        lua_pushstring(L, public_key);
        lua_setfield(L, -2, "public");

        lua_pushstring(L, secret_key);
        lua_setfield(L, -2, "secret");

        return 1;
    } else {
        lua_zmq_fail(L, "zmq_curve_keypair");
    }
}

// misc

// old style:
    //int seconds = luaL_checkinteger(L, 1);
    //zmq_sleep(seconds);
    //return 0;
static int lua_zmq_sleep( lua_State *L ) {
    double input = luaL_optnumber(L, 1, 0);
    struct timespec t;
    t.tv_sec  = (long)input;
    t.tv_nsec = (long)((input - (long)input) * 1e9);
    if ( nanosleep(&t, NULL) == -1 ) lua_errno(L);
    return 1;
}

// CLOCK_REALTIME
// CLOCK_REALTIME_COARSE
// CLOCK_MONOTONIC
// CLOCK_MONOTONIC_COARSE
// CLOCK_MONOTONIC_RAW
// CLOCK_BOOTTIME
// CLOCK_PROCESS_CPUTIME_ID
// CLOCK_THREAD_CPUTIME_ID
// old style:
    // #include <sys/time.h>
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // lua_pushnumber(L, tv.tv_sec + tv.tv_usec * 1e-6);
static int lua_zmq_microtime( lua_State *L ) {
    struct timespec t;
    if ( clock_gettime(CLOCK_REALTIME, &t) == -1 ) lua_errno(L);
    lua_pushnumber(L, t.tv_sec + t.tv_nsec * 1e-9);
    return 1;
}

/*
ZMQ_EXPORT int zmq_send_const (void *s, const void *buf, size_t len, int flags);
ZMQ_EXPORT int zmq_socket_monitor (void *s, const char *addr, int events);

ZMQ_EXPORT int zmq_poll (zmq_pollitem_t *items, int nitems, long timeout);

ZMQ_EXPORT int zmq_proxy (void *frontend, void *backend, void *capture);
ZMQ_EXPORT int zmq_proxy_steerable (void *frontend, void *backend, void *capture, void *control);

ZMQ_EXPORT int zmq_has (const char *capability);

ZMQ_EXPORT void *zmq_stopwatch_start (void);
ZMQ_EXPORT unsigned long zmq_stopwatch_stop (void *watch_);

ZMQ_EXPORT void *zmq_threadstart (zmq_thread_fn* func, void* arg);
ZMQ_EXPORT void zmq_threadclose (void* thread);
*/
