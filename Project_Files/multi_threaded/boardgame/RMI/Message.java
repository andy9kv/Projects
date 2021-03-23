
/* 

	This Message class uses RMI to pass messages back and forth
	between the Client and the Server. Each message is a Serialized
	version of this class, with the fields below holding the data
   
*/

/* Imports */

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.io.Serializable;

public class Message implements Serializable{

	// Necessary for throwing objects around in sockets
	private static final long serialVersionUID = 1599951L;

	/* ---------------------------------------------------------- */
	/*                      Message Structure                     */
	/* ---------------------------------------------------------- */

	public int width;               // width of the board
	public int height;              // height of the board
	public ArrayList<Item> items;   // list of items
	public int response;            // int response type (results)
	public int numArrows;           // arrows the player holds
	public boolean hasBow;          // if player holds bow
	public String cmd;              // String command
	public Date dateSent;           // Time server sent response
	public Date dateReceived;       // Time client received

	public Message(int width, int height) {
		this.width = width;
		this.height = height;
		this.items = null;
		this.response = -1;
		this.numArrows = -1;
		this.hasBow = false;
		this.cmd = "init";
		this.dateSent = null;
		this.dateReceived = null;
	}

	public Message(ArrayList<Item> items, int response,
	               int numArrows, boolean hasBow){
		this.width = -1;
		this.height = -1;
		this.items = items;
		this.response = response;
		this.numArrows = numArrows;
		this.hasBow = hasBow;
		this.cmd = null;
		this.dateSent = null;
		this.dateReceived = null;
	}

	public Message(String cmd){
		this.width = -1;
		this.height = -1;
		this.items = null;
		this.response = -1;
		this.numArrows = -1;
		this.hasBow = false;
		this.cmd = cmd;
		this.dateSent = null;
		this.dateReceived = null;
	}
}