
Andrew Morato
March 6 2019
README file

The program has a very simple graphical interface. It is a board
that has several Items in random initial locations,

         ----------------------
	     |                   P|
	     |    B               |
	     |                    |
	     |               a    |
	     |                    |
	     |         a          |
	     |                    | 
	     |                    | 
	     |               b    |
	     |                    |
	     ---------------------- 


You are the player 'P' and can move left, right, up, or down with
the following commands,

	Command Menu

   right  -->  move right
    left  -->  move left
      up  -->  move up
    down  -->  move down
      sr  -->  shoot bow right
      sl  -->  shoot bow left
    quit  -->  rage quit
   print  -->  print command list

You can't move into any boundry or into the bad guy 'B'. The bow 'b'
and arrows 'a' are scattered among the board. Pick them up by moving
over them (you'll be notified by the game of what you've picked up).
Once you have a bow and at least one arrow you can use "sr" or "sl"
to shoot an arrow to the bad guy. You can only shoot left or right.
If you hit the bad guy, the game is over. Every shot results in a
lost arrow. Pick up more arrows and keep trying (arrows get 
replenished in random locations on the board). Furthermore, the bad
guy will randomly move by a single tile every iteration of the game!

There are two version of this game, Socket and RMI. The
functionality is the same but they underlying server-client
communication is different,

|---------------------------- Socket ------------------------------|

	The code can be found in the Socket folder and complied with
	javac *.java. The only thing the Server will ever ask for is a
	port number to begin listening on. The Client will ask for the
	ip of the Server (127.0.0.1 for local will work fine) and then
	a port number to communicate with the Server. Then the Client
	will ask for the width and height of the board (20 width x 10
	height works great!).

	Here is an example:

	|------ Server Session -----|

	$ java Server 
	Please enter a port number:
	>> 59995

	Server is now online

	|------ Client Session -----|

	$ java Client 
	Please enter the Server's ip:
	>> 127.0.0.1
	Please enter a port number:
	>> 59995

	Game now begins.


|----------------------------- RMI --------------------------------|


	The code can be found in the RMI folder. To compile and run,
	please execute the following command on the Server side terminal

	>> javac *.java
	>> rmiregistry &
	>> java Server

	NOTE: I had issues with exiting RMI properly. After exiting the
	      game, if the previous RMI process is not cleaned up,
	      rmiregistry & will not run. To clean it up please use

	      >> ps x | grep rmi

	      to see the rmiregistry process. and 

	      >> kill proc#

	      to end it. Sorry :(

	The Server is now launched and online. Please execute

	>> java Client

	to launch the client on the client side terminal. The game now
	begins. I would recommend to enter 20 for the width and 10 for
	the height of the board.
