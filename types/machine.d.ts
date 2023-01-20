declare module "*machine.node" {
     /**
     * A Machine with 64 8bit instructions
     * Instruction and memory are combined in a single structure
     * Instuction consists of 2bits for the operation and 6bits for the memory address  
     * `__2bits__`:`_____6bits____`  
     * `operation`:`memory_address`     
     * 
     * **Memory Address**
     *  Pointer to the memory address where the operation will be performed
     * - 0b000000: 0
     * - 0b000001: 1  
     * - ...
     * - 0b111111: 63
     * 
     * **Operations**
     * - 0b00: Increment the value at the memory address
     * - 0b01: Decrement the value at the memory address
     * - 0b10: Jump to the memory address
     * - 0b11: Output the value at the memory address
     * 
     * **Output**
     * Return the representation of the value at the memory address 
     * 
     * 
     * @example
     * const machine = new Machine(new ArrayBuffer(64));
     */
    class Machine {
        /**
         * Create a new binary Machine instance 
         * @param memory ArrayBuffer of 64 8bit instructions
         */
        constructor(memory: ArrayBuffer);
        /**
         * Run the machine for a given number of instructions and return the steps: **Up** **Down** **Right** **Left**
         * @returns String of steps: **U=Up** **D=Down** **R=Right** **L=Left**
         * @example
         * const machine = new Machine(new ArrayBuffer(64));
         * const steps = machine.run(64);
         * console.log(steps)
         * >> UUURRRRDDDDLLLL
         * 
        **/
        run(instructionsCount: number): string;
    }
}
