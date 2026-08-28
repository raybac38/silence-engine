ffi = require("ffi")

ffi.cdef[[

    // entity manager
    uint64_t entity_manager_allocate_id(void);
    void entity_manager_free_id(uint64_t entity_id);

    // script system
    void script_system_attach_script(size_t entityId, const char* path);
    void script_system_remove_script(size_t entityId);
    const char *script_system_get_script_name(size_t entityId);

    typedef struct {
        float x, y, z;
    } Vec3;

    typedef struct {
        Vec3 position;
        Vec3 rotation;
        Vec3 scale;
    } Transform;

    // Geometry.h
    typedef struct 
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    } Vertex;

    typedef struct Mesh Mesh;

    Mesh geometry_load_file(const char *path);
    void geometry_free_mesh(Mesh * mesh);

    // transform system
    void transform_system_attach_transform(size_t index);
    void transform_system_remove_transform(size_t index);
    Transform *transform_system_get_transform(size_t index);

    // render system
    void render_system_attach_mesh(size_t entityId, Mesh *mesh);
    void render_system_remove_mesh(size_t entityId);

]]

-- Callback from the engine
entityId = nil

function _on_init(id)
    entityId = id
    if on_init and type(on_init) == "function" then
        on_init()
    end
end

function _on_update()
    if on_update and type(on_update) == "function" then
        on_update()
    end
end