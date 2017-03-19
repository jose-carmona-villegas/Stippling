/**
 * @file dotgenerationworker.cpp
 * @brief DotGenerationWorker class source file
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 2/May/2014
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */

#include "dotgenerationworker.h"

cv::Mat DotGenerationWorker::floydSteinbergDithering(cv::Mat toDither)
{
    cv::Mat imageGrayscale;
    // First, transform the image to grayscale.
    cvtColor(toDither,imageGrayscale,CV_RGB2GRAY);

    imwrite("imageGrayscale.png", imageGrayscale);

    int padding = 1;
    cv::Mat padded;
    padded.create(imageGrayscale.rows + 2*padding, imageGrayscale.cols + 2*padding, imageGrayscale.type());
    padded.setTo(cv::Scalar::all(0));

    copyMakeBorder(imageGrayscale, padded, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar(0));

    for (int row=1; row<padded.rows-1; ++row)
    {
        for (int column=1; column<padded.cols-1; ++column)
        {
            // In order to properly propagate the quantification error, the precision of the calculation
            // must be took with care, that's why we'll normalize all the colours to operate with them
            // and then they will be converted back to 8b scale.
            double oldPixel = padded.at<unsigned char>(row, column) / 255.0;
            unsigned char newPixel8b = oldPixel >= 0.5 ? 255 : 0;
            padded.at<unsigned char>(row, column) = newPixel8b;
            double newPixel = oldPixel >= 0.5 ? 1.0 : 0.0;
            double quantificationError = oldPixel - newPixel;

            double pixel;

            // Floyd-Steinberg dithering filter
            pixel = padded.at<unsigned char>(row, column+1) / 255.0;
            pixel += 7.0/16.0 * quantificationError;
            padded.at<unsigned char>(row, column+1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column-1) / 255.0;
            pixel += 3.0/16.0 * quantificationError;
            padded.at<unsigned char>(row+1, column-1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column) / 255.0;
            pixel += 5.0/16.0 * quantificationError;
            padded.at<unsigned char>(row+1, column) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column+1) / 255.0;
            pixel += 1.0/16.0 * quantificationError;
            padded.at<unsigned char>(row+1, column+1) = (unsigned char)(pixel * 255.0);
        }
    }

    cv::Rect myROI(1, 1, imageGrayscale.cols, imageGrayscale.rows);
    cv::Mat cropped = padded(myROI);

    imwrite("imageDithered_Floyd-Steinberg.png", cropped);

    return cropped;
}

cv::Mat DotGenerationWorker::stuckiDithering(cv::Mat toDither)
{
    cv::Mat imageGrayscale;
    // First, transform the image to grayscale.
    cvtColor(toDither,imageGrayscale,CV_RGB2GRAY);

    imwrite("imageGrayscale.png", imageGrayscale);

    int padding = 2;
    cv::Mat padded;
    padded.create(imageGrayscale.rows + 2*padding, imageGrayscale.cols + 2*padding, imageGrayscale.type());
    padded.setTo(cv::Scalar::all(0));

    copyMakeBorder(imageGrayscale, padded, padding, padding, padding, padding, cv::BORDER_CONSTANT, cv::Scalar(0));

    for (int row=2; row<padded.rows-2; ++row)
    {
        for (int column=2; column<padded.cols-2; ++column)
        {
            // In order to properly propagate the quantification error, the precision of the calculation
            // must be took with care, that's why we'll normalize all the colours to operate with them
            // and then they will be converted back to 8b scale.
            double oldPixel = padded.at<unsigned char>(row, column) / 255.0;
            unsigned char newPixel8b = oldPixel >= 0.5 ? 255 : 0;
            padded.at<unsigned char>(row, column) = newPixel8b;
            double newPixel = oldPixel >= 0.5 ? 1.0 : 0.0;
            double quantificationError = oldPixel - newPixel;

            double pixel;

            // Stucki dithering filter
            pixel = padded.at<unsigned char>(row, column+1) / 255.0;
            pixel += 8.0/42.0 * quantificationError;
            padded.at<unsigned char>(row, column+1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row, column+2) / 255.0;
            pixel += 4.0/42.0 * quantificationError;
            padded.at<unsigned char>(row, column+2) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column-2) / 255.0;
            pixel += 2.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+1, column-2) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column-1) / 255.0;
            pixel += 4.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+1, column-1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column) / 255.0;
            pixel += 8.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+1, column) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column+1) / 255.0;
            pixel += 4.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+1, column+1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+1, column+2) / 255.0;
            pixel += 2.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+1, column+2) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+2, column-2) / 255.0;
            pixel += 1.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+2, column-2) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+2, column-1) / 255.0;
            pixel += 2.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+2, column-1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+2, column) / 255.0;
            pixel += 4.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+2, column) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+2, column+1) / 255.0;
            pixel += 2.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+2, column+1) = (unsigned char)(pixel * 255.0);

            pixel = padded.at<unsigned char>(row+2, column+2) / 255.0;
            pixel += 1.0/42.0 * quantificationError;
            padded.at<unsigned char>(row+2, column+2) = (unsigned char)(pixel * 255.0);
        }
    }

    cv::Rect myROI(2, 2, imageGrayscale.cols, imageGrayscale.rows);
    cv::Mat cropped = padded(myROI);

    imwrite("imageDithered_Stucki.png", cropped);

    return cropped;
}

