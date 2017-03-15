//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "minivm.h"

#define DEBUG

extern bool is_running;

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    (*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->opcode     = 0;
    ctx->codesize   = 0;
    memset(ctx->heap, 0, HEAP_SIZE);
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = **pc;

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);

    // Increment to next instruction.
    (*pc)++;
}



//---------------------------------------------------------
// OPCODES

// halt: halt current proces
void halt(struct VMContext* ctx __attribute__((unused)),
          const uint32_t instr __attribute__((unused)))
{
    is_running = false;
#ifdef DEBUG
    printf("Halt\n");
#endif
}

// load r0, r1: r0 = heap[r1]
void load(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t src = EXTRACT_B2(instr);
    
    ctx->r[dst].value = ctx->heap[ctx->r[src].value];
#ifdef DEBUG
    printf("Load %x, %x\n", dst, src);
    printf("heap[%x]: %x\n", ctx->r[src].value, ctx->heap[ctx->r[src].value]);
    printf("r%x: %x\n", dst, ctx->r[dst].value);
#endif
}

// store r0, r1: heap[r0] = r1
void store(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t src = EXTRACT_B2(instr);
    
    ctx->heap[ctx->r[dst].value] = ctx->r[src].value;
#ifdef DEBUG
    printf("Store %x, %x\n", dst, src);
    printf("r%x: %x\n", src, ctx->r[src].value);
    printf("heap[%x]: %x\n", ctx->r[dst].value, ctx->heap[ctx->r[dst].value]);
#endif
}

// move r0, r1: r0 = r1
void move(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t src = EXTRACT_B2(instr);
    ctx->r[dst].value = ctx->r[src].value;
#ifdef DEBUG
    printf("Move %x, %x\n", dst, src);
    printf("r%x: %x\n", src, ctx->r[src].value);
    printf("r%x: %x\n", dst, ctx->r[dst].value);
#endif
}

// puti r0, i0: r0 = i0
void puti(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t src = EXTRACT_B2(instr);
    ctx->r[dst].value = src;
#ifdef DEBUG
    printf("PutI %x, %x\n", dst, src);
    printf("r%x: %x\n", dst, ctx->r[dst].value);
#endif
}

// add r0, r1, r2: r0 = r1 + r2
void add(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t op1 = EXTRACT_B2(instr);
    uint32_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value = ctx->r[op1].value + ctx->r[op2].value;
}

// sub r0, r1, r2: r0 = r1 - r2
void sub(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t op1 = EXTRACT_B2(instr);
    uint32_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value = ctx->r[op1].value - ctx->r[op2].value;
}

// gt r0, r1, r2: r0 = r1 > r2? 1 : 0
void gt(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t op1 = EXTRACT_B2(instr);
    uint32_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value = ctx->r[op1].value > ctx->r[op2].value ? 1 : 0;
}

// ge r0, r1, r2: r0 = r1 >= r2? 1 : 0
void ge(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t op1 = EXTRACT_B2(instr);
    uint32_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value = ctx->r[op1].value >= ctx->r[op2].value ? 1 : 0;
}

// eq r0, r1, r2: r0 = r1 == r2? 1 : 0
void eq(struct VMContext* ctx, const uint32_t instr)
{
    uint32_t dst = EXTRACT_B1(instr);
    uint32_t op1 = EXTRACT_B2(instr);
    uint32_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value = ctx->r[op1].value == ctx->r[op2].value ? 1 : 0;
}

