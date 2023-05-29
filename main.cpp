#include <QtGui>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QApplication>
#include <GL/gl.h>
#include <GL/glu.h>

class GLWidget : public QOpenGLWidget
{
public:
    explicit GLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void resizeGL(int w, int h) override
    {
        widthW = w;
        heightW = h;

        glViewport(0, 0, widthW, heightW);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, widthW, 0, heightW); // Set the orthographic projection to match the widget dimensions

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void paintGL() override
    {
        glViewport(0, 0, width(), height()); // Set the viewport to match the window dimensions

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, image.width(), 0, image.height()); // Set the orthographic projection to match the image dimensions

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawImageIn3D();

        // Add any additional rendering or OpenGL calls here
    }

    void drawImageIn3D()
    {
        // Load the PNG image
        QImage image("1.png");
        if (image.isNull())
        {
            qDebug() << "Failed to load image";
            return;
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width(), 0, height(), -1, 1); // Set the orthographic projection to match the widget dimensions

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);

        glEnable(GL_TEXTURE_2D);
        if (textureId == 0)
        {
            glGenTextures(1, &textureId);
        }
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits()); // Use GL_BGRA to fix color issues
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Calculate the position to center the image
        float xPos = (width() - image.width()) / 2.0;
        float yPos = (height() - image.height()) / 2.0;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); // Flip the texture vertically
        glVertex2f(xPos, yPos);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(xPos + image.width(), yPos);
        glTexCoord2f(1.0, 0.0);
        glVertex2f(xPos + image.width(), yPos + image.height());
        glTexCoord2f(0.0, 0.0);
        glVertex2f(xPos, yPos + image.height());
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
    }

private:
    GLuint textureId;
    QImage image;
    int widthW; // Width of the widget
    int heightW; // Height of the widget
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GLWidget w;
    w.resize(1200, 900);
    w.show();

    return a.exec();
}
