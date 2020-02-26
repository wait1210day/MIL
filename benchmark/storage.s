	.file	"storage.cc"
	.text
	.p2align 4
	.globl	_Z15__transfer_unitRdi
	.type	_Z15__transfer_unitRdi, @function
_Z15__transfer_unitRdi:
.LFB1610:
	.cfi_startproc
	movsd	(%rdi), %xmm0
	movsd	.LC0(%rip), %xmm1
	comisd	%xmm0, %xmm1
	ja	.L7
	cmpl	$5, %esi
	jg	.L7
	movsd	.LC1(%rip), %xmm2
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L10:
	cmpl	$6, %esi
	je	.L12
.L3:
	mulsd	%xmm2, %xmm0
	addl	$1, %esi
	comisd	%xmm0, %xmm1
	jbe	.L10
	movsd	%xmm0, (%rdi)
	movl	%esi, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L12:
	movsd	%xmm0, (%rdi)
	movl	$6, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE1610:
	.size	_Z15__transfer_unitRdi, .-_Z15__transfer_unitRdi
	.p2align 4
	.globl	_Z14calculate_timeR7timevalS0_
	.type	_Z14calculate_timeR7timevalS0_, @function
_Z14calculate_timeR7timevalS0_:
.LFB1612:
	.cfi_startproc
	pxor	%xmm0, %xmm0
	movsd	.LC2(%rip), %xmm2
	pxor	%xmm1, %xmm1
	cvtsi2sdq	8(%rsi), %xmm0
	cvtsi2sdq	(%rsi), %xmm1
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	pxor	%xmm1, %xmm1
	cvtsi2sdq	8(%rdi), %xmm1
	divsd	%xmm2, %xmm1
	pxor	%xmm2, %xmm2
	cvtsi2sdq	(%rdi), %xmm2
	addsd	%xmm2, %xmm1
	subsd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE1612:
	.size	_Z14calculate_timeR7timevalS0_, .-_Z14calculate_timeR7timevalS0_
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"direct_disk_io_time: open"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC4:
	.string	"Failed in direct_disk_io_time()"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB5:
	.text
.LHOTB5:
	.p2align 4
	.globl	_Z19direct_disk_io_timePhmmPKc
	.type	_Z19direct_disk_io_timePhmmPKc, @function
_Z19direct_disk_io_timePhmmPKc:
.LFB1613:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1613
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	movq	%rdi, %r14
	movq	%rcx, %rdi
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movq	%rsi, %rbp
	movl	$16386, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$48, %rsp
	.cfi_def_cfa_offset 96
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
.LEHB0:
	call	open@PLT
	movl	%eax, %r12d
	testl	%eax, %eax
	js	.L28
	xorl	%esi, %esi
	movq	%rsp, %rdi
	call	gettimeofday@PLT
	testq	%r13, %r13
	je	.L16
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L17:
	leaq	(%r14,%rbx), %rsi
	movq	%rbp, %rdx
	movl	%r12d, %edi
	addq	%rbp, %rbx
	call	write@PLT
	cmpq	%rbx, %r13
	ja	.L17
.L16:
	xorl	%esi, %esi
	leaq	16(%rsp), %rdi
	call	gettimeofday@PLT
	movl	%r12d, %edi
	call	close@PLT
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1
	movsd	.LC2(%rip), %xmm2
	movq	40(%rsp), %rax
	xorq	%fs:40, %rax
	cvtsi2sdq	24(%rsp), %xmm0
	cvtsi2sdq	16(%rsp), %xmm1
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	pxor	%xmm1, %xmm1
	cvtsi2sdq	8(%rsp), %xmm1
	divsd	%xmm2, %xmm1
	pxor	%xmm2, %xmm2
	cvtsi2sdq	(%rsp), %xmm2
	addsd	%xmm2, %xmm1
	subsd	%xmm1, %xmm0
	jne	.L29
	addq	$48, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L28:
	.cfi_restore_state
	leaq	.LC3(%rip), %rdi
	call	perror@PLT
	movl	%r12d, %edi
	call	close@PLT
