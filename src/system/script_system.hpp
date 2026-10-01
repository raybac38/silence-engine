#pragma once

#include <string>
#include <sol/sol.hpp>
#include "../manager/entity_manager.hpp"

namespace ScriptSystem
{
    struct Script
    {
        std::string path;
        sol::state luaState;
    };

    void attachScript(EntityManager::EntityId entityId, std::string path);

    void removeScript(EntityManager::EntityId entityId);

    std::string &getScriptName(EntityManager::EntityId entityId);

    void update(float dt);
};

extern "C"
{
    void script_system_attach_script(EntityManager::EntityId entityId, const char *path);

    void script_system_remove_script(EntityManager::EntityId entityId);

    const char *script_system_get_script_name(EntityManager::EntityId entityId);
}