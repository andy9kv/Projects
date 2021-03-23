
/* 

	This Message class controls passing messages via sockets. It
	largely consists of two parts:

	Structure:

		The fields of this class get Serialized and passed through
		sockets. They are the messages

	Static Functions:

		The static functions of this class enable passing messages,
		listening to requests, and waiting for data on a particular
		port number
   
*/

/* Imports */

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Date;
import java.text.SimpleDateFormat;

public class Message implements Serializable {

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

	public Message(int width, int height, String ip){
		this.width = width;
		this.height = height;
		this.items = null;
		this.response = -1;
		this.numArrows = -1;
		this.hasBow = false;
		this.cmd = ip;
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

	/* ---------------------------------------------------------- */
	/*                          Writing                           */
	/* ---------------------------------------------------------- */

	/* Writes the Message to the given ip address on the specified
	   port */
	public void write(String ip, int port){
		System.out.print("Writing... [to IP " + ip + " on port #"
			             + port + "]\n");
		try {

			InetAddress ip_addr = InetAddress.getByName(ip);
			Socket sock = new Socket(ip_addr, port);
			ObjectOutputStream out = new ObjectOutputStream(
				                         sock.getOutputStream());
			this.dateSent = new Date(System.currentTimeMillis());
			out.writeObject(this);
			out.close();
			sock.close();
		}
		catch(Exception e) {
			System.out.println("Exception in writing!");
			System.out.println("IP Address: " + ip);
			System.out.println("Port number: " + port);
			e.printStackTrace();
			return;
		}
	}

	/* ---------------------------------------------------------- */
	/*                         Listening                          */
	/* ---------------------------------------------------------- */

	/* Listens for any communication on the port number. Returns
	   the object of type Message that it received */
	public static Message listen(int port){
		Message msg = null;
		try {
			ServerSocket listener = new ServerSocket(port);
			/* Blocking operation until someone attempts to 
			   write to this ip on this port number */
			System.out.print("Listening... [IP " + Message.getIP() +
			                 " listening on port #" + port + "]\n");
			Socket sock = listener.accept();

			/* Once a connection is formed, code to read Message
			   Objects from the socket */
			InputStream in = sock.getInputStream();
			ObjectInputStream stream = new ObjectInputStream(in);
			msg = (Message) stream.readObject();
			msg.dateReceived= new Date(System.currentTimeMillis());
			long milli = msg.dateReceived.getTime() 
			          - msg.dateSent.getTime();
			SimpleDateFormat formatter= new SimpleDateFormat(
			"yyyy-MM-dd 'at' HH:mm:ss:SS z");
			if(msg.items == null){ /* if sent by Client */
				System.out.println("Message sent by Client on -> " 
				                  + formatter.format(msg.dateSent));
				System.out.println("Message received from Client on"
				     + " -> " + formatter.format(msg.dateReceived));
			} else { /* if sent by Server */
				System.out.println("Message sent by Server on -> " 
				                  + formatter.format(msg.dateSent));
				System.out.println("Message received from Server on"
				     + " -> " + formatter.format(msg.dateReceived));
			}
			System.out.println("Time elasped in milliseconds -> "
				               + milli + "\n");
			sock.close();
			listener.close();
		}
		catch(Exception e) {
			System.out.println("Exception in Listening!");
			System.out.println("Port number: " + port);
			e.printStackTrace();
			return null;
		}
		return msg;
	}

	/* ---------------------------------------------------------- */
	/*                     Get Own IP Address                     */
	/* ---------------------------------------------------------- */

	/* Establishes a connection to some random IP address in order
	   to get the caller's real IP address */
	public static String getIP(){
		String ip = null;
		try(final DatagramSocket socket = new DatagramSocket()){
			socket.connect(InetAddress.getByName("8.8.8.8"), 10002);
			ip = socket.getLocalAddress().getHostAddress();
		}
		catch (Exception E) {
			System.out.println("[Error] Could not get IP address!");
		}
		return ip;
	}

}