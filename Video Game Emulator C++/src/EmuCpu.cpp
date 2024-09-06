#include <iomanip>

#include "Emu.h"

EmuCpu::EmuCpu() {}

EmuCpu::EmuCpu(EmuMmu *mem_ptr)  // initializer
{
  _regs.fill(0);
  _pc = 0;
  _rega = 0;
  _regb = 0;
  _regc = 0;
  _shift_value = 0;
  _func = 0;
  _imm = 0;
  _mmu = mem_ptr;
}

uint16_t EmuCpu::get_pc() { return _pc; }

void EmuCpu::set_pc(uint16_t value) { _pc = value; }

void EmuCpu::set_imm(uint16_t value) { _imm = value; }

uint32_t EmuCpu::instr_fetch() {
  if (_pc == 0) {
    return 0;
  }

  uint32_t out = 0;
  for (uint16_t i = 0; i < 4; i++) {
    out <<= 8;
    out |= (_mmu->mem_read(_pc + i));
  }
  return out;
}

void EmuCpu::instr_execute(uint32_t instr) {
  // if (DEBUG){ std::cout << __PRETTY_FUNCTION__ << std::endl;}
  // get the opcode

  _op = ((instr >> 26) & 0x3F);
  if (_op == 11) {
    _rega = (instr >> 21) & 0x1F;
    _regb = (instr >> 16) & 0x1F;
    _regc = (instr >> 11) & 0x1F;
    _shift_value = (instr >> 6) & 0x1F;
    _func = instr & 0x3F;

    if (DEBUG) {
      std::cout << std::endl;
      std::cout << std::hex << std::setw(4) << std::setfill('0') << _pc
                << std::endl;
      std::cout << std::hex << std::setw(8) << std::setfill('0') << instr
                << std::endl;
      std::cout << "[_rega]: " << _rega << " (" << reg_read(_rega) << ")"
                << std::endl;
      std::cout << "[_regb]: " << _regb << " (" << reg_read(_regb) << ")"
                << std::endl;
      std::cout << "[_regc]: " << _regc << " (" << reg_read(_regc) << ")"
                << std::endl;
      std::cout << "[_shift_value]: " << _shift_value << std::endl;
      std::cout << "[_func]: " << _func << std::endl;
    }

    (this->*_R_instr_set[_func])();
    if (DEBUG) {
      std::cout << std::hex << _pc << std::endl;
    }
    // std::cout << std::hex << _pc << ": " << _rega << ", " << _regb << ", " <<
    // _regc << ", " << _shift_value << ", " << _func << std::endl;
  } else {
    _rega = (instr >> 21) & 0x1F;
    _regb = (instr >> 16) & 0x1F;
    _imm = instr & 0xFFFF;

    if (DEBUG) {
      std::cout << std::endl;
      std::cout << std::hex << std::setw(4) << std::setfill('0') << _pc
                << std::endl;
      std::cout << std::hex << std::setw(8) << std::setfill('0') << instr
                << std::endl;
      std::cout << "[_op]: " << _op << std::endl;
      std::cout << "[_rega]: " << _rega << " (" << reg_read(_rega) << ")"
                << std::endl;
      std::cout << "[_regb]: " << _regb << " (" << reg_read(_regb) << ")"
                << std::endl;
      std::cout << "[_imm]: " << std::hex << std::setw(4) << std::setfill('0')
                << _imm << std::endl;
    }
    (this->*_I_instr_set[_op])();
    if (DEBUG) {
      std::cout << std::hex << _pc << std::endl;
    }
    // std::cout << std::hex << _pc << ": " << _rega << ", " << _regb << ", " <<
    // _imm << std::endl;
  }
}

void EmuCpu::reg_write(int addr, int16_t data) {
  if (addr != 0) {
    _regs[addr] = data;
  }
}

int16_t EmuCpu::reg_read(int addr) { return _regs[addr]; }

//----------------------------------------
//          I-TYPE INSTRUCTIONS
//----------------------------------------

// NO OPERATION
void EmuCpu::NOP() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  set_pc(_pc + 4);
}

// STORE BYTE: op = 0
// M[R[a] + imm] = R[b](7:0)
void EmuCpu::SB() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Storing the value (" << std::hex
              << std::setw(2) << std::setfill('0')
              << ((int8_t)(reg_read(_regb) & 0xFF)) << ") to M["
              << reg_read(_rega) + _imm << "]" << std::endl;
  }
  _mmu->mem_write((reg_read(_rega) + _imm), (int8_t)(reg_read(_regb) & 0xFF));
  set_pc(_pc + 4);
}

// LOAD WORD: op = 4
// R[b] = M[R[a] + imm]
void EmuCpu::LW() {
  uint16_t out = 0;
  uint32_t read_from = reg_read(_rega) + _imm;
  out = (uint16_t)(_mmu->mem_read(read_from) << 8);
  out |= (uint16_t)(_mmu->mem_read(read_from + 1));
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Loading Word from M[" << read_from
              << "] (" << std::hex << std::setw(4) << std::setfill('0') << out
              << ") into R[" << _regb << "]" << std::endl;
  }
  reg_write(_regb, out);
  set_pc(_pc + 4);
}

// JUMP: op = 9
// PC = 4 * imm
void EmuCpu::J() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Jumping to (" << std::hex
              << std::setw(4) << std::setfill('0') << (uint16_t)(4 * _imm)
              << ")" << std::endl;
  }
  set_pc((uint16_t)(4 * _imm));
}

