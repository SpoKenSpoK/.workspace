import javax.swing.*;

public class FenetreGraphique extends JFrame{
	public ZoneDessin zoneDessin;

	public FenetreGraphique(String s){
		super(s);
		setSize(300, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);

		setLocationRelativeTo(null);
		setAlwaysOnTop(true);

		zoneDessin = new ZoneDessin();
		setContentPane(zoneDessin);

		setVisible(true);
	}
}
