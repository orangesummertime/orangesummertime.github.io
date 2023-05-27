#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>

class ImageViewer : public QGraphicsView
{
public:
    ImageViewer(QWidget *parent = nullptr)
        : QGraphicsView(parent)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
        setRenderHint(QPainter::SmoothPixmapTransform);
    }

    void setImage(const QPixmap &pixmap)
    {
        m_pixmapItem.setPixmap(pixmap);
        m_pixmapItem.setTransformationMode(Qt::SmoothTransformation);
        m_scene.addItem(&m_pixmapItem);
        setScene(&m_scene);
        setSceneRect(m_pixmapItem.boundingRect());
    }
    QPixmap pixmap() const
    {
        return m_pixmapItem.pixmap();
    }
protected:
    void wheelEvent(QWheelEvent *event) override
    {
        if (event->modifiers() & Qt::ControlModifier) {
            double scaleFactor = 1.15;
            if (event->delta() < 0)
                scaleFactor = 1.0 / scaleFactor;
            scale(scaleFactor, scaleFactor);
        } else {
            QGraphicsView::wheelEvent(event);
        }
    }

private:
    QGraphicsScene m_scene;
    QGraphicsPixmapItem m_pixmapItem;
};

#endif // IMAGEVIEWER_H
