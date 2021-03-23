import java.rmi.Naming;

public class RMIClient {  

   public static void main(String[] args) throws Exception {  
      
   	HelloIF obj = (HelloIF) Naming.lookup("HELLO");
   	Context c = new Context("big boi");
   	Context newC = obj.sendMsg(c);
   	System.out.println(newC.cat);
   } 
}