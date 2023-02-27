  global my_memmove               ; https://man7.org/linux/man-pages/man3/memmove.3.html

  extern my_memcpy
  section .text

my_memmove:
  cmp rsi, rdi
  jl back_loop
  call my_memcpy
  ret

back_loop:
  dec rdx

loop:
  cmp rdx, -1
  je return
  mov cl, byte [rsi+rdx]
  mov byte [rdi+rdx], cl
  dec rdx
  jmp loop

return:
  ret
