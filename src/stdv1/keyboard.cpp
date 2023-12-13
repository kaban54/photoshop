#include "keyboard.h"

const char SHIFT_NUMS[] = ")!@#$%^&*(";
const char SYMBS[]       = "[];,.\'/\\`=- ";
const char SHIFT_SYMBS[] = "{}:<>\"?|~+_ ";

char GetSymb (const KeyboardState& kstate) {
    KeyboardKey key = kstate.key;
    char ret = 0;

    if (key >= A_KEY && key <= Z_KEY) {
        ret = 'a' + key - A_KEY;
        if (kstate.shift) ret += 'A' - 'a';
    }
    else if (key >= NUM0_KEY && key <= NUM9_KEY) {
        if (kstate.shift) ret = SHIFT_NUMS[key - NUM0_KEY];
        else ret = '0' + key - NUM0_KEY;
    }
    else if (key >= LBracket_KEY && key <= Space_KEY) {
        if (kstate.shift) ret = SHIFT_SYMBS[key - LBracket_KEY];
        else ret = SYMBS[key - LBracket_KEY];
    }
    else if (key == Tab_KEY) ret = '\t';
    else if (key == Enter_KEY && kstate.shift) ret = '\n';

    return ret;
}