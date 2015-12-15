--

local zmq = require "zmq"
local trace = require "trace"

--

local ts1 = zmq.microtime()

zmq.sleep(1.23456)

local ts2 = zmq.microtime()

print("time spent:", ts2-ts1)
