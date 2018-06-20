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
  leave 
  ret 

mul:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl %ebx, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  call add
  movl %eax, -4(%ebp)
  movl -8(%ebp), %ebx
  imull %ebx
  movl %eax, -4(%ebp)
  leave 
  ret 

div:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl %ebx, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  cltd 
  idivl %ebx
  movl %eax, -4(%ebp)
  leave 
  ret 

main:
  pushl %ebp
  movl  %esp, %ebp
  subl $4, %esp
  movl $3, %eax
  movl %eax, -4(%ebp)
  subl $4, %esp
  movl $7, %eax
  movl %eax, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  call mul
  movl %eax, -4(%ebp)
  call l_putchar
  movl $142, %eax
  movl %eax, -4(%ebp)
  movl $2, %eax
  movl %eax, -8(%ebp)
  movl -4(%ebp), %eax
  movl -8(%ebp), %ebx
  call div
  movl %eax, -4(%ebp)
  call l_putchar
  subl $1, %esp
  movl $97, %eax
  movb %al, -9(%ebp)
  subl $1, %esp
  movl $65, %eax
  movb %al, -10(%ebp)
  movzbl -10(%ebp), %eax
  call l_putchar
  movzbl -9(%ebp), %eax
  call l_putchar
  call l_getchar
  movb %al, -9(%ebp)
  movzbl -9(%ebp), %eax
  call l_putchar
  movl $0, %eax
  leave 
  ret 


