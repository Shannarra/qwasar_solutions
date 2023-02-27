  global my_strchr                ;https://man7.org/linux/man-pages/man3/strchr.3.html

  section .text

my_strchr:
  dec rdi

loop:
  inc rdi
  cmp byte [rdi], sil
  je found
  cmp byte [rdi], 0
  jne loop
  xor rax, rax
  ret

found:
  mov rax, rdi
  ret
