#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <Eigen/Dense>

float PI = M_PI;
float fcos(float x) { return cos(x); }
float fsin(float x) { return sin(x); }
float ftan(float x) { return tan(x); }

typedef std::vector<Eigen::Vector4f> VertexList;

Eigen::Matrix4f rotate_z(float v) {
   const float d1 = 2.0f * PI / 360.0f;
   float rads = v * d1;
   Eigen::Matrix4f x{
      { fcos(rads),-fsin(rads), 0, 0 },
      { fsin(rads), fcos(rads), 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
   };
   return x;
}

Eigen::Matrix4f rotate(float v) {
   const float d1 = 2.0f * PI / 360.0f;
   float rads = v * d1;
   Eigen::Matrix4f x{
      { 1, 0, 0, 0 },
      { 0, fcos(rads),-fsin(rads), 0 },
      { 0, fsin(rads), fcos(rads), 0 },
      { 0, 0, 0, 1 }
   };
   return x;
}

Eigen::Matrix4f flipX{
   { -1, 0, 0, 0 },
   {  0, 1, 0, 0 },
   {  0, 0, 1, 0 },
   {  0, 0, 0, 1 }
};

Eigen::Matrix4f flipY{
   { 1, 0, 0, 0 },
   { 0,-1, 0, 0 },
   { 0, 0, 1, 0 },
   { 0, 0, 0, 1 },
};

Eigen::Matrix4f translate(Eigen::Vector4f v) {
   Eigen::Matrix4f x{
      { 1,0,0,v(0) },
      { 0,1,0,v(1) },
      { 0,0,1,v(2) },
      { 0,0,0,1 } };
   return x;
}

Eigen::Matrix4f scale(float v) {
   Eigen::Matrix4f x{
      { v,0,0,0 } ,
      { 0,v,0,0 },
      { 0,0,v,0 },
      { 0,0,0,1 } };
   return x;
}

static VertexList unitCubeCenteredAtOrigin() {
   Eigen::Vector4f one( -0.5, -0.5, -0.5, 1);
   Eigen::Vector4f two(  0.5, -0.5, -0.5, 1);
   Eigen::Vector4f thr(  0.5,  0.5, -0.5, 1);
   Eigen::Vector4f fou( -0.5,  0.5, -0.5, 1);

   Eigen::Vector4f fiv( -0.5, -0.5, 0.5, 1);
   Eigen::Vector4f six(  0.5, -0.5, 0.5, 1);
   Eigen::Vector4f sev(  0.5,  0.5, 0.5, 1);
   Eigen::Vector4f eig( -0.5,  0.5, 0.5, 1);
   return {one, two, thr, fou, one, fiv, six, two, six, sev, thr, sev, eig, fou, eig, fiv};
}



int main()
{
   VertexList vertices;

   sf::RenderWindow window(sf::VideoMode(800, 800), "Cube");
   sf::Clock clock;

   float aspect = 1;
   float fov = PI / 2;
   float far = 2;
   float near = 1;

   while (window.isOpen()) {
      auto time = clock.getElapsedTime().asMilliseconds();
      vertices = unitCubeCenteredAtOrigin();
      for ( auto& vertex : vertices ) {
         // Scale up cube and translate away from camera
         vertex =
            translate(Eigen::Vector4f(0,0,3,1)) *
            scale(2.0+fsin(time/1000.0)/2.0) *
            rotate(time * 90.0 / 1000.0) *
            rotate_z(time * 90.0 / 1100.0) *
            vertex;
      }

      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed)
            window.close();
         // Respond to key pressed events
         if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Escape){
               return 0;
            } else if (event.key.code == sf::Keyboard::Right){
            } else if (event.key.code == sf::Keyboard::Left ){
            } else if (event.key.code == sf::Keyboard::Down ){
            } else if (event.key.code == sf::Keyboard::Up ){
            } else if (event.key.code == sf::Keyboard::Add ){
               fov += 0.02;
            } else if (event.key.code == sf::Keyboard::Subtract ){
               fov -= 0.02;
            }
         }
      }

      // Distance from camera to projection windows based on fov angle
      // logicall this d factor reduces the distance points appear from the origin as the become further aways on the z axis
      float d = 1 / ftan(fov/2.0);

      // Range of clipping, we're not actually doing any clipping so this
      // really doesn't have any effect at the moment. We're scaling the
      // range of near to far on Z axis to -1.0 to +1.0.
      float r = far - near;

      Eigen::Matrix4f projection{
         { d / aspect, 0, 0, 0 },
         { 0, d, 0, 0 },
         { 0, 0, -(far+near)/ r, -(2*(far * near ))/r },
         // Save old value of Z so it doesn't just get set 1 one when dividec by Z.
         { 0, 0, 1, 0 },
            };

      sf::VertexArray cube(sf::LinesStrip, 0);
      for ( auto vertex : vertices ) {
         Eigen::Vector4f vert = projection * vertex;
         vert = vert / vert(3);
         cube.append( sf::Vector2f( vert(0), vert(1)) );
      }
      if ( vertices.begin() != vertices.end() ) {
         auto &vertex = *vertices.begin();
         Eigen::Vector4f vert = projection * vertex;
         vert = vert / vert(3);
         cube.append( sf::Vector2f( vert(0), vert(1)) );
      }

      window.clear( sf::Color::Black );

      sf::RectangleShape origin;
      window.setView( sf::View(sf::FloatRect(-1, -1, 2, 2 )) );

      origin.setSize(sf::Vector2f(0.020, 0.020));
      origin.setPosition(-0.010, -0.010);

      window.draw( origin );
      window.draw( cube );
      window.display();
   }
   return 0;
}
