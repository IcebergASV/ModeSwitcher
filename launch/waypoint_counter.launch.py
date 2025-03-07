import launch
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, LogInfo
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([
        # Declare the launch argument for the target waypoint
        DeclareLaunchArgument(
            'target_wp', default_value='10', description='Target waypoint number'
        ),

        # Log the argument value (for debugging)
        LogInfo(
            condition=launch.conditions.LaunchConfigurationEquals('target_wp', '10'),
            msg="Launching waypoint counter with target waypoint 10"
        ),

        # Launch the tracker node with the given target waypoint argument
        Node(
            package='waypoint_counter',  # Your package name
            executable='tracker',        # Executable name
            name='waypoint_counter',     # Node name
            output='screen',             # Output to screen
            parameters=[{
                'target_wp': LaunchConfiguration('target_wp')  # Pass the target waypoint argument
            }]
        ),
    ])
