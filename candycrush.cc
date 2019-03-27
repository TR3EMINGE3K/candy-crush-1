/*
 * Pour compiler ce code, il suffit de taper `make` dans un terminal.
 * Si tout ce passe bien, un exécutable du nom `skel` apparaît dans
 * le répertoire et vous pouvez l'exécuter en tapant `./skel`.
 *
 * Ne faites pas attention au warning qui vous indique que la variable
 * `dt` n'est pas utilisée. C'est normal pour l'instant.
 */

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
const int WIDTH = 10;
const int HEIGHT = 10;
const int ECRAN_X = 800;
const int ECRAN_Y = 600;

bool jouer = false;

int combo = 1;
int nombreCoup;
int score;

/*FIN VARIABLE*/

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
  RectangleShape tableauBonbon[WIDTH][HEIGHT];
  Bonbon bonbon;
  bonbon.couleur = 4;
  bonbon.bonus = 1;
  
  RectangleShape bonbonShape;
  bonbonShape.setSize(Vector2f(32, 32));
  bonbonShape.setPosition(Vector2f(0, 0));
  bonbonShape.setFillColor(Color::Red);
  
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++){
      bonbonShape.setPosition(Vector2f(j*33,i*33));
      tableauBonbon[i][j] = bonbonShape;
    }
  }
  
  /*FIN INITIALISATION TABLEAU*/
  
  /*
   * La boucle de jeu principale. La condition de fin est la fermeture de la
   * fenêtre qu'on provoque à l'aide d'un appel `window.close()`.
   */
  while (window.isOpen()) {
    /*
     * Un Event est un événement d'entrée. Il contient toutes les informations
     * nécessaires pour traiter tous les événements.
     *
     * Vous pouvez lire ce tutoriel pour comprendre comment récupérer les
     * informations relatives à chaque événement:
     * http://www.sfml-dev.org/tutorials/2.1/window-events.php
     */
    Event event;

    /*
     * Cette boucle permet de traiter tous les événements en attente.
     */
    while (window.pollEvent(event)) {
      /*
       * L'événement Event::Closed est reçu quand on clique sur la croix
       * dans la barre de la fenêtre. À ce moment là, il faut fermer la
       * fenêtre explicitement.
       */
      if (event.type == Event::Closed) {
        window.close();
      }

      /*
       * Les événements dont vous aurez besoin sont Event::KeyPressed,
       * Event::KeyReleased et Event::MouseButtonPressed.
       */

      if (event.type == Event::KeyPressed) {
          
      }

      if (event.type == Event::KeyReleased) {

      }

      if (event.type == sf::Event::MouseButtonPressed) {

      }
      
    }

    /*
     * Mise à jour de l'état du jeu.
     * On calcule le temps (en secondes) depuis la dernière frame qu'on place
     * dans la variable `dt`. Ensuite, il faut compléter suivant ce qui est
     * demandé.
     */
    window.clear(Color::White);

  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++){
        window.draw(tableauBonbon[i][j]);
    }
  }
    
    /*
     * Affichage de l'état du jeu.
     * À chaque tour de boucle, on efface tout grâce à `clear` (qui prend
     * en paramètre la couleur de fond), puis on dessine tous les éléments,
     * puis on affiche la nouvelle image grâce à `display`.
     */
    
    window.display();

  }

  return 0;
}
