
import java.io.Serializable;

public class Context implements Serializable{
    
	// Necessary for throwing objects around in sockets
	private static final long serialVersionUID = 1599951L;
    
	public String cat;

	public Context(String cat){
		this.cat = cat;
	}


}