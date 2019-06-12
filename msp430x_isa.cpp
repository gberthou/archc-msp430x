#include <iostream>

#include  "msp430x_isa.H"
#include  "msp430x_isa_init.cpp"
#include  "msp430x_bhv_macros.H"

#define REG_PC 0

//!'using namespace' statement to allow access to all msp430x-specific datatypes
using namespace msp430x_parms;

enum addressing_mode_e
{
    AM_REGISTER = 0,
    AM_INDEXED = 1,
    AM_INDIRECT_REG = 2,
    AM_INDIRECT_INCR = 3,
    AM_INVALID
};

static uint16_t doubleop_source(
    ac_memport<msp430x_parms::ac_word, msp430x_parms::ac_Hword>& DM,
    ac_regbank<16, msp430x_parms::ac_word, msp430x_parms::ac_Dword>& RB,
    ac_reg<unsigned> &ac_pc,
    uint16_t as, uint16_t bw, uint16_t rsrc)
{
    uint16_t operand;

    switch(as)
    {
        case AM_REGISTER:
            operand = RB[rsrc];
            if(bw)
                operand &= 0xff;
            break;

        case AM_INDEXED:
        {
            int16_t x = DM.read(ac_pc + 2);
            if(bw)
                operand = DM.read_byte(RB[rsrc] + x);
            else
                operand = DM.read(RB[rsrc] + x);
            RB[REG_PC] += 2;
            break;
        }

        case AM_INDIRECT_REG:
            if(bw)
                operand = DM.read_byte(RB[rsrc]);
            else
                operand = DM.read(RB[rsrc]);
            break;

        case AM_INDIRECT_INCR:
            operand = DM.read(RB[rsrc]);
            std::cout << "tmp=" << std::hex << RB[rsrc] << std::endl;
            // /!\ Here, pc may change if rsrc==0, which is the expected behavior
            // TODO: 20bit address mode?
            if(rsrc == REG_PC || !bw)
                RB[rsrc] += 2;
            else
                RB[rsrc] += 1;
            break;

        default:
            // Oops
            break;
    }
    ac_pc = RB[REG_PC];

    return operand;
}

static void doubleop_dest(
    ac_memport<msp430x_parms::ac_word, msp430x_parms::ac_Hword>& DM,
    ac_regbank<16, msp430x_parms::ac_word, msp430x_parms::ac_Dword>& RB,
    ac_reg<unsigned> &ac_pc,
    uint16_t operand,
    uint16_t ad, uint16_t bw, uint16_t rdst)
{
    switch(ad)
    {
        case AM_REGISTER:
            RB[rdst] = operand;
            break;

        case AM_INDEXED:
        {
            int16_t x = DM.read(ac_pc + 2);
            if(bw)
                DM.write_byte(RB[rdst] + x, operand);
            else
                DM.write(RB[rdst] + x, operand);
            RB[REG_PC] += 2;
            break;
        }

        default:
            // Oops
            break;
    }
    ac_pc = RB[REG_PC];
}

//!Behavior executed before simulation begins.
void ac_behavior( begin ){}

//!Behavior executed after simulation ends.
void ac_behavior( end ){}

//!Generic instruction behavior method.
void ac_behavior( instruction )
{
    std::cout << "pc=" << std::hex << ac_pc << std::endl;
    ac_pc += 2;
    RB[REG_PC] = ac_pc;
}
 
//! Instruction Format behavior methods.
void ac_behavior( Type_DoubleOp ) {}
void ac_behavior( Type_SimpleOp ) {}
void ac_behavior( Type_Jump ) {}
void ac_behavior( Type_PushPopM ){}
 
//!Instruction MOV behavior method.
void ac_behavior( MOV )
{
    uint16_t operand = doubleop_source(DM, RB, ac_pc, as, bw, rsrc);

    // rdst, as, bw, ad, rsrc, op
    printf("rdst=%x\nas=%x\nbw=%x\nad=%x\nrsrc=%x\n", rdst, as, bw, ad, rsrc);

    std::cout << "operand=" << std::hex << operand << std::endl;

    if(bw)
        operand &= 0xff;

    doubleop_dest(DM, RB, ac_pc, operand, ad, bw, rdst);
}

//!Instruction ADD behavior method.
void ac_behavior( ADD ){}

//!Instruction ADDC behavior method.
void ac_behavior( ADDC ){}

//!Instruction SUB behavior method.
void ac_behavior( SUB ){}

//!Instruction SUBC behavior method.
void ac_behavior( SUBC ){}

//!Instruction CMP behavior method.
void ac_behavior( CMP ){}

//!Instruction DADD behavior method.
void ac_behavior( DADD ){}

//!Instruction BIT behavior method.
void ac_behavior( BIT ){}

//!Instruction BIC behavior method.
void ac_behavior( BIC ){}

//!Instruction BIS behavior method.
void ac_behavior( BIS )
{
    uint16_t operand_src = doubleop_source(DM, RB, ac_pc, as, bw, rsrc);
    
    // rdst, as, bw, ad, rsrc, op
    printf("rdst=%x\nas=%x\nbw=%x\nad=%x\nrsrc=%x\n", rdst, as, bw, ad, rsrc);

    std::cout << "operand=" << std::hex << operand_src << std::endl;
}

//!Instruction XOR behavior method.
void ac_behavior( XOR ){}

//!Instruction AND behavior method.
void ac_behavior( AND ){}

//!Instruction RRC behavior method.
void ac_behavior( RRC ){}

//!Instruction RRA behavior method.
void ac_behavior( RRA ){}

//!Instruction PUSH behavior method.
void ac_behavior( PUSH ){}

//!Instruction SWPB behavior method.
void ac_behavior( SWPB ){}

//!Instruction CALL behavior method.
void ac_behavior( CALL ){}

//!Instruction RETI behavior method.
void ac_behavior( RETI ){}

//!Instruction SXT behavior method.
void ac_behavior( SXT ){}

//!Instruction JZ behavior method.
void ac_behavior( JZ ){}

//!Instruction JNZ behavior method.
void ac_behavior( JNZ ){}

//!Instruction JC behavior method.
void ac_behavior( JC ){}

//!Instruction JNC behavior method.
void ac_behavior( JNC ){}

//!Instruction JN behavior method.
void ac_behavior( JN ){}

//!Instruction JGE behavior method.
void ac_behavior( JGE ){}

//!Instruction JL behavior method.
void ac_behavior( JL ){}

//!Instruction JMP behavior method.
void ac_behavior( JMP ){}

//!Instruction PUSHPOPM behavior method.
void ac_behavior( PUSHPOPM ){}

