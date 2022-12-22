.data
array: dq 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

.text
.globl binary_search
binary_search:
  push rbp        ; Save old base pointer
  mov rsp, rbp    ; Set new base pointer

  mov edi, [rbp+16] ; Load lowerb
  mov esi, [rbp+24] ; Load upperb
  add esi, edi     ; Calculate mid
  shr esi, 1       ; Divide by 2
  cmp edi, esi     ; Compare lowerb and mid
  jge end          ; Jump to end if lowerb >= mid
  mov edx, [rbp+28] ; Load target
  mov ecx, [rbp+8]  ; Load array
  mov r8d, [rcx+rsi*4] ; Load array[mid]
  cmp edx, r8d     ; Compare array[mid] and target
  je end           ; Jump to end if equal
  jg greater       ; Jump to greater if array[mid] > target

less:
  mov esi, edi
  sub esi, 1
  jmp call

greater:
  mov esi, edi
  add esi, 1

call:
  mov edi, esi     ; Update lowerb
  mov esi, [rbp+24] ; Load upperb
  mov edx, [rbp+28]
