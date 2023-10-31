.PHONY: percolation

percolation:
	@cd src;${MAKE} percolation

plot:
	@cd pic; ${MAKE} plot

clean:
	@cd src; ${MAKE} clean;\
	cd ../pic; ${MAKE} clean;