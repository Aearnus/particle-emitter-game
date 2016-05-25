#include <SFML/Graphics.hpp>
#define WIDTH_WIN 1600
#define HEIGHT_WIN 900
#define SPARKLY_AMOUNT 1

const float centerX = WIDTH_WIN / 2;
const float centerY = HEIGHT_WIN / 2;

//wikipedia: linear_interpolation
float lerp(float v0, float v1, float t) {
  return (1-t)*v0 + t*v1;
}

class Enemy {
public:
  int life;
  float x;
  float y;
  const float width = 100;
  const float height = 60;
  float speed; //toward center
  Enemy(int life, float speed) {    
    this->life = life;
    this->speed = speed;
    x = WIDTH_WIN * std::cos(((float)rand() * 2 * 3.141) / (float)RAND_MAX);
    y = WIDTH_WIN * std::cos(((float)rand() * 2 * 3.141) / (float)RAND_MAX);
  }

  void tick() {
    x = lerp(x, centerX, 0.005);
    y = lerp(y, centerY, 0.005);
  }
};

class ParticleEmitter {
public:
  float x = WIDTH_WIN / 2;
  float y = HEIGHT_WIN / 2;
  struct Particle {
    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    long life = 0;
  };
  std::vector<Particle*> particles;
  ParticleEmitter() {
    
  }

  void reflectX(Particle* p) {
    p->dx = -(p->dx);
    p->x = p->x + p->dx;
  }

  void reflectY(Particle* p) {
    p->dy = -(p->dy);
    p->y = p->y + p->dy;
  }
  
  void updateParticle(Particle* p) {
    if ((p->x < 0) || (p->x > WIDTH_WIN)) {
	reflectX(p);
    }
    if ((p->y < 0) || (p->y > HEIGHT_WIN)) {
	reflectY(p);
    }
    p->x += p->dx;
    p->y += p->dy;
    p->life++;
  }
  void tickParticles() {
    for (unsigned int i = 0; i < particles.size(); i++) {
      updateParticle(particles[i]);
    }
  }
  void addParticle(float dx, float dy) {
    Particle* tempParticle = new Particle();
    tempParticle->x = x;
    tempParticle->y = y;
    //tempParticle->dx = (((float)(rand()-(RAND_MAX/2)))/RAND_MAX) * 20;
    //tempParticle->dy = (((float)(rand()-(RAND_MAX/2)))/RAND_MAX) * 20;
    tempParticle->dx = dx;
    tempParticle->dy = dy;
    particles.push_back(tempParticle);
  }
  void removeParticle(unsigned int index) {

  }
};

int main() {
  sf::RenderWindow window(sf::VideoMode(WIDTH_WIN, HEIGHT_WIN), "Powder");
  ParticleEmitter emitter = ParticleEmitter();
  emitter.x = centerX;
  emitter.y = centerY;
  std::vector<Enemy*> enemies;
  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
	window.close();
      }
      else if (e.type == sf::Event::MouseButtonPressed) {
	if (e.mouseButton.button == sf::Mouse::Left) {
	  //handle it
	}
      }
    }
    window.clear();
    //draw aim reticule
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float mouseMagnitude = std::hypot(mouseX - centerX, mouseY - centerY);
    float normalizedX = (mouseX - centerX) / mouseMagnitude;
    float normalizedY = (mouseY - centerY) / mouseMagnitude;
    sf::Vertex aimLine[] = {
      sf::Vertex(sf::Vector2f(WIDTH_WIN / 2, HEIGHT_WIN / 2)),
      sf::Vertex(sf::Vector2f((WIDTH_WIN / 2) + normalizedX * 20, (HEIGHT_WIN / 2) + normalizedY * 20))
    };
    window.draw(aimLine, 2, sf::Lines);

    //spawn, draw and update enemies
    if (rand() % 32 == 0) {
      Enemy* tempEnemy = new Enemy(100, 10);
      enemies.push_back(tempEnemy);
    }
    for (unsigned int i = 0; i < enemies.size(); i++) {
      enemies[i]->tick();
      sf::RectangleShape enemyRect(sf::Vector2f(enemies[i]->width, enemies[i]->height));
      enemyRect.setPosition(sf::Vector2f(enemies[i]->x, enemies[i]->y));
      enemyRect.setFillColor(sf::Color::White);
      window.draw(enemyRect);
    }
    
    //shoot and draw particles
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      for (unsigned int i = 0; i < SPARKLY_AMOUNT; i++) {
	emitter.addParticle(normalizedX * 20, normalizedY * 20);
      }
    }
    emitter.tickParticles();
    for (unsigned int i = 0; i < emitter.particles.size(); i++) {
      sf::RectangleShape rect(sf::Vector2f(3,3));
      rect.setPosition(sf::Vector2f((int)emitter.particles[i]->x, (int)emitter.particles[i]->y));
      rect.setFillColor(sf::Color(rand()%256,rand()%256,rand()%256));
      window.draw(rect);
    }
    window.display();
  }
}
