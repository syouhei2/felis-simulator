#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <list>
#include <functional>
#include <exception>
#include "opcode.hpp"

class Simulator
{
public:
    explicit Simulator(const std::string& binfile);
    void run();

private:
    std::ifstream m_binfile;

    bool m_halt = false;

    int64_t m_dynamic_inst_cnt = 0;

    using Instruction = uint32_t;  // 32bit Instruction code

    static constexpr size_t CODE_INITIAL_SIZE = 30000;
    std::vector<Instruction> m_codes;

    static constexpr size_t REG_SIZE = 32;
    static constexpr size_t FREG_SIZE = 32;

    struct State {
        uint32_t pc = 0;

        /*
         * General purpose registers
         * R0 is zero register
         */
        std::array<uint32_t, REG_SIZE> reg = {{0}};

        // Floating point registers
        std::array<float, FREG_SIZE> freg = {{0}};

        uint32_t hi = 0;
        uint32_t lo = 0;

        // TODO Memory
    };

    using StateIter = std::list<State>::const_iterator;

    // Operand
    /*
     * Type R:
     * -----------------------------------------
     * | opcode | rs | rt | rd | shamt |(fanct)|
     * -----------------------------------------
     * | 6      | 5  | 5  | 5  | 5     |(6    )|
     * -----------------------------------------
     */
    struct OperandR {
        uint32_t rs;
        uint32_t rt;
        uint32_t rd;
        uint32_t shamt;
        uint32_t fanct;
    };

    /*
     * Type I:
     * -----------------------------------------
     * | opcode | rs | rt | immediate          |
     * -----------------------------------------
     * | 6      | 5  | 5  | 16                 |
     * -----------------------------------------
     */
    struct OperandI {
        uint32_t rs;
        uint32_t rt;
        uint32_t immediate;
    };

    /*
     * Type J:
     * -----------------------------------------
     * | opcode | addr                         |
     * -----------------------------------------
     * | 6      | 26                           |
     * -----------------------------------------
     */
    struct OperandJ {
        uint32_t rs;
        uint32_t addr;
    };

    // Function for each instruction
    std::unordered_map<OpCode,
        std::function<State(Instruction, StateIter)>, OpCodeHash> m_inst_funcs;

    // Instruction called counter
    std::unordered_map<OpCode,
        int64_t, OpCodeHash> m_inst_cnt;

    // State history
    std::list<State> m_state_hist;
    StateIter m_state_iter;

    void initInstruction();

    OpCode decodeOpCode(Instruction);
    State exec(OpCode, Instruction, StateIter);

    OperandR decodeR(Instruction);
    OperandI decodeI(Instruction);
    OperandJ decodeJ(Instruction);

    /*
     * Extract bit string.
     * bitset(10110111000..., 0, 8) = 0..010110111
     */
    uint32_t bitset(uint32_t inst, int begin, int end);
    uint32_t bitset64(uint64_t inst, int begin, int end);

    /*
     * Sign extention for 5bit
     * signExt5(01010) -> 0..00001010
     * signExt5(11010) -> 1..11111010
     */
    uint32_t signExt5(uint32_t x);

    void printBitset(uint32_t bits, int begin, int end, bool endl = false);
    void printState(StateIter);
    void printCode(StateIter);
    void printHelp();

#include "instruction.hpp"
};
