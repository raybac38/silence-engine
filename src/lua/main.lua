require("src/lua/engine")

print("je suis du lua lancer depuis du cpp")

print(ffi.string( ffi.C.script_system_get_script_name(0)))