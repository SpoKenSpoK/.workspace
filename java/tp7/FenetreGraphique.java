import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.*;
import java.io.*;

public class FenetreGraphique extends JFrame implements ActionListener {

    public FenetreGraphique(String s) {

        //  Initialisation de la fenêtre
        super(s);   //< Nom de la fenêtre
        setSize(600,400); //< Taille
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //< Tache effectuée lors de fermeture

        // Ajout d'une bare de menu
        JMenuBar menuBar = new JMenuBar();
        setJMenuBar(menuBar);

        // Onglet du menu principal
        JMenu menu_fichier = new JMenu("Fichier");
        JMenu menu_couleur = new JMenu("Couleur");

        menuBar.add(menu_fichier);
        menuBar.add(menu_couleur);
        //---------------------------------

        // Item de l'onglet fichier
        JMenuItem itemFichier_charger = new JMenuItem("Charger");
        JMenuItem itemFichier_sauvegarder = new JMenuItem("Sauvegarder");
        JMenuItem itemFichier_quitter = new JMenuItem("Quitter");

        menu_fichier.add(itemFichier_charger);
        menu_fichier.add(itemFichier_sauvegarder);
        menu_fichier.add(new JSeparator());
        menu_fichier.add(itemFichier_quitter);

        // Aciton effectuée sur l'item quitter
        itemFichier_quitter.setActionCommand("exit");
        itemFichier_quitter.addActionListener(this);

        // Action effectuée sur l'item charger
        itemFichier_charger.setActionCommand("load");
        itemFichier_charger.addActionListener(this);

        // Action effectuée sur l'item sauvegarder
        itemFichier_sauvegarder.setActionCommand("save");
        itemFichier_sauvegarder.addActionListener(this);
        //----------------------------------

        // Item de l'onglet couleur
        JMenuItem itemCouleur_noir = new JMenuItem("Noir");
        JMenuItem itemCouleur_rouge = new JMenuItem("Rouge");
        JMenuItem itemCouleur_bleu = new JMenuItem("Bleu");
        JMenuItem itemCouleur_perso = new JMenuItem("Personnalisée");

        menu_couleur.add(itemCouleur_noir);
        menu_couleur.add(itemCouleur_rouge);
        menu_couleur.add(itemCouleur_bleu);
        menu_couleur.add(new JSeparator());
        menu_couleur.add(itemCouleur_perso);
        //-----------------------------------

		// Texte libre dans la fenêtre
		JTextPane textPane = new JTextPane();
		getContentPane().add(textPane);

		try{
			StyledDocument doc = (StyledDocument) textPane.getDocument();

			// Créer un objet de style
			Style style = doc.addStyle("StyleName", null);
			StyleConstants.setForeground(style, Color.black);	// Ecriture noire
			// Insertion dans le document
			doc.insertString(doc.getLength(), "Un peu de texte", style);
		}
		catch(BadLocationException e) {}

		//-----------------------------------
        setVisible(true); //<Affichage de la fenêtre
    }

    public void actionPerformed(ActionEvent evenement)
    {
		JTextPane textPane = new JTextPane();

        if(evenement.getActionCommand().equals("exit")){
            if( JOptionPane.showConfirmDialog(  null, //< icon
                                                "Voulez vous vraiment quitter ?", //< Texte
                                                "Quitter",  //< Titre
                                                JOptionPane.YES_NO_OPTION,  //< Option sur les boutons
                                                JOptionPane.ERROR_MESSAGE) == JOptionPane.YES_OPTION ) //< Condition

                System.exit(0); //< Si elle est vérifiée alors on sort du programme
        }

        if(evenement.getActionCommand().equals("load")){
            JFileChooser select = new JFileChooser();
            select.addChoosableFileFilter( new FileNameExtensionFilter("Fichier texte", "txt"));
            int resultat = select.showOpenDialog(null);
            if(resultat == JFileChooser.APPROVE_OPTION){
                try {
				getContentPane().add(textPane);

                System.out.println("Fichier choisi: " + select.getSelectedFile().getAbsolutePath());
				File file = new File(select.getSelectedFile().getAbsolutePath());
				textPane.setPage(file.toURI().toURL());

				JScrollPane scroll = new JScrollPane(textPane);
				getContentPane().add(scroll);

				} catch (IOException e) {}
			}
        }

        if(evenement.getActionCommand().equals("save")){
			JFileChooser select = new JFileChooser();
            select.addChoosableFileFilter( new FileNameExtensionFilter("Fichier texte", "txt"));
            int resultat = select.showOpenDialog(null);
            if(resultat == JFileChooser.APPROVE_OPTION){
				String texte = textPane.getText();
				PrintWriter writer;
				System.out.println(select.getSelectedFile().getAbsolutePath());
				try {
					writer = new PrintWriter( new FileWriter(select.getSelectedFile().getAbsolutePath(), true ));
					writer.println(texte);
					writer.close();
				} catch (IOException e) { e.printStackTrace(); }

			}
		}



    }
}
