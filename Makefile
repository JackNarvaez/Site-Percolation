PRINTMSG=--no-print-directory

.PHONY: percolation

percolation:
	@cd src;${MAKE} $(PRINTMSG) percolation

plot:
	@cd pic; ${MAKE} $(PRINTMSG) plot

clean:
	@cd src; ${MAKE} $(PRINTMSG) clean;\
	cd ../pic; ${MAKE} $(PRINTMSG) clean;