# SIC/XE Machine Architecture

## Specifications

### Memory:

- 8-bit bytes, total 1M(2^20) bytes.
- A word is formed of any 3 consecutive bytes.
- Addressing is done by:
    - any byte
    - the lowest byte of a word

### Registers:

| Mnemonic | Number | Special Use |
|:--------:|:------:|:------------|
| A  | 0 | Accumulator; used for arithmetic operations |
| X  | 1 | Index register; used for addressing |
| L  | 2 | Linkage register; the Jump to Subroutine (JSUB) instruction stores the return address in this register |
| B  | 3 | Base register; used for addressing |
| S  | 4 | General working register - no special use |
| T  | 5 | General working register - no special use |
| F  | 6 | Floating-point accumulator (48 bits) |
| PC | 8 | Program counter: contains the address of the next instruction to be fetched for execution |
| SW | 9 | Status word; contains variety of information, including a Condition Code (CC) |

### Data Formats:

- Integers: 24 bits, 2's complement for negative numbers.
- Characters: 8-bit ASCII codes.
- Floating point number: 48 bits
  | 1 | 11 | 36 |
  |:-:|:--:|:--:|
  | s | exponent | fraction |

  - The fraction is interpreted as a value between 0 and 1; that is, the assumed
  binary point is immediately before the high-order bit. For normalized
  floating-point numbers, the high-order bit of the fraction must be 1. The
  exponent is interpereted as an unsigned binary number between 0 and 2047.
  - If the exponent has value *e* and the fraction has value *f* the absolute
  value of the number represented is: *f* * 2^*(e-1024)

### Instruction Formats:

- Format 1 (1 byte):
  | 8 |
  |:-:|
  | op |

- Format 2 (2 bytes):
  | 8 | 4 | 4 |
  |:-:|:-:|:-:|
  | op | r1 | r2 |

- Format 3 (3 bytes):
  | 6 | 1 | 1 | 1 | 1 | 1 | 1 | 12 |
  |:-:|:-:|:-:|:-:|:-:|:-:|:-:|:--:|
  | op | n | i | x | b | p | e | displacement |

- Format 4 ( 4 bytes):
  | 6 | 1 | 1 | 1 | 1 | 1 | 1 | 20 |
  |:-:|:-:|:-:|:-:|:-:|:-:|:-:|:--:|
  | op | n | i | x | b | p | e | address |

The flag e is used to distinguish between Formats 3 and 4.
- *e* = 0 means **Format 3**, *e* = 1 means **Format 4**.

### Addressing Modes and Flag Bits

Bits *b* and *p* are used to determine which of *Base - relative* or
*Program - counter relative* modes is used in **Format 3** instruction.

If neither of the bits *b* and *p* are set to *1*, then *Direct* mode is
used in **Format 4** instruction.

| Mode | Indication | Target address calculation |
|:-----|:----------:|----------------------------|
| Base - relative            | n=1, i=1, x=0, b=1, p=0 | **TA** = (B) + displacement (0<=d<=4095) |
| Program - counter relative | n=1, i=1, x=0, b=0, p=1 | **TA** = (PC) + displacement (-2048<=d<=2047) |
| Direct                     | n=1, i=1, x=0, b=0, p=0 | **TA** = address (0<=d<=4095) |


Any of these modes can also be combined with *indexed* addressing, adding the
contents of X register to the target address calculation.

This can be done by setting *x* bit to 1.

| Mode | Indication | Target address calculation |
|:-----|:----------:|----------------------------|
| Base - relative (indexed)            | n=1, i=1, x=1, b=1, p=0 | **TA** = (X) + (B) + displacement (0<=d<=4095) |
| Program - counter relative (indexed) | n=1, i=1, x=1, b=0, p=1 | **TA** = (X) + (PC) + displacement (-2048<=d<=2047) |
| Direct (indexed)                     | n=1, i=1, x=1, b=0, p=0 | **TA** = (X) + address (0<=d<=4095) |


Bits *n* and *i* are used to specify how the target address is used in
**Formats 3 and 4**.

If bit *n* = 0, and bit *i* = 1, the target address itself is used as the operand
value; no memory reference is performed. This is called *Immediate* addressing.

If bit *n* = 1, and bit *i* = 0, the word at the address location given by the target
address is fetched; the *value* contained in this word is then taken as the
*address* of the operand value. This is called *Indirect* addressing.

If bits *n* and *i* are both set to 0 or 1, the target address is taken as the
location of the operand; this is reffered as *Simple* addressing. An example
of this addressing mode can be seen in the first table of this section, which
demonstrates the effects of *b* and *b* flags on Target address calculation.

**IMPORTANT**
SIC/XE instruction that specify *Simple* addressing mode are assembled with
bits *n* and *i* both set to 1. Asseblers for the standard version of SIC will
set both bits to 0. (This is because the 8-bit binary codes for all of the SIC
instruction end in 00.)

All SIC/XE machines have a special hardware feature designed to provide the
upward compatibility.

If bits *n* and *i* are both set to 0, then bits *b*, *p*, and *e* are considered
to be part of the address field of the instruction.

This makes Format 3 identical to the format used on the standard version of SIC,
providing desired compatibility.

### Instruction Set

- P -- Privileged instruction
- C -- Condition code CC set to indicate result of operation (<, =, or >)

