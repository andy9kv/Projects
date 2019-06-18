;; --------------------
;; Andrew Morato
;; Traveling Salesman
;; November 20, 2017
;; --------------------

;; NOTE: Many of the functions require a build graph to be in
;;       memory in order to work properly. To do this, simply
;;       run (build-graph-one) or (build-graph-two) before
;;       running any other functions

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; Universal helper functions

;; removes an element from a list by index
(defun rm-element(index lst)
	(cond ((> (length lst) 0) (cond ((= index 0) (cdr lst))
	      		(t (append (list (car lst))
	    	       (rm-element (- index 1) (cdr lst))))))
	  	  (t (list))))

;; returns the sublist between i and j
(defun sublist(lst i j)
	(cond ((= i j) (list (nth i lst))) ((> i j) (list))
		(t (append (list (nth i lst)) (sublist lst (+ i 1) j)))))

;; returns the cost between both verticies
(defun cost(v u)
	(defun cost-help(v u)
		(cond ((boundp 'weights) (let ((c -1)) (dolist (x weights)
			(cond ((eq (car (car (car x))) v)
				(let ((edges x)) (dolist (y edges)
					(cond ((eq u (car (cdr (car y))))
						(setq c (car (cdr y)))))))))) c))
			(t "graph not defined")))
	(let ((w (cost-help v u)))
		(let ((final (cond ((eq w -1) (cost-help u v)) (t w))))
			(cond ((eq final -1) "no such pair") (t final)))))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 1) How would you represent the edge 2 costs to support your code?
;;    Write the chosen lisp data structures for both graphs (2p)

;; takes in a list representation of a complete graph of the form
;; ((a b c d)(5 10 15)(20 15)(30))  where the first list has the
;; vertices (a->d), the next list holds the weights of
;; (a-b a-c a-d), the following (b-c b-d), and the last one (c-d)
(defun build-graph(lst)
	;; returns a workable list of edge weights
	(defun get-weights(sym lst)
		(cond ((> (length (car lst)) 0) 
			(append (list (append 
					(list (append (list sym) (list (car (car lst)))))
					(list (car (car (cdr lst))))))
				(get-weights sym (append 
					(list (cdr (car lst))) 
					(list (cdr (car (cdr lst))))))))
			(t (list))))
	(cond ((> (length (car lst)) 1)
			(setf weights (append 
				(list (get-weights (car (car lst)) (append
					(list (cdr (car lst))) (list (car (cdr lst))))))
				(build-graph
				  (append (list (cdr (car lst))) (cdr (cdr lst)))))))
		(t (list))))

;;builds the first graph using build-graph
(defun build-graph-one() 
	(build-graph (list '(A B C D E) '(300 100 100 75) '(50 100 125)
	                   '(75 125) '(50))))

;;builds the second graph build-graph
(defun build-graph-two() 
	(build-graph (list '(A B C E M S) '(10 15 14 11 10)
		               '(8 13 15 9) '(11 16 10) '(9 6) '(9))))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 2) Write a lisp function init-loop which takes an initial loop
;;    with all cities listed in alphabetical order and produces its
;;    random permutation. Your two code must produce a real random
;;    shuffle of the initial loop (3p)

(defun init-loop(lst)
	(cond ((> (length lst) 1)
		(let ((rand (random (length lst))))
		    (append (list (nth rand lst)) 
		    	    (init-loop (rm-element rand lst)))))
		      (t (list (car lst)))))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 3) Write a lisp function loop-cost which computes the cost for a
;;    given loop (5p)

(defun loop-cost(lst)
	(defun gen-cost(lst)
		(cond ((> (length lst) 1)
		   (+ (cost (car lst) (car (cdr lst))) (gen-cost (cdr lst))))
		  (t 0)))
	(+ (gen-cost lst) (cost (car lst) (car (last lst)))))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 4) Write a lisp function improve-loop which takes a given loop and
;;    improves it if possible. The improvement should use 2-opt
;;    heuristics which removes a pair of edges out of the loop and
;;    reconnects the loop to lower its cost if possible (10p)

;; NOTE: Currently, this function cannot make the loop worse so that
;;       it can be made even better in future iterations, the
;;       function can only immediately improve the loop

(defun improve-loop(lst)
	;; takes a loop and the indecies of two pairs and returns a loop
	;; that is optimized with respect to those pairs if possible,
	;; ensuring the new connection does not disconnect the graph
	(defun two-opt(lst i j)
		(cond ((> (abs (- i j)) 1) 
			(let ((a i) (v j)
			 (b (cond ((= (length lst) (+ i 1)) 0) (t (+ i 1))))
			 (u (cond ((= (length lst) (+ j 1)) 0) (t (+ j 1)))))
			 (cond ((> (+ (cost (nth a lst) (nth b lst))
			 			  (cost (nth v lst) (nth u lst))) 
			 		   (+ (cost (nth a lst) (nth v lst))
			 		   	  (cost (nth b lst) (nth u lst))))
				(append (sublist lst 0 a) (list (nth v lst))
						(reverse (sublist lst (+ b 1) (- v 1))) 
						(list (nth b lst)) 
						(sublist lst u (- (length lst) 1))))
			 	(t lst))))
			(t lst)))
	;; calls two-opt until an improvement to the loop is found
	(defun improve(lst i j)
		(let ((opt (two-opt lst i j))) (cond ((eq opt lst)
			(cond ((<= (length lst) (+ j 1)) 
				(cond ((<= (length lst) (+ i 1)) lst)
					  (t (improve lst (+ i 1) 0))))
				(t (improve lst i (+ j 1)))))
			(t opt))))
	(improve lst 0 0))

;;-------------------------------------------------------------------
;;-------------------------------------------------------------------

;; 5) Write a lisp function solve-tsp which solves TSP given an input
;;    graph defined in part 1. Your code should use the functions
;;    written in parts 2-4 (10p)

;; Calls improve-loop on a fixed amount of random permutations of
;; the vertices in the graph based on the size of n. This algorithm
;; solves TSP rougly 98% of the time, which can be increased by
;; enlarging the random solution attempts (currently at 2^n)

(defun solve-tsp()
	;; extracts the list of vertices from the weights data structure
	(defun get-vertices(lst)
		(cond ((eq lst NIL) (list))
			  (t (remove-duplicates (append (car (car (car lst))) 
			  		(get-vertices (rm-element 0 lst)))))))
	;; returns the best improved loop from n shuffled loops
	(defun approx-solution(lst n)
		(cond ((< n 0) lst)
			(t (let ((x (init-loop lst))) (cond 
				((< (loop-cost (improve-loop x)) (loop-cost lst))
					(approx-solution x (- n 1)))
			  	(t (approx-solution lst (- n 1))))))))
	(let ((vertices (get-vertices weights)))
		(approx-solution vertices (expt 2 (length vertices)))))