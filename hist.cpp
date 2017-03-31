#include "pin.H"
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <vector>
#include <sstream>

UINT64 numInsns = 0;

UINT64 additions = 0, subtractions = 0, multiplications = 0, divisions = 0,
       arithmetic = 0, othArith = 0, compare = 0, load_store = 0,
       conversion = 0, control = 0, logical = 0, shuffle = 0, integer = 0;

UINT64 totalUnhandledInstructions = 0;

/*
    INS_InsertCall(insn, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
 * analysis function (called whenever an instruction is executed)
 */
VOID count_load_store ()
{
    load_store++;
}

VOID count_add ()
{
    additions++;
    arithmetic++;
}

VOID count_sub ()
{
    subtractions++;
    arithmetic++;
}

VOID count_div ()
{
    divisions++;
    arithmetic++;
}

VOID count_mul ()
{
    multiplications++;
    arithmetic++;
}

VOID count_compare ()
{
    compare++;
}

VOID count_othArith ()
{
    othArith++;
    arithmetic++;
}

VOID count_conversion ()
{
    conversion++;
}

VOID count_control ()
{
    control++;
}

VOID count_logical ()
{
    logical++;
}

VOID count_shuffle ()
{
    shuffle++;
}

VOID count_integer ()
{
    integer++;
}

/*
 * instrumentation procedure (called when a new instruction is executed for the
 * first time)
 */
