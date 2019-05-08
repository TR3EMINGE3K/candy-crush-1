#include <iostream>
#include <list>
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

struct Tuple
{
  int x;
  int y;
};

/*FIN STRUCTURE*/

/*VARIABLE*/
const int ECRAN_X = 1200;
const int ECRAN_Y = 700;

const int WIDTH = 10;
const int HEIGHT = 10;

//Centrage des bonbons graphiquement
const int OFFSET_X = 355, OFFSET_Y = 100;

const int TAILLE_PLATEAU = 530;
const int TAILLE_BONBON = 48;
const int TAILLE_BONBON_OFFSET = TAILLE_BONBON + 5;

//Valeur possible d'un bonbon
const int MUR = 0;
const int TROU = 1;
const int VIDE = 2;
const int ORANGE = 3;
const int ROUGE = 4;
const int BLEU = 5;
const int VIOLET = 6;
const int VERT = 7;

Bonbon tableauBonbon[WIDTH][HEIGHT];
Tuple tableauCoord [WIDTH][HEIGHT];

bool jouer = false;
bool isClique = false;
bool matchi = false;

int combo = 1;
int nombreCoup = 0;
int scoreJoueur = 0;

//Position du bonbon 1 en taille réel
int clique_x = 0, clique_y = 0;
//Position du bonbon 2 en taille réel
int clique_x2 = 0, clique_y2 = 0;

/*FIN VARIABLE*/

/*DECLARATION FONCTION*/

//Fonctions Mineurs

