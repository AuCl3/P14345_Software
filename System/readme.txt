Project "System" important files:


	main.c

		main function, contains most peripheral
		setup and User Interface code.
 
	stm32F30x_it.c

		Interrupt function, contains TIMER
		ISR's used in this project.

	log2.s

		log Assembly function, used to reduce
		computation time of system, as the C log10
		function was too slow for project constaints.



