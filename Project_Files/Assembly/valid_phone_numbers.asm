
# Andrew Morato
# February 26, 2018

# Total Instructions: 1077
# R-type: 160 (14%)
# I-Type: 832 (77%)
# J-Type: 85 (7%)
# For some reason mips IC tool's %s does not add up to 100

.data
# Text to print
output: .asciiz "The number of valid phone numbers is: "
.text                                              

# In this description, jump represents the next valid nums from any digit,
# Example, 2 can go to 7 and 9 so 2's jumps (or children) are 7 and 9

# There are four (4) types of numbers, Corners, Special, Middle, and Bottom

# 1 2 3    Corners (C) = 1, 3, 7, 9
# 4 5 6    Special (S) = 4, 6
# 7 8 9    Middle  (M) = 2, 8
#   0      Bottom  (B) = 0

# This algorithm functions under the observation that:
#    1. ALL Corners jump to an Special and a Middle (2 jumps)
#    2. ALL Specials jump to two Corners and a Bottom (3 jumps)
#    3. ALL Middles jump to two Corners (2 jumps)
#    4. ALL Bottoms jump to two Specials (2 jumps)

# The valid starting digits are 2(M), 3(C), 4(S), 6(S), 7(C), 8(M) & 9(C),
# 5 can never be jumped to nor jump anywhere. Since the "trees" produced by
# each starting digit and their subsequent jumps would be the same exact 
# "tree" as one created by a starting digit of the same type, we only need
# to create trees for staring digits of type M, C, and S. Since two starting
# digits are of type M, two are of type S, and three are of type C, then the
# following formula would produce the number of valid phone numbers:

# valid_nums = 2 * createTree(M) + 2 * createTree(S) + 3 * createTree(C) 

# Note: This same concept of avoiding the repitition of the same exact
#       trees is used in the body of createTree 

# All that is left is to make createTree. Simply put, createTree takes in
# a type C, S, M, C and calls createTree recursivly on the types it could
# jump to:

#    C -> createTree(S), createTree(M)
#    S -> createTree(C), createTree(C), createTree(B)
#    M -> createTree(C), createTree(C)
#    B -> createTree(S), createTree(S)

# There has to be some way of knowing when to stop recursing. Therefore,
# createTree would also take in some length (number of times to keep
# calling itself), along with the type (C, S, M, B). Once we recurse the
# appropriate amount of times, we add the amount of jumps the current type
# has to some global counter and exit. With this in mind, the algorithm for
# createTree is specified below:

# createTree(length, type)
#    if length == 2
#       return 2
#       if type == S
#           return 1
#    else
#       if type == C
#           return createTree(length - 1, S) + createTree(length - 1, M)
#       else if type == S
#           return (2 * createTree(length - 1, C)) + createTree(length - 1, B)
#       else if type == M
#           return 2 * createTree(length - 1, C)
#       else (type must == B)
#           return 2 * createTree(length - 1, S)

# Finally, to represent the types C, S, M & B in mips, we can designate
# C to 1, S to 2, M to 3, and B to 4. The mips implementation is below

# Driver (Caller):-------------------------------------------------------------

# control flow that calls the recursive function for each valid starting
# number, then jumps to print the results and end the program
      # The registers $s4 - $s7 will always be certain constants for comparison
      li $s0, 2        # sets $s0 to 2 for comparison sake (S type)
      li $s5, 1        # sets to compare to C type
      li $s6, 3        # sets to compare to M type
      # Prepares for the series of recursive calls
      li $s1, 7        # sets $s1 to the length of phone numbers

      # Recall:
      # valid_nums = 2 * createTree(M) + 2 * createTree(S) + 3 * createTree(C)  

        # Calls and computes the number of valid phone nums for C types
      li $a0, 1           # loads 2(C) to call createTree
      jal createTree      # starts createTree with a type s and $s1 as length
      add $t0, $s3, $zero # moves the result of the last computation to $t0
      sll $s3, $s3, 1     # multiplies the result by 2 [2 * createTree(C)]
      add $t0, $t0, $s3   # adds [2*createTree(C)] to $t0 for [3*createTree(C)]

      # Calls and computes the number of valid phone nums for S types
      li $a0, 2           # loads 3(S) to call createTree
      jal createTree      # starts createTree with a type S and $s1 as length
      sll $s3, $s3, 1     # multiplies the result by 2 [2 * createTree(S)]
      add $t0, $t0, $s3   # adds the result of the last computation to $t0

      # Calls and computes the number of valid phone nums for M types
      li $a0, 3           # loads 3(M) to call createTree
      jal createTree      # starts createTree with a type M and $s1 as length
      sll $s3, $s3, 1     # multiplies the result by 2 [2 * createTree(M)]
      add $t0, $t0, $s3   # adds the result of the last computation to $t0

      j end_program


