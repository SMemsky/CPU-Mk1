// CPU Mk1 Microcode ROM source file
// To be assembled by mca into Logisim ROM/RAM image format
// mca mk1v1.mc microcode.txt

// ALU Selector
// 0) C = A			0 0 0
// 1) C = B			0 0 1
// 2) C = A AND B	0 1 0
// 3) C = A OR B	0 1 1
// 4) C = A + 1		1 0 0
// 5) C = B + 1		1 0 1
// 6) C = A + B		1 1 0
// 7) C = A - B		1 1 1

// B-Bus Selector
// 0) B		0 0
// 1) Mem	0 1
// 2) IC	1 0
// 3) Ext	1 1

// Opcodes
// 0x00	0	NOP -- Just fetch next instruction
// 0x03	3	MOVB -- Move value from A to B
// 0x05	5	MOVA -- Move value from B to A
// 0x0f	15	OUT
// 0x07	7	FETCH
// 0x0b	11	ADD
// 0x0d	13	SUB

// Microcode ROM

// Name	|M|O|B|A|I|BB|ALU|E|  NXT  |
// NOP/Zero instruction - increase PC and execute value on memory
0x00	|0|0|0|0|0|10|101|1|0000001| // Setup ALU and B-Bus
0x01	|0|0|0|0|1|10|101|1|0000010| // Increase program counter
0x02	|1|0|0|0|0|00|000|0|0000000| // Fetch instruction and jump to it
// MOVB -- Copy A contents to B
0x03	|0|0|0|0|0|00|000|1|0000100| // Prepare ALU and B-Bus
0x04	|0|0|1|0|0|00|000|1|0000000| // Set B and call NOP
// MOVA -- Copy B contents to A
0x05	|0|0|0|0|0|00|001|1|0000110| // Prepare ALU and B-Bus
0x06	|0|0|0|1|0|00|001|1|0000000| // Set A and call NOP
// FETCH -- Copy value from Mem to A
0x07	|0|0|0|0|0|10|101|1|0001000| // Prepare ALU and B-Bus
0x08	|0|0|0|0|1|10|101|1|0001001| // Increase PC
0x09	|1|0|0|0|0|01|001|1|0001010| // Prepare ALU and B-Bus and fetch data
0x0a	|0|0|0|1|0|01|001|1|0000000| // Set A and call NOP
// ADD -- Combine A and B and write to A
0x0b	|0|0|0|0|0|00|110|1|0001100| // Prepare ALU and B-Bus
0x0c	|0|0|0|1|0|00|110|1|0000000| // Set A to A+B and call NOP
// SUB -- Subtract B from A and write to A
0x0d	|0|0|0|0|0|00|111|1|0001110| // Prepare ALU and B-Bus
0x0e	|0|0|0|1|0|00|111|1|0000000| // Set A to A-B and call NOP
// OUT -- Write value from A to Out
0x0f	|0|0|0|0|0|00|000|1|0010000| // Prepare ALU and B-Bus
0x10	|0|1|0|0|0|00|000|1|0000000| // Set Out and call NOP
// HALT -- Go to infinite loop
0x11	|0|0|0|0|0|00|000|1|0010010| // Drop all states
0x12	|1|0|0|0|0|00|000|0|0000000| // Simply re-execute instruction on memory