| Mnemonic | Format | Opcode | Effect | Notes |
|----------|:------:|:------:|--------|-------|
| ADD m  | 3/4 | 18 | A <- (A) + (m..m + 2) |   |
| AND m  | 3/4 | 40 | A <- (A) & (m..m + 2) |   |
| COMP m | 3/4 | 28 | (A) : (m..m + 2) | C |
| DIV m  | 3/4 | 24 | A <- (A) / (m..m + 2) |   |
| J m    | 3/4 | 3C | PC <- m |   |
| JEQ m  | 3/4 | 30 | PC <- m if CC set to = |   |
| JGT m  | 3/4 | 34 | PC <- m if CC set to > |   |
| JLT m  | 3/4 | 38 | PC <- m if CC set to > |   |
| JSUB m | 3/4 | 48 | L <- (PC); PC <- m |   |
| LDA m  | 3/4 | 00 | A <- (m..m + 2) |   |
| LDCH m | 3/4 | 50 | A[rightmost byte] <- (m) |   |
| LDL m  | 3/4 | 08 | L <- (m..m + 2) |   |
| LDX m  | 3/4 | 04 | X <- (m..m + 2) |   |
| MUL m  | 3/4 | 20 | A <- (A) * (m..m + 2) |   |
| OR m   | 3/4 | 44 | A <- (A) v (m..m + 2) |   |
| RD m   | 3/4 | D8 | A[rightmost byte] <- data from device specified by (m) | P |
| RSUB   | 3/4 | 4C | PC <- (L) |   |
| STA m  | 3/4 | 0C | m..m + 2 <- (A) |   |
| STCH m | 3/4 | 54 | m <- (A)[rightmost byte] |   |
| STL m  | 3/4 | 14 | m..m + 2 <- (L) |   |
| STSW m | 3/4 | E8 | m..m + 2 <- (SW) | P |
| STX m  | 3/4 | 10 | m..m + 2 <- (X) |   |
| SUB m  | 3/4 | 1C | A<- (A) - (m..m + 2) |   |
| TD m   | 3/4 | E0 | Test device specified by (m) | P C |
| TIX m  | 3/4 | 2C | X <- (X) + 1; (X):(m..m + 2) | C |
| WD m   | 3/4 | DC | Device specified by (m) <- (A)[rightmost byte] | P |

### Input and Output

- Each I\O device is assigned a unique 8-bit code.
- All I\O are performed by transferring 1 byte at a time to or from the
rightmost 8 bits of register A.
- Three I\O instructions (each specify the device code as an operand):
    - TD (Test Device) -- check whether device is ready. CC 1 is ready 0 is not.
    - RD (Read Data) -- must wait until the device is ready. TD must be done for
    each byte to be read.
    - WD (Write Data) -- must wait until the device is ready. TD must be done for
    each byte to be written.

[A program that demonstrates the technique for performing I\O](fig2_1-p45.png)

### Assembler Directives

These statements are not translated into machine instruction, Instead, they
provide instructions to the assembler itself.

| Directive | Description |
|-----------|-------------|
| START | Specify name and starting address for the program. |
| END   | Indicate the end of the source program and (optionally) specify the first executable instruction in the program. |
| BYTE  | Generate one-word character or hexadecimal constant, occupying as many bytes as needed to represent the constant. |
| WORD  | Generate one-word integer constant. |
| RESB  | Reserve the indicated number of bytes for a data area. |
| RESW  | Reserve the indicated number of words for a data area. |

### Platform-independant Directives

| Directive | Description |
|-----------|-------------|
| EQU   | Defines the given symbol (o.e., enters it into SYMTAB) and assigns to it the value specified (p. 71). |
| ORG   | Indirectly assign values to symbols (via LOCCTR). |
| LTORG | Create a literal pool (p. 69). |

### Records/Object Code

The assembler must write the generated object code onto some output device. The
simple obect program format we use contains three types of records: Header, Text,
and End.

| Record | Description |
|--------|-------------|
| Header | Contains the program name, starting address, and length. |
| Text   | Contains the translated (i.e., machine code) instructions and data of the program, together with an indication of the addresses where these are to be loaded. |
| End    | Marks the end of the object program and specifies the address in the program where execution is to begin.* |

* This is taken from the operand of the program's End statement. If no operand is
  specified, the address of the first executable instruction is used.

The formats used for the record are as follows:

#### Header Record:

| Column | Contents |
|--------|----------|
| Col. 1     | H |
| Col. 2-7   | Program name |
| Col. 8-13  | Starting address of the object program (hexadecimal) |
| Col. 14-19 | Length of object program in bytes (hexadecimal) |

##### Text record:

| Column | Contents |
|--------|----------|
| Col. 1     | T |
| Col. 2-7   | Starting address for object code in this record (hexadecimal) |
| Col. 8-9   | Length of object code in this record in bytes (hexadecimal) |
| Col. 10-69 | Object code, represented in hexadecimal (2 columns per byte of object code) |

##### End record:

| Column | Contents |
|--------|----------|
| Col. 1   | E |
| Col. 2-7 | Address of first executable instruction in object program (hexadecimal) |

Term *column* refers to the **byte**.

[Fig. 2.2 Program with object code.](fig2_2-p47.png)

[Obect program corresponding to Fig. 2.2, using this format.](fig2_3-p49.png)
