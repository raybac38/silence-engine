#pragma once

#include <string>
#include <sol/sol.hpp>

namespace ScriptSystem
{
    struct Script
    {
        std::string path;
        sol::state luaState;
    };

    void attachScript(size_t entityId, std::string path);

    void removeScript(size_t entityId);

    std::string &getScriptName(size_t entityId);

    void update();
};

extern "C"
{
    void script_system_attach_script(size_t entityId, const char *path);

    void script_system_remove_script(size_t entityId);

    const char *script_system_get_script_name(size_t entityId);
}