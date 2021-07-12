#include <gazebo_msgs/SpawnModel.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <fstream>

std::string getFile(std::string filename)
{
  std::string buffer;
  char c;

  std::ifstream in(filename);
  if (!in)
  {
    std::cout << filename << " not found";
    exit(1);
  }
  while (in.get(c))
    buffer += c;
  in.close();

  return buffer;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sdf_spawner");
  ros::NodeHandle nh;

  std::string path = ros::package::getPath("spawn_urdf_sdf");  // add roslib to cmakelists in find_package
  std::string file_path = path + "/models/aruco_marker0/model.sdf";
  // std::cout << file_path;

  std::string sdf = getFile(file_path);
  //   std::cout << sdf;

  ros::ServiceClient spawnClient = nh.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
  gazebo_msgs::SpawnModel::Request spawn_model_req;
  gazebo_msgs::SpawnModel::Response spawn_model_resp;

  spawn_model_req.initial_pose.position.x = 2;
  spawn_model_req.initial_pose.position.y = 2;
  spawn_model_req.initial_pose.position.z = 0.2;
  spawn_model_req.initial_pose.orientation.x = 0.0;
  spawn_model_req.initial_pose.orientation.y = 0.0;
  spawn_model_req.initial_pose.orientation.z = 0.0;
  spawn_model_req.initial_pose.orientation.w = 1.0;
  spawn_model_req.reference_frame = "world";
  spawn_model_req.model_name = "marker";
  spawn_model_req.model_xml = sdf;

  bool call_service = spawnClient.call(spawn_model_req, spawn_model_resp);
  if (call_service)
  {
    if (spawn_model_resp.success)
    {
      ROS_INFO_STREAM("model has been spawned");
    }
    else
    {
      ROS_INFO_STREAM("model spawn failed");
    }
  }
  else
  {
    ROS_INFO("fail in first call");
    ROS_ERROR("fail to connect with gazebo server");
    return 0;
  }
}
