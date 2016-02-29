#include "grapher2d.h"

Grapher2D::Grapher2D(QWidget *parent)
    : QWidget(parent)
{
    setPalette(Qt::white);
    setAutoFillBackground(true);

    /// Априорная инициализация параметров системы координат
    setCSAngles();

    /// Априорная инициализация параметров координатных осей
    coordSystem.abs.shift = DEFABSSHIFT;
    coordSystem.ord.shift = DEFORDSHIFT;
    coordSystem.abs.measure = DEFABSMEASURE;
    coordSystem.ord.measure = DEFORDMEASURE;
    coordSystem.abs.expansion = DEFABSEXPAND;
    coordSystem.ord.expansion = DEFORDEXPAND;
    coordSystem.abs.values = true;
    coordSystem.ord.values = true;
    coordSystem.abs.dashLines = true;
    coordSystem.ord.dashLines = true;

    /// Априорная инициализация параметров зумирования
    coordSystem.zoom.value = DEFZOOMVALUE;
    coordSystem.zoom.min = DEFZOOMMIN;
    coordSystem.zoom.max = DEFZOOMMAX;
    coordSystem.zoom.step = DEFZOOMSTEP;
    coordSystem.zoom.center = false;

    /// Априорная инициализация типа курсора мыши
    mouse.cursorShape = Qt::OpenHandCursor;
}

Grapher2D::~Grapher2D()
{
}

void Grapher2D::mousePressEvent(QMouseEvent *_mEvent)
{
    if(!_mEvent)
        return;

    mouse.pos.click = _mEvent->pos();
}

void Grapher2D::mouseMoveEvent(QMouseEvent *_mEvent)
{
    if(!_mEvent)
        return;

    if(_mEvent->buttons() == Qt::LeftButton)
    {
        coordSystem.abs.shift += _mEvent->pos().x() - mouse.pos.click.x();
        coordSystem.ord.shift += _mEvent->pos().y() - mouse.pos.click.y();

        mouse.pos.click = _mEvent->pos();

        mouse.cursorShape = Qt::ClosedHandCursor;
        setCursor(Qt::CursorShape(mouse.cursorShape));
    }
    else
        mouse.pos.noClick = _mEvent->pos();

    repaint();
}

void Grapher2D::mouseReleaseEvent(QMouseEvent *_mEvent)
{
    if(!_mEvent)
        return;

    mouse.cursorShape = Qt::OpenHandCursor;
    setCursor(Qt::CursorShape(mouse.cursorShape));
}

void Grapher2D::wheelEvent(QWheelEvent *_wEvent)
{
    if(!_wEvent || !_wEvent->delta())
        return;

    if(!coordSystem.zoom.center)
    {
        coordSystem.abs.shift /= coordSystem.zoom.value;
        coordSystem.ord.shift /= coordSystem.zoom.value;
    }

    if(_wEvent->delta() > 0 &&
       (coordSystem.zoom.value + 10) * qPow(M_E, 0.1) - 10 <= coordSystem.zoom.max)
                coordSystem.zoom.value = (coordSystem.zoom.value + 10) * qPow(M_E, 0.1) - 10;
    else if(_wEvent->delta() < 0 &&
            (coordSystem.zoom.value + 10) / qPow(M_E, 0.05) - 10 >= coordSystem.zoom.min)
                coordSystem.zoom.value = (coordSystem.zoom.value + 10) / qPow(M_E, 0.05) - 10;

    if(!coordSystem.zoom.center)
    {
        coordSystem.abs.shift *= coordSystem.zoom.value;
        coordSystem.ord.shift *= coordSystem.zoom.value;
    }

    repaint();
}

