#include <GL/glut.h>
#include <cstdint>
#include <cmath>
#include <iostream>

uint_fast64_t MAX_IT = 64;

struct s_params {
    GLfloat min_x = -3.0;
    GLfloat max_x = 3.0;
    GLfloat min_y = -3.0;
    GLfloat max_y = 3.0;
} p;

inline void reset() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(p.min_x, p.max_x, p.max_y, p.min_y, -1.0, 1.0);
}

uint_fast64_t mandel(GLfloat x, GLfloat y) {
    GLfloat a = x;
    GLfloat b = y;
    uint_fast64_t i = 0u;

    for (;i < MAX_IT && a * a + b * b < 4.0; ++i) {
        GLfloat c = a;
        a = a * a - b * b + x;
        b = 2u * c * b + y;
    }
    return i < MAX_IT ? MAX_IT - i : 0u;
}

void drawBox(void)
{
    reset();

    GLfloat step_x = (p.max_x - p.min_x) / glutGet(GLUT_WINDOW_WIDTH);
    GLfloat step_y = (p.max_y - p.min_y) / glutGet(GLUT_WINDOW_HEIGHT);

    for (GLfloat x = p.min_x; x < p.max_x; x += step_x) {
        for (GLfloat y = p.min_y; y < p.max_y; y += step_y) {
            GLfloat m = (GLfloat)mandel(x, y);
            GLfloat c = m / (GLfloat)MAX_IT;
            glColor3f(c, c * c, c);
            glBegin(GL_POLYGON);
                glVertex3f(x, y, 0.0);
                glVertex3f(x + step_x, y, 0.0);
                glVertex3f(x + step_x, y + step_y, 0.0);
                glVertex3f(x, y + step_y, 0.0);
            glEnd();
        }
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBox();
    glutSwapBuffers();
}

void mouse(int button, int pressed, int x, int y)
{
    auto width = glutGet(GLUT_WINDOW_WIDTH);
    auto height = glutGet(GLUT_WINDOW_HEIGHT);

    constexpr GLfloat zoom = 1.15;
    constexpr uint_fast64_t iter_step = 16u;

    GLfloat o_size_x = p.max_x - p.min_x;
    GLfloat o_size_y = p.max_y - p.min_y;

    GLfloat cx = p.min_x + ((GLfloat)x) * o_size_x / width;
    GLfloat cy = p.min_y + ((GLfloat)y) * o_size_y / height;

    GLfloat n_size_x = o_size_x, n_size_y = o_size_y;

    if (0 == pressed) {
        if (0 == button) {
            n_size_x = o_size_x / zoom;
            n_size_y = o_size_y / zoom;
            MAX_IT += iter_step;
        } else if (2 == button) {
            n_size_x = o_size_x * zoom;
            n_size_y = o_size_y * zoom;
            MAX_IT -= iter_step;
        }
        p.min_x = cx - n_size_x / 2;
        p.max_x = cx + n_size_x / 2;
        p.min_y = cy - n_size_y / 2;
        p.max_y = cy + n_size_y / 2;

        std::cout << p.min_x << " " << p.min_y << " " << p.max_x << " " << p.max_y << '\n';
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 800);
    glutCreateWindow ("Mandelbrot");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    reset();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}