.global outToRegisterB
.type outToRegisterB, @function

outToRegisterB:
  push %ebp
  mov %esp, %ebp
  mov 8(%ebp),  %al
  mov 12(%ebp), %dx
  outb %al, %dx
  pop %ebp
  ret

.globl outToRegisterL
.type  outToRegisterL, @function

outToRegisterL:
    push   %ebp
    mov    %esp, %ebp
    mov    8(%ebp), %eax    # primo argomento: uint32_t data
    mov    12(%ebp), %dx    # secondo argomento: uint16_t port (in realtà 32-bit passato, usiamo i bit bassi)
    outl   %eax, %dx        # scrive 32-bit (outl) dalla EAX alla porta DX
    pop    %ebp
    ret

.globl inFromRegistersB
.type inFromRegistersB, @function

inFromRegistersB:
  push %ebp
  mov %esp, %ebp

  xor %eax, %eax

  mov 8(%ebp), %dx
  inb %dx, %al

  pop %ebp
  ret

.globl inFromRegistersL
.type inFromRegistersL, @function

inFromRegistersL:
    push %ebp
    mov  %esp, %ebp

    xor  %eax, %eax
    mov  8(%ebp), %dx
    inl  %dx, %eax

    pop  %ebp
    ret

