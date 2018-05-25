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

add:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl %ebx, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  addl %ebx, %eax
  movl %eax, -4(%ebp)
  movl -4(%ebp), %eax
  leave
  ret

mul:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl %ebx, -8(%ebp)
  subl $4, %esp
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  call add
  movl %eax, -4(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  imull %ebx
  movl %eax, -4(%ebp)
  movl -4(%ebp), %eax
  leave
  ret

main:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl $1, %eax
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl $4, %eax
  movl %eax, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  call mul
  movl %eax, -4(%ebp)
  subl $1, %esp
  movl $97, %eax
  movb %al, -9(%ebp)
  movzbl -9(%ebp), %eax
  call l_putchar
  call l_getchar
  movb %al, -9(%ebp)
  movzbl -9(%ebp), %eax
  call l_putchar
  movl $0, %eax
  leave
  ret

