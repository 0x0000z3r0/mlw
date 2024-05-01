
.global _start
.align 4      

_start:	
	mov	X16, #4		
	mov	x0, #1
	adr	X1, __str
	mov	X2, #4	    	
	svc	#0x80		
	
	ret

__str:      .ascii  "abc\n"
