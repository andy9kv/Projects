/*

	This is the Client classs, it receives data from the Server,
	displays it to the user and sends back any information to the
	server in a continuous cycle. 

*/

import java.util.ArrayList;
import java.util.Scanner;

public class Client {

	/* ---------------------------------------------------------- */
	/*                            Main                            */
	/* ---------------------------------------------------------- */

	public static void main(String[] args) {
		
		/* Connects to the server */
		Scanner sc = new Scanner(System.in);
		System.out.print("Please enter the Server's ip:\n>> ");
		String ip = sc.nextLine();
		System.out.print("Please enter a port number:\n>> ");
		String p = sc.nextLine();
		int port = Integer.parseInt(p);

		Client c = new Client();
		c.driver(sc, ip, port);
		System.out.println("bye bye\n");
	}

	/* ---------------------------------------------------------- */
	/*                            Driver                          */
	/* ---------------------------------------------------------- */

	public void driver(Scanner sc, String ip, int port){
		System.out.println("\n---> Welcome to the game <---\n");
		String intro = "The object of this game is to hit the bad "
		+ "guy [B] with an arrow [a]. You are the player [P] and "
		+ "can only launch an attack once you hold the bow [b]. A "
		+ "bow and arrow(s) will be scattered around the board. "
		+ "Pick them up by moving over them and use them to attack "
		+ "the enemy!";
		System.out.println(intro + "\n");
		System.out.print("Please enter the width of the board " +
			               "(min 5):\n>> ");
		int width = Integer.parseInt(sc.nextLine());
		System.out.print("Please enter the height of the board " +
			               "(min 5):\n>> ");
		int height = Integer.parseInt(sc.nextLine());
		height = (height > 5) ? height : 5;
		width = (width > 5) ? width : 5;
		Message msg = new Message(width, height, Message.getIP());
		msg = comm(ip, port, msg);
		ArrayList<Item> items = msg.items;
		displayCommandMenu();
		String resp = "", strResponse = "";

		while(!resp.equals("quit")){
			drawBoard(msg.items, width, height, msg.numArrows,
				      msg.hasBow, strResponse, resp, msg.response);
			System.out.print(">> ");
			resp = sc.nextLine();
			System.out.println();
			// responds to the user input
			switch (resp) {
				case "right":  /* move right */
				case "left":  /* move left */
				case "up": /* move up */
				case "down": /* move down */
					msg = comm(ip, port, new Message(resp));
					if(msg.response == 0){
						strResponse = "successful move";
					} else if(msg.response == 1){
						strResponse = "cannot move into wall";
					} else if(msg.response == 2){
						strResponse = "cannot move into boss";
					} else if(msg.response == 5){
						strResponse = "picked up a bow";
					} else if(msg.response == 6){
						strResponse = "picked up an arrow";
					}
					break;
				case "sr": /* shoot bow right */
				case "sl": /* shoot bow left */
					msg = comm(ip, port, new Message(resp));
					// SHOW SHOOTING ANIMATION
					if(msg.response == 3){
						strResponse = "hit the boss!";
						drawBoard(msg.items, width, height, 
							      msg.numArrows, msg.hasBow,
							      strResponse, resp, msg.response);
						msg = new Message("quit");
						msg.write(ip, port);
						System.out.println("You've won!");
						return;
					} else if(msg.response == 4){
						strResponse = "you missed. keep trying!";
					} else if(msg.response == 7){
						strResponse = "error. tried to shoot "
						              + "without an arrow/bow";
					}
					break;
				case "print": /* print command menu */
					displayCommandMenu();
					break;
				case "quit": /* rage quit the system */
					msg = new Message("quit");
					msg.write(ip, port);
					System.out.println("rage quitting now");
					return;
				default: strResponse = "invalid command!";
			}
		}
	}

	/* ---------------------------------------------------------- */
	/*                       Communication                        */
	/* ---------------------------------------------------------- */

	/* writes the given message to the ip and reads a new message
	   from that same ip. Returns the new Message */
	private Message comm(String ip, int port, Message msg){
		msg.write(ip, port);
		return Message.listen(port-1);
	}

	/* ---------------------------------------------------------- */
	/*                       Graphical Display                    */
	/* ---------------------------------------------------------- */

