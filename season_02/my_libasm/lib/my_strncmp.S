  global my_strncmp             ;  https://man7.org/linux/man-pages/man3/strcmp.3.html

  section .text

my_strncmp:
  dec rdi
  dec rsi
  inc rdx

loop:
  inc rdi
  inc rsi
  dec rdx
  cmp rdx, 0
  je exhausted
  cmp byte [rdi], 0
  je done
  mov cl, byte [rdi]
  cmp cl, byte [rsi]
  je loop

done:
  movzx rax, byte [rdi]
  movzx rcx, byte [rsi]
  sub   rax, rcx
  ret

exhausted:
  xor rax, rax
  ret
