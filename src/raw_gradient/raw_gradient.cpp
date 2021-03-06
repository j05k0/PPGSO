// Example raw_gradient
// - Illustrates the concept of a framebuffer
// - We do not really need any libraries or hardware to do computer graphics
// - In this case the framebuffer is simply saved as a raw RGB and TGA image

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

// Size of the framebuffer
const unsigned int SIZE = 512;
#define PI 3.14159265

// A simple RGB struct will represent a pixel in the framebuffer
// Note: unsigned char is range <0, 255>
//       signed char is range <-128, 127>
struct Pixel {
  unsigned char r, g, b;
};

struct Point {
    float x;
    float y;
};

void plot(int y, int x, Pixel framebuffer[][SIZE]){
    framebuffer[x][y].r = 0;
    framebuffer[x][y].g = 0;
    framebuffer[x][y].b = 0;
}

void Bresenham(int x1,
               int y1,
               int const x2,
               int const y2,
               Pixel framebuffer [][SIZE])
{
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    plot(x1, y1, framebuffer);

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;

            plot(x1, y1, framebuffer);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;

            plot(x1, y1, framebuffer);
        }
    }
}

Point lerp(Point A, Point B, float t){
    Point temp;
    temp.x = A.x + ((B.x - A.x) * t);
    temp.y = A.y + ((B.y - A.y) * t);
    return temp;
}

void Bezier(std::vector<Point> &out, std::vector<Point> in, float N){
    Point A,B,C,X,Y,P;
    float dt = 1/N;
    int j = 0;
    for(int i = 0; i < (in.size() - 1) / 3; i++) {
        for (float t = 0; t <= 1; t += dt) {
            A = lerp(in[j], in[j + 1], t);
            B = lerp(in[j + 1], in[j + 2], t);
            C = lerp(in[j + 2], in[j + 3], t);
            X = lerp(A, B, t);
            Y = lerp(B, C, t);
            P = lerp(X, Y, t);
            out.push_back(P);
        }
        j += 3;
    }

}

int main() {
  // Initialize a framebuffer
  // NOTE: The framebuffer is allocated on the stack. For bigger sizes this should be changed to a heap allocation.
  Pixel framebuffer[SIZE][SIZE];

  // Example: Generate a simple gradient
  /*for (unsigned int x = 0; x < SIZE; ++x) {
    for (unsigned int y = 0; y < SIZE; ++y) {
      framebuffer[x][y].r = static_cast<unsigned char>(x / 2);
      framebuffer[x][y].g = static_cast<unsigned char>(y / 2);
      framebuffer[x][y].b = 0;
    }
  }*/

  // Task1: Load RAW image file here instead

    //task 1
   /* FILE *pic;
    long lSize;

    pic = fopen("rs6.rgb", "rb"); //rb je binarne citanie, pri niektorych suboroch by to spado ak by nacitalo koncovy znak

    fseek (pic , 0 , SEEK_END);
    lSize = ftell (pic);
    rewind (pic);

    fread(framebuffer,1,lSize,pic);
    fclose(pic);

    for (unsigned int x = 0; x < SIZE; ++x) {
        for (unsigned int y = 0; y < SIZE; ++y) {
            framebuffer[x][y].r = static_cast<unsigned char>(255 - framebuffer[x][y].r);
            framebuffer[x][y].g = static_cast<unsigned char>(255 - framebuffer[x][y].g);
            framebuffer[x][y].b = static_cast<unsigned char>(255 - framebuffer[x][y].b);
        }
    }*/

    //task 2: Bressenham algorithm
/*
    int count = 5, i;
    //Point points[5];
    std::vector<Point> points (5);

    for (unsigned int x = 0; x < SIZE; ++x) {
        for (unsigned int y = 0; y < SIZE; ++y) {
            framebuffer[x][y].r = 255;
            framebuffer[x][y].g = 255;
            framebuffer[x][y].b = 255;
        }
    }

    int alpha = 0;
    for (i = 0; i < count; i++) {
        points[i].x = 100 * sin(alpha*PI/180)+ SIZE/2;
        points[i].y = -100* cos(alpha*PI/180)+ SIZE/2;
        alpha =  (alpha+144) % 360;
    }

    for (i = 0; i < count; i++) {
        Bresenham(points[i].x,points[i].y,points[(i+1)%count].x,points[(i+1)%count].y,framebuffer);
    }
*/

    //task 3: Bezier curve

    std::vector<Point> in, out;
    int alpha = -45, i;
    for (i = 0; i < 4; i++) {
        Point tmp;
        tmp.x = 100 * sin(alpha*PI/180)+ SIZE/2;
        tmp.y = -100* cos(alpha*PI/180)+ SIZE/2;
        alpha =  (alpha + 90) % 360;
        in.push_back(tmp);
    }

    Point start, end;
    start = in[0];
    end = in[3];
    for(i = 0; i < 3; i++){
        Point tmp;
        tmp.x = end.x + ((start.x - end.x) * ((i + 1) / 3));
        tmp.y = end.y + ((start.y - end.y) * ((i + 1) / 3));
        in.push_back(tmp);
    }

    Bezier(out, in, 1000000);

    for (unsigned int x = 0; x < SIZE; ++x) {
        for (unsigned int y = 0; y < SIZE; ++y) {
            framebuffer[x][y].r = 255;
            framebuffer[x][y].g = 255;
            framebuffer[x][y].b = 255;
        }
    }

    for(i = 0; i < out.size(); i++){
        plot(out[i].x, out[i].y, framebuffer);
    }

  // Save the raw image to a file
  std::cout << "Generating result.rgb file ..." << std::endl;
  std::ofstream raw("result.rgb", std::ios::binary);
  raw.write(reinterpret_cast<char *>(framebuffer), sizeof(framebuffer));
  raw.close();

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}
