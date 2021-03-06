#include "simulator.hpp"

Simulator::State Simulator::multu(Instruction inst, StateIter state_iter)
{
    auto now_state = *state_iter;
    auto new_state = now_state;

    auto op = decodeR(inst);

    uint64_t rs_signed = now_state.reg.at(op.rs);
    uint64_t rt_signed = now_state.reg.at(op.rt);
    uint64_t acc = rs_signed * rt_signed;

    new_state.pc += 4;
    new_state.hi = bitset64(acc, 0, 32);
    new_state.lo = bitset64(acc, 32, 64);

    return new_state;
}
