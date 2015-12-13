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

local sock = assert(ctx:socket(zmq.f.ZMQ_PUSH))

assert(sock:set(zmq.f.ZMQ_CURVE_SERVER, 1))
assert(sock:set(zmq.f.ZMQ_CURVE_SECRETKEY, server_key.secret))

assert(sock:bind("tcp://127.0.0.1:45001"))

--

math.randomseed(os.time())

local random_string = function( len )
    local st = {}

    for i=1,len do
        st[i] = string.char(math.random(32, 126))
    end

    return table.concat(st)
end

--

while true do
    local str = random_string(64)
    print("sending str:", str)
    assert(sock:send(str, 0))
    assert(os.execute("sleep 1"))
end
