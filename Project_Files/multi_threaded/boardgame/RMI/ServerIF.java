
import java.rmi.Remote; 
import java.rmi.RemoteException;  

// Creating Remote interface
public interface ServerIF extends Remote {  
  Message sendMsg(Message msg) throws RemoteException;  
} 