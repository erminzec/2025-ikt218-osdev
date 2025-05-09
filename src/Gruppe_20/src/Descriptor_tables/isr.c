#include "libc/isr.h"
#include "libc/common.h"
#include "libc/stdio.h"
#include "libc/system.h"
#include "libc/stdint.h"
#include "libc/print.h"
#include "io.h"
#include "interrupts.h"

// Array of registered interrupt handlers
static isr_t interrupt_handlers[256];
// Array to store context pointers for each handler

char *exception_messages[] = {
    "Division by zero exception",
    "Debug exception",
    "Non maskable interrupt",
    "Breakpoint exception",
    "Into detected overflow",
    "Out of bounds exception",
    "Invalid opcode exception",
    "No coprocessor exception",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt exception",
    "Coprocessor fault",
    "Alignment check exception",
    "Machine check exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


void isr_handler(registers_t regs)
{
    //Prints the interrupt to the terminal.
    printf("Recieved interrupt %d: %s\n", regs.int_no, exception_messages[regs.int_no]);
}

void irq_handler(registers_t regs)
{
    //Send an end of interrupt signal to the programmable interrupt controllers.
    //If the interrupt came from a slave PIC send a reset to the slave PIC aswell. 
    if (regs.int_no >= 40)
    {
        //Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master.
    outb(0x20, 0x20);

    //Checks if a function pointer is registered for the interrupt or IRQ.
    if (interrupt_handlers[regs.int_no] != 0)
    {
        //Retrieves the function pointer from the interrupt_handlers array.
        isr_t handler = interrupt_handlers[regs.int_no];
        //Calls the function with the regs struct as a parameter.
        handler(regs);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}