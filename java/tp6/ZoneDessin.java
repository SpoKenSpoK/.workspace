import java.awt.*;
import javax.swing.*;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;
import java.util.GregorianCalendar;
import java.util.Calendar;

public class ZoneDessin extends JPanel{
	private Image img;
	
	public ZoneDessin(){
		try{ img = ImageIO.read(new File("tux.png")); }
		catch(IOException e){ e.printStackTrace(); }
	}
	
	public void paint(Graphics g){
		affiche_fond(g);
		affiche_graduations(g);
		affiche_aiguilles(g);
	}
	
	public void affiche_fond(Graphics g){
		g.setColor(Color.black);
		g.fillOval( (getWidth()-getHeight())/2, 0, getHeight(), getHeight() );
		g.setColor(Color.yellow);
		g.fillOval( (getWidth()-getHeight())/2 +5, 5, getHeight()-10, getHeight()-10 );
		g.drawImage(img, ( getHeight()-(img.getWidth(this)) )/2 + (getWidth()-getHeight())/2, ( getHeight()-(img.getHeight(this)) )/2, null);
	}
	
	public void affiche_graduations(Graphics g){
		int xOrigin = getWidth()/2;
		int yOrigin = getHeight()/2;
		
		int angle = 360/12;
		int radius = 125;
		
		for(int i=1; i<=12 ;++i){
			g.setColor(Color.black);
			g.drawString(Integer.toString(i), xOrigin-5+(int)(Math.cos(Math.toRadians((i-3)*(-angle)))*radius), yOrigin+4-(int)(Math.sin(Math.toRadians((i-3)*(-angle)))*radius));
			//g.setColor(Color.green);
			//drawCenteredString(Integer.toString(i), xOrigin+(int)(Math.cos(Math.toRadians((i-3)*(-angle)))*radius), yOrigin-(int)(Math.sin(Math.toRadians((i-3)*(-angle)))*radius), g);
			
			g.drawLine( xOrigin+(int)(0.75*(Math.cos(Math.toRadians((i-3)*(-angle)))*radius)), 
						yOrigin-(int)(0.75*(Math.sin(Math.toRadians((i-3)*(-angle)))*radius)),
						xOrigin+(int)(0.90*(Math.cos(Math.toRadians((i-3)*(-angle)))*radius)),
						yOrigin-(int)(0.90*(Math.sin(Math.toRadians((i-3)*(-angle)))*radius)) );
		
		}
		
		// http://www.java2s.com/Tutorial/Java/0261__2D-Graphics/Centertext.htm
		
	}

	public void affiche_aiguilles(Graphics g){

		Calendar date = new GregorianCalendar();
		int hours = date.get(Calendar.HOUR_OF_DAY);
		int minutes = date.get(Calendar.MINUTE) - 15;
		int secondes = date.get(Calendar.SECOND) - 14;


		Graphics2D gg = (Graphics2D) g;

		//Hours
		BasicStroke line = new BasicStroke(10.0f);
		gg.setStroke(line);
		gg.setColor(Color.red);
		gg.drawLine(getWidth()/2, 
					getHeight()/2,  
					getWidth()/2+(int)(0.75*(Math.cos(Math.toRadians((hours-3)*(-30)))*125)), 
					getHeight()/2-(int)(0.75*(Math.sin(Math.toRadians((hours-3)*(-30)))*125)) );

		//Minutes
		line = new BasicStroke(5.0f);
		gg.setStroke(line);
		gg.setColor(Color.blue);
		gg.drawLine(getWidth()/2, 
					getHeight()/2,  
					getWidth()/2+(int)(0.75*(Math.cos(Math.toRadians((minutes)*(-360/60)))*125)), 
					getHeight()/2-(int)(0.75*(Math.sin(Math.toRadians((minutes)*(-360/60)))*125)) );

		//Secondes
		line = new BasicStroke(1.0f);
		gg.setStroke(line);
		gg.setColor(Color.black);
		gg.drawLine(getWidth()/2, 
					getHeight()/2,  
					getWidth()/2+(int)(0.75*(Math.cos(Math.toRadians((secondes)*(-360/60)))*125)), 
					getHeight()/2-(int)(0.75*(Math.sin(Math.toRadians((secondes)*(-360/60)))*125)) );

	}
	
/*	public void drawCenteredString(String s, int w, int h, Graphics g) {
    	FontMetrics fm = g.getFontMetrics();
    	int x = (w - fm.stringWidth(s));
    	//int y = (fm.getAscent() + (h - (fm.getAscent() + fm.getDescent())));
    	g.drawString(s, x, h);
  	}
  	*/

}
