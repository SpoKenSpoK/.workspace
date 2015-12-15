import java.util.Vector;
import java.io.Serializable;

public class Banque implements Serializable{
	private static final long serialVersionUID = 0;
	private Vector<Compte> bank;	//Vector contenant les comptes
	
	//Constructeur
	public Banque(Vector<Compte> bank){
		this.bank = bank;
	}
	
	//Getter & Setter
	public void setBank(Vector<Compte> bank){ this.bank = bank; }
	public Vector<Compte> getBank(){ return this.bank; }
	
	//Methode d'affichage
	public void display(){
		for(int i=0; i<this.bank.size(); ++i){
			System.out.println("Account number "+i+" : ");
			this.bank.elementAt(i).display();
			System.out.println("--------\n");
		}
	}
}
