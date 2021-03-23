
import java.rmi.Naming;

public class RMIServer { 

	public static void main(String args[]) throws Exception { 
		Hello obj = new Hello();
		Server srv = new Server();
		Naming.rebind("HELLO", srv);
		System.out.println("Server started");
		//Naming.unbind("HELLO");
	}
} 