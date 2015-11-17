import java.awt.*;
import javax.swing.*;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;

public class ZoneDessin extends JPanel{
	private Image img;
	
	public ZoneDessin(){
		try{ img = ImageIO.read(new File("tux.png")); }
		catch(IOException e){ e.printStackTrace(); }
	}
	
	public void paint(Graphics g){
		affiche_fond(g);
		affiche_graduations(g);
		//affiche_aiguilles(g);
	}
	
	public void affiche_fond(Graphics g){
		g.setColor(Color.black);
		g.fillOval( (getWidth()-getHeight())/2, 0, getHeight(), getHeight() );
		g.setColor(Color.yellow);
		g.fillOval( (getWidth()-getHeight())/2 +5, 5, getHeight()-10, getHeight()-10 );
		g.drawImage(img, ( getHeight()-(img.getWidth(this)) )/2 + (getWidth()-getHeight())/2, ( getHeight()-(img.getHeight(this)) )/2, null);
	}
	
	public void affiche_graduations(Graphics g){
		int xOrigin = getSize().width/2;
		int yOrigin = getSize().height/2;
		
		System.out.println(xOrigin);
		System.out.println(yOrigin);
		
		int angle = 360/12;
		int radius = 120;
		
		for(int i=1; i<=12 ;++i){
			g.setColor(Color.black);
			g.drawString(Integer.toString(i), xOrigin+(int)(Math.cos(Math.toRadians((i-3)*(-angle)))*radius), yOrigin-(int)(Math.sin(Math.toRadians((i-3)*(-angle)))*radius));
		}
		
		// http://www.java2s.com/Tutorial/Java/0261__2D-Graphics/Centertext.htm
		
	}

}