void Grapher2D::paintEvent(QPaintEvent *_pEvent)
{
    if(!_pEvent)
        return;

    translocationCoordinateSystem();

    QPainter p(this);
    p.translate(width() / 2.0 + coordSystem.abs.shift, height() / 2.0 + coordSystem.ord.shift);

    QVector <QPoint> points;

    /// --------------------------------------------------

    /// Построение координатных осей
    p.save();
    p.setPen(QPen(Qt::black, 2));

    /// Построение линий координатный осей
    points.push_back(QPoint(- width() / 2 - coordSystem.abs.shift, 0));
    points.push_back(QPoint(width() / 2 - coordSystem.abs.shift, 0));
    points.push_back(QPoint(0, - height() / 2 - coordSystem.ord.shift));
    points.push_back(QPoint(0, height() / 2 - coordSystem.ord.shift));
    p.drawLine(points[0], points[1]);
    p.drawLine(points[2], points[3]);
    points.clear();

    /// Построение стрелок направлений координатных осей
    points.push_back(QPoint(0, - height() / 2 - coordSystem.ord.shift));
    points.push_back(QPoint(- 5, - height() / 2 + 5 - coordSystem.ord.shift));
    points.push_back(QPoint(+ 5,- height() / 2 + 5 - coordSystem.ord.shift));
    points.push_back(QPoint(width() / 2 - coordSystem.abs.shift, 0));
    points.push_back(QPoint(width() / 2 - 5 - coordSystem.abs.shift, - 5));
    points.push_back(QPoint(width() / 2 - 5 - coordSystem.abs.shift, + 5));
    p.drawLine(points[0], points[1]);
    p.drawLine(points[0], points[2]);
    p.drawLine(points[3], points[4]);
    p.drawLine(points[3], points[5]);
    points.clear();

    p.restore();
    p.drawText(-12, 15, QString::number(0));

    /// Ось абсцисс
    for(int step = 0, value = 0;
        step <= width() / 2 + qAbs(coordSystem.abs.shift);
        step += coordSystem.abs.expansion * coordSystem.zoom.value *
                (coordSystem.zoom.value < coordSystem.zoom.step ? 100 : coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1),
        value += coordSystem.zoom.value < coordSystem.zoom.step ? 100 : coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1)
    {
        /// Положительная
        if(coordSystem.angles[0] || coordSystem.angles[4])
        {
            /// Цифры
            if(coordSystem.abs.values && step != 0)
            {
                p.drawText(step - 9, 15, QString::number(value));
                p.drawText(step - 9, height() / 2 - coordSystem.ord.shift, QString::number(value));
            }

            /// Пунктирные линии
            if(coordSystem.abs.dashLines)
            {
                p.save();
                p.setPen(Qt::DashLine);
                points.push_back(QPoint(step, - height() / 2 - coordSystem.ord.shift));
                points.push_back(QPoint(step, height() / 2 - coordSystem.ord.shift));
                p.drawLine(points[0], points[1]);
                points.clear();
                p.restore();
            }

            /// Штрихи
            for(int dashStep = step,
                dashValue = 0,
                dashSize;
                dashStep < step + coordSystem.abs.expansion * coordSystem.zoom.value *
                                   (coordSystem.zoom.value < coordSystem.zoom.step ? 100 :
                                    coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1);
                dashStep += coordSystem.abs.expansion * coordSystem.zoom.value * (coordSystem.zoom.value != 1 ? 1 : 10),
                ++dashValue)
            {
                dashSize = dashValue % 10 == 0 ? 6 : dashValue % 5 == 0 ? 4 : 2;
                points.push_back(QPoint(dashStep, - dashSize));
                points.push_back(QPoint(dashStep, dashSize));
                p.drawLine(points[0], points[1]);
                points.clear();
            }
        }

        /// Отрицательная
        if(coordSystem.angles[1] || coordSystem.angles[2])
        {
            /// Цифры
            if(coordSystem.abs.values && step != 0)
            {
                p.drawText(- step - 15, 15, QString::number(- value));
                p.drawText(- step - 15, height() / 2 - coordSystem.ord.shift, QString::number(- value));
            }

            /// Пунктирные линии
            if(coordSystem.abs.dashLines)
            {
                p.save();
                p.setPen(Qt::DashLine);
                points.push_back(QPoint(- step, - height() / 2 - coordSystem.ord.shift));
                points.push_back(QPoint(- step, height() / 2 - coordSystem.ord.shift));
                p.drawLine(points[0], points[1]);
                points.clear();
                p.restore();
            }

            /// Штрихи
            for(int dashStep = step,
                dashValue = 0,
                dashSize;
                dashStep < step + coordSystem.abs.expansion * coordSystem.zoom.value *
                                   (coordSystem.zoom.value < coordSystem.zoom.step ? 100 :
                                    coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1);
                dashStep += coordSystem.abs.expansion * coordSystem.zoom.value * (coordSystem.zoom.value != 1 ? 1 : 10),
                ++dashValue)
            {
                dashSize = dashValue % 10 == 0 ? 6 : dashValue % 5 == 0 ? 4 : 2;
                points.push_back(QPoint(- dashStep, - dashSize));
                points.push_back(QPoint(- dashStep, dashSize));
                p.drawLine(points[0], points[1]);
                points.clear();
            }
        }
    }

    /// Ось ординат
    for(int step = 0, value = 0;
        step <= height() / 2 + qAbs(coordSystem.ord.shift);
        step += coordSystem.ord.expansion * coordSystem.zoom.value *
                (coordSystem.zoom.value < coordSystem.zoom.step ? 100 : coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1),
        value += coordSystem.zoom.value < coordSystem.zoom.step ? 100 : coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1)
    {
        /// Положительная
        if(coordSystem.angles[0] || coordSystem.angles[1])
        {
            /// Цифры
            if(coordSystem.ord.values && step != 0)
            {
                p.drawText(-24, - step + 15, QString::number(value));
                p.drawText(- width() / 2 - coordSystem.abs.shift, - step + 15, QString::number(value));
            }

            /// Пунктирные линии
            if(coordSystem.ord.dashLines)
            {
                p.save();
                p.setPen(Qt::DashLine);
                points.push_back(QPoint(- width() / 2 - coordSystem.abs.shift, - step));
                points.push_back(QPoint(width() / 2 - coordSystem.abs.shift, - step));
                p.drawLine(points[0], points[1]);
                points.clear();
                p.restore();
            }

            /// Штрихи
            for(int dashStep = step,
                dashValue = 0,
                dashSize;
                dashStep < step + coordSystem.ord.expansion * coordSystem.zoom.value *
                                   (coordSystem.zoom.value < coordSystem.zoom.step ? 100 :
                                    coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1);
                dashStep += coordSystem.abs.expansion * coordSystem.zoom.value * (coordSystem.zoom.value != 1 ? 1 : 10),
                ++dashValue)
            {
                dashSize = dashValue % 10 == 0 ? 6 : dashValue % 5 == 0 ? 4 : 2;
                points.push_back(QPoint(- dashSize, - dashStep));
                points.push_back(QPoint(dashSize, - dashStep));
                p.drawLine(points[0], points[1]);
                points.clear();
            }
        }

        /// Отрицательная
        if(coordSystem.angles[2] || coordSystem.angles[3])
        {
            /// Цифры
            if(coordSystem.ord.values && step != 0)
            {
                p.drawText(-30, step + 15, QString::number(-value));
                p.drawText(- width() / 2 - coordSystem.abs.shift, step + 15, QString::number(-value));
            }

            /// Пунктирные линии
            if(coordSystem.ord.dashLines)
            {
                p.save();
                p.setPen(Qt::DashLine);
                points.push_back(QPoint(- width() / 2 - coordSystem.abs.shift, step));
                points.push_back(QPoint(width() / 2 - coordSystem.abs.shift, step));
                p.drawLine(points[0], points[1]);
                points.clear();
                p.restore();
            }

            /// Штрихи
            for(int dashStep = step,
                dashValue = 0,
                dashSize;
                dashStep < step + coordSystem.ord.expansion * coordSystem.zoom.value *
                                   (coordSystem.zoom.value < coordSystem.zoom.step ? 100 :
                                    coordSystem.zoom.value < qPow(coordSystem.zoom.step, 3) ? 10 : 1);
                dashStep += coordSystem.abs.expansion * coordSystem.zoom.value * (coordSystem.zoom.value != 1 ? 1 : 10),
                ++dashValue)
            {
                dashSize = dashValue % 10 == 0 ? 6 : dashValue % 5 == 0 ? 4 : 2;
                points.push_back(QPoint(- dashSize, dashStep));
                points.push_back(QPoint(dashSize, dashStep));
                p.drawLine(points[0], points[1]);
                points.clear();
            }
        }
    }
    p.end();
}


