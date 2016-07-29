ll = ./fb3-1.l
yy = ./fb3-1.y
mon = ./mongoose.c
fun = ./fb3-1func.c

Cal-simple: $(ll) $(yy) $(mon) $(fun)
	bison -d $(yy)
	flex -o fb3-1.lex.c $(ll)
	cc -o Cal-simple $(fun) $(mon) fb3-1.tab.c fb3-1.lex.c -ll
	rm fb3-1.tab.c fb3-1.lex.c fb3-1.tab.h


clean:
	rm Cal-simples