# createTree:------------------------------------------------------------------

# Precondition:  $s0 = 2 (for comparisons), $s1 = length, $a0 = type
# Postcondition: $s3 will hold the number of valid numbers for that subtree
# Recall, C = 1, S = 2, M = 3 & B = 4.
createTree: # base case
            bne $s1, $s0, recurse# executes if (length == 2)
            # adds 2 to the counter, adds 1 more if type == S
            addi $s3, $s3, 2     # every type has at least 2 valid jumps
            beq $a0, $s0, extra  # jumps if type == S (2)
            jr $ra               # returns to that address (previous function)
extra:      addi, $s3, $s3, 1    # increments the counter by another valid jump
            jr $ra               # returns to that address (previous function)

recurse:    addi, $sp, $sp, -4  # clears space for a word
            sw $ra, ($sp)       # stores the return address on the stack

            li $s3, 0            # clears register $s3 (return values)
            addi $s1, $s1, -1    # decrements the length

            beq $a0, $s5, c_type # if type == c, go to c block
            beq $a0, $s0, s_type # if type == s, go to s block
            beq $a0, $s6, m_type # if type == m, go to m block
            
            # if type == b, block is here [return 2 * createTree(length-1, S)]
            li $a0, 2            # loads 2(S) to createTree
            jal createTree         # calls createTree on S type
            addi $s1, $s1, 1     # restores the length to what it should be
            sll $s3, $s3, 1      # multiplies the result by 2
            lw $ra, ($sp)        # fetches the return address from the stack
            addi $sp, $sp, 4     # clears the stack of the return address
            jr $ra               # returns to the address in $ra (prev func)

c_type:     # block for type c, 
            # [return createTree(length - 1, S) + createTree(length - 1, M)]
            li $a0, 2            # loads 2(S) for createTree
            jal createTree       # calls createTree on C type
            addi $sp, $sp, -4    # decrements the stack for a word
            sw $s3, ($sp)        # stores the recent calculation on the stack
            li $s3, 0            # clears the return value for the call
            li $a0, 3            # prepares for createTree on M type
            jal createTree       # calls createTree for M type
            addi $s1, $s1, 1     # restores the length to what it should be
            lw $t1, ($sp)        # restores the previous tree calculation
            add $s3, $s3, $t1    # adds the previous and current calculations
            lw $ra, 4($sp)       # fetches the return address from the stack
            addi $sp, $sp, 8     # clears the stack of two words
            jr $ra               # returns to the address in $ra (prev func)

s_type:     # block for type s, 
            # [return (2 * createTree(length - 1, C)) + createTree(length - 1, B)]
            li $a0, 1            # loads 1(C) to createTree
            jal createTree       # calls createTree on C type
            sll $s3, $s3, 1      # multiplies the result by 2
            addi $sp, $sp, -4    # decrements the stack for a word
            sw $s3, ($sp)        # stores the recent calculation on the stack
            li $s3, 0            # clears the return value for the call
            li $a0, 4            # loads 4(B) for createTree
            jal createTree       # calls createTree for B type
            addi $s1, $s1, 1     # restores the length to what it should be
            lw $t1, ($sp)        # restores the previous tree calculation
            add $s3, $s3, $t1    # adds the previous and current calculations
            lw $ra, 4($sp)       # fetches the return address from the stack
            addi $sp, $sp, 8     # clears the stack of two words
            jr $ra               # returns to the address in $ra (prev func)

m_type:     # block for type m, [return 2 * createTree(length - 1, C)]
            li $a0, 1            # loads 1(C) to createTree
            jal createTree       # calls createTree on C type
            addi $s1, $s1, 1     # restores the length to what it should be
            sll $s3, $s3, 1      # multiplies the result by 2
            lw $ra, ($sp)        # fetches the return address from the stack
            addi $sp, $sp, 4     # clears the stack of the return address
            jr $ra               # returns to the address in $ra (prev func)


# End Program:-----------------------------------------------------------------

# Precondition: $t0 holds the number of valid phone numbers                                        
# Prints the results and exits the program
end_program: la $a0, output     # loads output string for syscall
             li $v0, 4          # specify Print String service
             syscall            # prints the output string
             la $a0, ($t0)      # loads number of valid phone #s for syscall
             li $v0, 1          # specify Print Integer service
             syscall            # prints the integer
             li $v0, 10         # system call for exit
             syscall            # Exit!
