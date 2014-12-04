RIT Senior Design
Group P14345
Fall 2014

This is the Repository for all Software completed
during the time on this project.

Processor: STM32F303VC

Dev Board: STM32F3 Discovery




Projects:

	ADC 		- ADC Testing
	LCD 		- LCD and UART Testing
	RotaryEncoder	- GPIO and RotEncoder Testing
	System		- System Level program (Final Project Program)
	System Init	- Initialization Testing
	TIM		- TIM Testing
	UART  		- UART Testing
	

	The project "System" contains the final project software,
	all other projects were merely for testing and learning
	how to program various peripherals used in this project.
	The other projects are still embedded for reference.




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





Software Flowchart:


	Final software flowchart for the project "System"

	This design was developed to Poll the User Interfce,
	while dedicating Audio Compression Algorithm to
	re-calculate at a rate of 10kHz (0.1ms per calculation)

	This is fine, considering the Inputs from the user
	interface change very slowly (>0.01s), in most cases


STM32 guide:

	Embedded in the top-level repository is a basic guide
	on setting up a working STM32F3 Discovery project using Keil.

	This guide is provided as there seems to be few resources
	on learning how to configure new Keil projects, and this
	guide is the process on how we were able to get Keil to work.

	Keil does have a code compile limit for the free version,
	which we did almost reach.  A great feature is the debug mode,
	which greatly helped to see system variables change in real-time,
	and double check the system status.

	There are other IDE's, but Keil was used as it was the first IDE
	that had been successfully compiled to the board. After that
	point, it was more beneficial to start working on the Processor
	software than continue to investigate alternative IDE's.




