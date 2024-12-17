#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" /// Image-loading library that supports several popular formats.
#include "glwheeloffortune.h"

#include <windows.h> // TODO: add commentary why it's important in here and readme file
#include <GL/freeglut.h> /// Used for glutStrokeString
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
    glDisable(GL_TEXTURE_2D);
}

/**
 * \brief GlWheelOfFortune::drawIcon() displays application logo before selecting any data to visualize.
 * \remark Used method is very basic because I've mentioned to keep it 2D and as simple and clear as possible.
 */
void GlWheelOfFortune::drawIcon()
{
    // Below trick works only because there is only one instance of this class in application by design.
    // Suits demonstration purpose of internal static variable inside a function.
    // (It's rather curiosity then recommended practice).
    static bool initialized = false; 
    static int width;
    static int height;
    if (!initialized) 
    {
        //unsigned int texture;
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        int nrChannels;
        // unsigned char* data = stbi_load("M:/Repo/Portfolio/WheelOfFortune/resources/images/logo.bmp", &width, &height, &nrChannels, 0);
        // unsigned char* data = stbi_load("M:/Repo/Portfolio/WheelOfFortune/resources/images/logo.jpg", &width, &height, &nrChannels, 0);
        unsigned char* data = stbi_load("M:/Repo/Portfolio/WheelOfFortune/resources/images/logo.png", &width, &height, &nrChannels, 0); // Reminder: above lines left for test cases of library loading (could by used in gTest example)
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // TODO: GL_RGBA
        }
        else
        {
            // std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2d(1.0, 1.0); glVertex2d(-0.9, -0.9);
        glTexCoord2d(0.0, 1.0); glVertex2d(0.9, -0.9);
        glTexCoord2d(0.0, 0.0); glVertex2d(0.9, 0.9);
        glTexCoord2d(1.0, 0.0); glVertex2d(-0.9, 0.9);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

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
    float theta = g_PI * 2.0f / static_cast<float>(numSegments);
    GLfloat tangentialFactor = tanf(theta);
    GLfloat radialFactor = cosf(theta);
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
    GLfloat pointX = -radius, pointY = radius;
    GLfloat startAngle = -angle / 2.0f;
    GLfloat endAngle = angle / 2.0f;
    GLfloat step = angle / (360.0f * radius);

    glPushMatrix();
    glRotatef(-rotation, 0.f, 0.f, 1.f);

    if (drawJustRim)
    {
        glLineWidth(4);
        glBegin(GL_LINE_LOOP);
    }
    else
    {
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.f, 0.f);
    }

    for (GLfloat currentAngle = startAngle; currentAngle <= endAngle; currentAngle += step)
    {
        GLfloat radianAngle = g_PI * currentAngle / 180.0f;
        GLfloat x = cos(radianAngle) * pointX;
        GLfloat y = sin(radianAngle) * pointY;
            glVertex2f(x, y);
    }

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
