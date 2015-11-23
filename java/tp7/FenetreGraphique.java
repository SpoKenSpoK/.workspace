import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.filechooser.FileNameExtensionFilter;

public class FenetreGraphique extends JFrame implements ActionListener {

    public FenetreGraphique(String s){

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

        setVisible(true); //<Affichage de la fenêtre
    }

    public void actionPerformed(ActionEvent evenement)
    {
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
            if(resultat == JFileChooser.APPROVE_OPTION)
                System.out.println("Fichier choisi: " + select.getSelectedFile().getAbsolutePath());
        }
    }
}
