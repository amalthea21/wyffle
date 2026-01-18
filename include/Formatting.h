#pragma once
#include <string>
#include <unordered_map>

// ============================================================================
// ANSI Formatting
// ============================================================================

namespace ansi {
    // Cursor hide/show
    const std::string HIDE_CURSOR = "\033[?25l";
    const std::string SHOW_CURSOR = "\033[?25h";

    // Reset
    const std::string RESET = "\033[0m";

    // Text styles
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string ITALIC = "\033[3m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK = "\033[5m";
    const std::string REVERSE = "\033[7m";
    const std::string HIDDEN = "\033[8m";
    const std::string STRIKETHROUGH = "\033[9m";

    // Foreground colors
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";

    // Bright foreground colors
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";

    // Background colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";

    // Bright background colors
    const std::string BG_BRIGHT_BLACK = "\033[100m";
    const std::string BG_BRIGHT_RED = "\033[101m";
    const std::string BG_BRIGHT_GREEN = "\033[102m";
    const std::string BG_BRIGHT_YELLOW = "\033[103m";
    const std::string BG_BRIGHT_BLUE = "\033[104m";
    const std::string BG_BRIGHT_MAGENTA = "\033[105m";
    const std::string BG_BRIGHT_CYAN = "\033[106m";
    const std::string BG_BRIGHT_WHITE = "\033[107m";

    // Cursor movement
    inline std::string moveTo(int row, int col) {
        return "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";
    }
    inline std::string moveUp(int n = 1) { return "\033[" + std::to_string(n) + "A"; }
    inline std::string moveDown(int n = 1) { return "\033[" + std::to_string(n) + "B"; }
    inline std::string moveRight(int n = 1) { return "\033[" + std::to_string(n) + "C"; }
    inline std::string moveLeft(int n = 1) { return "\033[" + std::to_string(n) + "D"; }
    const std::string HOME = "\033[H";

    // Screen clearing
    const std::string CLEAR_SCREEN = "\033[2J";
    const std::string CLEAR_LINE = "\033[2K";
    const std::string CLEAR_TO_END = "\033[0J";
    const std::string CLEAR_TO_START = "\033[1J";

    // Helper functions
    inline std::string rgb(int r, int g, int b) {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    inline std::string bgRgb(int r, int g, int b) {
        return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }
    inline std::string color256(int n) { return "\033[38;5;" + std::to_string(n) + "m"; }
    inline std::string bgColor256(int n) { return "\033[48;5;" + std::to_string(n) + "m"; }
}

// ============================================================================
// Keyboard Input Mapping
// ============================================================================

const std::unordered_map<int, std::string> singleByteKeys = {
    // Normal control characters
    {9, "Tab"},
    {10, "Enter"},
    {27, "Escape"},
    {32, "Space"},
    {127, "Backspace"},

    // Ctrl combinations
    {1, "Ctrl+A"},
    {2, "Ctrl+B"},
    {3, "Ctrl+C"},
    {4, "Ctrl+D"},
    {5, "Ctrl+E"},
    {6, "Ctrl+F"},
    {7, "Ctrl+G"},
    {8, "Ctrl+H"},
    {11, "Ctrl+K"},
    {12, "Ctrl+L"},
    {13, "Ctrl+M"},
    {14, "Ctrl+N"},
    {15, "Ctrl+O"},
    {16, "Ctrl+P"},
    {17, "Ctrl+Q"},
    {18, "Ctrl+R"},
    {19, "Ctrl+S"},
    {20, "Ctrl+T"},
    {21, "Ctrl+U"},
    {22, "Ctrl+V"},
    {23, "Ctrl+W"},
    {24, "Ctrl+X"},
    {25, "Ctrl+Y"},
    {26, "Ctrl+Z"}
};

const std::unordered_map<std::string, std::string> escapeSequences = {
    // Arrow keys
    {"\x1b[A", "Up"},
    {"\x1b[B", "Down"},
    {"\x1b[C", "Right"},
    {"\x1b[D", "Left"},

    // Function keys
    {"\x1bOP", "F1"},
    {"\x1bOQ", "F2"},
    {"\x1bOR", "F3"},
    {"\x1bOS", "F4"},
    {"\x1b[15~", "F5"},
    {"\x1b[17~", "F6"},
    {"\x1b[18~", "F7"},
    {"\x1b[19~", "F8"},
    {"\x1b[20~", "F9"},
    {"\x1b[21~", "F10"},
    {"\x1b[23~", "F11"},
    {"\x1b[24~", "F12"},

    // Other special keys
    {"\x1b[H", "Home"},
    {"\x1b[F", "End"},
    {"\x1b[2~", "Insert"},
    {"\x1b[3~", "Delete"},
    {"\x1b[5~", "PageUp"},
    {"\x1b[6~", "PageDown"},
    {"\x1b[1~", "Home"},
    {"\x1b[4~", "End"}
};