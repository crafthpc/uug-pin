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
       conversion = 0, control = 0, logical = 0, shuffle = 0, aes = 0;

UINT64 totalUnhandledInstructions = 0;

/*
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

VOID count_aes ()
{
    aes++;
}

/*
 * instrumentation procedure (called when a new instruction is executed for the
 * first time)
 */
VOID instrumentInstruction (INS ins, VOID *v)
{
    // Which increment we call depends on the opcode

    LEVEL_BASE::OPCODE iclass = INS_Opcode(ins);
    switch (iclass) {

    // ADDITION INSTRUCTIONS

    case XED_ICLASS_ADDPD:
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_HADDPD:
    case XED_ICLASS_HADDPS:
    case XED_ICLASS_VADDPD:
    case XED_ICLASS_VADDPS:
    case XED_ICLASS_VADDSD:
    case XED_ICLASS_VADDSS:
    case XED_ICLASS_VFMADDPD:
    case XED_ICLASS_VFMADDPS:
    case XED_ICLASS_VFMADDSD:
    case XED_ICLASS_VFMADDSS:
    case XED_ICLASS_VFMSUBADDPD:
    case XED_ICLASS_VFMSUBADDPS:
    case XED_ICLASS_VFNMADDPD:
    case XED_ICLASS_VFNMADDPS:
    case XED_ICLASS_VFNMADDSD:
    case XED_ICLASS_VFNMADDSS:
    case XED_ICLASS_VHADDPD:
    case XED_ICLASS_VHADDPS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add, IARG_END); break;

    
    // SUBTRACTION INSTRUCTIONS

    case XED_ICLASS_ADDSUBPD:
    case XED_ICLASS_ADDSUBPS:
    case XED_ICLASS_HSUBPD:
    case XED_ICLASS_HSUBPS:
    case XED_ICLASS_SUBPD:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_VADDSUBPD:
    case XED_ICLASS_VADDSUBPS:
    case XED_ICLASS_VFMADDSUBPD:
    case XED_ICLASS_VFMADDSUBPS:
    case XED_ICLASS_VFMSUBPD:
    case XED_ICLASS_VFMSUBPS:
    case XED_ICLASS_VFMSUBSD:
    case XED_ICLASS_VFMSUBSS:
    case XED_ICLASS_VFNMSUBPD:
    case XED_ICLASS_VFNMSUBPS:
    case XED_ICLASS_VFNMSUBSD:
    case XED_ICLASS_VFNMSUBSS:
    case XED_ICLASS_VHSUBPD:
    case XED_ICLASS_VHSUBPS:
    case XED_ICLASS_VSUBPD:
    case XED_ICLASS_VSUBPS:
    case XED_ICLASS_VSUBSD:
    case XED_ICLASS_VSUBSS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub, IARG_END); break;


    // MULTIPLICATION INSTRUCTIONS

    case XED_ICLASS_MULPD:
    case XED_ICLASS_MULPS:
    case XED_ICLASS_MULSD:
    case XED_ICLASS_MULSS:
    case XED_ICLASS_VMULPD:
    case XED_ICLASS_VMULPS:
    case XED_ICLASS_VMULSD:
    case XED_ICLASS_VMULSS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul, IARG_END); break;


    // DIVISION INSTRUCTIONS

    case XED_ICLASS_DIVPD:
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_DIVSS:
    case XED_ICLASS_VDIVPD:
    case XED_ICLASS_VDIVPS:
    case XED_ICLASS_VDIVSD:
    case XED_ICLASS_VDIVSS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div, IARG_END); break;


    // ARITHMETIC INSTRUCTIONS (OTHER)



    // COMPARISON INSTRUCTIONS

    case XED_ICLASS_CMPPD:
    case XED_ICLASS_CMPPS:
    case XED_ICLASS_CMPSD:
    case XED_ICLASS_CMPSD_XMM:
    case XED_ICLASS_CMPSS:
    case XED_ICLASS_VCMPPD:
    case XED_ICLASS_VCMPPS:
    case XED_ICLASS_VCMPSD:
    case XED_ICLASS_VCMPSS:
    case XED_ICLASS_COMISD:
    case XED_ICLASS_COMISS:
    case XED_ICLASS_VCOMISD:
    case XED_ICLASS_VCOMISS:
    case XED_ICLASS_PCMPEQB:
    case XED_ICLASS_PCMPEQD:
    case XED_ICLASS_PCMPEQQ:
    case XED_ICLASS_PCMPEQW:
    case XED_ICLASS_VPCMPEQB:
    case XED_ICLASS_VPCMPEQD:
    case XED_ICLASS_VPCMPEQQ:
    case XED_ICLASS_VPCMPEQW:
    case XED_ICLASS_PCMPESTRI:
    case XED_ICLASS_PCMPESTRM:
    case XED_ICLASS_PCMPISTRI:
    case XED_ICLASS_PCMPISTRM:
    case XED_ICLASS_VPCMPESTRI:
    case XED_ICLASS_VPCMPESTRM:
    case XED_ICLASS_VPCMPISTRI:
    case XED_ICLASS_VPCMPISTRM:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_compare, IARG_END); break;


    // CONTROL INSTRUCTIONS

    case XED_ICLASS_LDMXCSR:
    case XED_ICLASS_STMXCSR:
    case XED_ICLASS_VLDMXCSR:
    case XED_ICLASS_VSTMXCSR:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_control, IARG_END); break;


    // CONVERSION INSTRUCTIONS

    case XED_ICLASS_CVTDQ2PD:
    case XED_ICLASS_CVTDQ2PS:
    case XED_ICLASS_VCVTDQ2PD:
    case XED_ICLASS_VCVTDQ2PS:
    case XED_ICLASS_CVTPS2PD:
    case XED_ICLASS_VCVTPS2PD:
    case XED_ICLASS_CVTSD2SI:
    case XED_ICLASS_CVTSS2SI:
    case XED_ICLASS_VCVTSD2SI:
    case XED_ICLASS_VCVTSS2SI:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_conversion, IARG_END); break;


    // LOAD/STORE INSTRUCTIONS

    case XED_ICLASS_VBROADCASTF128:
    case XED_ICLASS_VBROADCASTF32X2:
    case XED_ICLASS_VBROADCASTF32X4:
    case XED_ICLASS_VBROADCASTF32X8:
    case XED_ICLASS_VBROADCASTF64X2:
    case XED_ICLASS_VBROADCASTF64X4:
    case XED_ICLASS_VBROADCASTSD:
    case XED_ICLASS_VBROADCASTSS:
    case XED_ICLASS_VEXTRACTF128:
    case XED_ICLASS_EXTRACTPS:
    case XED_ICLASS_VEXTRACTPS:
    case XED_ICLASS_CMOVS:
    case XED_ICLASS_KMOVD:
    case XED_ICLASS_MASKMOVDQU:
    case XED_ICLASS_MOVAPD:
    case XED_ICLASS_MOVAPS:
    case XED_ICLASS_MOVD:
    case XED_ICLASS_MOVDQ2Q:
    case XED_ICLASS_MOVDQA:
    case XED_ICLASS_MOVDQU:
    case XED_ICLASS_MOVHPD:
    case XED_ICLASS_MOVHPS:
    case XED_ICLASS_MOVLPD:
    case XED_ICLASS_MOVLPS:
    case XED_ICLASS_MOVNTDQ:
    case XED_ICLASS_MOVNTDQA:
    case XED_ICLASS_MOVNTPD:
    case XED_ICLASS_MOVNTPS:
    case XED_ICLASS_MOVNTQ:
    case XED_ICLASS_MOVNTSD:
    case XED_ICLASS_MOVNTSS:
    case XED_ICLASS_MOVSB:
    case XED_ICLASS_MOVSD:
    case XED_ICLASS_MOVSD_XMM:
    case XED_ICLASS_MOVSHDUP:
    case XED_ICLASS_MOVSLDUP:
    case XED_ICLASS_MOVSQ:
    case XED_ICLASS_MOVSS:
    case XED_ICLASS_MOVSW:
    case XED_ICLASS_MOVSX:
    case XED_ICLASS_MOVSXD:
    case XED_ICLASS_MOVUPD:
    case XED_ICLASS_MOVUPS:
    case XED_ICLASS_PACKSSWB:
    case XED_ICLASS_PACKUSWB:
    case XED_ICLASS_PALIGNR:
    case XED_ICLASS_PEXTRB:
    case XED_ICLASS_PEXTRD:
    case XED_ICLASS_PEXTRQ:
    case XED_ICLASS_PEXTRW:
    case XED_ICLASS_PEXTRW_SSE4:
    case XED_ICLASS_PMOVSXBD:
    case XED_ICLASS_PMOVSXBQ:
    case XED_ICLASS_PMOVSXBW:
    case XED_ICLASS_PMOVSXDQ:
    case XED_ICLASS_PMOVSXWD:
    case XED_ICLASS_PMOVSXWQ:
    case XED_ICLASS_PMOVZXBD:
    case XED_ICLASS_PMOVZXBQ:
    case XED_ICLASS_PMOVZXBW:
    case XED_ICLASS_PMOVZXDQ:
    case XED_ICLASS_PMOVZXWD:
    case XED_ICLASS_PMOVZXWQ:
    case XED_ICLASS_VMASKMOVDQU:
    case XED_ICLASS_VMASKMOVPD:
    case XED_ICLASS_VMASKMOVPS:
    case XED_ICLASS_VMOVAPD:
    case XED_ICLASS_VMOVAPS:
    case XED_ICLASS_VMOVD:
    case XED_ICLASS_VMOVDQA:
    case XED_ICLASS_VMOVDQA32:
    case XED_ICLASS_VMOVDQA64:
    case XED_ICLASS_VMOVDQU:
    case XED_ICLASS_VMOVDQU16:
    case XED_ICLASS_VMOVDQU32:
    case XED_ICLASS_VMOVDQU64:
    case XED_ICLASS_VMOVDQU8:
    case XED_ICLASS_VMOVHPD:
    case XED_ICLASS_VMOVHPS:
    case XED_ICLASS_VMOVLPD:
    case XED_ICLASS_VMOVLPS:
    case XED_ICLASS_VMOVNTDQ:
    case XED_ICLASS_VMOVNTDQA:
    case XED_ICLASS_VMOVNTPD:
    case XED_ICLASS_VMOVNTPS:
    case XED_ICLASS_VMOVSD:
    case XED_ICLASS_VMOVSHDUP:
    case XED_ICLASS_VMOVSLDUP:
    case XED_ICLASS_VMOVSS:
    case XED_ICLASS_VMOVUPD:
    case XED_ICLASS_VMOVUPS:
    case XED_ICLASS_VPACKSSWB:
    case XED_ICLASS_VPACKUSWB:
    case XED_ICLASS_VPALIGNR:
    case XED_ICLASS_VPEXTRB:
    case XED_ICLASS_VPEXTRD:
    case XED_ICLASS_VPEXTRQ:
    case XED_ICLASS_VPEXTRW:
    case XED_ICLASS_VPMASKMOVD:
    case XED_ICLASS_VPMOVD2M:
    case XED_ICLASS_VPMOVDB:
    case XED_ICLASS_VPMOVDW:
    case XED_ICLASS_VPMOVSDB:
    case XED_ICLASS_VPMOVSDW:
    case XED_ICLASS_VPMOVSQB:
    case XED_ICLASS_VPMOVSQD:
    case XED_ICLASS_VPMOVSQW:
    case XED_ICLASS_VPMOVSWB:
    case XED_ICLASS_VPMOVSXBD:
    case XED_ICLASS_VPMOVSXBQ:
    case XED_ICLASS_VPMOVSXBW:
    case XED_ICLASS_VPMOVSXDQ:
    case XED_ICLASS_VPMOVSXWD:
    case XED_ICLASS_VPMOVSXWQ:
    case XED_ICLASS_VPMOVZXBD:
    case XED_ICLASS_VPMOVZXBQ:
    case XED_ICLASS_VPMOVZXBW:
    case XED_ICLASS_VPMOVZXDQ:
    case XED_ICLASS_VPMOVZXWD:
    case XED_ICLASS_VPMOVZXWQ:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_load_store, IARG_END); break;


    // LOGICAL INSTRUCTIONS

    case XED_ICLASS_ANDNPD:
    case XED_ICLASS_ANDNPS:
    case XED_ICLASS_ANDPD:
    case XED_ICLASS_ANDPS:
    case XED_ICLASS_ORPD:
    case XED_ICLASS_ORPS:
    case XED_ICLASS_VANDNPD:
    case XED_ICLASS_VANDNPS:
    case XED_ICLASS_VANDPD:
    case XED_ICLASS_VANDPS:
    case XED_ICLASS_VEXPANDPD:
    case XED_ICLASS_VEXPANDPS:
    case XED_ICLASS_VORPD:
    case XED_ICLASS_VORPS:
    case XED_ICLASS_VXORPD:
    case XED_ICLASS_VXORPS:
    case XED_ICLASS_XORPD:
    case XED_ICLASS_XORPS:
    case XED_ICLASS_POR:
    case XED_ICLASS_PXOR:
    case XED_ICLASS_VPOR:
    case XED_ICLASS_VPORD:
    case XED_ICLASS_VPORQ:
    case XED_ICLASS_VPXOR:
    case XED_ICLASS_VPXORD:
    case XED_ICLASS_VPXORQ:
    case XED_ICLASS_PTEST:
    case XED_ICLASS_UCOMISD:
    case XED_ICLASS_UCOMISS:
    case XED_ICLASS_VPTEST:
    case XED_ICLASS_VPTESTMB:
    case XED_ICLASS_VPTESTMD:
    case XED_ICLASS_VPTESTMQ:
    case XED_ICLASS_VPTESTMW:
    case XED_ICLASS_VPTESTNMB:
    case XED_ICLASS_VPTESTNMD:
    case XED_ICLASS_VPTESTNMQ:
    case XED_ICLASS_VPTESTNMW:
    case XED_ICLASS_VUCOMISD:
    case XED_ICLASS_VUCOMISS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_logical, IARG_END); break;


    // SHUFFLE INSTRUCTIONS

    case XED_ICLASS_BLENDPD:
    case XED_ICLASS_BLENDPS:
    case XED_ICLASS_BLENDVPD:
    case XED_ICLASS_BLENDVPS:
    case XED_ICLASS_MOVDDUP:
    case XED_ICLASS_PBLENDVB:
    case XED_ICLASS_PBLENDW:
    case XED_ICLASS_VBLENDPD:
    case XED_ICLASS_VBLENDPS:
    case XED_ICLASS_VBLENDVPD:
    case XED_ICLASS_VBLENDVPS:
    case XED_ICLASS_VMOVDDUP:
    case XED_ICLASS_VPBLENDVB:
    case XED_ICLASS_VPBLENDW:
    case XED_ICLASS_VPERMILPD:
    case XED_ICLASS_VPERMILPS:
    case XED_ICLASS_PSHUFB:
    case XED_ICLASS_PSHUFD:
    case XED_ICLASS_PSHUFHW:
    case XED_ICLASS_PSHUFLW:
    case XED_ICLASS_PUNPCKHBW:
    case XED_ICLASS_PUNPCKHDQ:
    case XED_ICLASS_PUNPCKHQDQ:
    case XED_ICLASS_PUNPCKHWD:
    case XED_ICLASS_PUNPCKLBW:
    case XED_ICLASS_PUNPCKLDQ:
    case XED_ICLASS_PUNPCKLQDQ:
    case XED_ICLASS_PUNPCKLWD:
    case XED_ICLASS_VPERM2F128:
    case XED_ICLASS_VPSHUFB:
    case XED_ICLASS_VPSHUFD:
    case XED_ICLASS_VPSHUFHW:
    case XED_ICLASS_VPSHUFLW:
    case XED_ICLASS_VPUNPCKHBW:
    case XED_ICLASS_VPUNPCKHDQ:
    case XED_ICLASS_VPUNPCKHQDQ:
    case XED_ICLASS_VPUNPCKHWD:
    case XED_ICLASS_VPUNPCKLBW:
    case XED_ICLASS_VPUNPCKLDQ:
    case XED_ICLASS_VPUNPCKLQDQ:
    case XED_ICLASS_VPUNPCKLWD:
    case XED_ICLASS_SHUFPD:
    case XED_ICLASS_SHUFPS:
    case XED_ICLASS_UNPCKHPD:
    case XED_ICLASS_UNPCKHPS:
    case XED_ICLASS_UNPCKLPD:
    case XED_ICLASS_UNPCKLPS:
    case XED_ICLASS_VSHUFPD:
    case XED_ICLASS_VSHUFPS:
    case XED_ICLASS_VUNPCKHPD:
    case XED_ICLASS_VUNPCKHPS:
    case XED_ICLASS_VUNPCKLPD:
    case XED_ICLASS_VUNPCKLPS:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_shuffle, IARG_END); break;


    // AES INSTRUCTIONS

    case XED_ICLASS_AESDEC:
    case XED_ICLASS_AESDECLAST:
    case XED_ICLASS_AESENC:
    case XED_ICLASS_AESENCLAST:
    case XED_ICLASS_AESIMC:
    case XED_ICLASS_AESKEYGENASSIST:
    case XED_ICLASS_VAESDEC:
    case XED_ICLASS_VAESDECLAST:
    case XED_ICLASS_VAESENC:
    case XED_ICLASS_VAESENCLAST:
    case XED_ICLASS_VAESIMC:
    case XED_ICLASS_VAESKEYGENASSIST:
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_aes, IARG_END); break;


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
    cout << "AES operations         " << aes << endl;
}

int main(int argc, char *argv[])
{
    // initialize Pin
    if (PIN_Init(argc,argv)) {
        cout << "Usage: pin -t <tool name> -- <exefile>" << endl;
        return -1;
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

