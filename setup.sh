#!/bin/bash

install_lua() {
    echo "Installing Lua..."
    if command -v apt &> /dev/null; then
        sudo apt install -y lua5.3
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y lua
    elif command -v brew &> /dev/null; then
        brew install lua
    else
        echo "Package manager not found. Please install Lua manually."
        exit 1
    fi
}

if ! command -v lua &> /dev/null; then
    echo "Lua is not installed. Attempting to install Lua..."
    install_lua

    if ! command -v lua &> /dev/null; then
        echo "Failed to install Lua. Please install it manually."
        exit 1
    fi
else
    echo "Lua is already installed."
fi

if ! command -v luarocks &> /dev/null; then
    echo "LuaRocks is not installed. Please install it first."
    exit 1
fi

luarocks install luasocket --local

if luarocks list | grep -q "luasocket"; then
    echo "LuaSocket has been successfully installed."
else
    echo "Failed to install LuaSocket."
    exit 1
fi
