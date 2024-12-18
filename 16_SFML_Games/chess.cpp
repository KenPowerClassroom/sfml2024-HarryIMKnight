#include <SFML/Graphics.hpp>
#include <time.h>
#include "Connector.hpp"
using namespace sf;

int size = 56;
Vector2f os(28,28);

Sprite f[32]; //figures
std::string position=""; // Keeps track of the sequence of moves in standard algebraic notation

int board[8][8] = 
    {-1,-2,-3,-4,-5,-3,-2,-1,
     -6,-6,-6,-6,-6,-6,-6,-6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      6, 6, 6, 6, 6, 6, 6, 6,
      1, 2, 3, 4, 5, 3, 2, 1};

// Converts a position from screen coordinates to chess notation (e.g., (0,0) -> "a8")
std::string toChessNote(Vector2f p)
{
  std::string s = "";
  s += char(p.x/size+97);  // Convert the x-coordinate to the corresponding letter (a-h)
  s += char(7-p.y/size+49);  // Convert the y-coordinate to the corresponding number (1-8)
  return s;
}

Vector2f toCoord(char a,char b)
{
   int x = int(a) - 97;
   int y = 7-int(b)+49;
   return Vector2f(x*size,y*size);
}

void move(std::string str)
{
    Vector2f oldPos = toCoord(str[0],str[1]);
    Vector2f newPos = toCoord(str[2],str[3]);

    for(int i=0;i<32;i++)
     if (f[i].getPosition()==newPos) f[i].setPosition(-100,-100);
        
    for(int i=0;i<32;i++)
     if (f[i].getPosition()==oldPos) f[i].setPosition(newPos);

    //castling       //if the king didn't move
    if (str=="e1g1") if (position.find("e1")==-1) move("h1f1"); 
    if (str=="e8g8") if (position.find("e8")==-1) move("h8f8");
    if (str=="e1c1") if (position.find("e1")==-1) move("a1d1");
    if (str=="e8c8") if (position.find("e8")==-1) move("a8d8");
}

// Loads the chess pieces to the board based on the initial state and previous moves
void loadPosition()
{
    int k=0;
    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
     {
       int n = board[i][j];
       if (!n) continue;
       int x = abs(n)-1;
       int y = n>0?1:0;
       f[k].setTextureRect( IntRect(size*x,size*y,size,size) );
       f[k].setPosition(size*j,size*i);
       k++;
     }

    for(int i=0;i<position.length();i+=5)
      move(position.substr(i,4));
}


int chess()
{
    RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");
    
    const wchar_t const* engine = L"stockfish.exe";
    wchar_t dst[14];
    wcscpy_s(dst, engine);

    ConnectToEngine(dst);    

    Texture t1,t2;
    t1.loadFromFile("images/chess/figures.png"); 
    t2.loadFromFile("images/chess/board.png");

    for(int i=0;i<32;i++) f[i].setTexture(t1);
    Sprite sBoard(t2); 

    loadPosition();

    bool isMove=false;
    float dx=0, dy=0;
    Vector2f oldPos,newPos;
    std::string str;
    int n=0; 

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window) - Vector2i(os);

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            ////move back//////
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::BackSpace)
                { if (position.length()>6) position.erase(position.length()-6,5); loadPosition();}

            /////drag and drop///////
            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                  for(int i=0;i<32;i++)
                  if (f[i].getGlobalBounds().contains(pos.x,pos.y))
                      {
                       isMove=true; n=i;
                       dx=pos.x - f[i].getPosition().x;
                       dy=pos.y - f[i].getPosition().y;
                       oldPos  =  f[i].getPosition();
                      }

             if (e.type == Event::MouseButtonReleased)
                if (e.key.code == Mouse::Left)
                 {
                    isMove = false;  // Stop dragging the piece
                    Vector2f p = f[n].getPosition()+Vector2f(size/2,size/2);  // Get the center of the puece
                    newPos = Vector2f(size*int(p.x/size),size*int(p.y/size));  // Snap to nearest square
                    str = toChessNote(oldPos) + toChessNote(newPos);  // Create move string (e.g., "e2e4")
                    move(str);  // Execute the move
                    if (oldPos!=newPos) position+=str+" ";  // Add move to history if it's valid
                    f[n].setPosition(newPos);  // Move the piece visually
                 }                       
        }

       //comp move
       if (Keyboard::isKeyPressed(Keyboard::Space))
       {
         str =  getNextMove(position);
                   
         oldPos = toCoord(str[0],str[1]);
         newPos = toCoord(str[2],str[3]);
         
         for(int i=0;i<32;i++) if (f[i].getPosition()==oldPos) n=i;
         
         /////animation///////
         for(int k=0;k<50;k++)
          {
            Vector2f p = newPos - oldPos;
            f[n].move(p.x/50, p.y/50); 
            window.draw(sBoard);
            for(int i=0;i<32;i++) f[i].move(os);
            for(int i=0;i<32;i++) window.draw(f[i]); window.draw(f[n]);
            for(int i=0;i<32;i++) f[i].move(-os);
            window.display();
          }

        move(str);  position+=str+" ";
        f[n].setPosition(newPos); 
        }

        if (isMove) f[n].setPosition(pos.x-dx,pos.y-dy);

    ////// draw  ///////
    window.clear();
    window.draw(sBoard);
    for(int i=0;i<32;i++) f[i].move(os);
    for(int i=0;i<32;i++) window.draw(f[i]); window.draw(f[n]);
    for(int i=0;i<32;i++) f[i].move(-os);
    window.display();
    }

    CloseConnection();
    return 0;
}
