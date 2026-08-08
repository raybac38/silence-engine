
#include "entity_manager.h"
#include <optional>
#include <vector>

namespace EntityManager {

  namespace{
    vector<size_t> free_ids;
    int max_id = 0;
  }

  size_t allocateEntityId() {
    if(free_ids.size() > 0){
      size_t id = free_ids.back();
      free_ids.pop_back();
      return id;
    }
    else {
      return max_id++;
    }
  }

  void freeEntityId(size_t entityId){
    free_ids.push_back(entityId);
  } 
}