	/* Takes a list of Items (i.e. location - object pairs) and 
	   constructs a board with the Items in their respective 
	   locations. Outputs this to the terminal

	     ----------------------
	     |                   P|
	     |    *               |    This is an example of a
	     |                    |    20 x 10 board with Items
	     |                    |    at (5, 1), (10, 5) &
	     |                    |    (16, 9) and the character
	     |         *          |    at (19, 0). Dimensions of 
	     |                    |    this board are from (0, 0)
	     |                    |    to (19, 9)
	     |               *    |
	     |                    |
	     ----------------------                              */
	public void drawBoard(ArrayList<Item> items, int w, int h, 
		                 int numArrows, boolean bow, String msg,
		                 String cmd, int type){
		ArrayList<ArrayList<Character>> board;
		board = new ArrayList<ArrayList<Character>>();
		/* builds an empty board */
		for(int i = 0; i < h + 2; i++){
			ArrayList<Character> line = new ArrayList<Character>();
			Character frame = '|';
			if(i == 0 || i == h + 1){
				frame = '-';
			}
			line.add(frame);
			for(int j = 0; j < w; j++){ 
				if(frame == '-'){
					line.add(frame);
				} else {
					line.add(' ');
				}
			}
			line.add(frame);
			board.add(line);
		}

		/* fills the board with the Items */
		for(int i = 0; i < items.size(); i++){
			Item item = items.get(i);
			int x = item.location.x + 1, y = item.location.y + 1;
			board.get(y).set(x, item.type);
		}

		/* Draws an arrow shooting animation if bow was shot */
		Item boss = items.get(items.size()-1);
		int bx = boss.location.x, by = boss.location.y;
		Item player = items.get(items.size()-2);
		int px = player.location.x, py = player.location.y;
		System.out.println("[cmd: " + cmd + "]");
		if(cmd.equals("sr") && type != 7){
			int shotEnd = board.size() - 1;
			if(py == by && px < bx){ // player hits boss
				shotEnd = bx + 1;
			}
			for(int i = px + 2; i < shotEnd; i++){
				if(i == shotEnd - 1){ // last one
					board.get(py + 1).set(i, '>');
				} else {
					board.get(py + 1).set(i, '-');
				}
			}
		} else if(cmd.equals("sl") && type != 7){
			int shotEnd = 0;
			if(py == by && px > bx){ // player hits boss
				shotEnd = bx + 1;
			}
			for(int i = px; i > shotEnd; i--){
				if(i == shotEnd + 1){ // last one
					board.get(py + 1).set(i, '<');
				} else {
					board.get(py + 1).set(i, '-');
				}
			}
		}

		/* Converts the ArrayList representation of a board to
		   a String */

		String gameBoard = "";
		for(int i = 0; i < board.size(); i++){
			ArrayList<Character> arrayLine = board.get(i);
			String line = "";
			for(int j = 0; j < arrayLine.size(); j++){
				line += arrayLine.get(j);
			}
			if(i == 1){ /* Write state message */
				line += "     " + msg; 
			} else if(i == 2){ /* Write players location */
				line += "     Player Location: ["+px+" "+py+"]"; 
			} else if(i == 3){ /* Boss location */
				line += "     Boss Location: [" + bx + " " + by+"]";
			} else if(i == 4){ /* num arrows */
				line += "     Arrows: " + numArrows; 
			} else if(i == 5){ /* have bow */
				line += "     Hold bow: " + bow; 
			}
			gameBoard += " " + line + "\n";
		}

		System.out.println(gameBoard);
	}


	/* Command Menu */
	private void displayCommandMenu(){
		System.out.println(" Command Menu\n");
		System.out.println("   right  -->  move right");
		System.out.println("    left  -->  move left");
		System.out.println("      up  -->  move up");
		System.out.println("    down  -->  move down");
		System.out.println("      sr  -->  shoot bow right");
		System.out.println("      sl  -->  shoot bow left");
		System.out.println("    quit  -->  rage quit");
		System.out.println("   print  -->  print command list\n");
	}


}


/*

	TODO:

		1) add Date functionality to send for every command [server]
		2) add Date functionality to read for every command [client]
		   (display time sent, time received, and time elapsed)
		3) do the same in RMI/RPC















*/