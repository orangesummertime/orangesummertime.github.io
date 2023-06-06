        int width = neoImage.width();
        int height = neoImage.height();
        int diameter = 15;
        int sigmaSpace = 80;
        int sigmaColor = 80;
        // 创建输出图像
        QImage neoResult(width, height, QImage::Format_RGB888);
        // 计算权重表
        QVector<double> weightTable(diameter * diameter);
        double weightSum = 0.0;

        for (int i = 0; i < diameter; i++) {
            for (int j = 0; j < diameter; j++) {
                int x = i - diameter / 2;
                int y = j - diameter / 2;
                double weight = exp(-0.5 * ((x * x + y * y) / (sigmaSpace * sigmaSpace) +
                    pow(qRed(neoImage.pixel(i, j)) - qRed(neoImage.pixel(x + width / 2, y + height / 2)), 2) / (sigmaColor * sigmaColor)));
                weightTable[i * diameter + j] = weight;
                weightSum += weight;
            }
        }

        // 对每个像素进行双边滤波
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                double r = 0.0;
                double g = 0.0;
                double b = 0.0;
                double weightSum = 0.0;

                for (int k = 0; k < diameter; k++) {
                    for (int l = 0; l < diameter; l++) {
                        int x = i + k - diameter / 2;
                        int y = j + l - diameter / 2;

                        if (x >= 0 && x < width && y >= 0 && y < height) {
                            double weight = weightTable[k * diameter + l];
                            r += weight * qRed(neoImage.pixel(x, y));
                            g += weight * qGreen(neoImage.pixel(x, y));
                            b += weight * qBlue(neoImage.pixel(x, y));
                            weightSum += weight;
                        }
                    }
                }

                r /= weightSum;
                g /= weightSum;
                b /= weightSum;

                neoResult.setPixelColor(i, j, QColor(r, g, b));
            }
        }