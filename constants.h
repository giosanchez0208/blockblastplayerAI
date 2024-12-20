#ifndef BLOCK_BLAST_CONSTANTS_H
#define BLOCK_BLAST_CONSTANTS_H

#include <cstdint>

/*
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
    The masks below are constants which will help identify
    which rows/columns are full of 1's (blocks)
    later on in the code.
    ⋆⁺｡˚⋆˙‧₊☽ ◯ ☾₊‧˙⋆˚｡⁺⋆
*/

constexpr uint64_t ROW1 = 0xFF00000000000000; 
constexpr uint64_t ROW2 = 0x00FF000000000000; 
constexpr uint64_t ROW3 = 0x0000FF0000000000; 
constexpr uint64_t ROW4 = 0x000000FF00000000;
constexpr uint64_t ROW5 = 0x00000000FF000000; 
constexpr uint64_t ROW6 = 0x0000000000FF0000;
constexpr uint64_t ROW7 = 0x000000000000FF00; 
constexpr uint64_t ROW8 = 0x00000000000000FF;

constexpr uint64_t COL1 = 0x0101010101010101; 
constexpr uint64_t COL2 = 0x0202020202020202; 
constexpr uint64_t COL3 = 0x0404040404040404; 
constexpr uint64_t COL4 = 0x0808080808080808;
constexpr uint64_t COL5 = 0x1010101010101010; 
constexpr uint64_t COL6 = 0x2020202020202020; 
constexpr uint64_t COL7 = 0x4040404040404040;
constexpr uint64_t COL8 = 0x8080808080808080;

#endif