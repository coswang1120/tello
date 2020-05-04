#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class StopCommand : public Command {
    public:
        StopCommand();

        [[nodiscard]]
        string build() const override;
    };
}