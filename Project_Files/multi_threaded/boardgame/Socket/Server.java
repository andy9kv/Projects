/*
	This is the Server class which holds the state of the game and
	responds to the Client whenever it is invoked.

	The Server' is always listening for the Client's messages. Once
	a message is received , the Server changes the game state and
	sends an response back to the Client. Once a response it sent,
	the Server resumes listening. For multiple Clients, the Server
	could launch multiple threads to handle each received message.

*/

import java.lang.Math;
import java.util.ArrayList;
import java.util.Scanner;

public class Server {

	/* ---------------------------------------------------------- */
	/*        Global variables to keep track of game state        */
	/* ---------------------------------------------------------- */

	// board size
	private int width;
	private int height;

	/* list of items on the board excluding any characters */
	ArrayList<Item> itemsOnBoard;

	/* both characters on the board. Player must pick up the items
	   to defeat the boss! */
	private Item player;
	private Item bigBadBoss;

	/* other game state variables */
	private boolean doesPlayerHoldBow;
	private int numArrowsPlayerHas;

	/* ---------------------------------------------------------- */
	/*                  Initialization Functions                  */
	/* ---------------------------------------------------------- */

	// kicks off the game
	public static void main(String[] args) {
		Server server = new Server();
		server.entrypoint();
	}

	// kicks off the game
	public void entrypoint() {

		/* Gets the port number */
		Scanner sc = new Scanner(System.in);
		System.out.print("Please enter a port number:\n>> ");
		int port = Integer.parseInt(sc.nextLine());

		/* Waits to begin the game */
		System.out.println("Server is now online\n");
		Message msg = Message.listen(port);
		System.out.println("Inital message received from Client!");
		System.out.println("The game has begun..\n");

		/* Starts a new game */
		Server server = new Server();
		this.width = msg.width;
		this.height = msg.height;
		this.itemsOnBoard = generateRandomItems();
		this.player = generateRandomItem('P');
		this.bigBadBoss = generateRandomItem('B');
		this.numArrowsPlayerHas = 0;
		this.doesPlayerHoldBow = false;

		/* Sends the new game state to the Client and resumes
		   listening for a new command */
		sendResponse(msg.cmd, port, /*setup*/ 8);
		listen(msg.cmd, port);
		System.out.println("\nServer is exiting\n");
	}

	/* ---------------------------------------------------------- */
	/*                         Communication                      */
	/* ---------------------------------------------------------- */

	/* Continuously listens for a message from the Client and
	   replies by sending a response (detailed below). Resumes
	   listening afterward. */
	private void listen(String ip, int port){
		String cmd = "";
		while(true){
			Message msg = Message.listen(port);
			cmd = msg.cmd;
			if(cmd.equals("quit")){ break; }
			int result = handleCommand(cmd);
			sendResponse(ip, port, result);
		}
	}

	/* Sends the latest list of items and the player/boss locations
	   back to the Client along with a heartwarming response number
	   indicating the consequence of its actions. Responses are,
	       0 - moved without an issue
	       1 - ERROR invalid move, fell off edge of the map
	       2 - ERROR invalid move, moving into big bad boss
	       3 - shot arrow and hit the boss
	       4 - shot arrow and missed the boss
	       5 - picked up a bow
	       6 - picked up an arrow
	       7 - ERROR tried to shoot without an arrow/bow
	       8 - SETUP response message
	   The response message is two-fold. It contains a list of items
	   including the player and boss (their locations, types, etc) &
	   a response type which will be read by the Client */
	@SuppressWarnings("unchecked")
	private void sendResponse(String ip, int port, int type){
		ArrayList<Item> a = (ArrayList<Item>) itemsOnBoard.clone();
		a.add(this.player);
		a.add(this.bigBadBoss);
		System.out.println();
		Message msg = new Message(a, type, this.numArrowsPlayerHas,
		                          this.doesPlayerHoldBow);
		msg.write(ip, port-1);
	}

	/* ---------------------------------------------------------- */
	/*                          Game Setup                        */
	/* ---------------------------------------------------------- */

	/* generates a list of Items in random locations. Those items
	   are a bow('b') and multiple arrows('a') depending on the size
	   of the board. No two items can be in the same location */
	private ArrayList<Item> generateRandomItems(){
		// calculates the amount of arrows
		int size = height * width;
		int numArrows = (size / 50) + 1;

		// generates numArrows items of 'a' type and 1 'b' type
		ArrayList<Item> items = new ArrayList<Item>();
		items.add(generateRandomItem('b'));
		for(int i = 0; i < numArrows; i++) { 
			items.add(generateRandomItem('a')); 
		}
		return items;
	}

	/* generates an Item of given type in a random location that is
	   not occupied by any other Item in the board */
	private Item generateRandomItem(Character type){
		while(true){
			int x = (int)(Math.random() * (this.width));
			int y = (int)(Math.random() * (this.height));
			if(isLocationOccupied(x, y) == false){
				return new Item(x, y, type);
			}
		}
	}

	/* returns true if the given location on the board is occupied,
	   false otherwise. Checks all the Item's locations */
	private boolean isLocationOccupied(int x, int y){
		Item.Pair p = new Item.Pair(x, y);
		// checks the player's location
		if(this.player != null){
			if(this.player.location.equal(p) == true){
				return true;
			}
		}
		// checks the boss' location
		if(this.bigBadBoss != null){
			if(this.bigBadBoss.location.equal(p) == true){
				return true;
			}
		}
		// checks the loaction of pre-existing items
		if(this.itemsOnBoard != null){
			for(int i = 0; i < this.itemsOnBoard.size(); i++){
				if(itemsOnBoard.get(i).location.equal(p) == true){
					return true;
				}
			}
		}
		return false;
	}


