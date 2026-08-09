ffi = require("ffi")

ffi.cdef[[
    uint64_t entity_manager_allocate_id(void);
    void entity_manager_free_id(uint64_t entity_id);

    void script_system_attach_script(size_t entityId, const char* path);
    void script_system_remove_script(size_t entityId);
    const char *script_system_get_script_name(size_t entityId);
]]

