.data
array: .quad 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

.text
.globl binary_search
binary_search:
  pushq %rbp        # Save old base pointer
  movq %rsp, %rbp   # Set new base pointer

  movq 16(%rbp), %rdi # Load lowerb
  movq 24(%rbp), %rsi # Load upperb
  addq %rdi, %rsi    # Calculate mid
  shrq $1, %rsi       # Divide by 2
  cmpq %rdi, %rsi    # Compare lowerb and mid
  jge end            # Jump to end if lowerb >= mid
  movq 28(%rbp), %rdx # Load target
  movq 8(%rbp), %rcx  # Load array
  movq (%rcx,%rsi,8), %r8 # Load array[mid]
  cmpq %r8, %rdx     # Compare array[mid] and target
  je end             # Jump to end if equal
  jg greater         # Jump to greater if array[mid] > target

less:
  movq %rsi, %rdi
