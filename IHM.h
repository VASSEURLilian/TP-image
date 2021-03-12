#ifndef IHM_H
#define IHM_H

#define LARGEUR_FEN 800
#define HAUTEUR_FEN 600
#define LARGEUR_IMAGE 512
#define HAUTEUR_IMAGE 512

// initialise la fenêtre
void IHM_init();
// efface la fenetre
void IHM_efface();
// affiche la fenetre
void IHM_affiche();
// quitte l'application
char IHM_quitter();

// convertit une couleur R V B (entre 0 et 255) en entier non signé
unsigned long int IHM_couleur(unsigned char R, unsigned char V, unsigned char B);
// Dessine un pixel à la position (x,y) de la couleur donnée
void IHM_setPixel(unsigned int x, unsigned int y, unsigned long int couleur);
// Dessine un rectangle de HxL pixels dont le coin en haut à gauche est en (x,y) de la couleur donnée
void IHM_rectangle(unsigned int x, unsigned int y, unsigned int H, unsigned int L,unsigned long int couleur);

// renvoie le dernier caractère tapé au clavier
char IHM_getChar();
// renvoie la position x (y) courante de la souris
 int IHM_souris_x();
 int IHM_souris_y();
// renvoie vrai si le bouton gauche de la souris a été cliqué
char IHM_souris_boutonG();


// charger une image bmp depuis le fichier spécifié dans un tableau. Attention : seulement des images BMP 512x512 pixels en niveaux de gris 8bits
void IHM_charge_BMP(unsigned char * tab, char * nom_fichier );
// mise à jour de l'image à l'écran à partir du tableau donné en paramètre
void IHM_set_image(unsigned char * tab);
// affiche l'image chargée à la position top,left dans l'écran
void IHM_affiche_image(unsigned int left, unsigned int top);

#endif // IHM_H
