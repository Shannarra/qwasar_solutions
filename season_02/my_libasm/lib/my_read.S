  global my_read                   ;https://man7.org/linux/man-pages/man2/read.2.html

  section .text

my_read:
  mov rax, 0
  syscall
  ret
