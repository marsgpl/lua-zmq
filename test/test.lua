--

local zmq = require "zmq"
local trace = require "trace"

--

math.randomseed(os.time())

--

--trace(zmq.z85.decode("W(HZGcmk}ehUnUwTMJz2=HRyQ]wQww-do=7stEk)"))

---[[
local ctx = zmq.context()

local sock = assert(ctx:socket(zmq.f.ZMQ_PUSH))

assert(sock:bind("tcp://127.0.0.1:12345"))

assert(sock:set(zmq.f.ZMQ_BACKLOG, 1234))
assert(sock:set(zmq.f.ZMQ_MAXMSGSIZE, 1234))

--trace(sock, getmetatable(sock))

local fields = {
    --"ZMQ_AFFINITY",
    "ZMQ_BACKLOG",
    --"ZMQ_CURVE_PUBLICKEY",
    --"ZMQ_CURVE_SECRETKEY",
    --"ZMQ_CURVE_SERVERKEY",
    --"ZMQ_EVENTS",
    --"ZMQ_FD",
    --"ZMQ_GSSAPI_PLAINTEXT",
    --"ZMQ_GSSAPI_PRINCIPAL",
    --"ZMQ_GSSAPI_SERVER",
    --"ZMQ_GSSAPI_SERVICE_PRINCIPAL",
    --"ZMQ_HANDSHAKE_IVL",
    --"ZMQ_IDENTITY",
    --"ZMQ_IMMEDIATE",
    --"ZMQ_IPV6",
    "ZMQ_LAST_ENDPOINT",
    --"ZMQ_LINGER",
    "ZMQ_MAXMSGSIZE",
    --"ZMQ_MECHANISM",
    --"ZMQ_MULTICAST_HOPS",
    --"ZMQ_PLAIN_PASSWORD",
    --"ZMQ_PLAIN_SERVER",
    --"ZMQ_PLAIN_USERNAME",
    --"ZMQ_RATE",
    --"ZMQ_RCVBUF",
    --"ZMQ_RCVHWM",
    --"ZMQ_RCVMORE",
    --"ZMQ_RCVTIMEO",
    --"ZMQ_RECONNECT_IVL",
    --"ZMQ_RECONNECT_IVL_MAX",
    --"ZMQ_RECOVERY_IVL",
    --"ZMQ_SNDBUF",
    --"ZMQ_SNDHWM",
    --"ZMQ_SNDTIMEO",
    --"ZMQ_TCP_KEEPALIVE",
    --"ZMQ_TCP_KEEPALIVE_CNT",
    --"ZMQ_TCP_KEEPALIVE_IDLE",
    --"ZMQ_TCP_KEEPALIVE_INTVL",
    --"ZMQ_TOS",
    --"ZMQ_TYPE",
    --"ZMQ_ZAP_DOMAIN",
}

for _,v in ipairs(fields) do
    local r = assert(sock:get(zmq.f[v]))
    print("\27[0;34m"..v..":\27[0m\t"..tostring(r))
end
--]]

--[[
local ctx = zmq.context {
    [zmq.f.ZMQ_IO_THREADS] = 2,
    [zmq.f.ZMQ_MAX_SOCKETS] = 1000000,
}

print("ZMQ_IO_THREADS: ", ctx:get(zmq.f.ZMQ_IO_THREADS))
print("ZMQ_MAX_SOCKETS: ", ctx:get(zmq.f.ZMQ_MAX_SOCKETS))
--]]

--[[
for i=1,1000000 do
    zmq.context()
    collectgarbage()
    if (i%1000) == 0 then
        os.execute("sleep 0.01")
    end
    io.write("\rstep: " .. i .. "    ")
end

print("\nFIN")
--]]

--[[
local ctx = assert(zmq.context())
local s = assert(ctx:socket(zmq.flag.ZMQ_REP))
assert(s:bind("tcp://*:12345"))
os.execute("sleep 10")
--]]

--[[

local zmq = require "zmq"
local trace = require "trace"

--

math.randomseed(os.time())

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

local ctx = assert(zmq.context {
    [zmq.f.ZMQ_IO_THREADS] = 1,
    [zmq.f.ZMQ_MAX_SOCKETS] = 100000,
})

--print("ZMQ_IO_THREADS:", ctx:get(zmq.f.ZMQ_IO_THREADS))
--print("ZMQ_MAX_SOCKETS:", ctx:get(zmq.f.ZMQ_MAX_SOCKETS))

local sock = assert(ctx:socket(zmq.f.ZMQ_PULL))

assert(sock:set(zmq.f.ZMQ_CURVE_SERVERKEY, server_key.public))
assert(sock:set(zmq.f.ZMQ_CURVE_PUBLICKEY, client_key.public))
assert(sock:set(zmq.f.ZMQ_CURVE_SECRETKEY, client_key.secret))

assert(sock:connect("tcp://127.0.0.1:45001"))

while 1 do
    --print("trying to read ..")
    assert(sock:send("HELLO", 0))
    print(sock:recv(0))
    --assert(os.execute("sleep 1"))
end
]]
