.section .data
.section .text
    .global main

#初始化 add
add:
  pushl %ebp
  movl  %esp,     %ebp

#传入参数a，b
  subl  $8,       %esp
  movl  %eax,     -4(%ebp)
  movl  %ebx,     -8(%ebp)

#c = a+b
  subl  $4,       %esp
  movl  -4(%ebp), %eax
  addl  -8(%ebp), %eax
  movl  %eax,     -12(%ebp)

#return c
  movl -12(%ebp),   %eax
  leave
  ret

#初始化 main
main:
  pushl %ebp
  movl  %esp,     %ebp
#int a;
  subl  $4,       %esp

#a = add (32+24)
  movl  $32,      %eax
  movl  $24,      %ebx
  call  add
  movl  %eax,     -4(%ebp)

#putchar(a)
  subl   $1,       %esp
  movl   -4(%ebp), %eax

  movb   %al,      (%esp)
  call   putchar

#return 0
  mov $0, %eax
  leave
  ret
