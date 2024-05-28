#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <unistd.h>

#include <sys/syscall.h>

template <typename T, typename... A>
inline auto CreateAndStartThread(int core_id, const std::string &name, T &&func, A &&...args) noexcept
{
  auto t = new std::thread([&]()
                           { std::forward<T>(func)((std::forward<A>(args))...); });

  using namespace std::literals::chrono_literals;
  std::this_thread::sleep_for(1s);

  return t;
}
