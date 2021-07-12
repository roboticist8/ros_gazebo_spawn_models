#! /usr/bin/env python
import rospy
import rospkg
from gazebo_msgs.srv import SpawnModel, SpawnModelRequest, SpawnModelResponse
from geometry_msgs.msg import Pose , Quaternion
from tf.transformations import quaternion_from_euler

rospy.init_node('spawm_sdf',log_level=rospy.INFO)

spawn_model_proxy = rospy.ServiceProxy('gazebo/spawn_sdf_model', SpawnModel)

r = rospkg.RosPack()
file_path = r.get_path('spawn_urdf_sdf')
file_path = file_path+'/models/aruco_marker0/model.sdf'

f= open(file_path,'r')
sdf = f.read()
f.close()
# print(sdf)


initial_pose = Pose()
initial_pose.position.x = 1
initial_pose.position.y = 0.2
initial_pose.position.z = 0.3
orint = quaternion_from_euler(0, 1.57, 0)
initial_pose.orientation = Quaternion(orint[0],orint[1],orint[2],orint[3])


spawn_request = SpawnModelRequest()
spawn_request.model_name = 'marker'
spawn_request.model_xml = sdf
spawn_request.reference_frame = 'world'
spawn_request.initial_pose = initial_pose

spawn_resp = SpawnModelResponse()
rospy.wait_for_service('gazebo/spawn_sdf_model')
spawn_resp = spawn_model_proxy(spawn_request)
rospy.loginfo('Model spawn {},\n{}'.format(spawn_resp.success, spawn_resp.status_message))
