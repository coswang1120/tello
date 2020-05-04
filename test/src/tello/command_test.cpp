#include <gtest/gtest.h>
#include <tello/command/up_command.hpp>
#include <tello/command/down_command.hpp>
#include <tello/command/left_command.hpp>
#include <tello/command/right_command.hpp>
#include <tello/command/forward_command.hpp>
#include <tello/command/back_command.hpp>
#include <tello/command/clockwise_turn_command.hpp>
#include <tello/command/counterclockwise_turn_command.hpp>
#include <random>

using tello::Command;
using tello::CommandType;
using std::string;

static std::random_device rd;
static std::mt19937 mt(rd());

using namespace tello::command;

TEST(CommandFactory, UpCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    UpCommand result = UpCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, UpCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    UpCommand result = UpCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, UpCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    UpCommand result = UpCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("up ") + x_arg, result.build());
}

TEST(CommandFactory, DownCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    DownCommand result = DownCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, DownCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    DownCommand result = DownCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, DownCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    DownCommand result = DownCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("down ") + x_arg, result.build());
}

TEST(CommandFactory, LeftCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    LeftCommand result = LeftCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, LeftCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    DownCommand result = DownCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, LeftCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    LeftCommand result = LeftCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("left ") + x_arg, result.build());
}

TEST(CommandFactory, RightCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    RightCommand result = RightCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, RightCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    RightCommand result = RightCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, RightCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    RightCommand result = RightCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("right ") + x_arg, result.build());
}

TEST(CommandFactory, ForwardCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    ForwardCommand result = ForwardCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, ForwardCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    ForwardCommand result = ForwardCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, ForwardCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ForwardCommand result = ForwardCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("forward ") + x_arg, result.build());
}

TEST(CommandFactory, BackCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);

    // Act
    BackCommand result = BackCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, BackCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);

    // Act
    BackCommand result = BackCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, BackCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    BackCommand result = BackCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("back ") + x_arg, result.build());
}

TEST(CommandFactory, ClockwiseTurnCommand_xValueLowerThanMinGiven_getErrorString) {
    int x = 0;

    // Act
    ClockwiseTurnCommand result = ClockwiseTurnCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, ClockwiseTurnCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(361);
    int x = dist(mt);

    // Act
    ClockwiseTurnCommand result = ClockwiseTurnCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, ClockwiseTurnCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(1, 360);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ClockwiseTurnCommand result = ClockwiseTurnCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("cw ") + x_arg, result.build());
}

TEST(CommandFactory, CounterclockwiseTurnCommand_xValueLowerThanMinGiven_getErrorString) {
    int x = 0;

    // Act
    CounterclockwiseTurnCommand result = CounterclockwiseTurnCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, CounterclockwiseTurnCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(361);
    int x = dist(mt);

    // Act
    CounterclockwiseTurnCommand result = CounterclockwiseTurnCommand(x);

    // Assert
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, CounterclockwiseTurnCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(1, 360);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    CounterclockwiseTurnCommand result = CounterclockwiseTurnCommand(x);

    // Assert
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("ccw ") + x_arg, result.build());
}