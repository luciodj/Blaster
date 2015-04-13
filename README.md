# Blaster (IR-Click®)

This is a test project to demonstrate using Core Independent Peripherals (CIP) to modulate an infrared signal and reproduce a typical IR remote control transmission. This demo reproduces the Apple (new) IR remote control.

* Device: PIC16F1709 (any other PIC16F1 featuring PPS and CLC)
* Board: PICDEM® Low Pin Count or [Simplicity](https://github.com/luciodj/Simplicity)
* Debugger: PICkit® 3
* IR Click® board

## The Basic Idea
* Use TMR2 + PWM2 to generate a carrier at 38kHz.
* Use TMR1 and CLC1 to modulate the carrier using the NEC RC protocol.
* Drive the IR Click board output (PWM pin)
     
## Limitations
* If the PICDEM LPC / Simplicity board is powered by the PICkit 3, the transmission will have a minimal range (point directly at the receiver)



## Related Projects/Demos
* Check the [Simplicity](https://github.com/luciodj/Simplicity) project for more demo like this using the MPLAB Code Configurator and the MikroElektronika Click(tm) boards.
    
     

    