void affichageTabCLI(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            char charBonbon = ' ';
            
            if(tableauBonbon[i][j].couleur == 3){
                charBonbon = 'O';
            }
            else if(tableauBonbon[i][j].couleur == 4){
                charBonbon = 'R';
            }
            else if(tableauBonbon[i][j].couleur == 5){
                charBonbon = 'B';
            }
            else if(tableauBonbon[i][j].couleur == 6){
                charBonbon = 'P';
            }  
            else if(tableauBonbon[i][j].couleur == 7){
                charBonbon = 'V';
            }
            else{
                charBonbon = '.';
            }
            std::cout << charBonbon << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool verificationClique(Event event, int x, int y, float width, float height) {
  return event.mouseButton.x > x && event.mouseButton.x < x + width & event.mouseButton.y > y && event.mouseButton.y < y + height;
}

RectangleShape illuminationBonbon() {
  RectangleShape illuminationBonbon;
  if(isClique) {
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
  return illuminationBonbon;
}

int posReel2posTableau(int cord, char pos) {
  if(pos == 'x')
    return (cord - OFFSET_X)/TAILLE_BONBON_OFFSET;
  else
    return (cord - OFFSET_Y)/TAILLE_BONBON_OFFSET;
}

bool isNearToBonbon(int pos_x1, int pos_y1, int pos_x2, int pos_y2) {
  //Gauche - Droite - Haut - Bas
  return ((pos_x1==pos_x2+1)&&(pos_y1==pos_y2))||((pos_x1==pos_x2-1)&&(pos_y1==pos_y2))||((pos_x1==pos_x2)&&(pos_y1==pos_y2+1))||((pos_x1==pos_x2)&&(pos_y1==pos_y2-1));
}

bool isMatchPossible(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int couleur, char direction) {
  bool result = false;
  switch (direction)
  {
    /*GAUCHE*/
    case 'g':
      result = (pos_x2 - 2 >= 0) && (pos_x2 - 1 != pos_x1) && (tableauBonbon[pos_y2][pos_x2 - 1].couleur == couleur) && (tableauBonbon[pos_y2][pos_x2 - 2].couleur == couleur);
    break;
    /*DROITE*/
    case 'd':
      result = (pos_x2 + 2 < WIDTH) && (pos_x2 + 1 != pos_x1) && (tableauBonbon[pos_y2][pos_x2 + 1].couleur == couleur) && (tableauBonbon[pos_y2][pos_x2 + 2].couleur == couleur);
    break;
    /*HAUT*/
    case 'h':
      result = (pos_y2 - 2 >= 0) && (pos_y2 - 1 != pos_y1) && (tableauBonbon[pos_y2 - 1][pos_x2].couleur == couleur) && (tableauBonbon[pos_y2 - 2][pos_x2].couleur == couleur);
    break;
    /*BAS*/
    case 'b':
      result = (pos_y2 + 2 < HEIGHT) && (pos_y2 + 1 != pos_y1) && (tableauBonbon[pos_y2 + 1][pos_x2].couleur == couleur) && (tableauBonbon[pos_y2 + 2][pos_x2].couleur == couleur);
    break;
    /*ORIZONTALE*/
    case 'o':
      result = (pos_x2 - 1 >= 0) && (pos_x2 + 1 < WIDTH) && (pos_x2 - 1 != pos_x1) && (pos_x2 + 1 != pos_x1) && (tableauBonbon[pos_y2][pos_x2 - 1].couleur == couleur) && (tableauBonbon[pos_y2][pos_x2 + 1].couleur == couleur);
    break;
    /*VERTICALE*/
    case 'v':
      result = (pos_y2 - 1 >= 0) && (pos_y2 + 1 < HEIGHT) && (pos_y2 - 1 != pos_y1) && (pos_y2 + 1 != pos_y1) && (tableauBonbon[pos_y2 - 1][pos_x2].couleur == couleur) && (tableauBonbon[pos_y2 + 1][pos_x2].couleur == couleur);
    break;
  }
  //std::cout << "isMatchPossible : " << result << '\n';
  return result;
}

// Echange deux bonbons
void swap(int pos_x1, int pos_y1, int pos_x2, int pos_y2) {
  Bonbon swap = tableauBonbon[pos_y1][pos_x1];
  tableauBonbon[pos_y1][pos_x1] = tableauBonbon[pos_y2][pos_x2];
  tableauBonbon[pos_y2][pos_x2] = swap;
}

// Remplace les bonbons ayant la valeur vide dans le tableau et compléte avec de nouveaux bonbon
void remplacementVide() {
  for(int i = 0;i < HEIGHT;i++) {
    for(int j = 0;j < WIDTH;j++) {
      if (tableauBonbon[i][j].couleur == VIDE) {
        scoreJoueur += 20;
        int count_1 = 1, count_2 = 0;
        do {
          if(i - count_1 >= 0) {
            swap(j,i - count_2,j,i - count_1);
          }
          else {
            Bonbon bonbon;
            bonbon.couleur = ORANGE + (int)(Math::random() * ((VERT - ORANGE) + 1)); 
            bonbon.bonus = 1;
            tableauBonbon[0][j] = bonbon;             
          }
          count_1++;
          count_2++;
        } while (i - count_1 >= -1);
      }
    }
  }
}

//Fonctions Majeures
void destruction(int pos_x2, int pos_y2, int couleur, char direction) {
  int count = 0;
  bool finish = false;
  switch(direction) {

    /*GAUCHE*/
    case 'g':
      do{
        if(tableauBonbon[pos_y2][pos_x2 - count].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2][pos_x2 - count].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_x2 >= 0 && !finish);

    /*DROITE*/
    case 'd':
      do{
        if(tableauBonbon[pos_y2][pos_x2 + count].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2][pos_x2 + count].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_x2 < WIDTH && !finish);
    break;

    /*HAUT*/
    case 'h':
      do{
        if(tableauBonbon[pos_y2 - count][pos_x2].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2 - count][pos_x2].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_y2 >= 0 && !finish);
    break;

    /*BAS*/
    case 'b':
      do{
        if(tableauBonbon[pos_y2 + count][pos_x2].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2 + count][pos_x2].couleur = VIDE;
        }
        else 
          finish = true;
        count++;
      }while(pos_y2 < HEIGHT && !finish);
    break;

    /*ORIZONTALE*/    
    case 'o':
      /*GAUCHE*/
      do{
        if(tableauBonbon[pos_y2][pos_x2 - count].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2][pos_x2 - count].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_x2 >= 0 && !finish);
      finish = false;
      count = 1;
      /*DROITE*/
      do{
        if(tableauBonbon[pos_y2][pos_x2 + count].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2][pos_x2 + count].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_x2 < WIDTH && !finish);
    break;

    /*VERTICALE*/    
    case 'v':
      /*HAUT*/
      do{
        if(tableauBonbon[pos_y2 - count][pos_x2].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2 - count][pos_x2].couleur = VIDE;
        }
        else
          finish = true;
        count++;
      }while(pos_y2 >= 0 && !finish);
      finish = false;
      count = 1;
      /*BAS*/
      do{
        if(tableauBonbon[pos_y2 + count][pos_x2].couleur == couleur){
          scoreJoueur += 20;
          tableauBonbon[pos_y2 + count][pos_x2].couleur = VIDE;
        }
        else 
          finish = true;
        count++;
      }while(pos_y2 < HEIGHT && !finish);
    break;
        
  }
  remplacementVide();
}

