#include "glwheeloffortune.h"

#include <cmath>
#include <windows.h> // TODO: komentaz dlaczego ten include jest wazny
#include <GL/gl.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>

const float g_PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062f;

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

void GlWheelOfFortune::drawBoardConture()
{
    glLineWidth(10); // TODO: skalowanie razem z tekstem
    glColor3f(0.f, 0.f, 0.f);
    glColor3f(128.f / 255.f, 128.f / 255.f, 0.f);
    drawCircleShape(0.f, 0.f, 0.95f);
    glColor3f(0.f, 0.f, 0.f);
    drawCircleShape(0.f, 0.f, 0.95f, false, true, 3);
}

void GlWheelOfFortune::drawBoardCenter()
{
    glLineWidth(3); // TODO: skalowanie razem z tekstem
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

void GlWheelOfFortune::drawPiePiece(const string& text, const Color& color, float rotationAngle)
{
    const ColorF& pieColor = color.getFloat();
    glColor4f(pieColor.m_R, pieColor.m_G, pieColor.m_B, pieColor.m_A);
    drawPieShape(0.95f, m_PieAngle, rotationAngle);
    drawLabel(0.9f, 0.05f, rotationAngle, text, 1.75f);
}

void GlWheelOfFortune::drawCircleShape(float cx, float cy, float radius, bool fill, bool dashed, int lineStripFactor) 
{
    int numSegments = 360;
    float theta = g_PI * 2.0f / float(numSegments);
    float tangentialFactor = tanf(theta);
    float radialFactor = cosf(theta);
    float x = radius;
    float y = .0f;

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

            float tx = -y;
            float ty = x;

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

void GlWheelOfFortune::drawPieShape(float radius, float angle, float rotation)
{
    GLfloat pointAngle = .0f;
    GLfloat wheelFactor = 360.0f / angle;
    GLfloat arcPoints = angle; /// For demonstration purpose we can assume one point for one angle of arc will by enough.
    GLfloat pointX = -radius, pointY = radius;
    glPushMatrix();
        glRotatef(-rotation, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.f, 0.f);
        for (int i = std::floorf(-arcPoints / 2.f); i <= std::ceilf(arcPoints / 2.f); i++)
        {
            pointAngle = 2.f * g_PI * i / angle / wheelFactor;
            glVertex2f(cos(pointAngle) * pointX, sin(pointAngle) *  pointY);
        }
        glEnd();
        glFlush();
    glPopMatrix();
}

void GlWheelOfFortune::drawLabel(float x, float y, float angle, const string& name, float lineWidth) // TODO: wysrodkowanie od krawedzi do wewnetrznej dekoracji
{
    // TODO: Zmiana wielkosci tekstu progowo w zaleznosci od rozdzielczosci okna

    // TODO2: Nowy sposob renderowania tekstu (jako obiekty 3D lub z tekstury)
    // https://learnopengl.com/In-Practice/Text-Rendering
    // https://www.youtube.com/watch?v=QRw3nMffPDk&list=PLysLvOneEETPlOI_PI4mJnocqIpr2cSHS&index=34
    
    GLfloat r = min( (GLfloat)m_Size.m_Width / 2.f, (GLfloat)m_Size.m_Height / 2.f ) * 0.9f; /// Chcemy odsunac troche kotwice napisu od brzegu (stad mnoznik 0,9) TODO: na ang
    // To jest skrotowo, bo zaklada, ze w jest rowne h
    const unsigned char* testText = reinterpret_cast<const unsigned char*>(name.c_str());
    GLfloat textWidth = glutStrokeLengthf(GLUT_STROKE_ROMAN, testText);
    GLfloat textHeight = glutStrokeHeight(GLUT_STROKE_ROMAN);


    glPushMatrix();
    glPushAttrib(GL_LINE_BIT | GL_LINE_SMOOTH);
        glLoadIdentity();
        gluOrtho2D(0.0, m_Size.m_Width, 0.0, m_Size.m_Height);
        glTranslatef((GLfloat)m_Size.m_Width / 2.f, (GLfloat)m_Size.m_Height / 2.f /* + textHeight * 0.15f / 2.f */, 0.f);
        glColor3f(.0f, .0f, .0f);
        glRotatef(-1.f * angle, 0.f, 0.f, 1.f);
        glTranslatef(-1.f * r * x, -1.f * r * y, 0.f);
        glScalef(0.15f, 0.15f, 1.f); // uzyc textWidth i textHeight do okreslenia skali
        //glDisable(GL_LINE_SMOOTH);
        glLineWidth(lineWidth);
        glutStrokeString(GLUT_STROKE_ROMAN, testText);
        glFlush();
    glPopAttrib();
    glPopMatrix();
}