// BRANCH ON EQUAL: op = 25
// if (R[a] == R[b]) PC= PC + 4 + 4 * imm
void EmuCpu::BEQ() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  if (reg_read(_rega) == reg_read(_regb)) {
    set_pc((uint16_t)(_pc + 4 + (4 * _imm)));
  } else {
    set_pc(_pc + 4);
  }
}

// JUMP AND LINK: op = 34
// R[31] = PC+4; PC = 4 * imm
void EmuCpu::JAL() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Jumping to (" << std::hex
              << std::setw(4) << std::setfill('0') << (uint16_t)(4 * _imm)
              << ") and setting R[31] = " << std::hex << std::setw(4)
              << std::setfill('0') << uint16_t(_pc + 4) << std::endl;
  }
  reg_write(31, uint16_t(_pc + 4));
  set_pc((uint16_t)(4 * _imm));
}

// LOAD BYTE UNSIGNED: op = 50
// R[b] = R[a] + imm
void EmuCpu::LBU() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint8_t res = _mmu->mem_read(reg_read(_rega) + _imm);
  reg_write(_regb, (uint8_t)(res & 0xFF));
  set_pc(_pc + 4);
}

// ADD IMMEDIATE: op = 54
// R[b] = R[a] + imm
void EmuCpu::ADDI() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Adding R[" << _rega << "] ("
              << std::hex << std::setw(4) << std::setfill('0')
              << reg_read(_rega) << ") + " << std::hex << std::setw(4)
              << std::setfill('0') << _imm << " and storing it to R[" << _regb
              << "]" << std::endl;
  }
  reg_write(_regb, (reg_read(_rega) + _imm));
  set_pc(_pc + 4);
}

// STORE WORD: op = 61
// M[R[a] + imm] = R[b]
void EmuCpu::SW() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t word = reg_read(_regb);
  _mmu->mem_write((reg_read(_rega) + _imm), (uint8_t)((word & 0xFF00) >> 8));
  _mmu->mem_write((reg_read(_rega) + _imm + 1), (uint8_t)((word & 0x00FF)));
  set_pc(_pc + 4);
}

// BRANCH ON NOT EQUAL: op = 63
// if (R[a] != R[b]) PC=PC + 4 + 4 * imm
void EmuCpu::BNE() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Checking if R[" << _rega << "] ("
              << reg_read(_rega) << ")"
              << " != R[" << _regb << "] (" << reg_read(_regb) << "): ";
  }

  if (reg_read(_rega) != reg_read(_regb)) {
    if (DEBUG) {
      std::cout << "TRUE setting PC = (" << std::hex << std::setw(4)
                << std::setfill('0') << (uint16_t)(_pc + 4 + (4 * _imm)) << ")"
                << std::endl;
    }

    set_pc((uint16_t)(_pc + 4 + 4 * _imm));
  } else {
    if (DEBUG) {
      std::cout << "FALSE setting PC = (" << std::hex << std::setw(4)
                << std::setfill('0') << (uint16_t)(_pc + 4) << ")" << std::endl;
    }

    set_pc(_pc + 4);
  }
}

//----------------------------------------
//          R-TYPE INSTRUCTIONS
//----------------------------------------

// NOR: func = 0
// R[c] = ~(R[a] | R[b])
void EmuCpu::NOR() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = ~(reg_read(_rega) | reg_read(_regb));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// JUMP REGISTER: func = 2
// PC = R[a]
void EmuCpu::JR() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << " --> Jumping to R[a] (" << std::hex
              << std::setw(4) << std::setfill('0') << reg_read(_rega) << ")"
              << std::endl;
  }
  set_pc(reg_read(_rega));
}

// SHIFT RIGHT ARITHMETIC: func = 10
// R[c] = (signed) R[b] >> shift_value
void EmuCpu::SRA() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = ((signed)(reg_read(_regb) >> _shift_value));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// SUBTRACT: func = 16
// R[c] = R[a] - R[b]
void EmuCpu::SUB() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = (reg_read(_rega) - reg_read(_regb));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// OR: func = 27
// R[c] = R[a] | R[b]
void EmuCpu::OR() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = (reg_read(_rega) | reg_read(_regb));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// AND: func = 34
// R[c] = R[a] & R[b]
void EmuCpu::AND() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = (reg_read(_rega) & reg_read(_regb));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// ADD: func = 43
// R[c] = R[a] + R[b]
void EmuCpu::ADD() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = (reg_read(_rega) + reg_read(_regb));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// SET LESS THAN: func = 47
// R[c] = (R[a] < R[b])
void EmuCpu::SLT() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  int16_t res = (reg_read(_rega) < reg_read(_regb));
  // std::cout << res << std::endl;
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// SHIFT LEFT LOGICAL: func = 58
// R[c] = R[a] << shift
void EmuCpu::SLL() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = (reg_read(_regb) << _shift_value);
  reg_write(_regc, res);
  set_pc(_pc + 4);
}

// SHIFT RIGHT LOGICAL: func = 59
// R[c] = unsigned R[a] >> shift
void EmuCpu::SRL() {
  if (DEBUG) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  uint16_t res = ((unsigned)(reg_read(_regb) >> _shift_value));
  reg_write(_regc, res);
  set_pc(_pc + 4);
}
