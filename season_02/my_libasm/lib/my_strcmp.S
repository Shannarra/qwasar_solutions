  global my_strcmp              ;https://man7.org/linux/man-pages/man3/strcmp.3.html

  section .text

my_strcmp:
  dec rdi
  dec rsi

loop:
  inc rdi
  inc rsi
  cmp byte [rdi], 0
  je done

  mov dl, byte [rdi]
  cmp dl, byte [rsi]
  je loop

done:
  movzx rax, byte [rdi]
  movzx rdx, byte [rsi]
  sub rax, rdx
  ret
