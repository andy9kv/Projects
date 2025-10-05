# 8-bit computer

I designed and built an 8-bit computer on a bunch of breadboards tied together wiring some simple IC chips together, simliar to Intel's 8085 processor from 1976. Although its an old processor, many of the underlying concepts are used by modern processors. It is turing-complete. I built the computer using only basic digital and sequential logic gates.

#### The 8-bit machine consists of:
* Several registers (holds a byte that can be immediately loaded onto the bus)
* A bit shifter (for multiplication and division by 2)
* An ALU (arthmetic / logic unit)
* A clock (keeps components working on the same rhythm)
* A program counter (to index through instruciton memory)
* A tiny RAM (to store and retrieve bytes)
* A stack (allows for functions calls)
* An instruction memory (to contain the code)
* A control unit (to translate between instrucitons e.g. code and the actual operations of the components)
* A bus (veins of the computers to pass data between parts)
* A decial display window (to show human readable numbers on the bus!)

I wrote my own instruciton set for it that was kind of like machine/op code and eventually a compiler that translates a rudimentary programming language into the instruciton set. I used an arduino chip to download my compiled code (op code) into the instruciton memory which enabled running my code on the machine. I got it to sovle some math problems with some functions calls and even some recursion. I was hoping to connect it to a super old monitor over a VGA cable and display an image stored in RAM over a loop (1 byte at a time) which was theoretically possible. 

#### Media
Links to picures here on my GitHub and a very simple video of the computer counting in binary on YouTube

- [Available Control Flags](https://github.com/andy9kv/Projects/blob/master/Project_Files/8bit/media/control_flags.jpeg)
- [Control Unit and Stack Modules](https://github.com/andy9kv/Projects/blob/master/Project_Files/8bit/media/control_unit_stack_design.jpeg)
- [Schematics of simple IC chips I built the computer on](https://github.com/andy9kv/Projects/blob/master/Project_Files/8bit/media/ic_schematics.jpeg)
- [Sample instruction set from one of the many, many instruciton set pages](https://github.com/andy9kv/Projects/blob/master/Project_Files/8bit/media/sample_instruction_set.jpeg)
- [Very simple video of the computer counting in very crappy resolution that somehow gets only 10% better as the videa goes on](https://www.youtube.com/watch?v=aP5stwasfco)

<br><p align="center"><img width="80%" height="70%" src="Project_Files/8bit/media/full_8bit.jpg"></p><br>

#### Death

Eventually I ran into some power issues. I was splitting a phone charger and plugging that into my breadboards directly for power and ground. That worked for a while with some basic resistence calculations but soon enough the power supply became a mess after adding so many components to the 8-bit computer. Initially, certain chips started behaving unpredictably the futher they were from the power supply. I added some more phone chargers but that only lasted for so long. I told myself I'd come back and figure it out after reading through some electronic fundamentals but that didn't quite happen yet. 

I still have it in my basement, maybe I'll go back to it one day when I physically can't play soccer anymore.
