#pragma once

namespace EntityManager {

  /*
   * Allocate a new ID to declare a new entity
   */
  size_t allocateEntityId();
  
  /*
   * Free a entity Id to be reused
   */
  void freeEntityId(size_t entityId);
}
