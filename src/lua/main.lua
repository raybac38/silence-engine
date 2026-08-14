require("src/lua/engine")

print("je suis du lua lancer depuis du cpp")

print(ffi.string( ffi.C.script_system_get_script_name(0)))

function on_init()
    ffi.C.transform_system_attach_transform(entityId)
end



function on_update()
    transform = ffi.C.transform_system_get_transform(entityId)
    transform.position.x = transform.position.x + 1
    print(transform.position.x)
end
