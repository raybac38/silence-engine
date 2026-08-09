#pragma once
#include <cstddef>

namespace EntityManager
{

  /*
   * Allocate a new ID to declare a new entity
   */
  size_t allocateEntityId();

  /*
   * Free a entity Id to be reused
   */
  void freeEntityId(size_t entityId);
}

extern "C"
{
  size_t entity_manager_allocate_entity_id();

  void entity_manager_free_entity_id(size_t entityId);
}