/**
 * @file dotgenerationworker.h
 * @brief DotGenerationWorker class header file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 2/May/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#ifndef DOTGENERATIONWORKER_H
#define DOTGENERATIONWORKER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QObject>

#include "util.h" // NOT REENTRANT
#include "quadtree.h"
#include "configuration.h"
#include "entitytreecontroller.h"

class DotGenerationWorker : public QObject
{
    Q_OBJECT

public:
    enum DitheringMethod {FLOYD_STEINBERG, STUCKI};

protected:
    cv::Mat _toStipple;
    DitheringMethod _ditheringMethod;
    glm::vec2 _spriteSize;
    glm::vec2 _spritesMatrixSize;

    QuadTree * _stipplingDots;

    cv::Mat _solid3DModel;
    cv::Mat _edgeDetectedSolid3DModel;

    Configuration * _globalConfig;

    EntityTreeController * _entities;






    cv::Mat floydSteinbergDithering(cv::Mat toDither);
    cv::Mat stuckiDithering(cv::Mat toDither);
    cv::Mat sobelEdgeDetection(cv::Mat toDetect, int scale = 1);
    cv::Mat cannyEdgeDetection(cv::Mat toDetect, int lowThreshold, int highThreshold);
    cv::Mat thresholding(cv::Mat toThreshold, int threshold = 128, int lower = 0, int higher = 255); // Threshold, lower and higher have to be between 0 and 255.

public:
    DotGenerationWorker(cv::Mat toStipple,
                        DitheringMethod ditheringMethod,
                        glm::vec2 spriteSize,
                        glm::vec2 spritesMatrixSize,
                        cv::Mat solid3DModel,
                        Configuration * globalConfig,
                        EntityTreeController * entities);

    ~DotGenerationWorker();

    QuadTree * getStipplingDots();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
};

#endif // DOTGENERATIONWORKER_H


