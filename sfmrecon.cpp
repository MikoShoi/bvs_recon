#include "sfmrecon.h"

SfmRecon::SfmRecon(QObject *parent) : QObject(parent)
{
    setupConfig();
}

void                SfmRecon::reconstruct (std::string dirPath)
{
    //--create reconstructor object
    Reconstructor          reconstructor {config};

    //--pass to him set of images
    loadImages(reconstructor, dirPath);

    //--perform reconstruction
    Reconstructions reconstructions;
    reconstructor.BuildReconstruction( &reconstructions );

    //--save reconstructed points in cloud class memeber
    fillCloud(reconstructions);
}
const PointCloud&   SfmRecon::getCloud    ()
{
    return cloud;
}

void SfmRecon::setupConfig  ()
{
    int threadsNumber = 3;
//    int imageWidth    = 2560;

    config.num_threads                              = threadsNumber;
//    config.min_track_length                         = 2;
//    config.max_track_length                         = 50;
//    config.min_num_inlier_matches                   = 30;
    config.reconstruct_largest_connected_component  = true;

//    config.output_matches_file                                      = "matches";
//    config.matching_options.match_out_of_core                       = true;
//    config.matching_options.keypoints_and_descriptors_output_dir    = "tempDir";

    config.descriptor_type      = theia::DescriptorExtractorType::AKAZE;
    config.feature_density      = theia::FeatureDensity::DENSE;
    config.matching_strategy    = theia::MatchingStrategy::CASCADE_HASHING;

    theia::FeatureMatcherOptions &fmo           = config.matching_options;
    fmo.num_threads                             = threadsNumber;
//    fmo.keypoints_and_descriptors_output_dir    = "keyAndDesc";
//    fmo.cache_capacity                          = 256;
//    fmo.min_num_feature_matches                 = 30;
    fmo.keep_only_symmetric_matches             = true;

//    theia::TwoViewMatchGeometricVerification::Options &tvo  = fmo.geometric_verification_options;
//    tvo.min_num_inlier_matches                              = 30;
//    tvo.guided_matching                                     = true;
//    tvo.guided_matching_max_distance_pixels                 = 4.0;//2
//    tvo.guided_matching_lowes_ratio                         = 0.8;
//    tvo.triangulation_max_reprojection_error                = 7.0;
//    tvo.min_triangulation_angle_degrees                     = 15.0;//4.0;
//    tvo.final_max_reprojection_error                        = 4.0;//5.0
//    tvo.bundle_adjustment                                   = true;

//    theia::EstimateTwoViewInfoOptions &etvio    = tvo.estimate_twoview_info_options;
//    etvio.expected_ransac_confidence            = 0.75;
//    etvio.min_ransac_iterations                 = 128;
//    etvio.max_ransac_iterations                 = 1000;
//    etvio.ransac_type                           = theia::RansacType::RANSAC;
//    etvio.max_sampson_error_pixels              = 6.0;// * imageWidth/1024;
//    etvio.use_mle                               = true;

    theia::ReconstructionEstimatorOptions &reo  = config.reconstruction_estimator_options;
    reo.reconstruction_estimator_type           = theia::ReconstructionEstimatorType::INCREMENTAL;
//    reo.global_position_estimator_type          = theia::GlobalPositionEstimatorType::LINEAR_TRIPLET;
//    reo.global_rotation_estimator_type          = theia::GlobalRotationEstimatorType::ROBUST_L1L2;
    reo.num_threads                             = threadsNumber;
//    reo.max_reprojection_error_in_pixels        = 5.0;
//    reo.min_num_two_view_inliers                = 30;
//    reo.num_retriangulation_iterations          = 4;

//    reo.ransac_confidence                       = 0.75;
//    reo.ransac_min_iterations                   = 64;//50
//    reo.ransac_max_iterations                   = 256;
//    reo.ransac_use_mle                          = true;

//    reo.rotation_filtering_max_difference_degrees                       = 5.0;
//    reo.refine_relative_translations_after_rotation_estimation          = true;
//    reo.extract_maximal_rigid_subgraph                                  = true; //--false
//    reo.filter_relative_translations_with_1dsfm                         = true;
//    reo.translation_filtering_num_iterations                            = 48;
//    reo.translation_filtering_projection_tolerance                      = 0.1;
//    reo.rotation_estimation_robust_loss_scale                           = 0.1;

//    reo.refine_camera_positions_and_points_after_position_estimation    = true;
//    reo.multiple_view_localization_ratio                                = 0.8;
//    reo.absolute_pose_reprojection_error_threshold                      = 4.0;// * imageWidth/1024;
//    reo.min_num_absolute_pose_inliers                                   = 30;
//    reo.full_bundle_adjustment_growth_percent                           = 5.0; //check
//    reo.partial_bundle_adjustment_num_views                             = 20;//20
//    reo.min_triangulation_angle_degrees                                 = 15.0;
//    reo.triangulation_max_reprojection_error_in_pixels                  = 10;
//    reo.bundle_adjust_tracks                    = true;
    reo.bundle_adjustment_loss_function_type    = theia::LossFunctionType::HUBER;
//    reo.bundle_adjustment_robust_loss_width     = 10;
//    reo.min_cameras_for_iterative_solver        = 100;
//    reo.intrinsics_to_optimize                  = theia::OptimizeIntrinsicsType::NONE;

    reo.nonlinear_position_estimator_options.num_threads                = threadsNumber;
//    reo.nonlinear_position_estimator_options.min_num_points_per_view    = 100;
}
void SfmRecon::loadImages   (Reconstructor& rb, std::string dirPath)
{
    QDir imagesDir { QString::fromStdString(dirPath) };

    if( !imagesDir.exists() )
        throw Exception{"SfmRecon:addImages", "Given dir path no exist"};

    QStringList   filters{"*.jpg", "*.jpeg"};
    QFileInfoList filesInfo = imagesDir.entryInfoList( filters , QDir::Files);

    foreach (QFileInfo fileInfo, filesInfo)
    {
        rb.AddImage( fileInfo.absoluteFilePath().toStdString() );
    }

    rb.ExtractAndMatchFeatures();
}
void SfmRecon::fillCloud    (Reconstructions reconstructions)
{
    foreach (theia::Reconstruction *reconstruction, reconstructions)
    {
        foreach (const theia::TrackId trackId, reconstruction->TrackIds())
        {
            const auto* track = reconstruction->Track(trackId);
            if (track != nullptr && track->IsEstimated())
            {
                Eigen::Vector3d point = track->Point().hnormalized();
                cloud.push_back( QVector3D(point.x(), point.y(), point.z()) );
            }
        }
    }
}
