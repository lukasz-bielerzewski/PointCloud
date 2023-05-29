#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QImage>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <vector>
#include <QKeyEvent>

class PointCloudWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
public:
    PointCloudWidget(QWidget* parent = nullptr) : QOpenGLWidget(parent) {}

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT);

        // Load RGB image
        QImage rgbImage("1.png");
        if (rgbImage.isNull()) {
            qDebug("Failed to load RGB image.");
            return;
        }

        // Load depth image
        QImage depthImage("1b.png");
        if (depthImage.isNull()) {
            qDebug("Failed to load depth image.");
            return;
        }

        // Convert images to point cloud
        std::vector<QVector3D> pointCloud;
        for (int y = 0; y < rgbImage.height(); ++y) {
            for (int x = 0; x < rgbImage.width(); ++x) {
                QColor rgbColor = rgbImage.pixelColor(x, y);
                QColor depthColor = depthImage.pixelColor(x, y);

                float depth = depthColor.valueF(); // Assuming grayscale depth in the range [0, 1]
                float normalizedX = (2.0f * x / rgbImage.width()) - 1.0f;
                float normalizedY = (2.0f * y / rgbImage.height()) - 1.0f;
                QVector3D point(normalizedX, normalizedY, depth);
                pointCloud.push_back(point);
            }
        }

        // Apply transformations
        QMatrix4x4 transformation;
        transformation.translate(translation);
        transformation.rotate(rotation);

        // Render point cloud
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(1.0f);

        glBegin(GL_POINTS);
        for (const auto& point : pointCloud) {
            QColor rgbColor = rgbImage.pixelColor((point.x() + 1.0f) * 0.5f * rgbImage.width(),
                                                  (point.y() + 1.0f) * 0.5f * rgbImage.height());

            glColor3f(rgbColor.redF(), rgbColor.greenF(), rgbColor.blueF());

            QVector4D transformedPoint = transformation * QVector4D(point, 1.0f);
            glVertex3f(transformedPoint.x(), transformedPoint.y(), transformedPoint.z());
        }
        glEnd();
    }

    void keyPressEvent(QKeyEvent* event) override {
        const float translationStep = 0.1f;
        const float rotationStep = 5.0f;

        switch (event->key()) {
        case Qt::Key_Left:
            translation.setX(translation.x() - translationStep);
            break;
        case Qt::Key_Right:
            translation.setX(translation.x() + translationStep);
            break;
        case Qt::Key_Up:
            translation.setY(translation.y() - translationStep);
            break;
        case Qt::Key_Down:
            translation.setY(translation.y() + translationStep);
            break;
        case Qt::Key_A:
            rotation *= QQuaternion::fromEulerAngles(0.0f, -rotationStep, 0.0f);
            break;
        case Qt::Key_D:
            rotation *= QQuaternion::fromEulerAngles(0.0f, rotationStep, 0.0f);
            break;
        case Qt::Key_W:
            rotation *= QQuaternion::fromEulerAngles(-rotationStep, 0.0f, 0.0f);
            break;
        case Qt::Key_S:
            rotation *= QQuaternion::fromEulerAngles(rotationStep, 0.0f, 0.0f);
            break;
        case Qt::Key_Q:
            rotation *= QQuaternion::fromEulerAngles(0.0f, 0.0f, -rotationStep);
            break;
        case Qt::Key_E:
            rotation *= QQuaternion::fromEulerAngles(0.0f, 0.0f, rotationStep);
            break;
        default:
            QOpenGLWidget::keyPressEvent(event);
            return;
        }

        update();
    }

private:
    QVector3D translation;
    QQuaternion rotation;
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    PointCloudWidget widget;
    widget.resize(640, 480);
    widget.show();

    return app.exec();
}
