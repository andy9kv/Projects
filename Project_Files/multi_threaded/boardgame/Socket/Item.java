/* 
	This class defines an Item to simplify the game. Items have a 
	type and location. A list of Items are serialized and sent as
	responses from the Server to the Client
*/

import java.io.Serializable;

public class Item implements Serializable {

	// Necessary for throwing objects around in sockets
	private static final long serialVersionUID = 1599951L;

	public static class Pair implements Serializable {

		// Necessary for throwing objects around in sockets
		private static final long serialVersionUID = 1599951L;
		public int x; // x position
		public int y; // y position

		public Pair(int x, int y){
			this.x = x;
			this.y = y;
		}

		public boolean equal(Pair p){
			return (x == p.x && y == p.y);
		}
	}

	public Pair location;   // holds the Item's location
	public Character type;  // defines the type of the Item

	public Item(Pair location, Character type){
		this.location = location;
		this.type = type;
	}

	public Item(int x, int y, Character type){
		this(new Pair(x, y), type);
	}

	public String toString(){
		return "([" + location.x + ", " + location.y + "], " +
		        type +  ")";
	}
}































































