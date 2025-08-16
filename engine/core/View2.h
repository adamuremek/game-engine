#ifndef GAME_VIEW2_H
#define GAME_VIEW2_H

#include "ComponentPool.h"

template<typename A, typename B>
class View2 {
public:
    View2(ComponentPool<A>& a, ComponentPool<B>& b)
        : a_(&a), b_(&b), m_iterate_A_first(a.size() <= b.size())
    {}

    class Iter {
    public:
        ComponentPool<A>* a{};
        ComponentPool<B>* b{};
        bool iterate_A_first{};

        using ItA = typename ComponentPool<A>::iterator;
        using ItB = typename ComponentPool<B>::iterator;

        ItA it_a, end_a;
        ItB it_b, end_b;

        void skip_invalid() {
            if (iterate_A_first) {
                while (it_a != end_a && !b->contains(static_cast<Entity>(it_a->first)))
                    ++it_a;
            } else {
                while (it_b != end_b && !a->contains(static_cast<Entity>(it_b->first)))
                    ++it_b;
            }
        }

        auto operator*() const {
            if (iterate_A_first) {
                Entity e = static_cast<Entity>(it_a->first);
                return std::tuple<Entity, A&, B&> { e, *it_a->second, *b->get(e) };
            } else {
                Entity e = static_cast<Entity>(it_b->first);
                return std::tuple<Entity, A&, B&> { e, *a->get(e), *it_b->second };
            }
        }

        Iter& operator++() {
            if (iterate_A_first) {
                ++it_a;
            } else {
                ++it_b;
            }
            skip_invalid();
            return *this;
        }

        bool operator!=(const Iter& o) const {
            return iterate_A_first ? (it_a != o.it_a) : (it_b != o.it_b);
        }
    };

    Iter begin() {
        Iter it;
        it.a = a_;
        it.b = b_;
        it.iterate_A_first = m_iterate_A_first;
        if (m_iterate_A_first) {
            it.it_a = a_->begin();
            it.end_a = a_->end();
        } else {
            it.it_b = b_->begin();
            it.end_b = b_->end();
        }
        it.skip_invalid();
        return it;
    }

    Iter end() {
        Iter it;
        it.a = a_;
        it.b = b_;
        it.iterate_A_first = m_iterate_A_first;
        if (m_iterate_A_first) {
            it.it_a = a_->end();
            it.end_a = a_->end();
        } else {
            it.it_b = b_->end();
            it.end_b = b_->end();
        }
        return it;
    }

private:
    ComponentPool<A>* a_;
    ComponentPool<B>* b_;
    bool m_iterate_A_first;
};

#endif //GAME_VIEW2_H