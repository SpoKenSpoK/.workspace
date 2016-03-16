/**
 * inverse.c
 **/

#include <cv.h>
#include <highgui.h>

//using namespace std;

int main(int argc, char** argv )
{
    //D�claration / initalisation des images
    IplImage *img=cvLoadImage(argc == 2 ? argv[1] : "lena.jpg",0);
    IplImage *img_nvg=cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage *img_inv=cvCloneImage(img_nvg);

    //On d�clare un scalaire pour l'inversion ;)
    CvScalar scalaire;

    //Cr�ation des fen�tres dans lesquelles nous afficherons les images
    cvNamedWindow("img",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("nvg",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("inv",CV_WINDOW_AUTOSIZE);


    //On v�rifie l'origine de l'image charg�e
    //Si elle n'est pas en haut � gauche, il faut la corriger
    int flip=0;
    if(img->origin!=IPL_ORIGIN_TL)
    {
        flip=CV_CVTIMG_FLIP;
    }

    //Conversion en niveaux de gris
    cvConvertImage(img, img_nvg, flip);

    int x,y;
    float tab[3][3];

    int i, j;
    for(i=0; i<3; ++i)
        for(j=0; j<3; ++j)
            tab[i][j] = 1.0;

    //On va parcourir toute l'image en niveaux de gris
    //x -> dans le sens de la largeur
    //y -> dans le sens de la hauteur
    for(x=1; x<img_nvg->width-1; x++)
    {
        for(y=1; y<img_nvg->height-1; y++)
        {

            int moy;
            //On r�cup�re le pixel (x,y) de l'image en niveaux de gris.
            scalaire=cvGet2D(img_nvg, y, x);

            for(i=0; i<3; i++)
                for(j=0; j<3; ++j){
                    CvScalar filtre;
                    filtre = cvGet2D(img_nvg, y, x);
                    moy += filtre.val[0] * tab[i][j];
                }
            moy /= 9;

            scalaire.val[0] = moy;

            /*if(scalaire.val[0] >= 128)
                scalaire.val[0] = 255;
            else
                scalaire.val[0] = 0;*/

            //On "l'inverse"
            //scalaire.val[0]=255-scalaire.val[0];

            //On le remplace dans l'image "invers�e"
            cvSet2D(img_inv, y, x, scalaire);
        }
    }


    //Affichage des images
    cvShowImage("img", img);
    cvShowImage("nvg", img_nvg);
    cvShowImage("inv", img_inv);

    //On attend que l'utilisateur ait appuy� sur une touche pour continuer
    cvWaitKey(0);

    //Destruction des fen�tres et d�sallocation des images.
    cvDestroyWindow("img");
    cvDestroyWindow("nvg");
    cvDestroyWindow("inv");

    cvReleaseImage(&img);
    cvReleaseImage(&img_nvg);
    cvReleaseImage(&img_inv);

    return 0;
}
