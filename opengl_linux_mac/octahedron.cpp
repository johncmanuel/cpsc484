
#include "common_opengl.h"

#define phi 1.618033989
#define psi 0.618033989
#define PI 3.1415926535

GLfloat angle = 0.0f;
const int num_faces = 8;

double points[6][3] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                       {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

int faces[num_faces][3] = {{0, 2, 4}, {0, 4, 3}, {0, 3, 5}, {0, 5, 2},
                           {1, 2, 5}, {1, 5, 3}, {1, 3, 4}, {1, 4, 2}};

GLfloat colors[num_faces][3] = {{1, 0, 0},     {0, 1, 0},    {0, 0, 1},
                                {1, 1, 0},     {1, 0, 1},    {0, 1, 1},
                                {0.5, 0.5, 0}, {0.5, 0, 0.5}};

void reshape(int w, int h) {
  double aspect = double(w) / double(h);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspect, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

void idle() {
  // slowing down the rotation
  angle += 1.0;
  if (angle >= 360.0f) {
    angle = 0.0f;
  }
  glutPostRedisplay();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotated(angle, 1.0, 1.0, 0.0);

  for (int i = 0; i < num_faces; i++) {
    glBegin(GL_TRIANGLES);
    glColor3fv(colors[i]);
    for (int x = 0; x < 3; x++) {
      glVertex3dv(points[faces[i][x]]);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int x = 0; x < 3; x++) {
      glVertex3dv(points[faces[i][x]]);
    }
    glEnd();
  }
  glutSwapBuffers();
}

// setup GLUT for running the octahedron
void run_glut_octahedron(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Octahedron");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMainLoop();
}

int main(int argc, char *argv[]) {
  std::cout << "Starting program\n";
  run_glut_octahedron(argc, argv);
  std::cout << "...program completed\n";
  return 0;
}
