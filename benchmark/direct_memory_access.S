#define PTR_SIZE    8
#define WORD_SIZE   1
#define DWORD_SIZE  4
#define QWORD_SIZE  8

    .code64
    .section .data

    .section .text
    .global direct_memory_access

    /* Paramters:
        %rdi (word_ptr): Size of memory
        %rsi (word_ptr): timeval struct for start time
        %rdx (word_ptr): timeval struct for end time
    */
direct_memory_access:
    // prepare for function calling
    pushq   %rbp
    movq    %rsp, %rbp
    // allocate stack
    subq    $32, %rsp
    // save parameters
    movq    %rdi, -8(%rbp)
    movq    %rsi, -16(%rbp)
    movq    %rdx, -24(%rbp)
    // allocate heap memory
    call    malloc
    movq    %rax, -32(%rbp)
    cmpq    $0, %rax
    movl    $-1, %eax
    je      _dma_err_out
    // write zero into memory to avoid kernel's lazy allocation
    movq    -32(%rbp), %rdi
    xorq    %rsi, %rsi
    movq    -8(%rbp), %rdx
    call    memset
    // start timer
    movq    -16(%rbp), %rdi
    xorq    %rsi, %rsi
    call    gettimeofday
    // prepare and start loop
    movq    -8(%rbp), %rcx
    movq    -32(%rbp), %r8
_write_loop:
    movb    $0xff, (%r8)
    incq    %r8
    loop _write_loop
    // stop timer
    movq    -24(%rbp), %rdi
    xorq    %rsi, %rsi
    call    gettimeofday
    // free heap memory
    movq    -32(%rbp), %rdi
    call    free
    // Leaving function
    movl    $0, %eax
_dma_err_out:
    leave
    ret
