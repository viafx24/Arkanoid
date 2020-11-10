// un bug du jeux, la balle rebondit quand m�me si pas le paddle mais le bas.

// cet include correspond a un include global de plein d'include
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// une erreur est detect� concernant le main qui utiliserait trop d'octet dans la pile (stack) : il est alors conseill� des deplacer
// les donn�es vers le tas (heap). Une recherche rapide sur internet sugg�re l'utilisation du new lors de la cr�ation d'objet ce qui
// place les objets dans le tas. Je n'ai pas encore regard� toutes ces nuances et � quoi correspondent exactement pile et tas.
// il s'agit ici que de warning mais en garder en t�te car on y reviendra.

int main()
{
    srand(time(0));
    // la fen�tre principale de SFML
    RenderWindow app(VideoMode(520, 450), "Arkanoid!");

    // definira la vitesse de deplacement de la balle. si dx=1, la balle se deplace de  60px seconde 
    // (si je comprends bien)
    app.setFramerateLimit(60);


    // ca vient de moi: pour travailler avec VS.
    sf::Vector2i Position(10, 10);
    app.setPosition(Position);

    Texture t1,t2,t3,t4;
    t1.loadFromFile("images/block01.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/ball.png");
    t4.loadFromFile("images/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(300,440);


    // pourquoi 1000  et pas 100 : pour r�server de l'espace m�moire?
    Sprite block[1000];

    int n=0;

    // le decalage qu'on voit (les briques ne touchent pas les bords vient du int i=1
    // et non i=0
    for (int i=1;i<=10;i++)
    for (int j=1;j<=10;j++)
      {
         block[n].setTexture(t1);
         block[n].setPosition(i*43,j*20);
         n++;
      }

    // dx et dy : deplacement de  la balle en pixel � chaque frame (60/sec)
    //float dx=6, dy=5;

    float dx = 3, dy = 2;

    // position initial de la balle.
    float x=300, y=300;

    while (app.isOpen())
    {
       Event e;
       while (app.pollEvent(e))
       {
         if (e.type == Event::Closed)
             app.close();
       }

    // on increment x � chaque frame
    x+=dx;

    // on verifie les intersections de rectanble (la balle et les briques)
    // et on fait rebondir la balle en cas d'intersection.

    // apr�s FloatRect on avait x+3,y+3,6,6 au lieu de x,y,12,12: je ne vois pas diff�rence visible �vidente.

    // la pr�sence des deux boucles for (une pour x et une pour y) permet de s�parer les evenements de rebonds
    // suivant que la balle tape sur un cot� vertical (rebond en x) ou horizontal (rebond en y).
    // dans le cas o� la balle intersect � la fois en y et en x, il me semble probable qu'alors et le x et le y sont invers�;
    // mais je n'ai pas verifi� ces cas de figures.


    for (int i=0;i<n;i++)
        if ( FloatRect(x,y,12,12).intersects(block[i].getGlobalBounds()) ) 

            // l'utilisation du -100 permet de virer la brique de l'�cran
            // ce n'est pas tr�s propre mais la doc sur internet ne permet 
            // pas de trouver la r�ponse instantanement
            // j'aurais aimer un appel au destructeur
            // d'autre sur le net passe la transparence � zero
            // d'autres disent qu'il suffit de ne pas le "draw"
            // bref, tout cela n'est pas satisfaisant: quand on ne veut plus d'un objet
            // on le delete.

             {block[i].setPosition(-100,0); dx=-dx;}

    y+=dy;

    // on retrouve cette �criture bizarre mais invers�e cette fois: apr�s le for, si il y a un if, il n'y a pas besoin d'accolade
    // pour d�finir le d�but et la fin du for (c'est pareil avec un if en premier puis un for).

    for (int i=0;i<n;i++)
        if ( FloatRect(x,y,12,12).intersects(block[i].getGlobalBounds()) ) 
             {block[i].setPosition(-100,0); dy=-dy;}

    // rebonds sur les bords

    if (x<0 || x>520)  dx=-dx;
    if (y<0 || y>450)  dy=-dy;


    // pourquoi pas integrer � la gestion des events? je ne sais pas encore.
    // la fonction move (de sprite) permet de deplacer un objet relativement

    if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(6,0);
    if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-6,0);


    // si la balle arrive sur le paddle, alors rebondit en y avec une valeur al�atoire
    // pourquoi cette fois le FloatRect fait x,y,12,12 et pas x+3,y+3,6,6: a regarder?


    // le but du rand est de changer la vitesse de la balle
    // ce qui rend l'experience de jeu plus plaisante.

    if ( FloatRect(x,y,12,12).intersects(sPaddle.getGlobalBounds()) ) dy=-(rand()%5+2);

    // test sans le rand pour voir la diff�rence.
    //if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) dy = -dy;


    sBall.setPosition(x,y);


    // on retrouve la triade, clear, draw et display.
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    for (int i=0;i<n;i++)
     app.draw(block[i]);

    app.display();
    }

  return 0;
}

