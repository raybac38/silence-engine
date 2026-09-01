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



function on_update()
    transform = ffi.C.transform_system_get_transform(entityId)
    transform.rotation.x = transform.rotation.x + 0.1
    transform.position.x = math.sin(counter)
    transform.position.y = math.cos(counter)
    counter = counter + 0.1
    --print(transform.position.x)
end
