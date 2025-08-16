#ifndef GAME_SPARSESET_H
#define GAME_SPARSESET_H

#include "Types.h"
#include <limits>
#include <vector>

class EntitySparseSet {
public:
     static constexpr Entity INVALID = std::numeric_limits<Entity>::max();

     EntitySparseSet(size_t max_entities);
     void insert(Entity entity);
     void erase(Entity entity);
     bool contains(Entity entity) const;

     const std::vector<Entity>& entities() const { return m_dense; }
     size_t size() const { return m_dense.size(); }

private:
     std::vector<Entity> m_dense;
     std::vector<Entity> m_sparse;
};


#endif //GAME_SPARSESET_H