VOID instrumentInstruction (INS ins, VOID *v)
{
    //  Which increment we call depends on the opcode

    LEVEL_BASE::OPCODE iclass = INS_Opcode(ins);
    switch (iclass) {

    // simple data movement 
    /*
     *case XED_ICLASS_MOVSS:
     *    insertMovCall(ins, (AFUNPTR)shadowMovScalarMem32ToReg32,
     *                       (AFUNPTR)shadowMovScalarReg32ToMem32,
     *                       (AFUNPTR)shadowMovScalarReg32ToReg32); break;
     */
    case XED_ICLASS_MOVQ:
    case XED_ICLASS_MOVSD_XMM:
    case XED_ICLASS_MOVLPD:

    case XED_ICLASS_MOVAPD:
    case XED_ICLASS_MOVUPD:
    case XED_ICLASS_MOVNTPD:
    case XED_ICLASS_MOVNTDQ:

    case XED_ICLASS_MOVHPD:



    // complex data movement
    case XED_ICLASS_UNPCKLPD:
    case XED_ICLASS_PUNPCKLQDQ:
    case XED_ICLASS_UNPCKHPD:
    case XED_ICLASS_PUNPCKHQDQ:
    case XED_ICLASS_UNPCKLPS:
    case XED_ICLASS_UNPCKHPS:
    case XED_ICLASS_MOVDDUP: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_load_store, IARG_END); break;
  

    case XED_ICLASS_SHUFPD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_shuffle, IARG_END); break;

    // double-precision binary arithmetic operations
    case XED_ICLASS_ADDPD: 
    case XED_ICLASS_ADDSD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add, IARG_END); break;
    case XED_ICLASS_SUBPD: 
    case XED_ICLASS_SUBSD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub, IARG_END); break;
    case XED_ICLASS_MULPD: 
    case XED_ICLASS_MULSD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul, IARG_END); break;
    case XED_ICLASS_DIVPD: 
    case XED_ICLASS_DIVSD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div, IARG_END); break;
    case XED_ICLASS_MINSD: 
    case XED_ICLASS_MAXSD: 
    case XED_ICLASS_MINPD: 
    case XED_ICLASS_MAXPD:

    // double-precision unary arithmetic operations
    case XED_ICLASS_SQRTSD:  
    case XED_ICLASS_ROUNDSD:
    case XED_ICLASS_SQRTPD:
    case XED_ICLASS_VSQRTPD:
    case XED_ICLASS_ROUNDPD: 
    case XED_ICLASS_VROUNDPD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;

    // Stupid Floating-Point Tricks (TM)
    case XED_ICLASS_ANDPD:
    case XED_ICLASS_PAND:
    case XED_ICLASS_ANDNPD:
    case XED_ICLASS_PANDN:
    case XED_ICLASS_ORPD:
    case XED_ICLASS_POR: 
    case XED_ICLASS_XORPD:
    case XED_ICLASS_PXOR: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_logical, IARG_END); break;

    // conversions
    case XED_ICLASS_CVTSI2SD:
    case XED_ICLASS_CVTSS2SD:
    case XED_ICLASS_CVTPS2PD:
    case XED_ICLASS_CVTDQ2PD: INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_conversion, IARG_END); break;

    // we can safely ignore these (they have no effect on shadow values)
    //
    case XED_ICLASS_COMISS:         // ordered compare scalar single
    case XED_ICLASS_COMISD:         // ordered compare scalar double
    case XED_ICLASS_UCOMISS:        // unordered compare scalar single
    case XED_ICLASS_UCOMISD:        // unordered compare scalar double
    case XED_ICLASS_PCMPEQB:        // compare packed 8-bit ints:  equal-to
    case XED_ICLASS_PCMPEQW:        // compare packed 16-bit ints: equal-to
    case XED_ICLASS_PCMPEQD:        // compare packed 32-bit ints: equal-to
    case XED_ICLASS_PCMPGTB:        // compare packed 8-bit ints:  greater-than
    case XED_ICLASS_PCMPGTW:        // compare packed 16-bit ints: greater-than
    case XED_ICLASS_PCMPGTD:        // compare packed 32-bit ints: greater-than
    case XED_ICLASS_VPCMPEQB:       // compare packed 8-bit ints:  equal-to
    case XED_ICLASS_VPCMPEQW:       // compare packed 16-bit ints: equal-to
    case XED_ICLASS_VPCMPEQD:       // compare packed 32-bit ints: equal-to
    case XED_ICLASS_VPCMPGTB:       // compare packed 8-bit ints:  greater-than
    case XED_ICLASS_VPCMPGTW:       // compare packed 16-bit ints: greater-than
    case XED_ICLASS_VPCMPGTD:       // compare packed 32-bit ints: greater-than
    case XED_ICLASS_PCMPESTRI:      // compare explicit-length strings
    case XED_ICLASS_PCMPISTRI:      // compare implicit-length strings
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_compare, IARG_END); break;


    case XED_ICLASS_STMXCSR:        // save MXCSR register state
    case XED_ICLASS_LDMXCSR:        // load MXCSR register state
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_control, IARG_END); break;


    case XED_ICLASS_FXSAVE:         // save x87/MMX/SSE state
    case XED_ICLASS_FXRSTOR:        // load x87/MMX/SSE state
    case XED_ICLASS_PMOVMSKB:       // move byte mask
    case XED_ICLASS_VPMOVMSKB:      // move byte mask
    case XED_ICLASS_MOVMSKPS:       // move 128-bit mask (single precision)
    case XED_ICLASS_MOVMSKPD:       // move 128-bit mask (double precision)
    case XED_ICLASS_MASKMOVDQU:     // non-temporal mask store to memory
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;


    case XED_ICLASS_PEXTRB:         // extract 8 bits
    case XED_ICLASS_PEXTRW:         // extract 16 bits
    case XED_ICLASS_PEXTRD:         // extract 32 bits
    case XED_ICLASS_PEXTRQ:         // extract 64 bits
    case XED_ICLASS_VPEXTRB:        // extract 8 bits
    case XED_ICLASS_VPEXTRW:        // extract 16 bits
    case XED_ICLASS_VPEXTRD:        // extract 32 bits
    case XED_ICLASS_VPEXTRQ:        // extract 64 bits
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_load_store, IARG_END); break;


    case XED_ICLASS_CVTSS2SI:       // convert single to signed int
    case XED_ICLASS_CVTTSS2SI:      // convert single to signed int (truncated)
    case XED_ICLASS_CVTSD2SI:       // convert double to signed int
    case XED_ICLASS_CVTTSD2SI:      // convert double to signed int (truncated)
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_conversion, IARG_END); break;


