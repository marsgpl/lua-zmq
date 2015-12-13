--

local zmq = require "zmq"
local trace = require "trace"

--

local server_key = {
    public = "]NV${/dw[PE^Web:u(G00><dj]Xw!A{RhDhc[iNA",
    secret = "c!e])3SOqavTUO^00vv>d#B5x8LI:Dc?+/:M2z$s",
}

local client_key = {
    public = "y*%Evc(jvzJn+jlNQRd?F.OxIczs]d0pWD)X!jBl",
    secret = "stgZrM^-MU0sS)qGTRG]x8H?Jjl=lX+1yAz)$e7Z",
}

--

local ctx = assert(zmq.context())

local sock = assert(ctx:socket(zmq.f.ZMQ_REQ))

assert(sock:set(zmq.f.ZMQ_CURVE_SERVERKEY, server_key.public))
assert(sock:set(zmq.f.ZMQ_CURVE_PUBLICKEY, client_key.public))
assert(sock:set(zmq.f.ZMQ_CURVE_SECRETKEY, client_key.secret))

assert(sock:connect("tcp://127.0.0.1:45002"))

--

assert(sock:send("wanna lick my rabbit?", 0))
trace(sock:recv(0))
