  global my_strcasecmp          ;https://man7.org/linux/man-pages/man3/strcasecmp.3.html

  section .text

my_strcasecmp:
  dec rdi
  dec rsi


loop:
  inc rdi
  inc rsi
  mov dl, byte [rdi]
  cmp dl, 'A'
  jl  convert_c2
  cmp dl, 'Z'
  jg  convert_c2
  add dl, 32
convert_c2:
  mov cl, byte [rsi]
  cmp cl, 'A'
  jl  compare_chars
  cmp cl, 'Z'
  jg  compare_chars
  add cl, 32
compare_chars:
  cmp dl, cl
  jne done
  cmp byte [rdi], 0
  jne loop

done:
  movzx rax, dl
  movzx rcx, cl
  sub   rax, rcx
  ret