/// ПАРАМЕТРЫ СИСТЕМЫ КООРДИНАТ

/// Установка отображаемых координатных углов
bool Grapher2D::setCSAngles(bool _first, bool _second, bool _third, bool _fourth)
{
    bool temp[4];
    temp[0] = _first;
    temp[1] = _second;
    temp[2] = _third;
    temp[3] = _fourth;

    int count = 0;
    for(int i = 0; i < 4; ++i)
        if(temp[i]) ++count;

    switch(count)
    {
    case 2:
        if((temp[0] && temp[2]) ||
           (temp[1] && temp[3]))
        {
            for(int i = 0; i < 4; ++i)
                coordSystem.angles[i] = true;
            return false;
        }
    case 4:
    case 1:
        for(int i = 0; i < 4; ++i)
            coordSystem.angles[i] = temp[i];
        return true;
    case 3:
    default:
        for(int i = 0; i < 4; ++i)
            coordSystem.angles[i] = true;
        return false;
    }
}


/// ПАРАМЕТРЫ ОСИ АБСЦИСС

/// Установка количества единиц измерения в одном делении оси абсцисс
bool Grapher2D::setCSAbsMeasure(float _measure)
{
    if(_measure < 1.0)
        return false;

    coordSystem.abs.measure = _measure;
    return true;
}

