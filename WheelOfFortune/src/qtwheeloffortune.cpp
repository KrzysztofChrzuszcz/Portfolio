#include "qtwheeloffortune.h"
#include <QPen>
#include <QRect>
#include <QPolygonF>
#include <QPointF>
#include <QFont>
#include <QAction>
#include <QIcon>
#include <QOpenGLWidget>

QtWheelOfFortune::QtWheelOfFortune(QPainter& painter) :
	m_Painter(painter)
{
}

void QtWheelOfFortune::configureCanvas()
{
	m_Painter.setViewport(QRect(0, 0, m_Size.m_Width, m_Size.m_Height));
	m_Painter.setWindow(QRect(0, 0, m_Size.m_Width, m_Size.m_Height));

	m_Painter.setRenderHint(QPainter::Antialiasing);
	QPointF center(m_Size.m_Width / 2.0, m_Size.m_Height / 2.0);
	m_Painter.translate(center);
	m_Painter.scale(m_Size.m_Width, m_Size.m_Height);
}

void QtWheelOfFortune::drawIcon()
{
	QIcon* icon = new QIcon(":/images/logo.png");
	QRect rectangle(0, 0, m_Size.m_Width, m_Size.m_Height);
	icon->paint(&m_Painter, rectangle);
}

void QtWheelOfFortune::drawBoardConture()
{
	/// Qt instead floats prefers qreal format, which is based on double
	/// So in case of that class there is no need to use .f suffix
	QRectF rectangle(-0.45, -0.45, 0.9, 0.9);

	m_Painter.save();
	m_Painter.rotate(m_Rotation);
	// background circle
	QPen bottomPen(Qt::black, 0.02);
	m_Painter.setPen(bottomPen);
	m_Painter.drawArc(rectangle, 0, 360 * 16);
	// ornament circle
	QPen topPen(Qt::darkYellow, 0.02, Qt::DotLine);
	m_Painter.setPen(topPen);
	m_Painter.drawArc(rectangle, 0, 360 * 16);
	m_Painter.restore();
}

void QtWheelOfFortune::drawBoardCenter()
{
	m_Painter.save();
	m_Painter.rotate(m_Rotation);

	QRectF innerRectangle(-0.025, -0.025, 0.05, 0.05);
	QRectF outerRectangle(-0.05, -0.05, 0.10, 0.10);

	// filled circle in the back (background color)
	QPen backgroundPen(Qt::darkYellow, 0.005);
	m_Painter.setPen(backgroundPen);
	m_Painter.setBrush(Qt::darkYellow);
	m_Painter.drawEllipse(outerRectangle);

	// middle filled circle
	QPen innerPen(Qt::black, 0.01);
	m_Painter.setPen(innerPen);
	m_Painter.setBrush(Qt::black);
	m_Painter.drawEllipse(innerRectangle);

	// Ornament outer circle
	QPen outerPen(Qt::black, 0.005, Qt::DotLine, Qt::RoundCap);
	m_Painter.setPen(outerPen);
	m_Painter.drawArc(outerRectangle, 0, 360 * 16);
	m_Painter.restore();
}

void QtWheelOfFortune::drawPointer()
{
	QPen pen(Qt::darkYellow, 0.0025);
	m_Painter.setPen(pen);
	m_Painter.setBrush(Qt::black);
	QPolygonF polygonPoints;
	polygonPoints << QPointF(-0.5, 0.015) << QPointF(-0.4, 0) << QPointF(-0.5, -0.015);
	m_Painter.drawPolygon(polygonPoints);
}

void QtWheelOfFortune::drawPiePiece(const QString& text, const QColor& color, float rotationAngle)
{
	// draw pie piece
	m_Painter.save();
	m_Painter.rotate(rotationAngle);
	QRectF rectangle(-0.45, -0.45, 0.9, 0.9);
	m_Painter.setPen(Qt::NoPen);
	m_Painter.setBrush(color);
	m_Painter.drawPie(rectangle, (180 - m_PieAngle / 2.0f) * 16, m_PieAngle * 16);
	m_Painter.restore();

	// TODO: Solve issue with FONT size while scale is in range -0.5:0.5
	// draw label
	m_Painter.save();
	m_Painter.rotate(rotationAngle);
	m_Painter.scale(1.0 / m_Size.m_Width, 1.0 / m_Size.m_Height);
	m_Painter.setPen(Qt::black);
	QFont font = m_Painter.font();
	font.setPixelSize(22);
	m_Painter.setFont(font);
	QRectF rectangle2(m_Size.m_Width / -2.0 + m_Size.m_Width / 8.0, -12, m_Size.m_Width, m_Size.m_Height);
	QRectF boundingRect;
	m_Painter.drawText(rectangle2, 0, text, &boundingRect);
	m_Painter.restore();
}

void QtWheelOfFortune::drawPiePiece(const string& text, const Color& color, float rotationAngle)
{
	drawPiePiece(QString(text.c_str()), color.getQColor(), rotationAngle);
}