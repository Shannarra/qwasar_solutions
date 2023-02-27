  global my_write                 ;https://man7.org/linux/man-pages/man2/write.2.html

  section .text

my_write:
  mov rax, 1
  syscall
  ret
