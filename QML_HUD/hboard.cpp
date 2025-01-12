#include "hboard.h"
#include <tuple>

HorizontalBoard::HorizontalBoard(QQuickItem* parent)
    : AbstractControl(parent)
{
    // Defaults
    m_TrackToDialSpacing = 5;
    m_Diverted = false;
    //m_DialType = QString("none");
    m_DialType = QString("bottom"); // Possible variants: full | bottom | upper | none (or anything)
    m_Scale = 0.9;
}

void HorizontalBoard::setDiverted(bool isDiverted)
{
    if (m_Diverted == isDiverted)
        return;
    m_Diverted = isDiverted;
    emit divertedChanged();
    update();
}

void HorizontalBoard::setDialType(const QString& dialType)
{
    if (m_DialType == dialType)
        return;
    m_DialType = dialType;
    emit dialTypeChanged();
    update();
}

void inline paintWing(QPainter* painter, const QColor& color, qreal initialAlpha, const QPointF (&backgroundPoints)[4], const std::tuple<QPointF, QPointF>& normalVector)
{
    // TODO
}

void HorizontalBoard::paintBacklight(QPainter* painter)
{
    int offset = 10;
    const QPointF indicatorPoints[4] = {
      QPointF(1.0 * m_Size / 8.0,                                   m_Size / 2 + offset),
      QPointF(1.0 * m_Size / 8.0 + 6.0 * m_Value * m_Size / 8.0 ,   m_Size / 2 + offset),
      QPointF(0.0 * m_Size / 8.0 + 8.0 * m_Value * m_Size / 8.0 ,   4 * m_Size / 7 + offset),
      QPointF(0.0 * m_Size / 8.0,                                   4 * m_Size / 7 + offset)
    };

    painter->save();
    QColor blacklight = m_BacklightColor;
    blacklight.setAlpha(120);
    painter->setBrush(blacklight);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(indicatorPoints, 4);
    painter->restore();
}

void HorizontalBoard::paintBackground(QPainter* painter)
{
    int offset = 10;
    static const QPointF backgroundPoints[4] = {
      QPointF(1.0 * m_Size / 8.0, m_Size / 2 + offset),
      QPointF(7.0 * m_Size / 8.0, m_Size / 2 + offset),
      QPointF(8.0 * m_Size / 8.0, 4 * m_Size / 7 + offset),
      QPointF(0.0 * m_Size / 8.0, 4 * m_Size / 7 + offset)
    };

    QColor TransparentBackgoundColor = m_BackgroundColor;
    TransparentBackgoundColor.setAlphaF(0.6);
    QLinearGradient gradient(QPointF(m_Size / 2.0, m_Size / 2), QPointF(m_Size / 2.0, 3 * m_Size / 4));
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(0.8, TransparentBackgoundColor);
    painter->save();
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(backgroundPoints, 4);
    painter->restore();
}

void HorizontalBoard::paintTrack(QPainter* painter)
{
    static const QPointF trackPoints[4] = {
      QPointF(1.0 * m_Size / 8.0, m_Size / 2),
      QPointF(7.0 * m_Size / 8.0, m_Size / 2),
      QPointF(8.0 * m_Size / 8.0, 4 * m_Size / 7),
      QPointF(0.0 * m_Size / 8.0, 4 * m_Size / 7)
    };
    QColor TransparentBackgoundColor = m_NonActiveColor;
    TransparentBackgoundColor.setAlphaF(0.6);

    painter->save();
    painter->setBrush(TransparentBackgoundColor);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(trackPoints, 4);
    painter->restore();
}

void HorizontalBoard::paintIndicator(QPainter* painter)
{
    const QPointF indicatorPoints[4] = {
      QPointF(1.0 * m_Size / 8.0,                                   m_Size / 2),
      QPointF(1.0 * m_Size / 8.0 + 6.0 * m_Value * m_Size / 8.0 ,   m_Size / 2),
      QPointF(0.0 * m_Size / 8.0 + 8.0 * m_Value * m_Size / 8.0 ,   4 * m_Size / 7),
      QPointF(0.0 * m_Size / 8.0,                                   4 * m_Size / 7)
    };

    painter->save();
    painter->setBrush(m_ActiveColor);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(indicatorPoints, 4);
    painter->restore();
}

static const QString g_FullType("full");
static const QString g_Bottom("bottom");
static const QString g_Upper("upper");
void HorizontalBoard::paintDial(QPainter* painter)
{
    int lineLength = 5;
    int labelOffset = 10;
    for (int i = 0; i <= m_MaxValue; i++) // TODO: use m_MinValue
        if (i % 5 == 0) // TODO: Parameter for 5
        {
            qreal currentXfactor = i / m_MaxValue;
            if (i == 0 || i == m_MaxValue)
            {
                painter->setPen(QPen(QBrush(m_DialColor), 2)); // NOTE: For some reason in case of width equals 2 upper line looks like with width 1 but for higher values issue is not present. TODO: Ask CodeRabbitAI what It thinks about it.
                lineLength = 7;
            }
            else
            {
                painter->setPen(m_DialColor);
                lineLength = 5;
            }

            QPointF bottomPoint = QPointF(1.0 * m_Size / 8.0 + 6.0 * currentXfactor * m_Size / 8.0, m_Size / 2);
            QPointF topPoint = QPointF(1.0 * m_Size / 8.0 + 6.0 * currentXfactor * m_Size / 8.0, m_Size / 2 - lineLength);
            painter->save();
            // Draw upper dial
            if (m_DialType == g_FullType || m_DialType == g_Upper)
                painter->drawLine(bottomPoint, topPoint);

            // Draw basic laying dial
            QPointF frontPoint = QPointF(0.0 * m_Size / 8.0 + 8.0 * currentXfactor * m_Size / 8.0, 4 * m_Size / 7);
            painter->drawLine(bottomPoint, frontPoint);
            
            // Draw bottom dial
            QPointF frontBottomPoint = QPointF(0.0 * m_Size / 8.0 + 8.0 * currentXfactor * m_Size / 8.0, 4 * m_Size / 7 + lineLength);
            if (m_DialType == g_FullType || m_DialType == g_Bottom)
                painter->drawLine(frontPoint, frontBottomPoint);

            // Draw front points
            painter->setBrush(m_DialColor);
            painter->drawEllipse(frontPoint, 1, 1);
            painter->restore();

            if (i == 0 || i == m_MaxValue) // TODO: Consider common approach for bold markers for given step instead only for first and last
            {
                painter->save();
                QString label = QString::number(i);
                QFont font = painter->font();
                font.setPointSize(m_DialFontSize);

                QFontMetrics fm(font);
                int textWidth = fm.width(label);
                QPointF labelPoint = QPoint(frontBottomPoint.x() - textWidth / 2, frontBottomPoint.y() + fm.height());
                if (m_DialType == g_FullType || m_DialType == g_Upper)
                    labelPoint = QPointF(1.0 * m_Size / 8.0 + 6.0 * currentXfactor * m_Size / 8.0 - textWidth / 2, m_Size / 2 - labelOffset);
                if (m_DialType == g_Bottom)
                    labelPoint = QPointF(frontBottomPoint.x() - textWidth / 2, frontBottomPoint.y() + (lineLength - labelOffset) + fm.height());

                painter->drawText(labelPoint, label);
                painter->restore();
            }
        }
}