/*  NO CATEGORY RIGHT NOW

    case XED_ICLASS_LFENCE:         // serialize loads
    case XED_ICLASS_SFENCE:         // serialize stores
    case XED_ICLASS_MFENCE:         // serialize loads and stores
    case XED_ICLASS_PAUSE:          // improve spin-wait loops
    case XED_ICLASS_PREFETCHT0:     // prefetch data
    case XED_ICLASS_PREFETCHT1:     // prefetch data
    case XED_ICLASS_PREFETCHT2:     // prefetch data
    case XED_ICLASS_PREFETCHW:      // prefetch data
    case XED_ICLASS_PREFETCHNTA:    // prefetch data
    case XED_ICLASS_POPCNT:         // count number of 1 bits
        skipCheck = true;
        break;
*/
    // integer and single-precision operations; we'll mostly ignore these for
    // now except that we do need to invalidate any shadow values that may be
    // affected and inform the user that we're ignoring instructions
    //
    case XED_ICLASS_MOVD:           // 32-bit movement
    case XED_ICLASS_MOVNTI:         // non-temporal 32-bit movement
    case XED_ICLASS_MOVSS:          // single precision movement
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_load_store, IARG_END); break;

    case XED_ICLASS_ADDSS:          // single precision scalar addition
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add, IARG_END); break;

    case XED_ICLASS_SUBSS:          // single precision scalar subtraction
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub, IARG_END); break;

    case XED_ICLASS_MULSS:          // single precision scalar muliplication
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul, IARG_END); break;

    case XED_ICLASS_DIVSS:          // single precision scalar division
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div, IARG_END); break;

    case XED_ICLASS_SQRTSS:         // single precision scalar square root
    case XED_ICLASS_ROUNDSS:        // single precision scalar rounding
    case XED_ICLASS_MINSS:          // single precision scalar minimum
    case XED_ICLASS_MAXSS:          // single precision scalar maximum
    case XED_ICLASS_RCPSS:          // single precision scalar reciprocal
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;

    case XED_ICLASS_CVTSI2SS:       // convert signed int to single precision
    case XED_ICLASS_CVTSD2SS:       // convert double precision to single precision
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_conversion, IARG_END); break;

    case XED_ICLASS_CMPSD_XMM:      // compare scalar double
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_compare, IARG_END); break;

    // packed integer operations
    case XED_ICLASS_PADDB:          // add packed 8-bit integers
    case XED_ICLASS_PADDW:          // add packed 16-bit integers
    case XED_ICLASS_PADDD:          // add packed 32-bit integers
    case XED_ICLASS_PADDQ:          // add packed 64-bit integers
    case XED_ICLASS_PSUBB:          // subtract packed 8-bit integers
    case XED_ICLASS_PSUBW:          // subtract packed 16-bit integers
    case XED_ICLASS_PSUBD:          // subtract packed 32-bit integers
    case XED_ICLASS_PSUBQ:          // subtract packed 64-bit integers
    case XED_ICLASS_PMULUDQ:        // multiply unsigned packed 32-bit integers
    case XED_ICLASS_PMULLD:         // multiply signed packed 32-bit integers
    case XED_ICLASS_VPMULLD:        // multiply signed packed 32-bit integers
    case XED_ICLASS_PMULDQ:         // multiply signed packed 32-bit integers
    case XED_ICLASS_PSLLD:          // logical shift left packed 32-bit integers
    case XED_ICLASS_PSRLD:          // logical shift right packed 32-bit integers
    case XED_ICLASS_PSLLQ:          // logical shift left packed 64-bit integers
    case XED_ICLASS_PSRLQ:          // logical shift right packed 64-bit integers
    case XED_ICLASS_PSLLDQ:         // logical shift left packed 128-bit integers
    case XED_ICLASS_PSRLDQ:         // logical shift right packed 128-bit integers
    case XED_ICLASS_PSRAW:          // arithmetic shift right packed 16-bit integers
    case XED_ICLASS_PSRAD:          // arithmetic shift right packed 32-bit integers
    case XED_ICLASS_VPADDB:         // add packed 8-bit integers
    case XED_ICLASS_VPADDW:         // add packed 16-bit integers
    case XED_ICLASS_VPADDD:         // add packed 32-bit integers
    case XED_ICLASS_VPADDQ:         // add packed 64-bit integers
    case XED_ICLASS_VPSUBB:         // subtract packed 8-bit integers
    case XED_ICLASS_VPSUBW:         // subtract packed 16-bit integers
    case XED_ICLASS_VPSUBD:         // subtract packed 32-bit integers
    case XED_ICLASS_VPSUBQ:         // subtract packed 64-bit integers
    case XED_ICLASS_VPMULUDQ:       // multiply unsigned packed 32-bit integers
    case XED_ICLASS_VPSLLD:         // logical shift left packed 32-bit integers
    case XED_ICLASS_VPSRLD:         // logical shift right packed 32-bit integers
    case XED_ICLASS_VPSLLQ:         // logical shift left packed 64-bit integers
    case XED_ICLASS_VPSRLQ:         // logical shift right packed 64-bit integers
    case XED_ICLASS_VPSLLDQ:        // logical shift left packed 128-bit integers
    case XED_ICLASS_VPSRLDQ:        // logical shift right packed 128-bit integers
    case XED_ICLASS_VPSRAW:         // arithmetic shift right packed 16-bit integers
    case XED_ICLASS_VPSRAD:         // arithmetic shift right packed 32-bit integers
    case XED_ICLASS_PMINUB:         // minimum of packed unsigned 8-bit integers
    case XED_ICLASS_VPMINUB:        // minimum of packed unsigned 8-bit integers
    case XED_ICLASS_MOVDQA:         // move aligned 64-bit integers
    case XED_ICLASS_MOVDQU:         // move unaligned 64-bit integers
    case XED_ICLASS_LDDQU:          // move unaligned 128 bits
    case XED_ICLASS_PSHUFB:         // shuffle 8-bit integers
    case XED_ICLASS_PSHUFW:         // shuffle 16-bit integers
    case XED_ICLASS_PSHUFD:         // shuffle 32-bit integers (see above)
    case XED_ICLASS_VPSHUFB:        // shuffle 8-bit integers
    case XED_ICLASS_VPSHUFD:        // shuffle 32-bit integers (see above)
    case XED_ICLASS_PUNPCKLBW:      // interleave low 16-bit integers
    case XED_ICLASS_PUNPCKHBW:      // interleave high 16-bit integers
    case XED_ICLASS_PUNPCKLDQ:      // interleave low 64-bit integers
    case XED_ICLASS_PUNPCKHDQ:      // interleave high 64-bit integers
    case XED_ICLASS_MOVAPS:         // move aligned packed 32-bit floats
    case XED_ICLASS_MOVUPS:         // move unaligned packed 32-bit floats
    case XED_ICLASS_PINSRB:         // insert 8 bits
    case XED_ICLASS_PINSRW:         // insert 16 bits
    case XED_ICLASS_PINSRD:         // insert 32 bits
    case XED_ICLASS_PINSRQ:         // insert 64 bits
    case XED_ICLASS_VPINSRB:        // insert 8 bits
    case XED_ICLASS_VPINSRW:        // insert 16 bits
    case XED_ICLASS_VPINSRD:        // insert 32 bits
    case XED_ICLASS_VPINSRQ:        // insert 64 bits
    case XED_ICLASS_PALIGNR:        // align right
    case XED_ICLASS_VPALIGNR:       // align right
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_integer, IARG_END); break;

    // single-precision operations
    case XED_ICLASS_ADDPS:          // single precision packed addition
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add, IARG_END); break;
    case XED_ICLASS_SUBPS:          // single precision packed subtraction
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub, IARG_END); break;
    case XED_ICLASS_MULPS:          // single precision packed muliplication
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul, IARG_END); break;
    case XED_ICLASS_DIVPS:          // single precision packed division
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div, IARG_END); break;
    case XED_ICLASS_SQRTPS:         // single precision packed square root
    case XED_ICLASS_ROUNDPS:        // single precision packed rounding
    case XED_ICLASS_MINPS:          // single precision packed minimum
    case XED_ICLASS_MAXPS:          // single precision packed maximum
    case XED_ICLASS_RCPPS:          // single precision packed reciprocal
    case XED_ICLASS_VRCPPS:         // single precision packed reciprocal
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;
    case XED_ICLASS_ANDPS:          // single precision packed bitwise AND
    case XED_ICLASS_ANDNPS:         // single precision packed bitwise ANDN
    case XED_ICLASS_ORPS:           // single precision packed bitwise OR
    case XED_ICLASS_XORPS:          // single precision packed bitwise XOR
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_logical, IARG_END); break;
    case XED_ICLASS_SHUFPS:         // single precision shuffle
    case XED_ICLASS_VSHUFPS:        // single precision shuffle
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_shuffle, IARG_END); break;
    case XED_ICLASS_PCMPESTRM:      // compare explicit-length strings (save in XMM)
    case XED_ICLASS_PCMPISTRM:      // compare implicit-length strings (save in XMM)
    case XED_ICLASS_CMPPD:          // compare scalar double
    case XED_ICLASS_VCMPPD:         // compare scalar double
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_compare, IARG_END); break;
    case XED_ICLASS_CVTPS2DQ:       // convert packed singles to signed ints
    case XED_ICLASS_CVTTPS2DQ:      // convert packed singles to signed ints (truncated)
    case XED_ICLASS_CVTDQ2PS:       // convert signed ints to packed singles
    case XED_ICLASS_CVTPD2DQ:       // convert packed doubles to signed ints
    case XED_ICLASS_CVTTPD2DQ:      // convert packed doubles to signed ints (truncated)
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_conversion, IARG_END); break;

    // TODO: properly implement support for these AVX instructions
    // (for now, just reset the shadow values for the destination)
    //
    case XED_ICLASS_VADDSD: case XED_ICLASS_VADDPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add, IARG_END); break;
    case XED_ICLASS_VSUBSD: case XED_ICLASS_VSUBPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub, IARG_END); break;
    case XED_ICLASS_VMULSD: case XED_ICLASS_VMULPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul, IARG_END); break;
    case XED_ICLASS_VDIVSD: case XED_ICLASS_VDIVPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div, IARG_END); break;
    case XED_ICLASS_VMINSD: case XED_ICLASS_VMINPD:
    case XED_ICLASS_VMAXSD: case XED_ICLASS_VMAXPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;
    case XED_ICLASS_VUNPCKLPD: case XED_ICLASS_VUNPCKHPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_load_store, IARG_END); break;
    case XED_ICLASS_VSHUFPD:
    case XED_ICLASS_VBLENDVPD:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_shuffle, IARG_END); break;
    case XED_ICLASS_VZEROUPPER:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_othArith, IARG_END); break;

    // flag anything we didn't know how to handle
    default:
        LOG("WARNING - unhandled instruction: " + INS_Disassemble(ins)
            + " (" + hexstr(INS_Address(ins)) + ")\n");
        totalUnhandledInstructions++;
        break;

    } // (END GIANT SWITCH STATEMENT)

}

