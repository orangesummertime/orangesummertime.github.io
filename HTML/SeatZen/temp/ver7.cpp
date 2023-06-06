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
#include"ConfigManager.h"
//#include"jpeglib.h"
//#include"tiffio.h"
using namespace Qt;
void resizeWindow(QMainWindow* window,QSize size){
    window->resize(size);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建MainWindow
    QMainWindow MainWindow;
    //这里读取了config.xml
    ConfigManager config;
    QString title = config.readConfig("config.xml", "title");
    if (!title.isEmpty()) {
        MainWindow.setWindowTitle(title);
    }

    // 创建主窗口
    QWidget* mainWidget = new QWidget();
    mainWidget->setWindowTitle("Image Viewer");
    //mainWidget->setFixedSize(1366,768);
    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout();

    // 创建标签用于显示图片
//    QLabel* imageLabel = new QLabel();
    ImageViewer* imgViewer=new ImageViewer();
    layout->addWidget(imgViewer);

    // 创建按钮用于选择图片
    QPushButton* selectButton = new QPushButton("选择图片");
    layout->addWidget(selectButton);

    // 创建按钮用于处理图片
    QPushButton* processButton = new QPushButton("双边滤波处理");
    layout->addWidget(processButton);


    //添加注解
    QLabel* text=new QLabel("按住Ctrl使用鼠标滚轮来缩放图片");
    layout->addWidget(text);


    // 将布局设置为主窗口的布局
    mainWidget->setLayout(layout);
    // 处理选择图片的信号
    QObject::connect(selectButton, &QPushButton::clicked, [=] {
        QString fileName = QFileDialog::getOpenFileName(mainWidget, "Select Image", ".", "Image Files (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            QImage image(fileName);
            imgViewer->setImage(QPixmap::fromImage(image));
            QSize imageSize = image.size();
                // 添加一个偏移量，例如 100 像素
                QSize windowSize(imageSize.width() + 100, imageSize.height() + 200);
            mainWidget->setFixedSize(windowSize);
        }
    });

    //自动调整窗口大小
    QObject::connect(selectButton, &QPushButton::clicked, [&MainWindow] {
        MainWindow.hide();
        MainWindow.show();
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





        //创建处理后的图像展示窗口
        QMainWindow* processedWindow= new QMainWindow();
        ImageViewer* processedImg = new ImageViewer();
        processedImg->setImage(QPixmap::fromImage(processedImage));
        QWidget* processedWidget = new QWidget();
        processedWidget->setWindowTitle("处理结果");
        processedWidget->setFixedSize(processedImg->qimage().size().width()+200,processedImg->qimage().size().height()+300);
        // 创建垂直布局
        QVBoxLayout* processedLayout = new QVBoxLayout();

        QLabel* text2=new QLabel("处理结果：");
        processedLayout->addWidget(text2);

        //
        processedLayout->addWidget(processedImg);


        processedWidget->setLayout(processedLayout);

        processedWindow->setCentralWidget(processedWidget);
        processedWindow->setWindowTitle("使用OpenCV处理结果");
        //创建按钮用于保存图片
        QPushButton* saveButton = new QPushButton("保存为png/jpg/bmp图片");
        QPushButton* saveJpegButton = new QPushButton("使用libjpeg编码并保存为jpeg图片");
        QPushButton* saveTiffButton = new QPushButton("使用libtiff编码并保存为tiff图片");
        QPushButton* processAgainButton = new QPushButton("再次处理");
        processedLayout->addWidget(processAgainButton);
        processedLayout->addWidget(saveButton);
        processedLayout->addWidget(saveJpegButton);
        processedLayout->addWidget(saveTiffButton);

        QLabel* text3=new QLabel("按住Ctrl使用鼠标滚轮来缩放图片");
        processedLayout->addWidget(text3);

        QObject::connect(saveButton, &QPushButton::clicked, [=]() {
            if (!processedImg->pixmap()) {
                return;
            }
            QImage imgToSave = processedImg->pixmap().toImage();

            imgToSave.save(QFileDialog::getSaveFileName(processedWidget,"保存至","./output.png", "Image Files (*.png *.jpg *.bmp);"));

        });

        //再次处理图片
        QObject::connect(processAgainButton, &QPushButton::clicked,[=](){
            QImage newImage = processedImg->qimage();
            cv::Mat newMat(newImage.height(), newImage.width(), CV_8UC4, const_cast<uchar*>(newImage.bits()), newImage.bytesPerLine());
            cv::cvtColor(newMat, newMat, cv::COLOR_BGRA2BGR);

            // 使用双边滤波处理图片
            cv::Mat newResult;
            cv::bilateralFilter(newMat, newResult, 15, 80, 80);
            QImage tempImage(newResult.data, newResult.cols, newResult.rows, newResult.step, QImage::Format_RGB888);
            processedImg->setImage(QPixmap::fromImage(tempImage.rgbSwapped()));
        });

        QObject::connect(saveJpegButton, &QPushButton::clicked, [=]() {
            if (!processedImg->pixmap()) {
                return;
            }
            //TODO
            QImage imgToSave = processedImg->pixmap().toImage();

            imgToSave.save(QFileDialog::getSaveFileName(processedWidget,"保存至","./output.jpeg", "Jpeg File (*.jpeg *.jpg);"));


        });

        QObject::connect(saveTiffButton, &QPushButton::clicked, [=]() {
            if (!processedImg->pixmap()) {
                return;
            }
            //TODO
            QImage imgToSave = processedImg->pixmap().toImage();

            imgToSave.save(QFileDialog::getSaveFileName(processedWidget,"保存至","./output.tiff", "Tiff File (*.tiff);"));


        });


        processedWindow->show();
        //imgViewer->setImage(QPixmap::fromImage(processedImage));

    });

     //处理保存图片的信号

    MainWindow.setCentralWidget(mainWidget);
    MainWindow.show();
    // 显示主窗口
    //mainWidget->show();

    return a.exec();
}
