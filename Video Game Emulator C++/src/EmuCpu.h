#pragma once
#include <iostream>
#include <array>
#include <cstdint>
#include <memory>
#include "Emu.h"

#define NUM_REGS 32

class EmuCpu
{
    private:
        std::array<int16_t, NUM_REGS> _regs;
        uint16_t _pc;
        uint32_t _op, _shift_value, _func;
        int16_t _imm, _rega, _regb, _regc;
        typedef void(EmuCpu::*Func)();

        Func _I_instr_set[64] = {   
            &EmuCpu::SB,  &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::LW,  &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::J,   &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::BEQ, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::JAL, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::LBU, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::ADDI, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::SW,  &EmuCpu::NOP, &EmuCpu::BNE};
        
        Func _R_instr_set[64] = {
            &EmuCpu::NOR, &EmuCpu::NOP, &EmuCpu::JR,  &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::SRA, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::SUB, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::OR,  &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::AND, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::ADD, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::SLT,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP,
            &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::SLL, &EmuCpu::SRL, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP, &EmuCpu::NOP};    
        
        EmuMmu *_mmu;


    public:
            EmuCpu(void);
            EmuCpu(EmuMmu *mem_ptr);
            void reg_write(int addr, int16_t data);    //Write a uint16_t value to a register at addr
            int16_t reg_read(int addr);                // Read the uint16_t value from a register at addr
            uint16_t get_pc (void);
            void set_pc(uint16_t value);
            void set_imm(uint16_t value);
            uint32_t instr_fetch();
            void instr_execute(uint32_t instr);
            
            void NOP();                                 //No Operation (Default Instr) -- (TODO)

            //I-Type Instructions
            void SB(); // Store Byte
            void LW(); // Load Word
            void J(); // Jump
            void BEQ(); //Branch On Equal
            void JAL(); //Jump And Link
            void LBU(); //Load Byte Unsigned
            void ADDI(); //Add Immediate
            void SW(); //Store Word
            void BNE(); //Branch On Not Equal

            //R-Type Instructions
            void NOR(); //Nor -- (TODO)
            void JR(); //Jump Register -- (TODO)
            void SRA(); //Shift Right Arithmatic -- (TODO)
            void SUB(); //Subtract -- (TODO)
            void OR(); //Or -- (TODO)
            void AND(); //And -- (TODO)
            void ADD(); //Add -- (TODO)
            void SLT(); //Set Less Than -- (TODO)
            void SLL(); //Shift Less Logical -- (TODO)
            void SRL(); //Shift Right Logical -- (TODO) 
        
};  