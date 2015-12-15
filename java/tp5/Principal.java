import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.io.File;
import java.util.Vector;

public class Principal {
	public static void main(String [] arg){
		System.out.println("Welcome : \n"
				+ "What do you want to do ?\n"
				+ "(1) add a new account\n"
				+ "(2) find an existing account\n"
				+ "(3) display all accounts"
		);
		int choice = Lire.i();
		switch(choice){
			case 1 : save(); break;
			
			case 2:
				System.out.println("Savefile name : ");
				String filename = Lire.S();
				Banque bank_ = load(filename);
				System.out.println("Owner's name");
				String name = Lire.S();
			
				for(int i=0; i<bank_.getBank().size(); ++i){
					if(bank_.getBank().elementAt(i).getOwner().equals(name))	//Vérifie si il a bien un nom de propriétaire de compte égale au nom demandé	
						bank_.getBank().elementAt(i).display();					//Appel de la méthode display du compte
				}
			break;
			
			case 3:
				System.out.println("Savefile name : ");
				String filename_ = Lire.S();
				Banque bank = load(filename_);
				bank.display();
			break;
			
			default:
				System.out.println("error: wrong input");
		}
	}
	
	public static void save(){
		System.out.println("Account owner name : ");
		String name = Lire.S();
		System.out.println("Account balance : ");
		double balance = Lire.d();
		System.out.println("Savefile name : ");
		String savefile = Lire.S();
		
		Compte account = new Compte(name, balance);	//Création d'un nouveau compte
		Banque bank = load(savefile);	//On vient charger l'instance Banque et donc son vecteur
		bank.getBank().addElement(account);	//On ajoute le compte au vecteur se trouvant dans l'instance de Banque
		
		try{
			FileOutputStream fos = new FileOutputStream(savefile+".serial");	//Ouverture d'un flux en écriture sur un fichier
			ObjectOutputStream oos = new ObjectOutputStream(fos);	//Création d'un flux objet sur le flux en écriture
			try{
				oos.writeObject(bank);	//Ecriture de l'objet - sérialisation
				oos.flush();	//Nettoyage du flux
			}finally{
				try{ oos.close(); }	//Fermeture du flux objet
				finally{ fos.close(); }	//Fermeture du flux en écriture
			}
		}catch(IOException ioe){ ioe.printStackTrace(); }
	}
	
	
	
	public static Banque load(String name){
		Banque bank = null;
		File serial = new File(name+".serial");
		if(!serial.exists()){	//On vérifie lors du chargement si le fichier exste
			try{
				Vector<Compte> vec = new Vector<Compte>();	//On vient ici créer un nouveau vector vide pour le nouveau fichier .serial
				bank = new Banque(vec);	//L'instance de Banque "pointe" maintenant vers quelque chose de non null
				
				FileOutputStream fos = new FileOutputStream(name+".serial");	//création du flux/fichier inexistants
				ObjectOutputStream oos = new ObjectOutputStream(fos);	//Tout comme dans la fonction "save()" 			
				try{
					oos.writeObject(bank);											
					oos.flush();													
				}finally{
					try{ oos.close(); }												
					finally{ fos.close(); }											
				}
			}catch(IOException _ioe){ _ioe.printStackTrace(); }
		}
		else{
			try{
				FileInputStream fis = new FileInputStream(name+".serial");	//Ouverture d'un flux en lecture sur un fichier		
				ObjectInputStream ois = new ObjectInputStream(fis);	//Création d'un flux objet sur le flux en lecture
				try{
					bank = (Banque) ois.readObject();	//Lecture de l'objet en question, avec un cast vers la classe voulue - désérialisation
				}finally{
					try{ ois.close(); }	//Fermeture des flux
					finally{ fis.close(); }
				}
			}catch(IOException ioe){ ioe.printStackTrace(); }
				catch(ClassNotFoundException cnfe){ cnfe.printStackTrace(); }	//Au cas où la classe "castée" n'existe pas
		}
		
		return bank;
	}
}
