#pragma once

#include <iostream>
#include <vector>
#include <atomic>

template <typename T>
class LFQueue final
{
public:
    LFQueue(std::size_t num_elems) : m_store(num_elems, T()) /* pre-allocation of vector storage. */
    {
    }

    auto GetNextToWriteTo() noexcept
    {
        return &m_store[m_next_write_index];
    }

    auto UpdateWriteIndex() noexcept
    {
        m_next_write_index = (m_next_write_index + 1) % m_store.size();
        m_num_elements++;
    }

    auto GetNextToRead() const noexcept -> const T *
    {
        return (Size() ? &m_store[m_next_read_index] : nullptr);
    }

    auto updateReadIndex() noexcept
    {
        m_next_read_index = (m_next_read_index + 1) % m_store.size();
        m_num_elements--;
    }

    auto Size() const noexcept
    {
        return m_num_elements.load();
    }

    // Deleted default, copy & move constructors and assignment-operators.
    LFQueue() = delete;

    LFQueue(const LFQueue &) = delete;

    LFQueue(const LFQueue &&) = delete;

    LFQueue &operator=(const LFQueue &) = delete;

    LFQueue &operator=(const LFQueue &&) = delete;

private:
    std::vector<T> m_store;

    std::atomic<size_t> m_next_write_index = {0};
    std::atomic<size_t> m_next_read_index = {0};
    std::atomic<size_t> m_num_elements = {0};
};