/*
 * output procedure (called when program exits)
 */
VOID printResults (INT32 code, VOID *v)
{
    cout << "ARITHMETIC operations: " << arithmetic << endl;
    cout << "  Additions:       " << additions << endl;
    cout << "  Subtractions     " << subtractions <<  endl;
    cout << "  Multiplications  " << multiplications << endl;
    cout << "  Divisions        " << divisions << endl;
    cout << "  Other            " << othArith << endl;
    cout << "COMPARISON operations: " << compare << endl;
    cout << "CONTROL operations:    " << control << endl;
    cout << "CONVERSION operations  " << conversion << endl;
    cout << "LOAD/STORE operarions  " << load_store << endl;
    cout << "LOGICAL operations     " << logical << endl;
    cout << "SHUFFLE operations     " << shuffle << endl;
    cout << "INTEGER operations     " << integer << endl;
}

int main(int argc, char *argv[])
{
    // initialize Pin
    if (PIN_Init(argc,argv)) {
        cout << "Usage: pin -t <tool name> -- <exefile>" << endl;
//return -1;
    }
    PIN_InitSymbols();
    PIN_SetSyntaxATT();

    // install Pin callbacks
    INS_AddInstrumentFunction(instrumentInstruction, 0);
    PIN_AddFiniFunction(printResults, 0);

    // start the program (never returns!)
    PIN_StartProgram();

    return 0;
}

