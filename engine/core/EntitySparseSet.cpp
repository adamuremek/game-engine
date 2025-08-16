#include "EntitySparseSet.h"
#include <cassert>

EntitySparseSet::EntitySparseSet(size_t max_entities)
    : m_sparse(max_entities, INVALID)
{}

void EntitySparseSet::insert(Entity entity) {
    assert(entity < m_sparse.size());
    assert(m_sparse[entity] == INVALID && "Entity is already in set!");
    m_sparse[entity] = static_cast<Entity>(m_dense.size());
    m_dense.push_back(entity);
}


void EntitySparseSet::erase(Entity entity) {
    assert(contains(entity));
    Entity last = m_dense.back();
    size_t idx = m_sparse[entity];
    m_dense[idx] = last;
    m_sparse[last] = static_cast<Entity>(idx);
    m_dense.pop_back();
    m_sparse[entity] = INVALID;
}

bool EntitySparseSet::contains(Entity entity) const {
    return entity < m_sparse.size() && m_sparse[entity] != INVALID;
}









