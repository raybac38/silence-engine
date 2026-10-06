#include "../utils/sparse_set.tpp"
#include "./script_system.hpp"

namespace
{
    SparseSet<ScriptSystem::Script> sparseSet;
    std::vector<EntityManager::EntityId> onUpdateList;

    void setUpLuaState(sol::state &luaState, const std::string &scriptPath)
    {
        luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::ffi);
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

void ScriptSystem::attachScript(EntityManager::EntityId entityId, std::string path)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))
    {
        ScriptSystem::Script &script = sparseSet.at(id);

        script.luaState = sol::state();
        script.path = path;

        setUpLuaState(script.luaState, script.path);
    }
    else
    {
        ScriptSystem::Script newScript;
        newScript.path = path;
        sparseSet.insert(id, std::move(newScript));

        // Important, because callung setUpLuaState can use function call that require entityId to be present

        ScriptSystem::Script &script = sparseSet.at(id);
        setUpLuaState(script.luaState, script.path);
    }

    onUpdateList.push_back(entityId);
}

void ScriptSystem::removeScript(EntityManager::EntityId entityId)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))
    {
        sparseSet.delet(id);
    }
    else
    {
        throw std::runtime_error("Removing a script that doesn't exist");
    }
}

std::string &ScriptSystem::getScriptName(EntityManager::EntityId entityId)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))
    {
        return sparseSet.at(id).path;
    }
    else
    {
        throw std::runtime_error("Accessing script name that doesn't exist");
    }
}

void ScriptSystem::update(float dt)
{
    while (!onUpdateList.empty())
    {
        EntityManager::EntityId id = onUpdateList.back();
        onUpdateList.pop_back();
        luaState.new_usertype<EntityManager::EntityId>("EntityId",
                                                       "id", &EntityManager::EntityId::id);
        sol::protected_function onInit = sparseSet.at(id.id).luaState["_OnInit"];
        if (onInit.valid())
        {
            auto result = onInit(id);

            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << " [Lua Error Init] ID " << id.id << id.generation << " : " << err.what() << std::endl;
            }
        }
    }
    if (sparseSet.size() > 0)
    {
        for (ScriptSystem::Script &script : sparseSet.data())
        {
            sol::protected_function onUpdate = script.luaState["_OnUpdate"];
            if (onUpdate.valid())
            {
                auto result = onUpdate(dt);

                if (!result.valid())
                {
                    sol::error err = result;
                    std::cerr << " [Lua Error Update] : " << err.what() << std::endl;
                }
            }
        }
    }
}

extern "C"
{
    void script_system_attach_script(EntityManager::EntityId entityId, const char *path)
    {
        ScriptSystem::attachScript(entityId, path);
    }

    void script_system_remove_script(EntityManager::EntityId entityId)
    {
        ScriptSystem::removeScript(entityId);
    }

    const char *script_system_get_script_name(EntityManager::EntityId entityId)
    {
        std::string &name = ScriptSystem::getScriptName(entityId);
        return name.c_str();
    }
}
