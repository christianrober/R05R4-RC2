.file "arch/blackfin/kernel/asm-offsets.c";
.text;
	.align 4
.global _main;
.type _main, STT_FUNC;
_main:
	
->TASK_STATE 0 offsetof(struct task_struct, state)
	
->TASK_FLAGS 12 offsetof(struct task_struct, flags)
	
->TASK_PTRACE 16 offsetof(struct task_struct, ptrace)
	
->TASK_BLOCKED 484 offsetof(struct task_struct, blocked)
	
->TASK_THREAD 444 offsetof(struct task_struct, thread)
	
->TASK_THREAD_INFO 4 offsetof(struct task_struct, thread_info)
	
->TASK_MM 116 offsetof(struct task_struct, mm)
	
->TASK_ACTIVE_MM 120 offsetof(struct task_struct, active_mm)
	
->TASK_SIGPENDING 500 offsetof(struct task_struct, pending)
	
->CPUSTAT_SOFTIRQ_PENDING 0 offsetof(irq_cpustat_t, __softirq_pending)
	
->THREAD_KSP 0 offsetof(struct thread_struct, ksp)
	
->THREAD_USP 4 offsetof(struct thread_struct, usp)
	
->THREAD_SR 8 offsetof(struct thread_struct, seqstat)
	
->PT_SR 8 offsetof(struct thread_struct, seqstat)
	
->THREAD_ESP0 12 offsetof(struct thread_struct, esp0)
	
->THREAD_PC 16 offsetof(struct thread_struct, pc)
	
->KERNEL_STACK_SIZE 8192 THREAD_SIZE
	
->PT_ORIG_P0 212 offsetof(struct pt_regs, orig_p0)
	
->PT_ORIG_PC 0 offsetof(struct pt_regs, orig_pc)
	
->PT_R0 204 offsetof(struct pt_regs, r0)
	
->PT_R1 200 offsetof(struct pt_regs, r1)
	
->PT_R2 196 offsetof(struct pt_regs, r2)
	
->PT_R3 192 offsetof(struct pt_regs, r3)
	
->PT_R4 188 offsetof(struct pt_regs, r4)
	
->PT_R5 184 offsetof(struct pt_regs, r5)
	
->PT_R6 180 offsetof(struct pt_regs, r6)
	
->PT_R7 176 offsetof(struct pt_regs, r7)
	
->PT_P0 172 offsetof(struct pt_regs, p0)
	
->PT_P1 168 offsetof(struct pt_regs, p1)
	
->PT_P2 164 offsetof(struct pt_regs, p2)
	
->PT_P3 160 offsetof(struct pt_regs, p3)
	
->PT_P4 156 offsetof(struct pt_regs, p4)
	
->PT_P5 152 offsetof(struct pt_regs, p5)
	
->PT_FP 148 offsetof(struct pt_regs, fp)
	
->PT_USP 144 offsetof(struct pt_regs, usp)
	
->PT_I0 140 offsetof(struct pt_regs, i0)
	
->PT_I1 136 offsetof(struct pt_regs, i1)
	
->PT_I2 132 offsetof(struct pt_regs, i2)
	
->PT_I3 128 offsetof(struct pt_regs, i3)
	
->PT_M0 124 offsetof(struct pt_regs, m0)
	
->PT_M1 120 offsetof(struct pt_regs, m1)
	
->PT_M2 116 offsetof(struct pt_regs, m2)
	
->PT_M3 112 offsetof(struct pt_regs, m3)
	
->PT_L0 108 offsetof(struct pt_regs, l0)
	
->PT_L1 104 offsetof(struct pt_regs, l1)
	
->PT_L2 100 offsetof(struct pt_regs, l2)
	
->PT_L3 96 offsetof(struct pt_regs, l3)
	
->PT_B0 92 offsetof(struct pt_regs, b0)
	
->PT_B1 88 offsetof(struct pt_regs, b1)
	
->PT_B2 84 offsetof(struct pt_regs, b2)
	
->PT_B3 80 offsetof(struct pt_regs, b3)
	
->PT_A0X 76 offsetof(struct pt_regs, a0x)
	
->PT_A0W 72 offsetof(struct pt_regs, a0w)
	
->PT_A1X 68 offsetof(struct pt_regs, a1x)
	
->PT_A1W 64 offsetof(struct pt_regs, a1w)
	
->PT_LC0 60 offsetof(struct pt_regs, lc0)
	
->PT_LC1 56 offsetof(struct pt_regs, lc1)
	
->PT_LT0 52 offsetof(struct pt_regs, lt0)
	
->PT_LT1 48 offsetof(struct pt_regs, lt1)
	
->PT_LB0 44 offsetof(struct pt_regs, lb0)
	
->PT_LB1 40 offsetof(struct pt_regs, lb1)
	
->PT_ASTAT 36 offsetof(struct pt_regs, astat)
	
->PT_RESERVED 32 offsetof(struct pt_regs, reserved)
	
->PT_RETS 28 offsetof(struct pt_regs, rets)
	
->PT_PC 24 offsetof(struct pt_regs, pc)
	
->PT_RETX 20 offsetof(struct pt_regs, retx)
	
->PT_RETN 16 offsetof(struct pt_regs, retn)
	
->PT_RETE 12 offsetof(struct pt_regs, rete)
	
->PT_SEQSTAT 8 offsetof(struct pt_regs, seqstat)
	
->PT_SYSCFG 216 offsetof(struct pt_regs, syscfg)
	
->PT_IPEND 4 offsetof(struct pt_regs, ipend)
	
->SIZEOF_PTREGS 220 sizeof(struct pt_regs)
	
->PT_EXTRA1 220 sizeof(struct pt_regs)
	
->PT_EXTRA2 224 4 + sizeof(struct pt_regs)
	
->PT_EXTRA3 228 8 + sizeof(struct pt_regs)
	
->SIGSEGV 11 SIGSEGV
	
->SIGTRAP 5 SIGTRAP
	R0 = 0 (X);
	rts;
	.size	_main, .-_main
	.ident	"GCC: (GNU) 3.4.4 (Blackfin 05R4 20051205)"
