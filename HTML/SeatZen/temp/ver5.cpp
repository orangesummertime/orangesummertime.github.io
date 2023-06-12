#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include"ImageViewer.h"
#include<QMessageBox>
#include<QTranslator>
#include<QMainWindow>
using namespace Qt;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建MainWindow
    QMainWindow MainWindow;

    // 创建主窗口
    QWidget* mainWidget = new QWidget();
    mainWidget->setWindowTitle("Image Viewer");

    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout();

    // 创建标签用于显示图片
//    QLabel* imageLabel = new QLabel();
    ImageViewer* imgViewer=new ImageViewer();
    layout->addWidget(imgViewer);

    // 创建按钮用于选择图片
    QPushButton* selectButton = new QPushButton("Select Image");
    layout->addWidget(selectButton);

    // 创建按钮用于处理图片
    QPushButton* processButton = new QPushButton("Process");
    layout->addWidget(processButton);

    //创建按钮用于保存图片
    QPushButton* saveButton = new QPushButton("保存图片");
    layout->addWidget(saveButton);

    //添加注解
    QLabel* text=new QLabel("按住Ctrl使用鼠标滚轮来缩放图片");
    layout->addWidget(text);


    // 将布局设置为主窗口的布局
    mainWidget->setLayout(layout);

    // 处理选择图片的信号
    QObject::connect(selectButton, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(mainWidget, "Select Image", ".", "Image Files (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            QImage image(fileName);
            imgViewer->setImage(QPixmap::fromImage(image));
        }
    });

    // 处理处理按钮的信号
    QObject::connect(processButton, &QPushButton::clicked, [=]() {
        if (!imgViewer->pixmap()) {
            return;
        }

        // 将图片转换为 OpenCV 格式
        QImage image = imgViewer->pixmap().toImage();
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);

        // 使用双边滤波处理图片
        cv::Mat result;
        cv::bilateralFilter(mat, result, 15, 80, 80);

        // 将处理后的图片转换为 Qt 格式并显示
        QImage processedImage(result.data, result.cols, result.rows, result.step, QImage::Format_RGB888);
        //Mat图片默认为BGR顺序
        processedImage=processedImage.rgbSwapped();
        imgViewer->setImage(QPixmap::fromImage(processedImage));

    });

     //处理保存图片的信号
    QObject::connect(saveButton, &QPushButton::clicked, [=]() {
        if (!imgViewer->pixmap()) {
            return;
        }
        QImage imgToSave = imgViewer->pixmap().toImage();

        imgToSave.save(QFileDialog::getSaveFileName(mainWidget,"保存至","./", "Image Files (*.png *.jpg *.bmp);"));

    });

    MainWindow.setCentralWidget(mainWidget);
    MainWindow.show();
    // 显示主窗口
    //mainWidget->show();

    return a.exec();
}
