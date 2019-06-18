
import java.util.ArrayList;

class Test{

public static void main(String[] args) {
	
	Vec s1 = new Vec(0, 0);
 	Vec s2 = new Vec(2, 0);
  	Vec s3 = new Vec(2, 2);
 	Vec s4 = new Vec(0, 2);
 	Vec[] vs = new Vec[]{s1, s2, s3, s4};
 	Mat ms = new Mat(vs);
 	Poly s = new Poly(ms);

	Node n = new Node(s, "wow");
	
	n.rotate(180);
	n.translate(-2, -2);
	n.scale(5);

	n.transform(n);

}

}