	/* ---------------------------------------------------------- */
	/*                       Game Management                      */
	/* ---------------------------------------------------------- */

	/* adjusts the game state based on the user command. Returns a
	   result so that it could be sent back to the user. Results are
	       0 - moved without an issue
	       1 - ERROR invalid move, fell off edge of the map
	       2 - ERROR invalid move, moving into big bad boss
	       3 - shot arrow and hit the boss
	       4 - shot arrow and missed the boss
	       5 - picked up a bow
	       6 - picked up an arrow
	       7 - ERROR tried to shoot without an arrow/bow
	*/
	private int handleCommand(String cmd){
		int result = 0;
		int x = player.location.x, y = player.location.y;

		/* The boss moves every turn BEFORE the user takes
		   action WITHOUT the user seeing his new position */
		moveBoss();

		/* Acts based on the given command */
		switch(cmd){
			case "right":  /* move right */
				result = isValidLocation(x + 1, y);
				if(result != 0){
					return result;
				} else { 
					player.location.x += 1; 
					result = hasPlayerPickedUpAnything();
				}
				break;
			case "left":  /* move left */
				result = isValidLocation(x - 1, y);
				if(result != 0){
					return result;
				} else { 
					player.location.x -= 1;
					result = hasPlayerPickedUpAnything();
				}
				break;
			case "up": /* move up */
				result = isValidLocation(x, y - 1);
				if(result != 0){
					return result;
				} else { 
					player.location.y -= 1; 
					result = hasPlayerPickedUpAnything();
				}
				break;
			case "down": /* move down */
				result = isValidLocation(x, y + 1);
				if(result != 0){
					return result;
				} else { 
					player.location.y += 1; 
					result = hasPlayerPickedUpAnything();
				}
				break;
			case "sr": /* shoot bow right */
				if(this.numArrowsPlayerHas == 0 ||
				   this.doesPlayerHoldBow == false){
					return 7;
				}
				this.numArrowsPlayerHas -= 1;
				if(bigBadBoss.location.y == player.location.y &&
				   bigBadBoss.location.x > player.location.x){
					return 3; // hit boss!
				} else { return 4; }
			case "sl": /* shoot bow left */
				if(this.numArrowsPlayerHas == 0 ||
				   this.doesPlayerHoldBow == false){
					return 7;
				}
				this.numArrowsPlayerHas -= 1;
				if(bigBadBoss.location.y == player.location.y &&
				   bigBadBoss.location.x < player.location.x){
					return 3; // hit boss!
				} else { return 4; }
		}

		return result;
	}


	/* The boss is moved one tile randomly (if valid), either up,
	   down, right, or left */
	private void moveBoss(){
		int x = bigBadBoss.location.x;
		int y = bigBadBoss.location.y;
		while(true){
			int move = (int)(Math.random() * (4));
			switch(move){
				case 0: // move right
					if(isValidLocation(x + 1, y) == 0){
						bigBadBoss.location.x += 1;
						return;
					}
					break;
				case 1: // move left
					if(isValidLocation(x - 1, y) == 0){
						bigBadBoss.location.x -= 1;
						return;
					}
					break;
				case 2: // move up
					if(isValidLocation(x, y - 1) == 0){
						bigBadBoss.location.y -= 1;
						return;
					}
					break;
				case 3: // move down
					if(isValidLocation(x, y + 1) == 0){ 
						bigBadBoss.location.y += 1;
						return;
					}
					break;
			}
		}
	}

	/* returns 
	      0 if the move is valid
	      1 if the move goes off the edge
	      2 if the move is into the bad guy
	      3 if the move is into the player 
	*/
	private int isValidLocation(int x, int y){
		if(x < 0 || x >= this.width){ return 1; }
		if(y < 0 || y >= this.height){ return 1; }
		if(x == bigBadBoss.location.x && y == bigBadBoss.location.y)
			return 2;
		if(x == player.location.x && y == player.location.y)
			return 3;
		return 0;
	}


	/* Scans through the list of items to check if the player has
	   "moved" into a position that is occupied by an Item. If that
	   is the case, the player adds the Item to his stock and the
	   Item is removed from the Item list. If the Item is an arrow,
	   it is replenished in the board. Returns a,
	        0 - did not pick up anything
	        5 - picked up a bow
	        6 - picked up an arrow
	*/
	private int hasPlayerPickedUpAnything(){
		Item.Pair p = player.location;
		Item item = null;
		// iterates over the list of items
		for(int i = 0; i < itemsOnBoard.size(); i++){
			if(itemsOnBoard.get(i).location.equal(p)){
				item = itemsOnBoard.get(i);
				this.itemsOnBoard.remove(i);
				break;
			}
		}

		/* adds the item to the player inventory */
		if(item == null) { return 0; }
		if(item.type == 'b' ){ // picked up bow
			this.doesPlayerHoldBow = true;
			return 5;
		} 
		else if(item.type == 'a'){ // picked up arrow
			this.numArrowsPlayerHas++;
			// replenish an arrow to the board
			this.itemsOnBoard.add(generateRandomItem('a'));
			return 6;
		}
		return 0;
	}


}