
import java.rmi.Remote; 
import java.rmi.RemoteException;  

// Creating Remote interface for our application 
public interface HelloIF extends Remote {  
   Context sendMsg(Context c) throws RemoteException;  
} 