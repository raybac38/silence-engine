require("src/lua/engine")

print("je suis du lua lancer depuis du cpp")

print(ffi.string( ffi.C.script_system_get_script_name(0)))
counter = 0

function on_init()
    print("on init" .. entityId)
    ffi.C.transform_system_attach_transform(entityId)
    print("attach render system");
    meshHandle = ffi.C.ressource_manager_acquire_mesh("assets/obj/cube.obj")
    print("attach au render system")
    ffi.C.render_system_attach_mesh(entityId, meshHandle)
    print("return")
end

x = 0
y = 0

function on_update(dt)
    transform = ffi.C.transform_system_get_transform(entityId)

    if ffi.C.input_is_held(KeyCode.Z) then
        y = y + 1 * dt
    end
    if ffi.C.input_is_held(KeyCode.S) then
        y = y - 1 * dt
    end
    if ffi.C.input_is_held(KeyCode.D) then
        x = x + 1 * dt
    end
    if ffi.C.input_is_held(KeyCode.Q) then
        x = x - 1 * dt
    end

    transform.position.x = x
    transform.position.y = y

    

    --print(transform.position.x)
end
