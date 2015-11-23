import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class FenetreGraphique extends JFrame implements ActionListener {

    public FenetreGraphique(String s){
        setSize(600,400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        setVisible(true);

        JMenuBar menuBar = new JMenuBar();
        setJMenuBar(menuBar);

        JMenu menu_touch = new JMenu();
        menuBar.add(menu_touch);






    }

}
