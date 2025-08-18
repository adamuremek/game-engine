#ifndef GAME_SYSTEMS_H
#define GAME_SYSTEMS_H

#include "World.h"
#include <vector>
#include <algorithm>
#include <cassert>

template<typename... Args>
class _Event {
public:
    using Fn = void(*)(Args...);

    static void add(Fn fn, int priority = 0) {
        reg().add(fn, priority);
    }
    static void finalize() { reg().finalize(); }
    static void fire(Args... args) { reg().fire(args...); }
    static std::size_t size() { return reg().size(); }

    struct AutoReg {
        AutoReg(Fn fn, int priority = 0) {
            _Event::add(fn, priority);
        }
    };

private:
    struct Entry { int priority; Fn fn; };

    class Registry {
    public:
        void add(Fn fn, int priority) {
            assert(!m_frozen && "Registered after finalize()");
            m_entries.push_back({priority, fn});
            m_dirty = true;
        }

        void finalize() {
            if (m_frozen) return;

            if (m_dirty) {
                std::stable_sort(
                    m_entries.begin(),
                    m_entries.end(),
                    [](const Entry& a, const Entry& b){ return a.priority < b.priority; }
                );
                m_dirty = false;
            }

            m_frozen = m_entries.data();
            m_count = m_entries.size();
        }

        void fire(Args... args) const {
            const Entry* data = m_frozen ? m_frozen : m_entries.data();
            const std::size_t n = m_frozen ? m_count : m_entries.size();
            for (std::size_t i = 0; i < n; ++i) {
                data[i].fn(args...);
            }
        }

        std::size_t size() const {
            return m_frozen ? m_count : m_entries.size();
        }

    private:
        std::vector<Entry> m_entries;
        Entry* m_frozen = nullptr;
        std::size_t m_count = 0;
        bool m_dirty = false;
    };

    static Registry& reg() {
        static Registry R;
        return R;
    }
};


class Systems {
public:
    using OnStartup = _Event<World&>;
    using OnUpdate = _Event<World&, float>;

    static void register_on_startup(OnStartup::Fn fn, int priority = 0) {
        OnStartup::add(fn, priority);
    }

    static void register_on_update(OnUpdate::Fn fn, int priority = 0) {
        OnUpdate::add(fn, priority);
    }

    static void finalize() {
        OnUpdate::finalize();
    }

    static void fire_startup(World& world) {
        OnStartup::fire(world);
    }

    static void fire_update(World& world, float dt) {
        OnUpdate::fire(world, dt);
    }
};

// ---- Registration convenience ----
#define SYS_CONCAT_INNER(a,b) a##b
#define SYS_CONCAT(a,b) SYS_CONCAT_INNER(a,b)

#define SYSTEMS_ON_STARTUP(fn) \
namespace { Systems::OnStartup::AutoReg SYS_CONCAT(_sys_upd_, __COUNTER__){ (Systems::OnStartup::Fn)(fn) }; }

#define SYSTEMS_ON_UPDATE(fn) \
namespace { Systems::OnUpdate::AutoReg SYS_CONCAT(_sys_upd_, __COUNTER__){ (Systems::OnUpdate::Fn)(fn) }; }

#endif // GAME_SYSTEMS_H
