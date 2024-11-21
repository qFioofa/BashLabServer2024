local socket = require("socket")

local function connect_to_server()
    while true do
        local client = socket.connect("localhost", 12345)
        if client then
            return client
        else
            print("Unable to connect to server, retrying in 5 seconds...")
            socket.sleep(5)
        end
    end
end

local client = connect_to_server()
print("Connected to server!")

while true do
    print("Enter command (ping, exit, connect): ")
    local input = io.read()

    if input == "exit" then
        client:send(input .. "\n")
        print("Exiting client...")
        client:close()
        break
    elseif input == "connect" then
        print("Attempting to reconnect...")
        client:close()
        client = connect_to_server()
        print("Reconnected to server!")
    else
        client:send(input .. "\n")
        local response, err = client:receive()
        if not err then
            print("Response from server: " .. response)
        else
            print("Error receiving from server: " .. err)
            print("Attempting to reconnect...")
            client:close()
            client = connect_to_server()
            print("Reconnected to server!")
        end
    end
end

client:close()
