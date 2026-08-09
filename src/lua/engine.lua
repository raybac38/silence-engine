local ffi = require("ffi")

ffi.cdef[[
    uint64_t entity_manager_allocate_id(void);
    void entity_manager_free_id(uint64_t entity_id);

    void script_system_attach_script(size_t entityId, std::string path);
    void script_system_remove_script(size_t entityId);
]]

