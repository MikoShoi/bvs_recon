#include "iostream"

#include "sfmrecon.h"
#include "logger.h"
#include "pointset.h"
#include "cloud.h"
#include "file.h"
#include "mathematica.h"

#include <QDir>
#include <QDebug>
#include <QVector>
#include <QVector3D>

using MikoCgal::PointSet;
using MikoCgal::Cloud;
using MikoCgal::File;

using namespace std;

int main(int argc, char *argv[])
{
  if ( argc != 3 )
    throw Exception{"Needed 2 params, images dir path and  model file path"};

  const std::string imagesDirPath   = argv[1]
                    , modelFilePath = argv[2]
                    , mFilePath     = "mTempFile.xyz";

  if ( imagesDirPath.empty() || modelFilePath.empty() )
    throw Exception{"Function main", "Empty parameters"};

//-- reconstruct images in imagesDirPath
  SfmRecon sfmRecon {};
  sfmRecon.reconstruct(imagesDirPath);

//-- wrap reconstructed point cloud to special container
  PointCloud      sfmCloud = std::move( sfmRecon.getCloud() );
  QVector<double> mInCloud {};

  for (QVector3D v : sfmCloud)
  {
      mInCloud.push_back( v.x() );
      mInCloud.push_back( v.y() );
      mInCloud.push_back( v.z() );
  }

//-- and pass it to mathematica program
  Mathematica m {};
  m.sendRequest("Needs", "BvsMathematicaRecon`");
  m.sendRequest("ProcessCloud", mInCloud, mFilePath);

//-- read processed points and pass it to next processing step
  Points    mOutCloud = std::move( File(mFilePath).getPoints() );
  PointSet  ps {};
  ps.loadPoints(mOutCloud);

  const double  cloudAverageSpacing     = ps.getAverageSpacing(24)
              , percentOfPointsToRetain = 10.0
              , neighborRadiusFactor    = 10 * cloudAverageSpacing;

  ps.wlopSimplify (percentOfPointsToRetain, neighborRadiusFactor);
  Points wlopedCloud ( std::move( ps.getPoints() ) );

//-- finally reconstruct processed cloud and write model to modelFilePath
  Cloud cp {};
  cp.loadPoints(wlopedCloud);
  cp.smooth(24);
  cp.estimateNormals(24);
  cp.scaleReconstruction(modelFilePath);

  //-- TODO: Too strong function programming.
  //-- code hard to read: Cloud, PointCloud, PointSet, Points

  return 0;
}