cv::Mat DotGenerationWorker::sobelEdgeDetection(cv::Mat toDetect, int scale)
{
    cv::Mat result, gray;

    //cv::GaussianBlur( _solid3DModel, _sobelSolid3DModel, Size(3,3), 0, 0, BORDER_DEFAULT );
    cv::cvtColor( toDetect, gray, CV_RGB2GRAY );

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    int delta = 0;
    int ddepth = CV_16S;

    /// Gradient X
    cv::Sobel( gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    cv::Sobel( gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, result );

    //cv::imwrite("sobelX.png", abs_grad_x);
    //cv::imwrite("sobelY.png", abs_grad_y);
    //cv::imwrite("sobel.png", result);

    return result;
}

cv::Mat DotGenerationWorker::cannyEdgeDetection(cv::Mat toDetect, int lowThreshold, int highThreshold)
{
    cv::Mat result, gray;

    //cv::GaussianBlur( _solid3DModel, _sobelSolid3DModel, Size(3,3), 0, 0, BORDER_DEFAULT );
    cv::cvtColor( toDetect, gray, CV_RGB2GRAY );

    cv::Canny(gray, result, lowThreshold, highThreshold, 3);

    //cv::imwrite("canny.png", result);

    return result;
}

cv::Mat DotGenerationWorker::thresholding(cv::Mat toThreshold, int threshold, int lower, int higher)
{
    cv::Mat result, gray;
    // First, transform the image to grayscale.
    if(toThreshold.channels() > 1)
    {
        cv::cvtColor(toThreshold, gray, CV_RGB2GRAY);
    }
    else
    {
        gray = toThreshold;
    }

    result.create(gray.rows, gray.cols, gray.type());
    result.setTo(cv::Scalar::all(0));

    for (int row=0; row<gray.rows; ++row)
    {
        for (int column=0; column<gray.cols; ++column)
        {

            if( gray.at<unsigned char>(row, column) <= threshold)
            {
                result.at<unsigned char>(row, column) = (unsigned char)(0);
            }
            else
            {
                result.at<unsigned char>(row, column) = (unsigned char)(255);
            }
        }
    }

    return result;
}

DotGenerationWorker::DotGenerationWorker(cv::Mat toStipple,
                                         DitheringMethod ditheringMethod,
                                         glm::vec2 spriteSize,
                                         glm::vec2 spritesMatrixSize,
                                         cv::Mat solid3DModel,
                                         Configuration * globalConfig,
                                         EntityTreeController * entities)
{
    _toStipple = toStipple;
    _ditheringMethod = ditheringMethod;
    _spriteSize = spriteSize;
    _spritesMatrixSize = spritesMatrixSize;

    _solid3DModel = solid3DModel;

    _stipplingDots = 0;

    _globalConfig = globalConfig;

    _entities = entities;
}

DotGenerationWorker::~DotGenerationWorker()
{
    // This is a worker thread, the employer should be the one deleting the stuff produced.
    _stipplingDots = 0;
}

QuadTree * DotGenerationWorker::getStipplingDots()
{
    return _stipplingDots;
}

void DotGenerationWorker::process()
{
    out << "Beginning stippling process..." << endl;

    cv::Mat imageDithered;
    switch(_ditheringMethod)
    {
        case FLOYD_STEINBERG:
            imageDithered = floydSteinbergDithering(_toStipple);
            break;
        case STUCKI:
            imageDithered = stuckiDithering(_toStipple);
            break;
    }

    out << "Dithered image created..." << endl;

    switch(_globalConfig->edgeDetectionMethod())
    {
    case SOBEL:
        _edgeDetectedSolid3DModel = sobelEdgeDetection(_solid3DModel);
        break;
    case CANNY:
        _edgeDetectedSolid3DModel = cannyEdgeDetection(_solid3DModel, 50, 150);
        break;
    }

    cv::imwrite("debugEdgeGlobalSolid.png", _edgeDetectedSolid3DModel);

    // Get the nodes breadth first
    QVector<EntityTreeNode*> * nodes = _entities->traverseBreadthFirst();

    foreach(EntityTreeNode * node, *nodes)
    {
        if(!node->configuration()->isDefault())
        {
            EdgeDetectionMethod method;
            if(node->configuration()->hasSpecificEdgeDetectionMethod())
            {
                method = node->configuration()->edgeDetectionMethod();
            }
            else
            {
                method = _globalConfig->edgeDetectionMethod();
            }
            switch(method)
            {
            case SOBEL:
                node->setEdgeDetection(sobelEdgeDetection(node->solidRendering()));
                break;
            case CANNY:
                node->setEdgeDetection(cannyEdgeDetection(node->solidRendering(), 50, 150));
                break;
            }

            cv::imwrite(QString("debugEdgeSpecific" + node->name() + ".png").toStdString(), node->edgeDetection());
        }
    }

    out << "Edge detection completed..." << endl;

    out << "Beginning dot creation..." << endl;

    int stippledImageRows = imageDithered.rows * _spriteSize.y / _globalConfig->packingFactor();
    int stippledImageCols = imageDithered.cols * _spriteSize.x / _globalConfig->packingFactor();

    out << "Original image size: " << imageDithered.rows << " x " << imageDithered.cols << " px" <<endl;
    out << "Stippled image size: " << stippledImageRows << " x " << stippledImageCols << " px" << endl;
    out << "Packing factor: " << _globalConfig->packingFactor() << endl;

    int depth = 5;

    // TODO, choose a depth that is admitted by the size of the stippled image.
    // The depth should be calculated so that the leaf node's area isn't too big nor too small.

    _stipplingDots = new QuadTree(glm::vec4(0,0,stippledImageCols,stippledImageRows), depth);
    //out << "_stipplingDots = " << _stipplingDots << endl;

    //out << "QuadTree created with " << _stipplingDots->numberOfNodes() << " nodes and a depth of " << depth << "." << endl;
    //out << "QuadTree has " << _stipplingDots->numberOfLeaves() << " leaf nodes." << endl;
    //out << "A leaf node's area is equal to " << stippledImageRows / sqrt(_stipplingDots->numberOfLeaves()) << " x " << stippledImageCols / sqrt(_stipplingDots->numberOfLeaves()) << " px." << endl;


    srand(_globalConfig->rngSeed());

    out << "Progress: 0%" << endl;
    int lastProgress = 0;
    for (int row=0; row<imageDithered.rows; ++row)
    {
        for (int column=0; column<imageDithered.cols; ++column)
        {
            // Pick a random dot sprite
            int chosenDot = rand() % ((int(_spritesMatrixSize.x * _spritesMatrixSize.y)) - 1);


            // Search for the first node (deepest first) that has a green solid rendering
            // for the given pixel and has a non default specific configuration. If none
            // is found, generate the dot using general configuration if any node has been
            // flagged as green.
            EntityTreeNode * foundNode = 0;
            bool anyGreen = false;
            //foreach(EntityTreeNode * node, *nodes)
            for(int i=nodes->size()-1; i >= 0; --i)
            {
                cv::Vec3b bgrPixel = nodes->at(i)->solidRendering().at<cv::Vec3b>(row, column);
                bool isRed = ( (bgrPixel[0] == 0) && (bgrPixel[1] == 0)  && (bgrPixel[2] == 255) );

                anyGreen = anyGreen || !isRed;

                if(!isRed && !nodes->at(i)->configuration()->isDefault())
                {
                    foundNode = nodes->at(i);
                    break;
                }
            }


            cv::Mat specificEdgeDetectedSolid3DModel;
            if(foundNode != 0)
            {
                specificEdgeDetectedSolid3DModel = foundNode->edgeDetection();
            }

            bool correctDitheringValue = (imageDithered.at<unsigned char>(row, column) <= 128);
            bool isRed = !anyGreen;

            bool belongsToModel = ( correctDitheringValue && !isRed );

            bool externalToModelButChosen = false;
            if(!belongsToModel)
            {
                // Check the chances of still generating the dot
                int unmodelledChance = rand() % 100;
                //out << "UnmodelledChance: " << unmodelledChance << endl;
                if(correctDitheringValue && (unmodelledChance >= 100 - _globalConfig->unmodelledStipplingChance()))
                {
                    externalToModelButChosen = true;
                }
                else
                {
                    externalToModelButChosen = false;
                }
            }

            bool isEdgeModel;
            if(foundNode != 0)
            {
                // Use specific configuration
                isEdgeModel = (specificEdgeDetectedSolid3DModel.at<unsigned char>(row, column) != 0);
            }
            else
            {
                // Use general configuration
                isEdgeModel = (_edgeDetectedSolid3DModel.at<unsigned char>(row, column) != 0);
            }

            bool belongsToModelAndChosen = false;
            if(belongsToModel)
            {
                // Check the chances of generating the dot in the modelled area
                // If the dot corresponds to an edge of the model, it will always be generated
                int modelledChance = rand() % 100;
                //out << "ModelledChance: " << modelledChance << endl;
                if( isEdgeModel)
                {
                    // Is part of the silhouette
                    belongsToModelAndChosen = true;
                }
                // Is internal
                else
                {
                    if(foundNode != 0 && foundNode->configuration()->percentageInternalGeneration() != -1)
                    {
                        // Use specific configuration
                        if( modelledChance > 100 - foundNode->configuration()->percentageInternalGeneration() )
                        {
                            belongsToModelAndChosen =  true;
                        }
                        else
                        {
                            belongsToModelAndChosen = false;
                        }
                    }
                    else
                    {
                        // Use general configuration
                        if( modelledChance >= 100 - _globalConfig->modelledStipplingChance() )
                        {
                            belongsToModelAndChosen =  true;
                        }
                        else
                        {
                            belongsToModelAndChosen = false;
                        }
                    }
                }
            }

            if(externalToModelButChosen || belongsToModelAndChosen)
            {
                //out << "Creating dot " << (row+1)*(column+1) << " out of " << imageDithered.rows * imageDithered.cols << endl;

                float posX = float(column * _spriteSize.x / _globalConfig->packingFactor());
                float posY = float(stippledImageRows - (row * _spriteSize.y / _globalConfig->packingFactor()));
                StippleDot * dot = new StippleDot(glm::vec2(posX, posY),
                                              _spriteSize,
                                              chosenDot);
                if(isEdgeModel)
                {
                    if(foundNode != 0)
                    {
                        // Use specific configuration
                        int silhouetteSuffersOffsetChance = rand() % 100;
                        if(silhouetteSuffersOffsetChance > 100 - foundNode->configuration()->percentageSilhouetteDispersion())
                        {
                            dot->setCanHaveOffsetApplied(true);
                        }
                        else
                        {
                            dot->setCanHaveOffsetApplied(false);
                        }
                    }
                    else
                    {
                        // Use general configuration
                        dot->setCanHaveOffsetApplied(false);
                    }
                }
                else
                {
                    dot->setCanHaveOffsetApplied(true);
                }

                _stipplingDots->add(dot);
            }

            float progress = ( 100.0f * ((row*imageDithered.cols)+column+1) / float(imageDithered.rows*imageDithered.cols) );
            if(int(progress) >= lastProgress+10)
            {
                lastProgress += 10;
                out << "Progress: " << lastProgress << "%" << endl;
            }
        }
    }

    nodes->clear();
    delete nodes;
    nodes = 0;

    out << "Finished stippling process." << endl;

    emit finished();
}
