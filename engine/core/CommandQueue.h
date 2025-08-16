#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H

#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <vector>

class ICommand;

class CommandQueue {
public:
    using Command = std::function<void()>;
    CommandQueue() = default;

    void submit(Command cmd);
    Command wait_and_pop();
    std::optional<Command> try_pop();
    std::vector<Command> drain();
    bool is_empty();

private:
    mutable std::mutex m_mutex;
    std::queue<Command> m_queue;
    std::condition_variable m_condition;
};


#endif //GAME_COMMANDQUEUE_H