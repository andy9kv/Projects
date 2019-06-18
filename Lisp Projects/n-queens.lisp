
;; --------------------------
;; Andrew Morato
;; N-Queens
;; October 6, 2017
;; --------------------------

;; Debugging -----

;; Prints the represented board
(defun print-board(lst)
	(defun get-row(pos n)
		(setf row (list))
		(loop for x from 1 upto n do
		(cond ((= pos x) (setf row (append row (list 1))))
			  (t (setf row (append row (list 0))))))
	row)
	(dolist (x lst)
		(print (get-row x (length lst))))
	(values))


;; 1) Checks if the Queens' position on the board are illegal
;;    Returns true if the placement is valid, else NIL
(defun check-legal(lst)
	(defun check-diagonal(lst)
		(setf valid t)
		(setf i 1)
		(dolist (x lst)
			(setf sum 0)
			(setf j 1)
			(dolist (y lst)
				(cond ((not (= y 0))
					(cond ((= (abs (- y x)) (abs (- i j))) (setf sum (+ sum 1))))))
				(setf j (+ j 1)))
			(cond ((> sum 1) (setf valid NIL)))
			(setf i (+ i 1)))
		valid)
	(defun check-col(lst)
		(cond ((= (length (remove-duplicates lst)) (length lst)) t)
			  (t NIL)))
	(cond (( not (check-diagonal lst)) NIL)
		  ((not (check-col lst)) NIL)
		  (t t)))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 2) Generates permutations of 1...n. n = 3 would generate 3 fitted
;;    into any positions of ((2 1) (1 2)) <-- (1...2). Returns the
;;    number of valid orderings that solve the n-Queens problem for 
;;    all possible orderings of 1...n
(defun gen-perm(n)
	;; Takes a number and returns a lst with it inserted at pos
	;; For example, ins-num 9 3 (1 2 3 4) would return (1 2 3 9 4)
	(defun ins-num(num pos lst)
		(cond ((> (length lst) 0)
			     (cond ((= pos 0) (append (list num) lst))
			           (t (append (list (car lst)) (ins-num num (- pos 1) (cdr lst))))))
			  (t (list num))))
	;; Takes a list and returns a list of (length of lst + 1) lists
	;; with sym inserted in any possible position AND a count of valid
	;; (sols) orderings for the n-queens problem at the list head
	(defun ins-everywhere(sym lst)
		(setf fnl (list))
		(setf sols 0)
		(loop for i from 0 upto (length lst) do
			(setf ordering (ins-num sym i lst))
			(setf fnl (append fnl (list ordering)))
			(cond ((check-legal ordering) (setf sols (+ sols 1)))))
		(append (list sols) fnl))
	;; Takes a list of lists (lst) and returns another list of lists
	;; with sym inserted into any possible position in those lists
	;; AND a count of the valid orderings for the n-Queens problem
	;; attached at the head of the list
	(defun build-perm(sym lst)
		(setf lst (cdr lst))
		(setf perms (list))
		(setf solutions 0)
		(dolist (x lst)
			(setf orderings (ins-everywhere sym x))
			(setf solutions (+ solutions (car orderings)))
			(setf perms (append perms (cdr orderings))))
		(append (list solutions) perms))
	(setf permutation (list 0 (list 1)))
	(cond ((> n 3)
		     (loop for x from 2 upto n do
			     (setf permutation (build-perm x permutation)))
		     (car permutation))
	      (t (car permutation))))

	;; For n = 4, there are 2 solutions
	;; For n = 5, there are 10 solutions
	;; For n = 6, there are 4 solutions
	;; For n = 7, there are 40 solutions (~0.5 sec to compute)
	;; For n = 8, there are 92 solutions (~5 sec to compute)
	;; For n = 9, there are 352 solutions (~5 min to compute)
	;; For n >= 10, the machine takes too long to compute

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 3) Returns a list (of Queens' positions) that satisfies the n-Queens
;;   problem for any given n AND the amount of shuffles done
(defun find-sol(n)
	(defun shuffle(n)
		;; removes an element from the list
		(defun rm-element(index lst)
			(cond ((> (length lst) 0) 
				      (cond ((= index 0) (cdr lst))
			    	  (t (append (list (car lst)) (rm-element (- index 1)(cdr lst))))))
			  	  (t (list))))
		;; returns an element from the list
		(defun get-element(index lst)
			(cond ((> (length lst) 1) 
				      (cond ((= index 0) (car lst))
			    	  (t (get-element (- index 1) (cdr lst)))))
			  	  (t (car lst))))
		;; makes paramenter n into a list (1 2 3 ... n)
		(setf lst (list))
		(loop for i from 1 upto n do (setf lst (append lst (list i)))) 
		;; gets random elements from lst and adds them to final until
		;; lst is empty, returns final
		(setf final (list))
		(loop for x from 1 upto (length lst) do
			(setf rand (random (length lst)))
			(setf final (append final (list (get-element rand lst))))
			(setf lst (rm-element rand lst)))
		final)
	(setf shuffled 0)
	(cond ((< n 4) (list 'no 'solution 'possible))
		  (t 
			(loop
				(setf shuffled (+ shuffled 1))
				(setf lst (shuffle n))
				(cond ((check-legal lst) 
				(return (append (list lst) (list (list 'shuffles '-> shuffled))))))))))

	;; Since the list is shuffled at each iteratrion, I cannot expect
	;; to get a solution in the same amount of shuffles on different
	;; executions. In the first run:
		;; For n = 4, 5 shuffles
		;; For n = 5, 3 shuffles
		;; For n = 6, 307 shuffles
		;; For n = 7, 133 shuffles
		;; For n = 8, 296 shuffles
		;; For n = 9, 2363 shuffles (~0.5 sec to compute)
		;; For n = 10, 1425 shuffles (~0.5 sec to compute)
		;; For n = 11, 49349 shuffles (~7 sec to compute)
		;; For n = 12, 3635 shuffles (~1 sec to compute)
		;; For n = 13, 53411 shuffles (~10 sec to compute)
		;; For n = 14, 289201 shuffles (~52 sec to compute)
		;; For n = 15, 290983 shuffles (~2.1 min to compute)
		;; For n = 16, 7249481 shuffles (~1 hr to compute)
		;; For n >= 17, the machine takes too long to compute