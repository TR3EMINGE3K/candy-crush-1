/*
 * Pour compiler ce code, il suffit de taper `make` dans un terminal.
 * Si tout ce passe bien, un exécutable du nom `skel` apparaît dans
 * le répertoire et vous pouvez l'exécuter en tapant `./skel`.
 *
 * Ne faites pas attention au warning qui vous indique que la variable
 * `dt` n'est pas utilisée. C'est normal pour l'instant.
 */

#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/*
 * Ce morceau de code pour permet de tirer un nombre flottant au hasard
 * entre 0.0 et 1.0. Pour cela, il suffit de l'appeler de la manière
 * suivante:
 *
 *     float r = Math::random();
 */
struct Math {
  static float random() {
    static mt19937 engine(time(nullptr));
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
  }
};
/*STRUCTURE*/

struct Bouton {
  float x;
  float y;
  float width;
  float height;
  Color color;
};

/* 
 * Structure du bonbon il a une couleur et un possible bonus
 */
struct Bonbon {
    int couleur;
    int bonus;
};

/* 
 * Structure de l'objectif le score est un booleén et un entier
 */
struct Objectif {
    bool isScore;
    int nbrScore;
    int nbrRouge;
    int nbrBleu;
    int nbrOrange;
    int nbrVert;
    int nbrViolet;
};
/*FIN STRUCTURE*/

/*VARIABLE*/
const int ECRAN_X = 1200;
const int ECRAN_Y = 700;

const int WIDTH = 10;
const int HEIGHT = 10;

const int MUR = 0;
const int TROU = 1;
const int VIDE = 2;
const int ORANGE = 3;
const int ROUGE = 4;
const int BLEU = 5;
const int VIOLET = 6;
const int VERT = 7;

Bonbon tableauBonbon[WIDTH][HEIGHT];

bool jouer = false;
bool isClique = false;

int combo = 1;
int nombreCoup;
int score;
int clique_x = 0;
int clique_y = 0;
int clique_x2 = 0;
int clique_y2 = 0;

/*FIN VARIABLE*/

/*DECLARATION FONCTION*/

void destruction(int direction,int pos_x2,int pos_y2,int couleur){
    int count = 0;
    switch(direction)
    {
        /*Gauche*/
        case 1:
            do{
                tableauBonbon[pos_y2][pos_x2 - count].couleur = VIDE;
                count++;
            }while(pos_x2 >= 0 && tableauBonbon[pos_y2][pos_x2 - count].couleur == couleur);
            break;
        /*Haut*/
        case 2:
            do{
                tableauBonbon[pos_y2 - count][pos_x2].couleur = VIDE;
                count++;
            }while(pos_y2 < HEIGHT && tableauBonbon[pos_y2 - count][pos_x2].couleur == couleur);
            break;
        /*Bas*/
        case 3:
            do{
                tableauBonbon[pos_y2 + count][pos_x2].couleur = VIDE;
                count++;
            }while(pos_y2 >= 0 && tableauBonbon[pos_y2 + count][pos_x2].couleur == couleur);
            break;
        /*Droite*/
        case 4:
            do{
                tableauBonbon[pos_y2][pos_x2 + count].couleur = VIDE;
                count++;
            }while(pos_x2 < WIDTH && tableauBonbon[pos_y2][pos_x2 + count].couleur == couleur);
            break;
        /*Milieu Horizontale*/    
        case 5:
            /*GAUCHE*/
            do{
                tableauBonbon[pos_y2][pos_x2 - count].couleur = VIDE;
                count++;
            }while(pos_x2 >= 0 && tableauBonbon[pos_y2][pos_x2 - count].couleur == couleur);
            count = 0;
            /*DROITE*/
            do{
                tableauBonbon[pos_y2][pos_x2 + count].couleur = VIDE;
                count++;
            }while(pos_x2 < WIDTH && tableauBonbon[pos_y2][pos_x2 + count].couleur == couleur);
            break;
        /*Milieu Verticale*/    
        case 6:
            /*BAS*/
            do{
                tableauBonbon[pos_y2 - count][pos_x2].couleur = VIDE;
                count++;
            }while(pos_y2 < HEIGHT && tableauBonbon[pos_y2 - count][pos_x2].couleur == couleur);
            count = 0;
            /*HAUT*/
            do{
                tableauBonbon[pos_y2 + count][pos_x2].couleur = VIDE;
                count++;
            }while(pos_y2 >= 0 && tableauBonbon[pos_y2 + count][pos_x2].couleur == couleur);
            break;
         
    }
}

