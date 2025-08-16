#ifndef GAME_COMPONENTPOOL_H
#define GAME_COMPONENTPOOL_H

#include "Types.h"
#include <unordered_map>
#include <memory>

template<typename T>
class ComponentPool {
public:
    using Map = std::unordered_map<uint32_t, std::unique_ptr<T>>;
    using iterator = typename Map::iterator;
    using const_iterator = typename Map::const_iterator;

    void add(Entity entity, std::unique_ptr<T> component) {
        m_components[entity] = std::move(component);
    }

    T* get(Entity entity) {
        auto it = m_components.find(entity);
        return (it != m_components.end()) ? it->second.get() : nullptr;
    }

    const T* get(Entity entity) const {
        auto it = m_components.find(entity);
        return (it != m_components.end()) ? it->second.get() : nullptr;
    }

    void remove(Entity entity) {
        m_components.erase(entity);
    }

    bool contains(Entity entity) const {
        return m_components.find(entity) != m_components.end();
    }

    size_t size() const { return m_components.size(); }

    iterator begin() { return m_components.begin(); }
    iterator end()   { return m_components.end();   }
    const_iterator begin() const { return m_components.begin(); }
    const_iterator end()   const { return m_components.end();   }

private:
    Map m_components;
};

#endif //GAME_COMPONENTPOOL_H