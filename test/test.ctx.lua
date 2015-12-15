--

local zmq = require "zmq"
local trace = require "trace"

--

for i=1,10 do
    trace(zmq.context():debug())
end
