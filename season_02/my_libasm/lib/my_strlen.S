        global  my_strlen ; Implements: https://man7.org/linux/man-pages/man3/strlen.3.html

        section .text

my_strlen:
        mov rax, -1

loop:
        inc rax
        cmp byte [rdi + rax], 0
        jne loop

done:
        ret
