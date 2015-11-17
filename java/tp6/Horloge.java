import java.util.Timer;
import java.util.TimerTask;

public class Horloge {
	public static void main(String [] arg){
		FenetreGraphique fenetre;
		fenetre = new FenetreGraphique("The Bruh Clock");
		Horloge(fenetre);

	}

	public static void Horloge(final FenetreGraphique f){
		Timer timer;
		TimerTask task = new TimerTask(){ 
			public void run(){ f.repaint(); }
		};

		timer = new Timer();
		timer.scheduleAtFixedRate(task, 0, 1000);
	}
	

}