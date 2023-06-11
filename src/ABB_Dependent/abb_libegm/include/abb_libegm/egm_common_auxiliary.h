/***********************************************************************************************************************
 *
 * Copyright (c) 2015, ABB Schweiz AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 *
 *    * Redistributions of source code must retain the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer.
 *    * Redistributions in binary form must reproduce the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer in the documentation
 *      and/or other materials provided with the
 *      distribution.
 *    * Neither the name of ABB nor the names of its
 *      contributors may be used to endorse or promote
 *      products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***********************************************************************************************************************
 */

#ifndef EGM_COMMON_AUXILIARY_H
#define EGM_COMMON_AUXILIARY_H

#include "abb_libegm_export.h"

#include "egm.pb.h"         // Generated by Google Protocol Buffer compiler protoc
#include "egm_wrapper.pb.h" // Generated by Google Protocol Buffer compiler protoc

#include "egm_common.h"

namespace abb
{
namespace egm
{
/**
 * \brief Saturate a specified value between a lower and a upper bound.
 *
 * \param value for the value to saturate.
 * \param lower for the lower bound.
 * \param upper for the upper bound.
 *
 * \return double with the saturated value.
 */
double saturate(const double value, const double lower, const double upper);

/**
 * \brief Multiply all values in a joints object with a factor.
 *
 * \param p_j for the joints object.
 * \param factor for the value to multiply with.
 */
void multiply(wrapper::Joints* p_j, const double factor);

/**
 * \brief Multiply all values in a Cartesian object with a factor.
 *
 * \param p_c for the Cartesian object.
 * \param factor for the value to multiply with.
 */
void multiply(wrapper::Cartesian* p_c, const double factor);

/**
 * \brief Multiply all values in an Euler object with a factor.
 *
 * \param p_e for the Euler object.
 * \param factor for the value to multiply with.
 */
void multiply(wrapper::Euler* p_e, const double factor);

/**
 * \brief Multiply all values in a quaternion object with a factor.
 *
 * \param p_q for the quaternion object.
 * \param factor for the value to multiply with.
 */
void multiply(wrapper::Quaternion* p_q, const double factor);

/**
 * \brief Multiply two quaternions together.
 *
 * \param q1 for the first quaternion.
 * \param q2 for the second quaternion.
 *
 * \return Quaternion containing the result.
 */
wrapper::Quaternion multiply(const wrapper::Quaternion& q1, const wrapper::Quaternion& q2);

/**
 * \brief Calculate the dot product between two quaternions.
 *
 * \param q1 for the first quaternion.
 * \param q2 for the second quaternion.
 *
 * \return double containing the dot product value.
 */
double dotProduct(const wrapper::Quaternion& q1, const wrapper::Quaternion& q2);

/**
 * \brief Calculate the Euclidean norm of a quaternion.
 *
 * \param q for the quaternion to calculate the norm of.
 *
 * \return double containing the Euclidean norm value.
 */
double euclideanNorm(const wrapper::Quaternion& q);

/**
 * \brief Normalize a quaternion.
 *
 * \param p_q for the quaternion to normalize.
 */
void normalize(wrapper::Quaternion* p_q);

/**
 * \brief Convert ZYX Euler angles to a quaternion.
 *
 * \param p_q for containing the calculated quaternion.
 * \param e for the ZYX Euler angles to convert.
 */
void convert(wrapper::Quaternion* p_q, const wrapper::Euler& e);

/**
 * \brief Convert a quaternion to ZYX Euler angles.
 *
 * \param p_e for containing the calculated ZYX Euler angles.
 * \param q for the quaternion to convert.
 */
void convert(wrapper::Euler* p_e, const wrapper::Quaternion& q);

/**
 * \brief Convert angular velocities to quaternion derivate.
 *
 * \param p_dq for containing the calculated quaternion derivate.
 * \param previous_q for the previous quaternion.
 * \param av for the angular velocities to convert.
 */
void convert(wrapper::Quaternion* p_dq, const wrapper::Quaternion& previous_q, const wrapper::Euler& av);

/**
 * \brief Estimate joint velocities.
 *
 * The estimation will fail if the sample time is less than or equal to zero.
 *
 * \param p_estimate for containing the estimated joint velocities.
 * \param current current joint positions.
 * \param previous previous joint positions.
 * \param sample_time current sample time.
 *
 * \return bool indicating if the estimation succeeded.
 */
bool estimateVelocities(wrapper::Joints* p_estimate,
                        const wrapper::Joints& current,
                        const wrapper::Joints& previous,
                        const double sample_time);

/**
 * \brief Estimate Cartesian angular velocities.
 *
 * The estimation will fail if the sample time is less than or equal to zero.
 *
 * \param p_estimate for containing the estimated Cartesian angular velocities.
 * \param current current quaternion.
 * \param previous previous quaternion.
 * \param sample_time current the sample time.
 *
 * \return bool indicating if the estimation succeeded.
 */
bool estimateVelocities(wrapper::Euler* p_estimate,
                        const wrapper::Quaternion& current,
                        const wrapper::Quaternion& previous,
                        const double sample_time);

/**
 * \brief Estimate Cartesian velocities.
 *
 * The estimation will fail if the sample time is less than or equal to zero.
 *
 * \param p_estimate for containing the estimated Cartesian velocities.
 * \param current current Cartesian position and orientation.
 * \param previous previous Cartesian position and orientation.
 * \param sample_time current the sample time.
 *
 * \return bool indicating if the estimation succeeded.
 */
bool estimateVelocities(wrapper::CartesianVelocity* p_estimate,
                        const wrapper::CartesianPose& current,
                        const wrapper::CartesianPose& previous,
                        const double sample_time);

/**
 * \brief Find the maximum difference between two joints objects.
 *
 * The maximum difference is determined by the absolute difference for each joint pair.
 * The number of joints therefore needs to be the same, and the components should represent
 * the same robot joints or external axes.
 *
 * \param j1 for the first joints object.
 * \param j2 for the second joints object.
 *
 * \return double with the maximum difference.
 */
double findMaxDifference(const wrapper::Joints& j1, const wrapper::Joints& j2);

/**
 * \brief Find the maximum difference between two Cartesian objects.
 *
 * The maximum difference is determined by the absolute difference for each position pair.
 * The components should represent the position of the same robot (in the same frame of reference).
 *
 * \param c1 for the first Cartesian object.
 * \param c2 for the second Cartesian object.
 *
 * \return double with the maximum difference.
 */
double findMaxDifference(const wrapper::Cartesian& c1, const wrapper::Cartesian& c2);

/**
 * \brief Find the maximum difference between two Euler objects.
 *
 * The maximum difference is determined by the absolute difference for each orientation pair.
 * The components should represent the orientation of a robot (in the same frame of reference).
 *
 * \param e1 for the first Euler object.
 * \param e2 for the second Euler object.
 *
 * \return double with the maximum difference.
 */
double findMaxDifference(const wrapper::Euler& e1, const wrapper::Euler& e2);

/**
 * \brief Copy the data present in one joints object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Joints* p_target, const wrapper::Joints& source);

/**
 * \brief Copy the data present in one joint space object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::JointSpace* p_target, const wrapper::JointSpace& source);

/**
 * \brief Copy the data present in one Cartesian object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Cartesian* p_target, const wrapper::Cartesian& source);

/**
 * \brief Copy the data present in one Euler object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Euler* p_target, const wrapper::Euler& source);

/**
 * \brief Copy the data present in one quaternion object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Quaternion* p_target, const wrapper::Quaternion& source);

/**
 * \brief Copy the data present in one Cartesian pose object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::CartesianPose* p_target, const wrapper::CartesianPose& source);

/**
 * \brief Copy the data present in one Cartesian velocity object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::CartesianVelocity* p_target, const wrapper::CartesianVelocity& source);

/**
 * \brief Copy the data present in one Cartesian space object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::CartesianSpace* p_target, const wrapper::CartesianSpace& source);

/**
 * \brief Copy the data present in one robot object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Robot* p_target, const wrapper::Robot& source);

/**
 * \brief Copy the data present in one external object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::External* p_target, const wrapper::External& source);

/**
 * \brief Copy the data present in one output object to another.
 *
 * \param p_target for the target where the copied data should be put.
 * \param source containing the data to copy.
 */
void copyPresent(wrapper::Output* p_target, const wrapper::Output& source);

/**
 * \brief Parse an abb::egm::EgmHeader object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Header* p_target, const EgmHeader& source);

/**
 * \brief Parse an abb::egm::EgmRobot object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Status* p_target, const EgmRobot& source);

/**
 * \brief Parse an abb::egm::EgmClock object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Clock* p_target, const EgmClock& source);

/**
 * \brief Parse two abb::egm::EgmJoints objects.
 *
 * \param p_target_robot for containing the parsed robot data.
 * \param p_target_external for containing the parsed external data.
 * \param source_robot containing robot data to parse.
 * \param source_external containing external data to parse.
 * \param axes specifying the number of axes of the robot.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Joints* p_target_robot,
           wrapper::Joints* p_target_external,
           const EgmJoints& source_robot,
           const EgmJoints& source_external,
           const RobotAxes axes);

/**
 * \brief Parse an abb::egm::EgmPose object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::CartesianPose* p_target, const EgmPose& source);

/**
 * \brief Parse an abb::egm::EgmFeedBack object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 * \param axes specifying the number of axes of the robot.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Feedback* p_target, const EgmFeedBack& source, const RobotAxes axes);

/**
 * \brief Parse an abb::egm::Planned object.
 *
 * \param p_target for containing the parsed data.
 * \param source containing data to parse.
 * \param axes specifying the number of axes of the robot.
 *
 * \return bool indicating if the parsing was successful or not.
 */
bool parse(wrapper::Planned* p_target, const EgmPlanned& source, const RobotAxes axes);

/**
 * \brief Reset all values (i.e. set to zero) in a joints object.
 *
 * \param p_joints for the object to reset.
 * \param number_of_joints for the desired number of joints to reset.
 */
void reset(wrapper::Joints* p_joints, const unsigned int number_of_joints);

/**
 * \brief Reset all values (i.e. set to zero) in a Cartesian object.
 *
 * \param p_cartesian for the object to reset.
 */
void reset(wrapper::Cartesian* p_cartesian);

/**
 * \brief Reset all values (i.e. set to zero) in a Euler object.
 *
 * \param p_euler for the object to reset.
 */
void reset(wrapper::Euler* p_euler);

/**
 * \brief Verify that a double value is neither NaN nor infinity.
 *
 * \param value to check.
 *
 * \return bool true if the value is ok.
 */
bool verify(const double value);

/**
 * \brief Verify that a joints object contain neither NaN nor infinity values.
 *
 * \param joints to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::Joints& joints);

/**
 * \brief Verify that a Cartesian object contain neither NaN nor infinity values.
 *
 * \param cartesian to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::Cartesian& cartesian);

/**
 * \brief Verify that a Euler object contain neither NaN nor infinity values.
 *
 * \param euler to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::Euler& euler);

/**
 * \brief Verify that a quaternion object contain neither NaN nor infinity values.
 *
 * \param quaternion to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::Quaternion& quaternion);

/**
 * \brief Verify that a Cartesian pose object contain neither NaN nor infinity values.
 *
 * \param pose to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::CartesianPose& pose);

/**
 * \brief Verify that a Cartesian velocity object contain neither NaN nor infinity values.
 *
 * \param velocity to check.
 *
 * \return bool true if the values are ok.
 */
bool verify(const wrapper::CartesianVelocity& velocity);

} // end namespace egm
} // end namespace abb

#endif // EGM_COMMON_AUXILIARY_H