#include "glwheeloffortune.h"

#include <windows.h> // TODO: add commentary why it's important in here and readme file
#include <GL/gl.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>

#include <cmath>
// Provides maximal possible precision
const long double g_PI = acos(-1.0L);

GlWheelOfFortune::GlWheelOfFortune()
{
}

void GlWheelOfFortune::configureCanvas()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void GlWheelOfFortune::drawIcon()
{
    // TODO:
    // glBindTexture( GL_TEXTURE_2D, texture );
    // https://community.khronos.org/t/loading-a-bitmap-image-to-use-it-as-a-texture-background-on-canvas-for-drawing/72323/8
    
    // glTexImage2D();
    // https://learnopengl.com/Getting-started/Textures
}

void GlWheelOfFortune::drawBoardContour()
{
    glLineWidth(10); // TODO: scaling with text
    glColor3f(0.f, 0.f, 0.f);
    glColor3f(128.f / 255.f, 128.f / 255.f, 0.f);
    drawCircleShape(0.f, 0.f, 0.95f);
    glColor3f(0.f, 0.f, 0.f);
    drawCircleShape(0.f, 0.f, 0.95f, false, true, 3);
}

void GlWheelOfFortune::drawBoardCenter()
{
    glLineWidth(3); // TODO: scaling with text
    glColor3f(128.f / 255.f, 128.f / 255.f, 0.0);
    drawCircleShape(0.f, 0.f, 0.105f, true, false); // background circle
    glColor3f(0.f, 0.f, 0.f);
    drawCircleShape(0.f, 0.f, 0.1f, false, true); // ornament inner circle
    drawCircleShape(0.f, 0.f, 0.06f, true, false); // middle filled circle
}

void GlWheelOfFortune::drawPointer()
{
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.f, 0.03f, -1.f);
        glVertex3f(-0.825f, 0.f, -1.f);
        glVertex3f(-1.f, -0.03f, -1.f);
    glEnd();
    glLineWidth(2);
    glColor3f(128.f / 255.f, 128.f / 255.f, 0.f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-1.f, 0.03f, -1.f);
        glVertex3f(-0.825f, 0.f, -1.f);
        glVertex3f(-1.f, -0.03f, -1.f);
    glEnd();
    glFlush();
}

extern const Color g_HighlightRimColor; // Example usage of extern keyword, for demonstration purposes
void GlWheelOfFortune::drawPiePiece(const string& text, const Color& color, float rotationAngle, bool highlight)
{
    const ColorF& pieColor = color.getFloat();
    glColor4f(pieColor.m_R, pieColor.m_G, pieColor.m_B, pieColor.m_A);
    drawPieShape(0.95f, m_PieAngle, rotationAngle);
    if (highlight)
    {
        glColor4f(g_HighlightRimColor.getFloat().m_R, g_HighlightRimColor.getFloat().m_G, g_HighlightRimColor.getFloat().m_B, g_HighlightRimColor.getFloat().m_A);
        drawPieShape(0.936f, m_PieAngle, rotationAngle, true);
    }
    drawLabel(0.9f, rotationAngle, text, highlight ? 2.f : 1.75f, highlight);
}

void GlWheelOfFortune::drawCircleShape(float cx, float cy, float radius, bool fill, bool dashed, int lineStripFactor) 
{
    int numSegments = 360;
    float theta = g_PI * 2.0f / float(numSegments);
    float tangentialFactor = tanf(theta);
    float radialFactor = cosf(theta);
    GLfloat x = radius;
    GLfloat y = .0f;

    glPushMatrix();
    glPushAttrib(GL_LINE_BIT | GL_LINE_SMOOTH);
    if (dashed)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(lineStripFactor, 0x00FF);
    }
        glEnable(GL_LINE_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glRotatef(-1.f * m_Rotation, 0.f, 0.f, 1.f);
    if (fill)
        glBegin(GL_POLYGON);
    else
        glBegin(GL_LINE_LOOP);

        for (int ii = 0; ii < numSegments; ii++)
        {
            glVertex2f(x + cx, y + cy);

            GLfloat tx = -y;
            GLfloat ty = x;

            x += tx * tangentialFactor;
            y += ty * tangentialFactor;

            x *= radialFactor;
            y *= radialFactor;
        }
        glEnd();
        glFlush();
    glPopAttrib();
    glPopMatrix();
}

void GlWheelOfFortune::drawPieShape(float radius, float angle, float rotation, bool drawJustRim)
{
    GLfloat pointAngle = .0f;
    GLfloat wheelFactor = 360.0f / angle;
    GLfloat pointX = -radius, pointY = radius;
    glPushMatrix();
        glRotatef(-rotation, 0.f, 0.f, 1.f);
    if (drawJustRim)
    {
        glLineWidth(4);
        glBegin(GL_LINE_LOOP);
    }
    else
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.f, 0.f);
    // for (int i = std::floorf(-angle / 2.f); i < std::ceilf(angle / 2.f); i++) // TODO: Adjust that part to fit all input files without overlapping and cracks between pieces
        for (int i = std::floorf(-angle / 2.f); i <= std::ceilf(angle / 2.f); i++)
        {
            pointAngle = 2.f * g_PI * i / angle / wheelFactor;
            glVertex2f(cos(pointAngle) * pointX, sin(pointAngle) *  pointY);
        } // NOTE: Needs more work. Pie pieces are a little bit in collision
        glEnd();
        glFlush();
    glPopMatrix();
}

void GlWheelOfFortune::drawLabel(float radiusRatio, float rotationAngle, const string& name, float lineWidth, bool highlight)
{
    // TODO1: text scaling
    // TODO2: New way of text rendering
    // https://learnopengl.com/In-Practice/Text-Rendering
    // https://www.youtube.com/watch?v=QRw3nMffPDk&list=PLysLvOneEETPlOI_PI4mJnocqIpr2cSHS&index=34
    
    GLfloat r = min( (GLfloat)m_Size.m_Width / 2.f, (GLfloat)m_Size.m_Height / 2.f ) * 0.9f; /// I'd like to shift text anchor a little from contour

    const unsigned char* testText = reinterpret_cast<const unsigned char*>(name.c_str());
    GLfloat textWidth = glutStrokeLengthf(GLUT_STROKE_ROMAN, testText);
    GLfloat textHeight = glutStrokeHeight(GLUT_STROKE_ROMAN);

    GLfloat scaleFactor;
    if (!highlight)
        scaleFactor = 0.15f;
    else
        scaleFactor = 0.2f;

    glPushMatrix();
    glPushAttrib(GL_LINE_BIT | GL_LINE_SMOOTH);
        glLoadIdentity();
        gluOrtho2D(0.0, m_Size.m_Width, 0.0, m_Size.m_Height);
        glTranslatef((GLfloat)m_Size.m_Width / 2.f, (GLfloat)m_Size.m_Height / 2.f, 0.f); /// set middle of canvas
        glColor3f(.0f, .0f, .0f);
        glRotatef(-1.f * rotationAngle, 0.f, 0.f, 1.f); // set rotation angle
        glTranslatef(0.f, -1.f * scaleFactor * textHeight / 2.f, 0.f); // adjust text anchor
        glTranslatef(-1.f * r * radiusRatio, 0.f, 0.f); // shift to contour side
        glScalef(scaleFactor, scaleFactor, 1.f); // use textWidth i textHeight to calculate scale
        //glDisable(GL_LINE_SMOOTH);
        glLineWidth(lineWidth);
        glutStrokeString(GLUT_STROKE_ROMAN, testText);
        glFlush();
    glPopAttrib();
    glPopMatrix();
}
