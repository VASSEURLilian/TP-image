#include <stdlib.h>
#include <math.h>
#include "IHM.h"

// position de l'histogramme dans l'interface
#define HISTO_L 525
#define HISTO_B 580

void set_pixel(unsigned char *im, unsigned short int c, unsigned short int l,int coul);
void croix_blanche(unsigned char *im);
void croix_noir(unsigned char *im);
unsigned char get_pixel(unsigned char *im, unsigned short int c, unsigned short int l);
void affichezoom(unsigned char *im, unsigned short int top,  unsigned short int, unsigned short int x, unsigned short int y, unsigned short int f);
void affichehisto(unsigned int *histo);
void calculerhisto(unsigned char *im, unsigned int *histo);
void binariser(unsigned char *im, int seuil);

int main ( int argc, char** argv )
{

    // mes variables
    char c;
    unsigned char coul;
    unsigned int histo[256];
    int seuil=128;
    int min, max, i, j;
    int affhistolena, affhistoarchive=0;



    // tableau contenant l'image
    unsigned char mimage[HAUTEUR_IMAGE * LARGEUR_IMAGE];

    //   ma position
    int x;
    int y;

    // initialisation de la fenêtre
    IHM_init();

    // initialisation du tableau à partir du fichier image
    IHM_charge_BMP(mimage, "img/lena_gray.bmp");
    // copie du tableau dans l'affichage
    IHM_set_image(mimage);

    // boucle principale de l'IHM
    while (!IHM_quitter())
    {
        // on commence par effacer
        IHM_efface();


        x = IHM_souris_x();
        y = IHM_souris_y();
        c = IHM_getChar();

        // les traitements en fonction des caractères tapés
        switch(c)
        {
        case 'b':
            // faire la binarisation
            binariser(mimage, seuil);
            IHM_set_image(mimage);
            break;
        case 'l':
            IHM_charge_BMP(mimage, "img/lena_gray.bmp");
            IHM_set_image(mimage);
            affhistoarchive=0;
            break;
        case 'a':
            IHM_charge_BMP(mimage, "img/archives.bmp");
            IHM_set_image(mimage);
            affhistolena=0;
            break;
        case 'h':
            // re-calculer l'histogramme
            calculerhisto(mimage,histo);
            affhistolena=1;
            affhistoarchive=1;
            break;
        case 'c':
            //on affiche la croix noir
            croix(mimage);///partie 1
            break;
        }

        // ajouter l'image :
        IHM_affiche_image(10,10);

        // position du zoom
        IHM_rectangle(550,10,8*11,8*11,IHM_couleur(255,255,255));
        affichezoom(mimage,550,10,x,y,8);


        // position de l'histogramme
        i=0;
        while(histo[i]==0)
        {
            i++;
        }
        min=i;
        j=255;
        while(histo[j]==0)
        {
            j=j-1;
        }
        max=255-j;
        IHM_rectangle(HISTO_L,(HISTO_B - 110),110,256+10,IHM_couleur(255,255,255));
        if(x>=530+min && x<=785-max && y>=480 && y<=580)
        {
            IHM_rectangle(x,475,105,1,IHM_couleur(0,255,0));
            if(IHM_souris_boutonG() && (affhistolena || affhistoarchive))
            {
                seuil=x-530;
            }
        }
        affichehisto(histo);

        IHM_set_image(mimage);

        // on met à jour l'affichage
        IHM_affiche();
    }
    // fin de la boucle principale

    return 0;
}

void set_pixel(unsigned char *im, unsigned short int c, unsigned short int l,int coul)
{
    if (c>=0 && c<LARGEUR_IMAGE && l>=0 && l<HAUTEUR_IMAGE)
    {
        im[c+l*LARGEUR_IMAGE]=coul;
    }

}

void croix(unsigned char *im)
{
    int i;

    for (i=0;i<=511;i++)
    {
        set_pixel(im, i, 256, IHM_couleur(255,255,255));
        set_pixel(im, 256, i, IHM_couleur(255,255,255));
        set_pixel(im, i, i, IHM_couleur(0,0,0));
        set_pixel(im, 511-i, i, IHM_couleur(0,0,0));
    }
}

unsigned char get_pixel(unsigned char *im, unsigned short int c, unsigned short int l)
{
    int coul;

    if (c<LARGEUR_IMAGE && l<LARGEUR_IMAGE )
    {
        coul=im[c+l*LARGEUR_IMAGE];
        return coul;
    }
    else
    {
        return 0;
    }
}

void affichezoom(unsigned char *im, unsigned short int top,  unsigned short int left, unsigned short int x, unsigned short int y, unsigned short int f)
{
    unsigned char i;
    unsigned char k;
    int coul;

    x=x-10;

    y=y-10;
    if (x<LARGEUR_IMAGE && y<LARGEUR_IMAGE)
    {
        x=x-5;
        y=y-5;
        for (i=0; i<=10;i++)
        {
            for(k=0; k<=10; k++)
            {
                coul=get_pixel(im,x+k,y+i);
                IHM_rectangle(top+f*k, left+f*i, f, f, IHM_couleur(coul,coul,coul));
            }
        }
    }
}

void affichehisto(unsigned int *histo)
{
    unsigned int i,valmax;

    for (i=0;i<=255;i++)
    {
        if(histo[i]>valmax)
        {
            valmax=histo[i];
        }
    }

    for(i=0;i<=255;i++)
    {
        IHM_rectangle(HISTO_L+i+5,HISTO_B-5,-((histo[i]*100)/valmax),1,0);
    }
    IHM_rectangle(HISTO_L+4,HISTO_B-1,1,257,IHM_couleur(255,0,0));
    IHM_rectangle(HISTO_L+4,HISTO_B-5,1,257,IHM_couleur(255,0,0));
    IHM_rectangle(HISTO_L+4,HISTO_B-1,-4,1,IHM_couleur(255,0,0));
    IHM_rectangle(HISTO_L+261,HISTO_B-1,-4,1,IHM_couleur(255,0,0));
    for(i=0;i<=255;i++)
    {
        IHM_rectangle(HISTO_L+i+5,HISTO_B-1,-3,1,IHM_couleur(i,i,i));
    }
}

void calculerhisto(unsigned char *im, unsigned int *histo)
{
    int i,j,k;
    for(k=0;k<=255;k++)
    {
        histo[k]=0;
    }
    for(i=0;i<LARGEUR_IMAGE;i++)
    {
        for(j=0;j<HAUTEUR_IMAGE;j++)
        {
            for(k=0;k<=255;k++)
            {
                if(k==im[i*LARGEUR_IMAGE+j])
                {
                    histo[k]=histo[k]+1;
                }
            }
        }
    }
}

void binariser(unsigned char *im, int seuil)
{
    int i,j;
    //seuil=IHM_couleur(seuil,seuil,seuil);


    for(i=0;i<LARGEUR_IMAGE;i++)
    {
        for(j=0;j<HAUTEUR_IMAGE;j++)
        {
            if(im[i*LARGEUR_IMAGE+j]>seuil)
            {
                im[i*LARGEUR_IMAGE+j]=IHM_couleur(255,255,255);
            }
            else
            {

                set_pixel(im,j,i,0);
            }
        }
    }
}