.LEHE0:
	movl	$16, %edi
	call	__cxa_allocate_exception@PLT
	leaq	.LC4(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %rbp
.LEHB1:
	call	_ZNSt13runtime_errorC1EPKc@PLT
.LEHE1:
	jmp	.L25
.L29:
	call	__stack_chk_fail@PLT
.L20:
	movq	%rax, %r12
	jmp	.L18
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1613:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1613-.LLSDACSB1613
.LLSDACSB1613:
	.uleb128 .LEHB0-.LFB1613
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB1613
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L20-.LFB1613
	.uleb128 0
.LLSDACSE1613:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC1613
	.type	_Z19direct_disk_io_timePhmmPKc.cold, @function
_Z19direct_disk_io_timePhmmPKc.cold:
.LFSB1613:
.L25:
	.cfi_def_cfa_offset 96
	.cfi_offset 3, -48
	.cfi_offset 6, -40
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	.cfi_offset 14, -16
	movq	_ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx
	leaq	_ZTISt13runtime_error(%rip), %rsi
	movq	%rbp, %rdi
.LEHB2:
	call	__cxa_throw@PLT
.L18:
	movq	%rbp, %rdi
	call	__cxa_free_exception@PLT
	movq	%r12, %rdi
	call	_Unwind_Resume@PLT
.LEHE2:
	.cfi_endproc
.LFE1613:
	.section	.gcc_except_table
.LLSDAC1613:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC1613-.LLSDACSBC1613
.LLSDACSBC1613:
	.uleb128 .LEHB2-.LCOLDB5
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC1613:
	.section	.text.unlikely
	.text
	.size	_Z19direct_disk_io_timePhmmPKc, .-_Z19direct_disk_io_timePhmmPKc
	.section	.text.unlikely
	.size	_Z19direct_disk_io_timePhmmPKc.cold, .-_Z19direct_disk_io_timePhmmPKc.cold
.LCOLDE5:
	.text
.LHOTE5:
	.section	.text.unlikely
.LCOLDB6:
	.text
.LHOTB6:
	.p2align 4
	.globl	_Z13transfer_unitB5cxx11d
	.type	_Z13transfer_unitB5cxx11d, @function
_Z13transfer_unitB5cxx11d:
.LFB1611:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1611
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$408, %rsp
	.cfi_def_cfa_offset 464
	leaq	128(%rsp), %r13
	movsd	%xmm0, (%rsp)
	leaq	16(%rsp), %r14
	movq	%fs:40, %rax
	movq	%rax, 392(%rsp)
	xorl	%eax, %eax
	movq	%r13, %rdi
	call	_ZNSt8ios_baseC2Ev@PLT
	movq	8+_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rbp
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax
	xorl	%esi, %esi
	movq	%rax, 128(%rsp)
	xorl	%eax, %eax
	movw	%ax, 352(%rsp)
	movq	-24(%rbp), %rdi
	movq	16+_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax
	movq	$0, 344(%rsp)
	addq	%r14, %rdi
	movq	$0, 360(%rsp)
	movq	$0, 368(%rsp)
	movq	$0, 376(%rsp)
	movq	$0, 384(%rsp)
	movq	%rbp, 16(%rsp)
	movq	%rax, (%rdi)
.LEHB3:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT
.LEHE3:
	leaq	24+_ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax
	leaq	80(%rsp), %r15
	movq	$0, 32(%rsp)
	movq	%rax, 16(%rsp)
	addq	$40, %rax
	movq	%r15, %rdi
	movq	%rax, 128(%rsp)
	leaq	16+_ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax
	movq	%rax, 24(%rsp)
	movq	$0, 40(%rsp)
	movq	$0, 48(%rsp)
	movq	$0, 56(%rsp)
	movq	$0, 64(%rsp)
	movq	$0, 72(%rsp)
	call	_ZNSt6localeC1Ev@PLT
	leaq	16+_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax
	movq	%r13, %rdi
	leaq	24(%rsp), %rsi
	movq	%rax, 24(%rsp)
	leaq	112(%rsp), %rax
	movl	$16, 88(%rsp)
	movq	%rax, 8(%rsp)
	movq	%rax, 96(%rsp)
	movq	$0, 104(%rsp)
	movb	$0, 112(%rsp)
.LEHB4:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT
.LEHE4:
	movsd	.LC0(%rip), %xmm0
	xorl	%ebx, %ebx
	comisd	(%rsp), %xmm0
	ja	.L35
	movsd	.LC1(%rip), %xmm1
	jmp	.L36
	.p2align 4,,10
	.p2align 3
.L59:
	cmpl	$6, %ebx
	je	.L35
.L36:
	movsd	(%rsp), %xmm2
	addl	$1, %ebx
	mulsd	%xmm1, %xmm2
	comisd	%xmm2, %xmm0
	movsd	%xmm2, (%rsp)
	jbe	.L59
.L35:
	movsd	(%rsp), %xmm0
	movq	%r14, %rdi
.LEHB5:
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movq	%rax, %r8
	movslq	%ebx, %rbx
	leaq	unit_transfer_table(%rip), %rax
	movq	(%rax,%rbx,8), %rbx
	testq	%rbx, %rbx
	je	.L63
	movq	%rbx, %rdi
	movq	%r8, (%rsp)
	call	strlen@PLT
	movq	(%rsp), %r8
	movq	%rbx, %rsi
	movq	%rax, %rdx
	movq	%r8, %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.LEHE5:
.L40:
	movq	64(%rsp), %rax
	leaq	16(%r12), %rbx
	movq	$0, 8(%r12)
	movq	%rbx, (%r12)
	movb	$0, 16(%r12)
	testq	%rax, %rax
	je	.L41
	movq	48(%rsp), %r8
	movq	56(%rsp), %rcx
	cmpq	%r8, %rax
	ja	.L64
	subq	%rcx, %r8
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
.LEHB6:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm@PLT
.L43:
	leaq	24+_ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rax
	movq	96(%rsp), %rdi
	movq	%rax, 16(%rsp)
	addq	$40, %rax
	movq	%rax, 128(%rsp)
	leaq	16+_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax
	movq	%rax, 24(%rsp)
	cmpq	8(%rsp), %rdi
	je	.L45
	call	_ZdlPv@PLT
.L45:
	leaq	16+_ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax
	movq	%r15, %rdi
	movq	%rax, 24(%rsp)
	call	_ZNSt6localeD1Ev@PLT
	movq	-24(%rbp), %rax
	movq	%rbp, 16(%rsp)
	movq	%r13, %rdi
	movq	16+_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rdx
	movq	%rdx, 16(%rsp,%rax)
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax
	movq	%rax, 128(%rsp)
	call	_ZNSt8ios_baseD2Ev@PLT
	movq	392(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L65
	addq	$408, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L64:
	.cfi_restore_state
	subq	%rcx, %rax
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
	movq	%rax, %r8
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm@PLT
.LEHE6:
	jmp	.L43
	.p2align 4,,10
	.p2align 3
.L63:
	movq	(%r8), %rax
	movq	-24(%rax), %rdi
	addq	%r8, %rdi
	movl	32(%rdi), %esi
	orl	$1, %esi
.LEHB7:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT
.LEHE7:
	jmp	.L40
	.p2align 4,,10
	.p2align 3
.L41:
	leaq	96(%rsp), %rsi
	movq	%r12, %rdi
.LEHB8:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@PLT
.LEHE8:
	jmp	.L43
.L65:
	call	__stack_chk_fail@PLT
.L51:
	movq	%rax, %rbp
	jmp	.L48
.L53:
	movq	%rax, %rbx
	jmp	.L32
.L52:
	movq	%rax, %rbx
	jmp	.L34
.L54:
	movq	%rax, %rbp
	jmp	.L46
	.section	.gcc_except_table
.LLSDA1611:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1611-.LLSDACSB1611
.LLSDACSB1611:
	.uleb128 .LEHB3-.LFB1611
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L52-.LFB1611
	.uleb128 0
	.uleb128 .LEHB4-.LFB1611
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L53-.LFB1611
	.uleb128 0
	.uleb128 .LEHB5-.LFB1611
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L51-.LFB1611
	.uleb128 0
	.uleb128 .LEHB6-.LFB1611
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L54-.LFB1611
	.uleb128 0
	.uleb128 .LEHB7-.LFB1611
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L51-.LFB1611
	.uleb128 0
	.uleb128 .LEHB8-.LFB1611
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L54-.LFB1611
	.uleb128 0
.LLSDACSE1611:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC1611
	.type	_Z13transfer_unitB5cxx11d.cold, @function
_Z13transfer_unitB5cxx11d.cold:
.LFSB1611:
.L46:
	.cfi_def_cfa_offset 464
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	movq	(%r12), %rdi
	cmpq	%rdi, %rbx
	je	.L48
	call	_ZdlPv@PLT
.L48:
	movq	%r14, %rdi
	call	_ZNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED1Ev@PLT
	movq	%rbp, %rdi
.LEHB9:
	call	_Unwind_Resume@PLT
.L32:
	leaq	16+_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE(%rip), %rax
	movq	96(%rsp), %rdi
	movq	%rax, 24(%rsp)
	cmpq	8(%rsp), %rdi
	je	.L33
	call	_ZdlPv@PLT
.L33:
	leaq	16+_ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax
	movq	%r15, %rdi
	movq	%rax, 24(%rsp)
	call	_ZNSt6localeD1Ev@PLT
	movq	-24(%rbp), %rax
	movq	%rbp, 16(%rsp)
	movq	16+_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE(%rip), %rcx
	movq	%rcx, 16(%rsp,%rax)
.L34:
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax
	movq	%r13, %rdi
	movq	%rax, 128(%rsp)
	call	_ZNSt8ios_baseD2Ev@PLT
	movq	%rbx, %rdi
	call	_Unwind_Resume@PLT
.LEHE9:
	.cfi_endproc
.LFE1611:
	.section	.gcc_except_table
.LLSDAC1611:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC1611-.LLSDACSBC1611
.LLSDACSBC1611:
	.uleb128 .LEHB9-.LCOLDB6
	.uleb128 .LEHE9-.LEHB9
	.uleb128 0
	.uleb128 0
.LLSDACSEC1611:
	.section	.text.unlikely
	.text
	.size	_Z13transfer_unitB5cxx11d, .-_Z13transfer_unitB5cxx11d
	.section	.text.unlikely
	.size	_Z13transfer_unitB5cxx11d.cold, .-_Z13transfer_unitB5cxx11d.cold
.LCOLDE6:
	.text
.LHOTE6:
	.section	.rodata.str1.1
.LC8:
	.string	"'s QWORD data was copied in "
.LC9:
	.string	" seconds ("
.LC10:
	.string	"/s)"
	.section	.text.unlikely
.LCOLDB11:
	.section	.text.startup,"ax",@progbits
.LHOTB11:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB1615:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1615
	movabsq	$8589934592, %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	addq	$-128, %rsp
	.cfi_def_cfa_offset 144
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
.LEHB10:
	call	_Znam@PLT
	movabsq	$8589934592, %rdx
	movq	%rax, %rbp
	addq	%rax, %rdx
	.p2align 4,,10
	.p2align 3
.L67:
	movq	$0, (%rax)
	addq	$8, %rax
	cmpq	%rax, %rdx
	jne	.L67
	leaq	16(%rsp), %rdi
	xorl	%esi, %esi
	call	gettimeofday@PLT
	movq	%rbp, %rax
	movabsq	$8589934592, %rdx
	addq	%rbp, %rdx
	.p2align 4,,10
	.p2align 3
.L68:
	movq	$255, (%rax)
	addq	$8, %rax
	cmpq	%rax, %rdx
	jne	.L68
	xorl	%esi, %esi
	leaq	32(%rsp), %rdi
	call	gettimeofday@PLT
	movq	%rbp, %rdi
	call	_ZdaPv@PLT
	pxor	%xmm1, %xmm1
	pxor	%xmm0, %xmm0
	movsd	.LC2(%rip), %xmm2
	cvtsi2sdq	40(%rsp), %xmm1
	leaq	48(%rsp), %rdi
	movq	$2, 16+_ZSt4cout(%rip)
	cvtsi2sdq	32(%rsp), %xmm0
	divsd	%xmm2, %xmm1
	addsd	%xmm0, %xmm1
	pxor	%xmm0, %xmm0
	cvtsi2sdq	24(%rsp), %xmm0
	divsd	%xmm2, %xmm0
	pxor	%xmm2, %xmm2
	cvtsi2sdq	16(%rsp), %xmm2
	addsd	%xmm2, %xmm0
	subsd	%xmm0, %xmm1
	movsd	.LC7(%rip), %xmm0
	movapd	%xmm0, %xmm3
	divsd	%xmm1, %xmm3
	movsd	%xmm1, 8(%rsp)
	movsd	%xmm3, (%rsp)
	call	_Z13transfer_unitB5cxx11d
.LEHE10:
	movq	56(%rsp), %rdx
	movq	48(%rsp), %rsi
	leaq	_ZSt4cout(%rip), %rdi
.LEHB11:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movl	$28, %edx
	leaq	.LC8(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movsd	8(%rsp), %xmm0
	movq	%rbp, %rdi
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movl	$10, %edx
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movsd	(%rsp), %xmm0
	leaq	80(%rsp), %rdi
	call	_Z13transfer_unitB5cxx11d
.LEHE11:
	movq	88(%rsp), %rdx
	movq	80(%rsp), %rsi
	movq	%rbp, %rdi
.LEHB12:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movl	$3, %edx
	leaq	.LC10(%rip), %rsi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	%rbp, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@PLT
.LEHE12:
	movq	80(%rsp), %rdi
	leaq	96(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L69
	call	_ZdlPv@PLT
.L69:
	movq	48(%rsp), %rdi
	leaq	64(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L70
	call	_ZdlPv@PLT
.L70:
	movq	120(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L82
	subq	$-128, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L82:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
.L77:
	movq	%rax, %rbp
	jmp	.L71
.L76:
	movq	%rax, %rbp
	jmp	.L73
	.section	.gcc_except_table
.LLSDA1615:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1615-.LLSDACSB1615
.LLSDACSB1615:
	.uleb128 .LEHB10-.LFB1615
	.uleb128 .LEHE10-.LEHB10
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB11-.LFB1615
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L76-.LFB1615
	.uleb128 0
	.uleb128 .LEHB12-.LFB1615
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L77-.LFB1615
	.uleb128 0
.LLSDACSE1615:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC1615
	.type	main.cold, @function
main.cold:
.LFSB1615:
.L71:
	.cfi_def_cfa_offset 144
	.cfi_offset 6, -16
	movq	80(%rsp), %rdi
	leaq	96(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L73
	call	_ZdlPv@PLT
.L73:
	movq	48(%rsp), %rdi
	leaq	64(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L74
	call	_ZdlPv@PLT
.L74:
	movq	%rbp, %rdi
.LEHB13:
	call	_Unwind_Resume@PLT
.LEHE13:
	.cfi_endproc
.LFE1615:
	.section	.gcc_except_table
.LLSDAC1615:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC1615-.LLSDACSBC1615
.LLSDACSBC1615:
	.uleb128 .LEHB13-.LCOLDB11
	.uleb128 .LEHE13-.LEHB13
	.uleb128 0
	.uleb128 0
.LLSDACSEC1615:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE11:
	.section	.text.startup
.LHOTE11:
	.p2align 4
	.type	_GLOBAL__sub_I_unit_transfer_table, @function
_GLOBAL__sub_I_unit_transfer_table:
.LFB2177:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2177:
	.size	_GLOBAL__sub_I_unit_transfer_table, .-_GLOBAL__sub_I_unit_transfer_table
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_unit_transfer_table
	.globl	unit_transfer_table
	.section	.rodata.str1.1
.LC12:
	.string	"Bytes"
.LC13:
	.string	"KiB"
.LC14:
	.string	"MiB"
.LC15:
	.string	"GiB"
.LC16:
	.string	"TiB"
.LC17:
	.string	"EiB"
	.section	.data.rel.local,"aw"
	.align 32
	.type	unit_transfer_table, @object
	.size	unit_transfer_table, 48
unit_transfer_table:
	.quad	.LC12
	.quad	.LC13
	.quad	.LC14
	.quad	.LC15
	.quad	.LC16
	.quad	.LC17
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1083179008
	.align 8
.LC1:
	.long	0
	.long	1062207488
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.align 8
.LC7:
	.long	0
	.long	1107296256
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Arch Linux 9.2.1+20200130-2) 9.2.1 20200130"
	.section	.note.GNU-stack,"",@progbits
