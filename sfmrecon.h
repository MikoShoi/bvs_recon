#ifndef SFMRECON_H
#define SFMRECON_H

#include <QObject>
#include <QFileInfoList>
#include <QVector3D>
#include <QVector>
#include <QDir>

#include <theia/theia.h>
#include <theia/sfm/two_view_match_geometric_verification.h>

#include "exception.h"

typedef theia::ReconstructionBuilderOptions   Config;
typedef std::vector<theia::Reconstruction *>  Reconstructions;
typedef theia::ReconstructionBuilder          Reconstructor;
typedef QVector<QVector3D>                    PointCloud;

class SfmRecon : public QObject
{
    Q_OBJECT
public:
    explicit SfmRecon(QObject *parent = 0);

    void              reconstruct (std::string dirPath);
    const PointCloud& getCloud    ();

private:
    void setupConfig    ();
    void loadImages     (Reconstructor& rb, std::string dirPath);
    void fillCloud      (Reconstructions reconstructions);

    PointCloud  cloud   {};
    Config      config;
};

#endif // SFMRECON_H
