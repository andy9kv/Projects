import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.Serializable;
// Implementing the remote interface 
public class Hello extends UnicastRemoteObject implements HelloIF {  
   
    protected Hello() throws RemoteException { super(); }

   // Implementing the interface method 
   public Context sendMsg(Context c) throws RemoteException {  
      System.out.println("This is an example RMI program");
      System.out.println(c.cat);  
      c.cat = "oh da hello";
      return c;
   }  
} 