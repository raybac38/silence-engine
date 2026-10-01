#pragma once
#include <cstddef>
#include <stdint.h>

namespace EntityManager
{
  struct EntityId
  {
    uint32_t generation;
    uint32_t id;
  };

  /*
   * Allocate a new ID to declare a new entity
   */
  EntityId allocateEntityId();

  /*
   * Free a entity Id to be reused
   */
  void freeEntityId(EntityId entityId);
}

extern "C"
{
  EntityManager::EntityId tity_manager_allocate_entity_id();

  void entity_manager_free_entity_id(EntityManager::EntityId entityId);
}