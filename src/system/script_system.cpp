#include "script_system.h"

#include "../utils/sparse_set.tpp"

namespace
{
    SparseSet<ScriptSystem::Script> sparseSet;

    void setUpLuaState(sol::state &luaState, const std::string &scriptPath)
    {
        luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::ffi);
        luaState.script_file("src/lua/engine.lua");

        if (!scriptPath.empty())
        {
            luaState.script_file(scriptPath);
        }
        else
        {
            printf("[WARNING] ScrpitSystem : script path is empty\n");
        }
    };
}

void ScriptSystem::attachScript(size_t entityId, std::string path)
{
    if (sparseSet.has(entityId))
    {
        ScriptSystem::Script &script = sparseSet.at(entityId);

        script.luaState = sol::state();
        script.path = path;

        setUpLuaState(script.luaState, script.path);
    }
    else
    {
        ScriptSystem::Script newScript;
        newScript.path = path;
        sparseSet.insert(entityId, std::move(newScript));

        // Important, because callung setUpLuaState can use function call that require entityId to be present

        ScriptSystem::Script &script = sparseSet.at(entityId);
        setUpLuaState(script.luaState, script.path);
    }
}

void ScriptSystem::removeScript(size_t entityId)
{
    if (sparseSet.has(entityId))
    {
        sparseSet.delet(entityId);
    }
    else
    {
        throw std::runtime_error("Removing a script that doesn't exist");
    }
}

std::string &ScriptSystem::getScriptName(size_t entityId)
{
    if (sparseSet.has(entityId))
    {
        return sparseSet.at(entityId).path;
    }
    else
    {
        throw std::runtime_error("Accessing script name that doesn't exist");
    }
}

extern "C"
{
    void script_system_attach_script(size_t entityId, const char *path)
    {
        ScriptSystem::attachScript(entityId, path);
    }

    void script_system_remove_script(size_t entityId)
    {
        ScriptSystem::removeScript(entityId);
    }

    const char *script_system_get_script_name(size_t entityId)
    {
        std::string &name = ScriptSystem::getScriptName(entityId);
        return name.c_str();
    }
}
