#ifndef GLVIEW_H
#define GLVIEW_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

#include <iostream>

#include <QtOpenGL>

#include "model.h"

class GlView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlView(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~GlView();

    void setModel(Model * model) {
        model_ = model;
    }

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    QOpenGLShaderProgram* m_program;

    GLuint m_transform;
    GLuint m_position;
    GLuint m_color;

    GLuint verticesBuffer_;
    GLuint colorsBuffer_;
    GLuint elementsBuffer_;

    Model * model_;
};

#endif // GLVIEW_H
