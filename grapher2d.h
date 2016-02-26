#ifndef GRAPHER2D_H
#define GRAPHER2D_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <qmath.h>

class Grapher2D : public QWidget
{
    Q_OBJECT

    static const float DEFABSSHIFT      = 1.0;      /// Сдвиг по оси абсцисс по умолчанию
    static const float DEFORDSHIFT      = 1.0;      /// Сдвиг по оси ординат по умолчанию
    static const float DEFABSMEASURE    = 1.0;      /// Количество единиц измерения в одном делении оси абсцисс по умолчанию
    static const float DEFORDMEASURE    = 1.0;      /// Количество единиц измерения в одном делении оси ординат по умолчанию
    static const float DEFABSEXPAND     = 1.0;      /// Растяжение оси абсцисс по умолчанию
    static const float DEFORDEXPAND     = 1.0;      /// Растяжение оси ординат по умолчанию

    static const int DEFZOOMVALUE       = 10;       /// Количество пикселей в одном делении по умолчанию
    static const int DEFZOOMMIN         = 1;        /// Нижняя граница зумирования по умолчанию
    static const int DEFZOOMMAX         = 1000;     /// Верхняя граница зумирования по умолчанию
    static const int DEFZOOMSTEP        = 4;        /// Шаг изменения зумирования по умолчанию

public:
    Grapher2D(QWidget *parent = 0);
    ~Grapher2D();

protected:
    virtual void mousePressEvent(QMouseEvent * _mEvent);
    virtual void mouseMoveEvent(QMouseEvent * _mEvent);
    virtual void mouseReleaseEvent(QMouseEvent * _mEvent);
    virtual void wheelEvent(QWheelEvent * _wEvent);
    virtual void paintEvent(QPaintEvent * _pEvent);

    /// Параметры системы координат

    /// Отображаемые координатные углы
    bool setCSAngles(bool _first = true, bool _second = true,
                     bool _third = true, bool _fourth = true);
    inline bool *getCSAngles();

    /// Смещение системы координат
    inline float getCSAbsTranslate();
    inline float getCSOrdTranslate();

    /// Масштаб системы координат
    inline float getCSAbsScale();
    inline float getCSOrdScale();

    /// Параметры координатных осей

    /// Сдвиг по оси абсцисс
    inline void setCSAbsShift(float _shift = DEFABSSHIFT);
    inline float getCSAbsShift();

    /// Сдвиг по оси ординат
    inline void setCSOrdShift(float _shift = DEFORDSHIFT);
    inline float getCSOrdShift();

    /// Количество единиц измерения в одном делении
    bool setCSAxisMeasure(float _abs = DEFABSMEASURE, float _ord = DEFORDMEASURE);
    inline float getCSAbsMeasure();
    inline float getCSOrdMeasure();

    /// Растяжение осей
    bool setCSAxisExpansion(float _abs = DEFABSEXPAND, float _ord = DEFORDEXPAND);
    inline float getCSAbsExpansion();
    inline float getCSOrdExpansion();

    /// Отображение цифровых значений
    void setCSAxisValues(bool _abs = true, bool _ord = true);
    inline bool isCSAbsValues();
    inline bool isCSOrdValues();

    /// Отображение пунктирных линий
    void setCSAxisDashLines(bool _abs = true, bool _ord = true);
    inline bool isCSAbsDashLines();
    inline bool isCSOrdDashLines();

    /// Параметры зумирования

    /// Количество пикселей в одном делении
    bool setCSZoom(int _zoom = DEFZOOMVALUE);
    inline int getCSZoom();

    /// Нижняя граница зумирования
    bool setCSZoomMin(int _zoomMin = DEFZOOMMIN);
    inline int getCSZoomMin();

    /// Верхняя граница зумирования
    bool setCSZoomMax(int _zoomMax = DEFZOOMMAX);
    inline int getCSZoomMax();

    /// Шаг изменения зума
    bool setCSZoomStep(int _zoomStep = DEFZOOMSTEP);
    inline int getCSZoomStep();

    /// Зумирование относительно центра системы координат
    void setCSZoomCenter(bool _zoomCenter = false);
    inline bool isCSZoomCenter();

    /// Параметры курсора мыши

    /// Последняя позиция курсора мыши при нажатии
    inline QPoint getMPosClick();

    /// Последняя позиция курсора мыши без нажатия
    inline QPoint getMPosNoClick();

    /// Текущий тип курсора
    void setMCursorShape(Qt::CursorShape _cursorShape = Qt::OpenHandCursor);
    inline Qt::CursorShape getMCursorShape();

private:
    /// Смещение системы координат
    void translocationCoordinateSystem();

    struct CoordinateSystem     /// Структура системы координат (СК)
    {
        bool angles[4];         /// Координатные углы

        struct Axis             /// Структура координатной оси
        {
            float shift;        /// Сдвиг относительно центра окна
            float measure;      /// Количество единиц измерения в одном делении
            float expansion;    /// Коэффициентр растяжения
            bool values;        /// Флаг отображения цифровых значений
            bool dashLines;     /// Флаг отображения пунктирных линий
        } abs, ord;             /// Оси абсцисс и ординат

        struct Zoom             /// Структура зумирования
        {
            int value;          /// Количество пикселей в одном делении
            int min;            /// Нижняя граница зумирования
            int max;            /// Верхняя граница зумирования
            int step;           /// Шаг изменения зума
            bool center;        /// Зумирование относительно центра СК
        } zoom;
    } coordSystem;

    struct Mouse                        /// Структура курсора мыши
    {
        struct Pos                      /// Структура последней позиции курсора
        {
            QPoint click;               /// Позиция курсора при нажатии
            QPoint noClick;             /// Позиция курсора без нажатия
        } pos;

        Qt::CursorShape cursorShape;    /// Текущий тип курсора
    } mouse;
};

#endif // GRAPHER2D_H
