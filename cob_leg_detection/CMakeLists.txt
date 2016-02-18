cmake_minimum_required(VERSION 2.8.3)
project(cob_leg_detection)

set(catkin_RUN_PACKAGES
	cob_people_tracking_filter
	cob_perception_msgs
	dynamic_reconfigure
	geometry_msgs
	image_geometry
	laser_geometry
    people_tracking_filter
	roscpp
	rospy
	sensor_msgs
	std_msgs
	std_srvs
	tf
	visualization_msgs
)

set(catkin_BUILD_PACKAGES
	${catkin_RUN_PACKAGES}
	cmake_modules
)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
	${catkin_BUILD_PACKAGES}
)

find_package(OpenCV REQUIRED)	
find_package(orocos_kdl REQUIRED)
find_package(Eigen REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(BFL REQUIRED bfl) #orocos-bfl
pkg_check_modules(BULLET bullet)

## dynamic reconfigure config
generate_dynamic_reconfigure_options(
  cfg/LegDetection.cfg
  cfg/DualTracker.cfg
)

catkin_package()

## Specify additional locations of header files
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${orocos_kdl_INCLUDE_DIRS}
  ${BFL_INCLUDE_DIRS}
  ${EIGEN_INCLUDE_DIRS}
  ${BULLET_INCLUDE_DIRS}
)

link_directories(${BFL_LIBRARY_DIRS})

add_definitions(${EIGEN_DEFINITIONS})

add_library(hungarian
              include/cob_leg_detection/libhungarian/hungarian.c
            )

add_library(dual_people_leg_tracker
            src/advanced_tracker_particle.cpp
            src/people_particle_filter.cpp
            src/models/occlusion_model.cpp
            src/models/advanced_measmodel_pos.cpp
            src/models/advanced_sysmodel_pos_vel.cpp
            src/distributions/multivariate_gaussian_pos_vel.cpp
            src/distributions/gaussian_pos_vel_mod.cpp
            src/leg_feature.cpp
            src/people_tracker.cpp
            src/kalman/kalman_filter.cpp
            src/association/association.cpp
            )

## Declare a cpp executable
add_executable(leg_detection               
               src/laser_processor.cpp
               src/leg_detector.cpp 
               src/calc_leg_features.cpp)

add_executable(dual_tracker
                  src/dual_tracker.cpp 
               )
               
## Add cmake target dependencies of the executable/library
add_dependencies(leg_detection ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(dual_tracker ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})


## Specify libraries to link a library or executable target against
target_link_libraries(leg_detection
   ${catkin_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${orocos_kdl_LIBRARIES}
   ${BFL_LIBRARIES}
)
target_link_libraries(dual_tracker dual_people_leg_tracker ${catkin_LIBRARIES} ${BFL_LIBRARIES} ${BULLET_LIBRARIES} ${OpenMP_CXX_LIBRARIES} hungarian)


#############
## Install ##
#############
## Mark executables and/or libraries for installation
install(TARGETS leg_detection
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
	FILES_MATCHING PATTERN "*.h"
	PATTERN ".svn" EXCLUDE
)

install(FILES config/trained_leg_detector.yaml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)
