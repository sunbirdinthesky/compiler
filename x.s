.section .data
.section .text
.global main

l_getchar:
  pushl %ebp
  movl  %esp, %ebp
  movzbl %al, %eax
  call getchar
  leave
  ret

l_putchar:
  pushl %ebp
  movl  %esp, %ebp
  subl $1, %esp
  movb %al, -1(%ebp)
  call putchar
  leave
  ret

main:
  pushl %ebp
  movl  %esp, %ebp
  subl $1, %esp
  movl $65, %eax
  movb %al, -1(%ebp)
  movzbl -1(%ebp), %eax
  movl $10, %ebx
  addl %ebx, %eax
  movb %al, -1(%ebp)
  movzbl -1(%ebp), %eax
  call l_putchar
  subl $1, %esp
  movl $98, %eax
  movb %al, -2(%ebp)
  movzbl -2(%ebp), %eax
  call l_putchar
  movzbl -1(%ebp), %eax
  call l_putchar
  movl $0, %eax
  leave
  ret