// Test si il existe encore des bonbons aligner suite à la descente de nouveau bonbon
bool verificationLopops(){
  bool isLigne = false;
  int i = 0, j = 0;
  do {
    do {
      int count = 1, iterate = 1;
      int couleur = tableauBonbon[i][j].couleur;
      while(j + iterate < WIDTH && tableauBonbon[i][j + iterate - 1].couleur == couleur && !isLigne){
        if(tableauBonbon[i][j + iterate].couleur == couleur) {
          count++;
        }
        if(count >= 3)
          isLigne = true;
        iterate++;
      }
      j++;
    }while(j < WIDTH && !isLigne);
    j = 0;
    i++;
  }while(i < HEIGHT && !isLigne);
  i = 0, j = 0;
  while(i < HEIGHT && !isLigne) {
    while(j < WIDTH && !isLigne) {
      int count = 1, iterate = 1;
      int couleur = tableauBonbon[i][j].couleur;
      while(i + iterate < HEIGHT && tableauBonbon[i + iterate - 1][j].couleur == couleur && !isLigne) {
        if(tableauBonbon[i + iterate][j].couleur == couleur) {
          count++;
        }
        if(count >= 3)
          isLigne = true;
        iterate++;
      }
      j++;
    }
    j = 0;
    i++;
  }
  return isLigne;
}

// Vérifie si il existe des alignements si c'est le cas alors il remplace les alignements par des cases vides
void verificationTableau() {
  
  std::list <Tuple> coordinate;
  
  for(int i = 0; i < HEIGHT; i++) {
    for(int j = 0; j < WIDTH; j++) {
      int count = 1;
      Tuple cord;
      cord.x = j;
      cord.y = i;
      coordinate.push_front(cord);
      int couleur = tableauBonbon[i][j].couleur;
      while(j + count < WIDTH && tableauBonbon[i][j + count].couleur == couleur) {
        cord.x = j + count;
        cord.y = i;
        coordinate.push_front(cord);
        count++;
      }
      if (count >= 3) {
        do{
          tableauBonbon[coordinate.front().y][coordinate.front().x].couleur = VIDE;
          coordinate.pop_front();
        }while(coordinate.size() != 0);
        remplacementVide();
      }
      else{
        do{
          coordinate.pop_front();
        }while(coordinate.size() != 0);
      }
    }
  }
  for(int i = 0; i < HEIGHT; i++) {
    for(int j = 0; j < WIDTH; j++) {
      int count = 1;
      Tuple cord;
      cord.x = j;
      cord.y = i;
      coordinate.push_front(cord);
      int couleur = tableauBonbon[i][j].couleur;
      while(i + count < HEIGHT && tableauBonbon[i + count][j].couleur == couleur) {
        cord.x = j;
        cord.y = i + count;
        coordinate.push_front(cord);
        count++;
      }
      if (count >= 3) {
        do{
          tableauBonbon[coordinate.front().y][coordinate.front().x].couleur = VIDE;
          coordinate.pop_front();
        }while(coordinate.size() != 0);
        remplacementVide();
      }
      else{
        do{
          coordinate.pop_front();
        }while(coordinate.size() != 0);
      }
    }
  }
}

