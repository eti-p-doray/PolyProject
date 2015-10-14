#include "glview.h"

static const char *vertexShaderSource =
        "attribute highp vec4 position;\n"
        "attribute lowp vec4 color;\n"
        "varying lowp vec4 v_color;\n"
        "uniform highp mat4 transform;\n"
        "void main() {\n"
        "   v_color = color;\n"
        "   gl_Position = transform * position;\n"
        "}\n";


//"gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);"
static const char *fragmentShaderSource =
        "varying lowp vec4 v_color;\n"
        "void main() {\n"
        "   gl_FragColor = v_color;\n"
        "}\n";

GlView::GlView(QWidget * parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);

    model_ = nullptr;
}

GlView::~GlView()
{
    glDeleteBuffers(1, &verticesBuffer_);
    glDeleteBuffers(1, &colorsBuffer_);
    glDeleteBuffers(1, &elementsBuffer_);
    delete m_program;

}

QSize GlView::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GlView::sizeHint() const
{
    return QSize(800, 600);
}

void GlView::initializeGL()
{
    initializeOpenGLFunctions();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_position = m_program->attributeLocation("position");
    m_color = m_program->attributeLocation("color");
    m_transform = m_program->uniformLocation("transform");

    GLfloat vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
      };

    GLfloat colors[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };

    GLushort elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 0, 3,
        3, 7, 4,
        // left
        4, 5, 1,
        1, 0, 4,
        // right
        3, 2, 6,
        6, 7, 3,
    };

    glGenBuffers(1, &verticesBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(m_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &colorsBuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(m_color, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &elementsBuffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void GlView::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    HandBasis basis;
    QQuaternion orientation;
    if (model_ != nullptr) {
        basis = model_->getBasis();
        orientation = model_->getImu();
    }


    glEnableVertexAttribArray(m_position);
    glEnableVertexAttribArray(m_color);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer_);

    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            QMatrix4x4 matrix;
            matrix.perspective(60.0f, float(width())/float(height()), 0.1f, 1000.0f);
            matrix.translate(0, -2, -5);
            matrix.scale(0.01, 0.01, 0.01);

            matrix.translate(basis.fingers[i].joints[j].position);
            matrix *= basis.fingers[i].joints[j].orientation.transposed();
            matrix.scale(basis.fingers[i].joints[j].scale);

            m_program->setUniformValue(m_transform, matrix);
            glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
        }
    }

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, float(width())/float(height()), 0.1f, 1000.0f);
    matrix.translate(0, -2, -5);
    matrix.scale(0.01, 0.01, 0.01);

    matrix.translate(basis.palm.position);
    matrix *= basis.palm.orientation.transposed();
    matrix.scale(basis.palm.scale);

    m_program->setUniformValue(m_transform, matrix);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    matrix = QMatrix4x4();
    matrix.perspective(60.0f, float(width())/float(height()), 0.1f, 1000.0f);
    matrix.translate(0, 0, -8);
    matrix.scale(0.5, 0.5, 0.5);

    matrix.rotate(orientation);

    m_program->setUniformValue(m_transform, matrix);
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


    glDisableVertexAttribArray(m_color);
    glDisableVertexAttribArray(m_position);

    m_program->release();

}

void GlView::resizeGL(int width, int height)
{
    const qreal retinaScale = devicePixelRatio();
    glViewport( 0, 0, (GLint)width * retinaScale, (GLint)height * retinaScale );
}
