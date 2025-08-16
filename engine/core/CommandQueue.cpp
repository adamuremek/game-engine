#include "CommandQueue.h"

void CommandQueue::submit(Command cmd) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(cmd));
    }
    m_condition.notify_one();
}

CommandQueue::Command CommandQueue::wait_and_pop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [&] {return !m_queue.empty(); });
    auto cmd = std::move(m_queue.front());
    m_queue.pop();
    return cmd;
}

std::optional<CommandQueue::Command> CommandQueue::try_pop() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty()) {
        return nullptr;
    }
    auto cmd = std::move(m_queue.front());
    m_queue.pop();
    return cmd;
}

std::vector<CommandQueue::Command> CommandQueue::drain() {
    std::vector<Command> cmds;
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_queue.empty()) {
        cmds.push_back(std::move(m_queue.front()));
        m_queue.pop();
    }
    return cmds;
}

bool CommandQueue::is_empty() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}




