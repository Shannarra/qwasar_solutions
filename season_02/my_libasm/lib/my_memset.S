  global my_memset                ;https://man7.org/linux/man-pages/man3/memset.3.html

  section .text

my_memset:
  mov rax, rdi

loop:
  cmp rdx, 0
  je return
  mov byte [rdi], sil
  inc rdi
  dec rdx
  jmp loop

return:
  ret
