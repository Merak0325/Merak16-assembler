lh a1, 0x0(a0)
li a2, 0x1
li a3, 0x1
add a1, a1, a2
add a2, a2, a3
li a5, 21
soe a2, a5
boz 0xfff8
mvlh a8, a1
sh a1, 0x2(a0)
JAL 0x2c
li a5, 0x4
sh a1, 0x0(a5)
halt
li a9, 92
li a8, 21 
and a10, a8, a9
or a11, a8, a9
xor a12, a8, a9
sub a13, a8, a9
li a8, 3
li a9, -100
sll a10, a9, a8
sra a11, a9, a8
srl a12, a9, a8
not a13, a11
com a14, a10
mvh a14, a10
mvhl a1, a14
jalr a15, 0x0