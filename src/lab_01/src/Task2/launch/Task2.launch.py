# launch/Task2.launch.py
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='lab_01',
            executable='distance_sensor_sim.py',
            name='distance_sensor_sim'
        ),
        Node(
            package='lab_01',
            executable='emergency_stop.py',
            name='emergency_stop_node'
        )
    ])
