.data                    # 数据段声明
  msg : .string "Hello, world!\n" # 要输出的字符串
  len = . - msg                   # 字串长度

.text                    # 代码段声明
.global main           # 指定入口函数

print:                  # 在屏幕上显示一个字符串
  movl $len, %edx  # 参数三：字符串长度
  movl $90, %ecx  # 参数二：要显示的字符串
  movl $1, %ebx    # 参数一：文件描述符(stdout) 
  movl $4, %eax    # 系统调用号(sys_write) 
  int  $0x80       # 调用内核功能
  ret

main:
  call print
  call print
  
  movl $0,%ebx     # 参数一：退出代码
  movl $1,%eax     # 系统调用号(sys_exit) 
  int  $0x80       # 调用内核功能
