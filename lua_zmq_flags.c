//

// context options:
lua_add_int_const(L, ZMQ_IO_THREADS);
lua_add_int_const(L, ZMQ_MAX_SOCKETS);
lua_add_int_const(L, ZMQ_SOCKET_LIMIT);
lua_add_int_const(L, ZMQ_THREAD_PRIORITY);
lua_add_int_const(L, ZMQ_THREAD_SCHED_POLICY);

// socket types:
lua_add_int_const(L, ZMQ_PAIR);
lua_add_int_const(L, ZMQ_PUB);
lua_add_int_const(L, ZMQ_SUB);
lua_add_int_const(L, ZMQ_REQ);
lua_add_int_const(L, ZMQ_REP);
lua_add_int_const(L, ZMQ_DEALER);
lua_add_int_const(L, ZMQ_ROUTER);
lua_add_int_const(L, ZMQ_PULL);
lua_add_int_const(L, ZMQ_PUSH);
lua_add_int_const(L, ZMQ_XPUB);
lua_add_int_const(L, ZMQ_XSUB);
lua_add_int_const(L, ZMQ_STREAM);

// socket options:
lua_add_int_const(L, ZMQ_AFFINITY);
lua_add_int_const(L, ZMQ_IDENTITY);
lua_add_int_const(L, ZMQ_SUBSCRIBE);
lua_add_int_const(L, ZMQ_UNSUBSCRIBE);
lua_add_int_const(L, ZMQ_RATE);
lua_add_int_const(L, ZMQ_RECOVERY_IVL);
lua_add_int_const(L, ZMQ_SNDBUF);
lua_add_int_const(L, ZMQ_RCVBUF);
lua_add_int_const(L, ZMQ_RCVMORE);
lua_add_int_const(L, ZMQ_FD);
lua_add_int_const(L, ZMQ_EVENTS);
lua_add_int_const(L, ZMQ_TYPE);
lua_add_int_const(L, ZMQ_LINGER);
lua_add_int_const(L, ZMQ_RECONNECT_IVL);
lua_add_int_const(L, ZMQ_BACKLOG);
lua_add_int_const(L, ZMQ_RECONNECT_IVL_MAX);
lua_add_int_const(L, ZMQ_MAXMSGSIZE);
lua_add_int_const(L, ZMQ_SNDHWM);
lua_add_int_const(L, ZMQ_RCVHWM);
lua_add_int_const(L, ZMQ_MULTICAST_HOPS);
lua_add_int_const(L, ZMQ_RCVTIMEO);
lua_add_int_const(L, ZMQ_SNDTIMEO);
lua_add_int_const(L, ZMQ_LAST_ENDPOINT);
lua_add_int_const(L, ZMQ_ROUTER_MANDATORY);
lua_add_int_const(L, ZMQ_TCP_KEEPALIVE);
lua_add_int_const(L, ZMQ_TCP_KEEPALIVE_CNT);
lua_add_int_const(L, ZMQ_TCP_KEEPALIVE_IDLE);
lua_add_int_const(L, ZMQ_TCP_KEEPALIVE_INTVL);
lua_add_int_const(L, ZMQ_IMMEDIATE);
lua_add_int_const(L, ZMQ_XPUB_VERBOSE);
lua_add_int_const(L, ZMQ_ROUTER_RAW);
lua_add_int_const(L, ZMQ_IPV6);
lua_add_int_const(L, ZMQ_MECHANISM);
lua_add_int_const(L, ZMQ_PLAIN_SERVER);
lua_add_int_const(L, ZMQ_PLAIN_USERNAME);
lua_add_int_const(L, ZMQ_PLAIN_PASSWORD);
lua_add_int_const(L, ZMQ_CURVE_SERVER);
lua_add_int_const(L, ZMQ_CURVE_PUBLICKEY);
lua_add_int_const(L, ZMQ_CURVE_SECRETKEY);
lua_add_int_const(L, ZMQ_CURVE_SERVERKEY);
lua_add_int_const(L, ZMQ_PROBE_ROUTER);
lua_add_int_const(L, ZMQ_REQ_CORRELATE);
lua_add_int_const(L, ZMQ_REQ_RELAXED);
lua_add_int_const(L, ZMQ_CONFLATE);
lua_add_int_const(L, ZMQ_ZAP_DOMAIN);
lua_add_int_const(L, ZMQ_ROUTER_HANDOVER);
lua_add_int_const(L, ZMQ_TOS);
lua_add_int_const(L, ZMQ_CONNECT_RID);
lua_add_int_const(L, ZMQ_GSSAPI_SERVER);
lua_add_int_const(L, ZMQ_GSSAPI_PRINCIPAL);
lua_add_int_const(L, ZMQ_GSSAPI_SERVICE_PRINCIPAL);
lua_add_int_const(L, ZMQ_GSSAPI_PLAINTEXT);
lua_add_int_const(L, ZMQ_HANDSHAKE_IVL);
lua_add_int_const(L, ZMQ_SOCKS_PROXY);
lua_add_int_const(L, ZMQ_XPUB_NODROP);

// message options:
lua_add_int_const(L, ZMQ_MORE);
lua_add_int_const(L, ZMQ_SRCFD);
lua_add_int_const(L, ZMQ_SHARED);

// send/recv options:
lua_add_int_const(L, ZMQ_DONTWAIT);
lua_add_int_const(L, ZMQ_SNDMORE);

// security mechanisms:
lua_add_int_const(L, ZMQ_NULL);
lua_add_int_const(L, ZMQ_PLAIN);
lua_add_int_const(L, ZMQ_CURVE);
lua_add_int_const(L, ZMQ_GSSAPI);

// socket transport events (TCP and IPC only):
lua_add_int_const(L, ZMQ_EVENT_CONNECTED);
lua_add_int_const(L, ZMQ_EVENT_CONNECT_DELAYED);
lua_add_int_const(L, ZMQ_EVENT_CONNECT_RETRIED);
lua_add_int_const(L, ZMQ_EVENT_LISTENING);
lua_add_int_const(L, ZMQ_EVENT_BIND_FAILED);
lua_add_int_const(L, ZMQ_EVENT_ACCEPTED);
lua_add_int_const(L, ZMQ_EVENT_ACCEPT_FAILED);
lua_add_int_const(L, ZMQ_EVENT_CLOSED);
lua_add_int_const(L, ZMQ_EVENT_CLOSE_FAILED);
lua_add_int_const(L, ZMQ_EVENT_DISCONNECTED);
lua_add_int_const(L, ZMQ_EVENT_MONITOR_STOPPED);
lua_add_int_const(L, ZMQ_EVENT_ALL);

// I/O multiplexing:
lua_add_int_const(L, ZMQ_POLLIN);
lua_add_int_const(L, ZMQ_POLLOUT);
lua_add_int_const(L, ZMQ_POLLERR);
