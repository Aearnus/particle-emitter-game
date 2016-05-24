#include <SFML/Graphics.hpp>
#define WIDTH_WIN 1600
#define HEIGHT_WIN 900
#define SPARKLY_AMOUNT 1

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
  float centerX = WIDTH_WIN / 2;
  float centerY = HEIGHT_WIN / 2;
  emitter.x = centerX;
  emitter.y = centerY;
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
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float mouseMagnitude = std::sqrt((mouseX - centerX) * (mouseX - centerX) + (mouseY - centerY) * (mouseY - centerY));
    float normalizedX = (mouseX - centerX) / mouseMagnitude;
    float normalizedY = (mouseY - centerY) / mouseMagnitude;
    //draw aimy thing
    sf::Vertex aimLine[] = {
      sf::Vertex(sf::Vector2f(WIDTH_WIN / 2, HEIGHT_WIN / 2)),
      sf::Vertex(sf::Vector2f((WIDTH_WIN / 2) + normalizedX * 20, (HEIGHT_WIN / 2) + normalizedY * 20))
    };
    window.draw(aimLine, 2, sf::Lines);
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
