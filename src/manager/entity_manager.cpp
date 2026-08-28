/**@file */
#include "entity_manager.h"
#include <optional>
#include <vector>

namespace
{
  // Ajout de std:: et passage de max_id en size_t
  std::vector<size_t> free_ids;
  size_t max_id = 0;
}

size_t EntityManager::allocateEntityId()
{
  if (!free_ids.empty()) // .empty() est plus idiomatique en C++
  {
    size_t id = free_ids.back();
    free_ids.pop_back();
    return id;
  }
  else
  {
    return max_id++;
  }
}

void EntityManager::freeEntityId(size_t entityId)
{
  free_ids.push_back(entityId);
}

extern "C"
{
  size_t entity_manager_allocate_entity_id()
  {
    return EntityManager::allocateEntityId();
  }

  void entity_manager_free_entity_id(size_t entityId)
  {
    EntityManager::freeEntityId(entityId);
  }
}