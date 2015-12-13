--

local zmq = require "zmq"
local trace = require "trace"

--

local ctx = assert(zmq.context())

local sock = assert(ctx:socket(zmq.f.ZMQ_STREAM))

assert(sock:bind("tcp://127.0.0.1:45003"))

--

local client_id, msg

while true do
    client_id = assert(sock:recv(0))
    msg = assert(sock:recv(0))
    trace(client_id, msg)
    assert(sock:send(client_id, 0, 1))
    assert(sock:send("lol wtf :D\r\n", 0))
end
