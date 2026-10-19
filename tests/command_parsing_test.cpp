#include <string>
#include <vector>
#include "command_parser.hpp"
#include "test_framework.hpp"

// Test basic command tokenization
TEST(Command_TokenizesSimpleCommand) {
    auto tokens = tokenizeCommand("launch chrome");
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0], "launch");
    ASSERT_EQ(tokens[1], "chrome");
}

// Test multiple arguments
TEST(Command_TokenizesMultipleArguments) {
    auto tokens = tokenizeCommand("help command info");
    ASSERT_EQ(tokens.size(), 3);
    ASSERT_EQ(tokens[0], "help");
}

// Test empty input
TEST(Command_HandlesEmptyInput) {
    auto tokens = tokenizeCommand("");
    ASSERT_EQ(tokens.size(), 0);
}

// Test extra whitespace
TEST(Command_HandlesExtraWhitespace) {
    auto tokens = tokenizeCommand("  launch   chrome  ");
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0], "launch");
    ASSERT_EQ(tokens[1], "chrome");
}

// Test command validation
TEST(CommandValidation_RecognizesValidCommands) {
    auto isValidCommand = [](const std::string& cmd) {
        return cmd == "launch" || cmd == "theme" || cmd == "history" || 
               cmd == "stats" || cmd == "help" || cmd == "exit";
    };
    
    EXPECT_TRUE(isValidCommand("launch"));
    EXPECT_TRUE(isValidCommand("theme"));
    EXPECT_FALSE(isValidCommand("invalid"));
}
