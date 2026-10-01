/**@file */
#include "entity_manager.hpp"
#include <optional>
#include <vector>
#include <stack>
#include <cstdint>

namespace
{

  std::stack<uint32_t> reusable_id;
  std::vector<uint32_t> generation;
  uint32_t max_id;
}

EntityManager::EntityId EntityManager::allocateEntityId()
{

  EntityManager::EntityId entityId;
  uint32_t id = 0;
  uint32_t gen = 0;

  if (!reusable_id.empty())
  {
    id = reusable_id.top();
    reusable_id.pop();
    gen = generation.at(id) + 1;
    generation.at(id) = gen;
  }
  else
  {

    max_id = max_id + 1;
    id = max_id;
    gen = 0;
    generation.push_back(gen);
  }

  entityId.id = id;
  entityId.generation = gen;

  return entityId;
}

void EntityManager::freeEntityId(EntityManager::EntityId entityId)
{
  reusable_id.push(entityId.id);
}

extern "C"
{
  EntityManager::EntityId entity_manager_allocate_entity_id()
  {
    return EntityManager::allocateEntityId();
  }

  void entity_manager_free_entity_id(EntityManager::EntityId entityId)
  {
    EntityManager::freeEntityId(entityId);
  }
}