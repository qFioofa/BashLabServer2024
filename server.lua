local socket = require("socket")

local server = assert(socket.bind("*", 12345))
print("Server started, waiting for connections...")

while true do
    local client = server:accept()
    print("Client connected: " .. client:getpeername())

    local ip, port = client:getpeername()
    client:send("Welcome to the Lua server\n")

    while true do
        local line, err = client:receive()
        if not err then
            print("Received from client " .. ip .. ":" .. port .. ": " .. line)

            if line == "ping" then
                client:send("Server is alive\n")
            elseif line == "exit" then
                client:send("Disconnecting...\n")
                client:close()
                print("Client disconnected: " .. ip .. ":" .. port)
                break
            else
                client:send("Unknown command\n")
            end
        else
            print("Client disconnected: " .. ip .. ":" .. port)
            break
        end
    end
end
