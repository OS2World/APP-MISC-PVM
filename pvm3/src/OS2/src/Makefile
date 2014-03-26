# interface for standard pvmd script 
# (emx/os2 can not exec *.cmd files) 

../../lib/OS2/pvmd.exe: src/pvmd.c
	gcc -o $@ $< ./pvm3.a

clean:
	rm deathapi.o os2spawn.o rexec.o ruserpas.o stdlog.o sthoster.o
