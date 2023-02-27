  global my_index                 ;https://man7.org/linux/man-pages/man3/index.3.html
  extern my_strchr

  section .text

my_index:                       ; SEE: https://stackoverflow.com/a/4091879/2237433
  call my_strchr
  ret
