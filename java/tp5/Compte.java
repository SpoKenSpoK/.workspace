import java.io.Serializable;

public class Compte implements Serializable {
	private static final long serialVersionUID = 0;	//Nécessaire pour l'implémentation de l'interface Serializable (se fait automatiquement sinon)
	private String name_owner; //<Nom du propriétaire du compte
	private double balance = 0; //<Solde du compte
	
	//Constructeur
	public Compte(String owner, double balance){
		this.name_owner = owner;
		this.balance = balance;
	}
	
	//Getter & Setter 
	public void setOwner(String owner){ this.name_owner = owner; }
	public String getOwner(){ return this.name_owner; }
	
	public void setBalance(double balance){ this.balance = balance; }
	public double getBalance(){ return this.balance; }
	
	//Methode d'affichage
	public void display(){
		System.out.println("Owner of account : "+this.getOwner());
		System.out.println("Account balance : "+this.getBalance()+" €");
	}
}
