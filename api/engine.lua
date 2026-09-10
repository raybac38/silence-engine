ffi = require("ffi")

ffi.cdef [[
    // entity manager
    size_t entity_manager_allocate_entity_id(void);
    void entity_manager_free_id(size_t entity_id);

    // script system
    void script_system_attach_script(size_t entityId, const char* path);
    void script_system_remove_script(size_t entityId);
    const char *script_system_get_script_name(size_t entityId);

    typedef struct {
        float x, y, z;
    } Vec3;

    typedef struct {
        float x, y;
    } Vec2;

    typedef struct {
        Vec3 position;
        Vec3 rotation;
        Vec3 scale;
    } Transform;

    // Ressource manager

    typedef struct {
        uint32_t index;
        uint32_t generation;
    } MeshHandle;

    MeshHandle ressource_manager_acquire_mesh(const char* path);
    void ressource_manager_release_mesh(MeshHandle handle);

    // transform system
    void transform_system_attach_transform(size_t index);
    void transform_system_remove_transform(size_t index);

    void transfrom_system_set_position(size_t index, Vec3 position);
    void transfrom_system_translate(size_t index, Vec3 vector);
    void transfrom_system_set_rotation(size_t index, Vec3 rotation);
    void transfrom_system_rotate(size_t index, Vec3 vector);
    void transfrom_system_set_scale(size_t index, Vec3 scale);
    void transfrom_system_scale(size_t index, Vec3 vector);

    // render system
    void render_system_attach_mesh(size_t entityId, MeshHandle handle);
    void render_system_remove_mesh(size_t entityId);
    void render_system_set_camera(size_t entityId);

    // Input System
    bool input_is_held(int action_id);
    bool input_is_pressed(int action_id);
    bool input_is_released(int action_id);
    Vec2 input_get_mouse_position();
    Vec2 input_get_mouse_delta();
    Vec2 input_get_mouse_scroll();

]]

-- UTILS

function Vec3(x, y, z, w)
    return ffi.new("Vec4", x, y, z, w)
end

function Vec3(x, y, z)
    return ffi.new("Vec3", x, y, z)
end

function Vec2(x, y)
    return ffi.new("Vec2", x, y)
end

-- ENTITY MANAGER

EntityManager = {}

function EntityManager.newEntity() return ffi.C.entity_manager_allocate_entity_id() end

function EntityManager.destroyEntity(entityId) return ffi.C.entity_manager_free_id(entityId) end

-- SCRIPT SYSTEM

ScriptSystem = {}

function ScriptSystem.attachScript(entityId, path) return ffi.C.script_system_attach_script(entityId, path) end

function ScriptSystem.removeScript(entityId) return ffi.C.script_system_remove_script(entityId) end

function ScriptSystem.getScriptName(entityId) return ffi.C.script_system_get_script_name(entityId) end

-- RESSOURCE MANAGER

RessourceManager = {}

function RessourceManager.acquireMesh(path) return ffi.C.ressource_manager_acquire_mesh(path) end

function RessourceManager.releaseMesh(handle) return ffi.C.ressource_manager_release_mesh(handle) end

-- TRANSFORM SYSTEM

TransformSystem = {}

function TransformSystem.attachTransform(entityId) return ffi.C.transform_system_attach_transform(entityId) end

function TransformSystem.removeTransform(entityId) return ffi.C.transform_system_remove_transform(entityId) end

function TransformSystem.setPosition(entityId, position) return ffi.C.transfrom_system_set_position(entityId, position) end

function TransformSystem.translate(entityId, vector) return ffi.C.transfrom_system_translate(entityId, vector) end

function TransformSystem.setRotation(entityId, rotation) return ffi.C.transfrom_system_set_rotation(entityId, rotation) end

function TransformSystem.rotate(entityId, vector) return ffi.C.transfrom_system_rotate(entityId, vector) end

function TransformSystem.setScale(entityId, scale) return ffi.C.transfrom_system_set_scale(entityId, scale) end

function TransformSystem.scale(entityId, vector) return ffi.C.transfrom_system_scale(entityId, vector) end

KeyCode = {
    Space = 44, W = 26, Z = 122, Q = 113, A = 4, S = 115, D = 100
}

-- RENDER SYSTEM

RenderSystem = {}

function RenderSystem.attachMesh(entityId, meshHandle) return ffi.C.render_system_attach_mesh(entityId, meshHandle) end

function RenderSystem.removeMesh(entityId) return ffi.C.render_system_remove_mesh(entityId) end

function RenderSystem.setCamera(entityId) return ffi.C.render_system_set_camera(entityId) end

-- INPUT SYSTEM

Input = {}
function Input.GetKey(keycode) return ffi.C.input_is_held(keycode) end

function Input.GetKeyDown(keycode) return ffi.C.input_is_pressed(keycode) end

function Input.GetKeyUp(keycode) return ffi.C.input_is_released(keycode) end

function Input.GetMousePosition() return ffi.C.input_get_mouse_position() end

function Input.GetMouseDelta() return ffi.C.input_get_mouse_delta() end

function Input.GetMouseScroll() return ffi.C.input_get_mouse_scroll() end

-- GAMEOBJECT

entityId = nil

function _on_init(id)
    entityId = id
    if on_init and type(on_init) == "function" then
        on_init()
    end
end

function _on_update(dt)
    if on_update and type(on_update) == "function" then
        on_update(dt)
    end
end
