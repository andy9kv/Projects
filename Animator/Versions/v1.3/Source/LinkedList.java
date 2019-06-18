
/*
* Andy Morato
* A LinkedList implementation of my own making to
* faciliate integration with existing code
* Created December 26th, 2017
*/

public class LinkedList<T> {

/*-----Instance Variables-----*/

public Lnode<T> head;
public int length;

/*------------------------Linked List Nodes------------------------*/

/*Inner class that supports nodes*/
public class Lnode<T> {
	/*-----Instance Variables-----*/
	public T data;
	public Lnode<T> next;
	/*---------Constructor--------*/
	public Lnode(T data){ this.data = data;}
}


/*---------------------------Constructors--------------------------*/

/*Default Constructor*/
public LinkedList(){ this.length = 0; }

/*Constructor that takes the initial data.*/
public LinkedList(T start){ 
	this.head = new Lnode<T>(start);
	this.length = 1;
}


/*---------------------------List Access---------------------------*/

/*Returns the ith node in the list (begin at 0)*/
public Lnode<T> get(int i){
	Lnode<T> p = head;
	for(int index = 0; index < i; index++){
		if(p.next != null)
			p = p.next;
		else
			return null;
	}
	return p;
}

/*Returns the ith node's data in the list*/
public T getData(int i){ return get(i).data;}

/*Returns the last element in the list*/
public Lnode<T> getLast(){
	if(head == null) { return head; }
	else {
		Lnode<T> p = head;
		while(p.next != null) { p = p.next; }
		return p;
	}
}

/*Adds Lnode n to the end of the list*/
public void append(Lnode<T> n){
	if(head == null) { this.head = n; }
	else {
		Lnode<T> p = getLast();
		p.next = n;
	}
	this.length = this.length + 1;
}

/*Adds 'data' to the end of the list*/
public void append(T data) { append(new Lnode<T>(data)); }

/*Inserts data after the ith node in the list*/
public void insert(Lnode<T> n, int i){
	Lnode<T> prev = get(i);
	if(prev != null) { 
		Lnode<T> next = prev.next;
		prev.next = n;
		n.next = next;
		this.length = this.length + 1;
	}
}

/*Inserts data after the ith node in the list*/
public void insert(T data, int i) { insert(new Lnode<T>(data), i); }

/*Removes the element at the ith position in the list*/
public void remove(int i){
	if(i == 0) {  //new list head
		this.head = head.next; 
		this.length = this.length - 1;
	}
	else {
		Lnode<T> prev = get(i - 1);
		if(prev != null) { //ith - 1 element exists
			Lnode<T> remove = prev.next;
			if(remove != null) { //ith - 1 element is not the tail
				Lnode<T> next = remove.next;
				prev.next = next;
				this.length = this.length - 1;
			}
		}
	}
}


/*---------------------------Node Access---------------------------*/

/*Sets the next field of the ith node in the linked list to a
new node holding T data*/
public void setNext(int i, T data){
	Lnode<T> prev = get(i);
	Lnode<T> n = new Lnode<T>(data);
	prev.next = n;
}

/*Sets the head of the linked list to a new node containing T data*/
public void setHead(T data) { this.head = new Lnode<T>(data); }

}