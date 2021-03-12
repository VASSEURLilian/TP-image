#include "IHM.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdio.h>

// variables globales nécessaire

// la fenetre
// Notre fenêtre et le clavier
SDL_Window* sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Event evenements;


// type pour les couleurs
typedef  struct
{
    unsigned char R;
    unsigned char V;
    unsigned char B;
    unsigned char A;
} TYPE_IHM_RVB;

typedef  union
{
    TYPE_IHM_RVB rvb;
    unsigned long int val;
} TYPE_IHM_COUL;

// pour ne pas recharger le logo ...
SDL_Texture * logo;
// gestion des evenements
char _quitter_;
char _lastchar; // clavier
int _mouse_x;
int _mouse_y;
unsigned char _mouse_b;

// pour l'image
// l'image brute de pixels
Uint32* _image;
// la texture qui sera affichée
SDL_Texture * _imageTxt;

void __IHM_traiter_event();

void IHM_init()
{
    _quitter_ = 0;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
        //return -1;
    }
    if(SDL_CreateWindowAndRenderer( LARGEUR_FEN, HAUTEUR_FEN, SDL_WINDOW_SHOWN, &sdlWindow, &sdlRenderer)){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window & Renderer create error", SDL_GetError(),sdlWindow);
            exit(-1);
    }
    SDL_Surface * image = SDL_LoadBMP("img/geii.bmp");
    if (image == NULL)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Image init error", SDL_GetError(),sdlWindow);
            exit(-1);
        }
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    logo = SDL_CreateTextureFromSurface(sdlRenderer,image);
    if (logo == NULL)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Texture init error", SDL_GetError(), sdlWindow);
            exit(-1);
        }
    // c'est bon on peu la libérer
    SDL_FreeSurface(image);

    _image = malloc(HAUTEUR_IMAGE * LARGEUR_IMAGE * sizeof(Uint32)); // Oups on ne libère jamais...

    _imageTxt = SDL_CreateTexture(sdlRenderer,
                               SDL_PIXELFORMAT_ABGR8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               LARGEUR_IMAGE, HAUTEUR_IMAGE);

    SDL_Rect dstrect = { 0, 0, 88, 64 };
    SDL_RenderCopy(sdlRenderer, logo, NULL, &dstrect);
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
}

char IHM_quitter()
{
    SDL_Delay(2); // pour réduire la CPU utilisée
    __IHM_traiter_event();
    return _quitter_;
}

void IHM_efface()
{
    // centre the bitmap on screen
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    // draw logo
    SDL_Rect dstrect = { 0, 0, 88, 64 };
    SDL_RenderCopy(sdlRenderer, logo, NULL, &dstrect);
}

void IHM_affiche()
{
// on affiche tout ce qui a été dessiné
    SDL_RenderPresent(sdlRenderer);
}

unsigned long int IHM_couleur(unsigned char R, unsigned char V, unsigned char B)
{
    TYPE_IHM_COUL coul;
    coul.rvb.R = R;
    coul.rvb.V = V;
    coul.rvb.B = B;
    coul.rvb.A = 255;
    return coul.val;
}

void IHM_setPixel(unsigned int x, unsigned int y, unsigned long int c)
{
    TYPE_IHM_COUL coul;
    coul.val = c;
    SDL_SetRenderDrawColor(sdlRenderer, coul.rvb.R, coul.rvb.V, coul.rvb.B, coul.rvb.A);
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = 1;
    rectangle.h = 1;
    SDL_RenderFillRect(sdlRenderer, &rectangle);
}

void IHM_rectangle(unsigned int x, unsigned int y, unsigned int H, unsigned int L,unsigned long int c)
{
    TYPE_IHM_COUL coul;
    coul.val = c;
    SDL_SetRenderDrawColor(sdlRenderer, coul.rvb.R, coul.rvb.V, coul.rvb.B, coul.rvb.A);
    SDL_Rect rect;
    rect.x = x ;
    rect.y = y ;
    if(H> 0){
        rect.h = H;
    }else{
        rect.h = 0;
    }
    if(L>0){
        rect.w = L;
    }else{
        rect.w = 0;
    }
    SDL_RenderFillRect(sdlRenderer, &rect);
}

char IHM_getChar()
{
    char c= _lastchar;
    _lastchar = 0;
    return c;
}

int IHM_souris_x()
{
    return _mouse_x;
}

int IHM_souris_y()
{
    return _mouse_y;
}

char IHM_souris_boutonG()
{
    char ok = _mouse_b&SDL_BUTTON(1);
    _mouse_b = 0;
    return ok;
}


void __IHM_traiter_event()
{

    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
        // exit if the window is closed
        case SDL_QUIT:
            _quitter_ = 1;
            break;

        // check for keypresses
        case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
            if (event.key.keysym.sym == SDLK_ESCAPE)
                _quitter_= 1;
            else if(event.key.keysym.sym > SDLK_SPACE && event.key.keysym.sym < SDLK_z)
            {
                _lastchar = event.key.keysym.sym;
            }
            break;
        }

        case SDL_MOUSEMOTION:
        {
            _mouse_x = event.motion.x;
            _mouse_y = event.motion.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            _mouse_b = event.motion.state;
            break;
        }
        } // end switch
    } // end of message processing
}

/** METHODES POUR LES IMAGES **/

// charger une image bmp dans un tableau
void IHM_charge_BMP(unsigned char * tab, char * nom_fichier )
{
    unsigned int i,j;
    SDL_Surface * im = SDL_LoadBMP(nom_fichier);

    if(im != NULL)
    {
        for(j = 0; j < HAUTEUR_IMAGE; j++)
        {
            for(i = 0; i < LARGEUR_IMAGE; i++)
            {
                Uint8 *p = (Uint8 *)im->pixels + j * im->pitch + i * im->format->BytesPerPixel ;
                *(tab++) = (unsigned char)*p;

            }
        }
        SDL_FreeSurface(im);
    }
    else
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Probleme de chargement de l'image", SDL_GetError(), sdlWindow);
        printf("Probleme avec le fichier %s \n",nom_fichier);
    }

}
// charge l'image du tableau dans la structure
void IHM_set_image(unsigned char * tab)
{
    unsigned int i,j;
    // d'abord une conversion 8bit => 32bits
    Uint32 *p = _image;
    for(j = 0; j < HAUTEUR_IMAGE; j++)
    {
        for(i = 0; i < LARGEUR_IMAGE; i++)
        {
            *p++ = IHM_couleur(*tab, *tab,*tab); // conversion 8bits -> 32bits
            tab++;
            //*p++ = *tab++;// on copy juste les contenus, la palette fait le reste
        }
    }
    // puis la mise à jour de la texture
    SDL_UpdateTexture(_imageTxt, NULL, _image, LARGEUR_IMAGE * sizeof (Uint32));

}

// affiche l'image
void IHM_affiche_image(unsigned int x, unsigned int y)
{
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.h = HAUTEUR_IMAGE;
    dstrect.w = LARGEUR_IMAGE;
    SDL_RenderCopy(sdlRenderer,_imageTxt, NULL, &dstrect);
}
