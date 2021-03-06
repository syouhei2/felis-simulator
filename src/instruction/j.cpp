#include "simulator.hpp"

Simulator::State Simulator::j(Instruction inst, StateIter state_iter)
{
    auto now_state = *state_iter;
    auto new_state = now_state;

    auto op = decodeJ(inst);

    new_state.pc = (now_state.pc & 0xfffffff) | (op.addr << 2);

    return new_state;
}