void swap(int pos_x1,int pos_y1,int pos_x2,int pos_y2){
    Bonbon swap = tableauBonbon[pos_y1][pos_x1];
    tableauBonbon[pos_y1][pos_x1] = tableauBonbon[pos_y2][pos_x2];
    tableauBonbon[pos_y2][pos_x2] = swap;
}

void match(int pos_x1,int pos_y1,int pos_x2,int pos_y2){
    if(((pos_y2 == pos_y1 - 1)&&(pos_x1==pos_x2)) || ((pos_y2 == pos_y1 +1)&&(pos_x1==pos_x2)) || ((pos_x2 == pos_x1 -1)&&(pos_y1==pos_y2)) || ((pos_x2 == pos_x1 +1)&&(pos_y1==pos_y2))){
        /*Milieu Horizontale*/
        if(pos_x2 - 1 >= 0 && pos_x2 < WIDTH && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 - 1].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 + 1].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(5,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
        /*Milieu Verticale*/
        else if(pos_y2 - 1 >= 0 && pos_y2 < HEIGHT && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 - 1][pos_x2].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 + 1][pos_x2].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(6,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
        /*Gauche*/
        else if(pos_x2 - 1 >= 0 && pos_x2 - 2 >= 0 && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 - 1].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 - 2].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(1,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
        /*Bas*/
        else if(pos_y2 - 1 >= 0 && pos_y2 - 2 >= 0 && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 - 1][pos_x2].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 - 2][pos_x2].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(2,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
        /*Haut*/
        else if(pos_y2 + 1 < HEIGHT && pos_y2 + 2 < HEIGHT && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 + 1][pos_x2].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2 + 2][pos_x2].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(3,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
        /*Droite*/
        else if(pos_x2 + 1 < WIDTH && pos_y2 + 2 < WIDTH && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 + 1].couleur && tableauBonbon[pos_y1][pos_x1].couleur == tableauBonbon[pos_y2][pos_x2 + 2].couleur){
            swap((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
            destruction(4,pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur);
        }
    }
}


/*FIN DECLARATION FONCTION*/

int main() {
  /*
   * Une RenderWindow est une fenêtre qui permet de récupérer des événements
   * d'entrée (comme le clavier et la souris) et d'afficher des entités.
   *
   * La documentation se trouve ici:
   * http://www.sfml-dev.org/documentation/2.1/classsf_1_1RenderWindow.php
   */
  RenderWindow window(VideoMode(ECRAN_X, ECRAN_Y), "Candy Crush");
  
  Bouton boutonJouer;
  boutonJouer.x = ECRAN_X*0.25;
  boutonJouer.y = ECRAN_Y*0.75;
  boutonJouer.width = ECRAN_X/2;
  boutonJouer.height = 80;
  boutonJouer.color = Color::Black;
  // Create a graphical text to display
  Font font;
  if (!font.loadFromFile("arial.ttf"))
    return EXIT_FAILURE;
  Text textBouton("JOUER", font, 40);
  textBouton.setColor(Color::White);
  textBouton.setPosition(325, 465);
  
    // Create a graphical text to display
  Text textTitre("CANDY CRUSH", font, 65);
  textTitre.setColor(Color::Black);
  textTitre.setPosition(200, 150);
  
  /*INITIALISATION MENU*/
  while (!(jouer) && window.isOpen()){
      
      Event event;

      while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x > boutonJouer.x && event.mouseButton.x < boutonJouer.x + boutonJouer.width & event.mouseButton.y > boutonJouer.y && event.mouseButton.y < boutonJouer.y + boutonJouer.height)
        {
            jouer = true;
        }

      }
      
    }
      
      window.clear(Color::White);
      
      RectangleShape boutonJouerShape;
      boutonJouerShape.setSize(Vector2f(boutonJouer.width, boutonJouer.height));
      boutonJouerShape.setPosition(Vector2f(boutonJouer.x, boutonJouer.y));
      boutonJouerShape.setFillColor(boutonJouer.color);
      window.draw(boutonJouerShape);
      
      window.draw(textBouton);
      window.draw(textTitre);
      
      window.display();
  }
  /*FIN INITIALISATION MENU*/
  
  /*
   * Une Clock permet de compter le temps. Vous en aurez besoin pour savoir
   * le temps entre deux frames.
   */
  Clock clock;
  
  /*INITIALISATION DU TABLEAU*/
  
  for(int i = 0; i < 10; i++){
      for(int j = 0; j < 10; j++){
          Bonbon bonbon;
          bonbon.couleur = ORANGE + (int)(Math::random() * ((VERT - ORANGE) + 1)); 
          bonbon.bonus = 1;
          tableauBonbon[i][j] = bonbon; 
    }
  }

  
  /*FIN INITIALISATION TABLEAU*/
  
  /*
   * La boucle de jeu principale. La condition de fin est la fermeture de la
   * fenêtre qu'on provoque à l'aide d'un appel `window.close()`.
   */
  while (window.isOpen()) {

    Event event;

    /*
     * Cette boucle permet de traiter tous les événements en attente.
     */
    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == Event::KeyPressed) {
          
      }

      if (event.type == Event::KeyReleased) {

      }

      if (event.type == sf::Event::MouseButtonPressed) {
          if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.x >= 355 && event.mouseButton.x < 355 + 530 & event.mouseButton.y > 100 && event.mouseButton.y < 100 + 530){
              if(isClique){
                  clique_x2 = (int) event.mouseButton.x;
                  clique_y2 = (int) event.mouseButton.y;
                  match((clique_x - 355)/53,(clique_y - 100)/53,(clique_x2 - 355)/53,(clique_y2 - 100)/53);
                  isClique = false;
                }
            else{
                isClique = true;
                clique_x = (int) event.mouseButton.x;
                clique_y = (int) event.mouseButton.y;
                //std::cout << event.mouseButton.x << ',' << event.mouseButton.y << '\n';
            }
          }
          else{
             isClique = false;
          }
      }
      
    }

    window.clear(Color::White);
    
    /*DECLARATION BACKGROUND TABLEAU GRAPHIQUE*/
    RectangleShape backgroundTableau;
    backgroundTableau.setSize(Vector2f(545, 545));
    backgroundTableau.setPosition(Vector2f(345,90));
    backgroundTableau.setFillColor(Color::Black);
    window.draw(backgroundTableau);
    /*FIN DECLARATION BACKGROUND TABLEAU GRAPHIQUE*/
    
    /*Creation des bonbons graphiquement*/
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            RectangleShape bonbonShape;
            bonbonShape.setSize(Vector2f(48, 48));
            bonbonShape.setPosition(Vector2f(j*53 + 355,i*53 + 100));
            switch (tableauBonbon[i][j].couleur)
            {
                case ORANGE:
                    bonbonShape.setFillColor(Color(237,127,16));
                    break;
                case ROUGE:
                    bonbonShape.setFillColor(Color::Red);
                    break;
                case BLEU:
                    bonbonShape.setFillColor(Color::Blue);
                    break;
                case VIOLET:
                    bonbonShape.setFillColor(Color(108,2,119));
                    break;
                case VERT:
                    bonbonShape.setFillColor(Color(84,249,141));
                    break;                    
            }
            
            window.draw(bonbonShape);
        }
    }
    /*Fin Creation des bonbons graphiquement*/
    
    /*DECLARATION ILLUMINATION BONBON*/
    RectangleShape illuminationBonbon;
    if(isClique){
        illuminationBonbon.setSize(Vector2f(48, 48));
        illuminationBonbon.setPosition(Vector2f(((clique_x - 355)/53)*53 + 355,((clique_y - 100)/53)*53 + 100));
        illuminationBonbon.setFillColor(Color(255,255,255,20));
        illuminationBonbon.setOutlineThickness(2);
        illuminationBonbon.setOutlineColor(Color::White);
    }
    else {
        illuminationBonbon.setSize(Vector2f(50, 50));
        illuminationBonbon.setPosition(Vector2f(0,0));
        illuminationBonbon.setFillColor(Color::White);
    }

    window.draw(illuminationBonbon);
    /*FIN DECLARATION ILLUMINATION BONBON*/
    
    window.display();

  }

  return 0;
}