bool match(int pos_x1, int pos_y1, int pos_x2, int pos_y2) {
  bool result = true;
  if (isNearToBonbon(pos_x1,pos_y1,pos_x2,pos_y2)) {
    if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'o')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'o');
      nombreCoup--;
    }
    else if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'v')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'v');
      nombreCoup--;
    }  
    else if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'g')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'g');
      nombreCoup--;
    }
    else if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'d')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'d');
      nombreCoup--;
    }
    else if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'h')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'h');
      nombreCoup--;
    }
    else if (isMatchPossible(pos_x1,pos_y1,pos_x2,pos_y2,tableauBonbon[pos_y1][pos_x1].couleur,'b')) {
      swap(pos_x1,pos_y1,pos_x2,pos_y2);
      destruction(pos_x2,pos_y2,tableauBonbon[pos_y2][pos_x2].couleur,'b');
      nombreCoup--;
    }
    else{
      result = false;
    }
  }
  return result;
}

/*FIN DECLARATION FONCTION*/

int main() {

  RenderWindow window(VideoMode(ECRAN_X, ECRAN_Y), "Candy Crush");
  
  /*LES IMAGES DE MEDHI LOUISON @JETEMPOISONNE*/
  Texture orange;
  if (!orange.loadFromFile("orange.png")){
  }
  Texture rouge;
  if (!rouge.loadFromFile("rouge.png")){
  }
  Texture bleu;
  if (!bleu.loadFromFile("bleu.png")){
  }
  Texture vert;
  if (!vert.loadFromFile("vert.png")){
  }
  Texture violet;
  if (!violet.loadFromFile("violet.png")){
  }
  /**/
  
  /*CREATION DES OBJETS DU MENU*/
  //Creation du bouton pour jouer
  Bouton boutonJouer;
  boutonJouer.x = ECRAN_X*0.25;
  boutonJouer.y = ECRAN_Y*0.75;
  boutonJouer.width = ECRAN_X/2;
  boutonJouer.height = 80;
  boutonJouer.color = Color::Black;

  Font font;
  if (!font.loadFromFile("arial.ttf"))
    return EXIT_FAILURE;
  Text textBouton("JOUER", font, 40);
  textBouton.setColor(Color::White);
  textBouton.setPosition(ECRAN_X*0.15 + boutonJouer.width*0.6, ECRAN_Y*0.75 + boutonJouer.height/4);
  
  Text textTitre("CANDY CRUSH", font, 65);
  textTitre.setColor(Color::Black);
  textTitre.setPosition((5*ECRAN_X)/16, ECRAN_Y/8);
  /*FIN CREATION DES OBJETS DU MENU*/

  /*INITIALISATION MENU*/
  while (!(jouer) && window.isOpen()) {
      
    Event event;

    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }
      
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && verificationClique(event, boutonJouer.x, boutonJouer.y, boutonJouer.width, boutonJouer.height)) {
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
  
  Objectif objectif;
  objectif.isScore = true;
  objectif.nbrScore = 10000 + (int)(Math::random() * ((20000 - 10000) + 1));
  nombreCoup = 10 + (int)(Math::random() * ((30 - 10) + 1));
  /*
  if(objectif.isScore){
    Text textObjectif("Vous devez avoir un score de : ", font, 20);
    textObjectif.setColor(Color::Black);
    textObjectif.setPosition(10,50);
  }
  else{
    Text textObjectif("Les bonbons que vous devez exploser sont : ", font, 20);
    textObjectif.setColor(Color::Black);
    textObjectif.setPosition(10,50);
  }
  */
  Text textObjectif("Vous devez avoir un score de : " + std::to_string(objectif.nbrScore), font, 20);
  textObjectif.setColor(Color::Black);
  textObjectif.setPosition(10,50);
  /*
   * Une Clock permet de compter le temps. Vous en aurez besoin pour savoir
   * le temps entre deux frames.
   */
  Clock clock;
  
  /*INITIALISATION DU TABLEAU*/
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      Bonbon bonbon;
      bonbon.couleur = ORANGE + (int)(Math::random() * ((VERT - ORANGE) + 1)); 
      bonbon.bonus = 1;
      tableauBonbon[i][j] = bonbon;
      Tuple coord;
      coord.x = OFFSET_X + (TAILLE_BONBON*j);
      coord.y = OFFSET_Y + (TAILLE_BONBON*i);
      tableauCoord[i][j] = coord;
    }
  }
  do{
    verificationTableau();
  }while(verificationLopops());
  scoreJoueur = 0;
  /*FIN INITIALISATION TABLEAU*/
  
  /*
   * La boucle de jeu principale. La condition de fin est la fermeture de la
   * fenêtre qu'on provoque à l'aide d'un appel `window.close()`.
   */
  while (window.isOpen() && nombreCoup > 0) {

    Event event;

    /*
     * Cette boucle permet de traiter tous les événements en attente.
     */
    while (window.pollEvent(event)) {

      if (event.type == Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {

        if (event.mouseButton.button == sf::Mouse::Left && verificationClique(event, OFFSET_X, OFFSET_Y, TAILLE_PLATEAU, TAILLE_PLATEAU)) {

          if(isClique) {
            clique_x2 = (int) event.mouseButton.x;
            clique_y2 = (int) event.mouseButton.y;
            matchi = match(posReel2posTableau(clique_x,'x'),posReel2posTableau(clique_y,'y'),posReel2posTableau(clique_x2,'x'),posReel2posTableau(clique_y2,'y'));
            isClique = false;
          }

          else {
            isClique = true;
            clique_x = (int) event.mouseButton.x;
            clique_y = (int) event.mouseButton.y;
            //std::cout << "x1: " << posReel2posTableau(clique_x,'x') << ", y1: " << posReel2posTableau(clique_y,'y') << '\n';
          }
        }

        else {
          isClique = false;
        }
      }
    }

    window.clear(Color::White);
    /*
      530 = position max de bonbon en x et y
      53 = position des bonbons afin de créer un effet de bord
      offset_x et offset_y = déplacement du tableau de bonbon par deux vecteur x et y
    */

    /*DECLARATION BACKGROUND TABLEAU GRAPHIQUE*/
    RectangleShape backgroundTableau;
    backgroundTableau.setSize(Vector2f(TAILLE_PLATEAU + 15, TAILLE_PLATEAU + 15));
    backgroundTableau.setPosition(Vector2f(OFFSET_X - 10, OFFSET_Y - 10));
    backgroundTableau.setFillColor(Color::Black);
    window.draw(backgroundTableau);
    /*FIN DECLARATION BACKGROUND TABLEAU GRAPHIQUE*/
    
    /*AFFICHAGE BONBON*/
    for(int i = 0; i < 10; i++){
      for(int j = 0; j < 10; j++){
        RectangleShape bonbonShape;
        bonbonShape.setSize(Vector2f(TAILLE_BONBON, TAILLE_BONBON));
        bonbonShape.setPosition(Vector2f(j*TAILLE_BONBON_OFFSET + OFFSET_X, i*TAILLE_BONBON_OFFSET + OFFSET_Y));
        switch (tableauBonbon[i][j].couleur) {
          case ORANGE:
              bonbonShape.setTexture(&orange);
          break;
          case ROUGE:
              bonbonShape.setTexture(&rouge);
          break;
          case BLEU:
            bonbonShape.setTexture(&bleu);
          break;
          case VIOLET:
            bonbonShape.setTexture(&violet);
          break;
          case VERT:
            bonbonShape.setTexture(&vert);
          break;                    
        }
        window.draw(bonbonShape);
      }
    }
    /*FIN AFFICHAGE BONBON*/
    
    if(matchi){
      do{
        verificationTableau();
        combo++;
      }while(verificationLopops());
      combo = 0;
      matchi = false;
    }

    /*DECLARATION ILLUMINATION BONBON*/
    window.draw(illuminationBonbon());
    window.draw(textObjectif);
    Text textJoueur("Votre score actuel est de : " + std::to_string(scoreJoueur), font, 20);
    textJoueur.setColor(Color::Black);
    textJoueur.setPosition(10, 100);
    window.draw(textJoueur);
    Text textCoup("Il vous reste " + std::to_string(nombreCoup) + " coup", font, 20);
    textCoup.setColor(Color::Black);
    textCoup.setPosition(10, 150);
    window.draw(textCoup);
    /*FIN DECLARATION ILLUMINATION BONBON*/
    
    window.display();
  }

  return 0;
}