/// Установка параметра растяжения оси абсцисс
bool Grapher2D::setCSAbsExpansion(float _expansion)
{
    if(_expansion < 1.0)
        return false;

    coordSystem.abs.expansion = _expansion;
    return true;
}


/// ПАРАМЕТРЫ ОСИ ОРДИНАТ

/// Установка количества единиц измерения в одном делении оси ординат
bool Grapher2D::setCSOrdMeasure(float _measure)
{
    if(_measure < 1.0)
        return false;

    coordSystem.ord.measure = _measure;
    return true;
}

/// Установка параметра растяжения оси ординат
bool Grapher2D::setCSOrdExpansion(float _expansion)
{
    if(_expansion < 1.0)
        return false;

    coordSystem.ord.expansion = _expansion;
    return true;
}


/// ПАРАМЕТРЫ ЗУМИРОВАНИЯ

/// Установка количества пикселей в одном делении
bool Grapher2D::setCSZoom(int _zoom)
{
    if(_zoom < coordSystem.zoom.min || _zoom > coordSystem.zoom.max)
        return false;

    coordSystem.zoom.value = _zoom;

    return true;
}

/// Установка нижней границы зумирования
bool Grapher2D::setCSZoomMin(int _zoomMin)
{
    if(_zoomMin < 1 || _zoomMin > 100 || _zoomMin > coordSystem.zoom.max)
        return false;

    coordSystem.zoom.min = _zoomMin;

    return true;
}

/// Установка верхней границы зумирования
bool Grapher2D::setCSZoomMax(int _zoomMax)
{
    if(_zoomMax < 10 || _zoomMax > 1000 || _zoomMax < coordSystem.zoom.min)
        return false;

    coordSystem.zoom.max = _zoomMax;

    return true;
}

/// Установка шага изменения зума
bool Grapher2D::setCSZoomStep(int _zoomStep)
{
    if(_zoomStep < 1 || _zoomStep > 10)
        return false;

    coordSystem.zoom.step = _zoomStep;

    return true;
}

/// Смещение системы координат
void Grapher2D::translocationCoordinateSystem()
{
    if(!coordSystem.angles[0] && !coordSystem.angles[1] &&
       coordSystem.ord.shift > - height() / 2.0)
            coordSystem.ord.shift = - height() / 2.0;
    if(!coordSystem.angles[1] && !coordSystem.angles[2] &&
       coordSystem.abs.shift > - width() / 2.0)
            coordSystem.abs.shift = - width() / 2.0;
    if(!coordSystem.angles[2] && !coordSystem.angles[3] &&
       coordSystem.ord.shift < height() / 2.0)
            coordSystem.ord.shift = height() / 2.0;
    if(!coordSystem.angles[0] && !coordSystem.angles[3] &&
       coordSystem.abs.shift < width() / 2.0)
            coordSystem.abs.shift = width() / 2.0;
}
