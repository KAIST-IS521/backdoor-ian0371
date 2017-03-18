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

//#define DEBUG

extern bool is_running;

bool is_superuser = false;

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    switch(i) {
    case 0x00: case 0x10: case 0x20:
    case 0x30: case 0x40: case 0x50:
    case 0x60: case 0x70: case 0x80:
    case 0x90: case 0xa0: case 0xb0:
    case 0xc0: case 0xd0:
        (*ctx->funtable[i])(ctx, instr);
        break;
    default:
        printf("Illegal Instruction\n");
        exit(1);
    }   
}

// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->pc         = 0;
    ctx->opcode     = 0;
    ctx->codesize   = 0;
    memset(ctx->heap, 0, HEAP_SIZE);
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = *ctx->pc;

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);

    // Increment to next instruction.
    ctx->pc++;
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
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t src = EXTRACT_B2(instr);

    uint32_t addr = ctx->r[src].value;

    if(addr >= HEAP_SIZE) {
        printf("[Error] Segmentation Fault: "
        "Heap Address Access Violation\n");
        exit(1);
    }
    
    ctx->r[dst].value = ctx->heap[addr];
#ifdef DEBUG
    printf("Load %x, %x\n", dst, src);
    printf("heap[%x]: %x\n", addr, ctx->heap[addr]);
    printf("r%x: %x\n", dst, ctx->r[dst].value);
#endif
}

// store r0, r1: heap[r0] = r1
void store(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t src = EXTRACT_B2(instr);

    uint32_t addr = ctx->r[dst].value;

    if(addr >= HEAP_SIZE) {
        printf("[Error] Segmentation Fault: "
        "Heap Address Access Violation\n");
        exit(1);
    }
    
    ctx->heap[addr] = ctx->r[src].value;
#ifdef DEBUG
    printf("Store %x, %x\n", dst, src);
    printf("r%x: %x\n", src, ctx->r[src].value);
    printf("heap[%x]: %x\n", addr, ctx->heap[addr]);
#endif
}

// move r0, r1: r0 = r1
void move(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t src = EXTRACT_B2(instr);
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
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t src = EXTRACT_B2(instr);
    ctx->r[dst].value = src;
#ifdef DEBUG
    printf("PutI %x, %x\n", dst, src);
    printf("r%x: %x\n", dst, ctx->r[dst].value);
#endif
}

// add r0, r1, r2: r0 = r1 + r2
void add(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t op1 = EXTRACT_B2(instr);
    uint8_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value =
        ctx->r[op1].value + ctx->r[op2].value;
}

// sub r0, r1, r2: r0 = r1 - r2
void sub(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t op1 = EXTRACT_B2(instr);
    uint8_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value =
        ctx->r[op1].value - ctx->r[op2].value;
}

// gt r0, r1, r2: r0 = r1 > r2? 1 : 0
void gt(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t op1 = EXTRACT_B2(instr);
    uint8_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value =
        ctx->r[op1].value > ctx->r[op2].value ? 1 : 0;
}

// ge r0, r1, r2: r0 = r1 >= r2? 1 : 0
void ge(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t op1 = EXTRACT_B2(instr);
    uint8_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value =
        ctx->r[op1].value >= ctx->r[op2].value ? 1 : 0;
}

// eq r0, r1, r2: r0 = r1 == r2? 1 : 0
void eq(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t op1 = EXTRACT_B2(instr);
    uint8_t op2 = EXTRACT_B3(instr);

    ctx->r[dst].value =
        ctx->r[op1].value == ctx->r[op2].value ? 1 : 0;
}

// ite r0, i0, i1: pc = r0 > 0? i0 : i1
void ite(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t dst = EXTRACT_B1(instr);
    uint8_t addr1 = EXTRACT_B2(instr);
    uint8_t addr2 = EXTRACT_B3(instr);

    if(addr1 * 4 >= ctx->codesize) {
        printf("[Error] Segmentation Fault: "
        "Code Address Access Violation\n");
        exit(1);
    }

    if(addr2 * 4 >= ctx->codesize) {
        printf("[Error] Segmentation Fault: "
        "Code Address Access Violation\n");
        exit(1);
    }

    ctx->pc = ctx->r[dst].value > 0 ?
              (uint32_t *)ctx->opcode + addr1:
              (uint32_t *)ctx->opcode + addr2;
    ctx->pc--; // to be consistent with stepVMContext
}

// jump i0: pc = i0
void jump(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t addr = EXTRACT_B1(instr);

    if(addr * 4 >= ctx->codesize) {
        printf("[Error] Segmentation Fault: "
        "Code Address Access Violation\n");
        exit(1);
    }

    ctx->pc = (uint32_t *)ctx->opcode + addr;
    ctx->pc--; // to be consistent with stepVMContext
}

void vm_puts(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t reg = EXTRACT_B1(instr);

    uint32_t addr = ctx->r[reg].value;
    if(addr >= HEAP_SIZE) {
        printf("[Error] Segmentation Fault: "
        "Heap Address Access Violation\n");
        exit(1);
    }
    if(is_superuser && 
        strcmp("Success\n",&ctx->heap[addr])!=0) return;
        // print nothing other than Success
    printf("%s", (char *)(ctx->heap + addr));
}

void vm_gets(struct VMContext* ctx, const uint32_t instr)
{
    uint8_t reg = EXTRACT_B1(instr);

    uint32_t i;
    uint8_t ch;

    if(is_superuser) {
        // don't ask for password, backdoor will fill it instead
        memcpy(&ctx->heap[ctx->r[reg].value], "password\x00", 9);
        return;
    }

    for(i = ctx->r[reg].value; (ch = getchar()) != '\n'; ++i) {
        if(i >= HEAP_SIZE - 1) {
            printf("[Error] Segmentation Fault: "
            "Heap Address Access Violation\n");
            exit(1);
        }
        ctx->heap[i] = ch;
    }
    ctx->heap[i] = '\0';
    if(strcmp("superuser", &ctx->heap[ctx->r[reg].value])==0) {
        // don't ask for username, backdoor will fill it instead
        is_superuser = true;
        memcpy(&ctx->heap[ctx->r[reg].value], "user\x00", 5);
    }
}

