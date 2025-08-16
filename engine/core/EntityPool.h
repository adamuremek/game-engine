#ifndef GAME_ENTITYPOOL_H
#define GAME_ENTITYPOOL_H

#include "Types.h"
#include <vector>

class EntityPool {
public:
    EntityPool(size_t max_entities);

    Entity create();
    void destroy(Entity entity);

private:
    std::vector<Entity> m_free;
    size_t m_next;
    size_t m_capacity;
};


#endif //GAME_ENTITYPOOL_H