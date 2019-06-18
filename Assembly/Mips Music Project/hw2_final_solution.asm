
# Andrew Morato (amorato) G00892138

# syscall 33
# $a0 = pitch (0-127)
# $a1 = duration in milliseconds
# $a2 = instrument (0-127)
# $a3 = volume (0-127)
  
  li $v0, 33
  li $a0, 60
  li $a1, 250
  li $a2, 4
  li $a3, 127
  
song: li $a1, 250
      li $a0, 62
      syscall
      syscall
      syscall
      li $a0, 58
      syscall
      li $a0, 62
      syscall
      li $a0, 65
      syscall 
      
      li $a1, 400
      li $a0, 56
      syscall
      
      li $a1, 350    
      li $a0, 66
      syscall
      li $a0, 63
      syscall
      li $a0, 60
      syscall 
      li $a0, 55
      syscall
      li $a0, 62
      syscall
      li $a0, 59
      syscall
      li $a0, 56
      syscall
      
      li $a2, 275
      li $a0, 63
      syscall
      li $a0, 66
      syscall
      li $a0, 68
      syscall
      li $a0, 70
      syscall
      li $a0, 66
      syscall
      li $a0, 69
      syscall
      li $a0, 63
      syscall
      li $a0, 66
      syscall
      li $a0, 63
      syscall
      li $a0, 60
      syscall

      #j song
      
      
  

# Precondition: $s3 holds the number of valid phone numbers                                        
# Prints the results and exits the program
end_program: li $v0, 10         # system call for exit
             syscall            # Exit!


# Helper Functions:------------------------------------------------------------
  
# returns the the address stored in the first word in the stack
ret:  lw $ra, ($sp)        # stores the return address in the stack
      addi, $sp, $sp, 4    # increments the stack by a word (pops the stack)
      jr $ra               # returns to that address   
