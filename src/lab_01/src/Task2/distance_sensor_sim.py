#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64
import random

class DistanceSensorNode(Node):
    def __init__(self):
        super().__init__('distance_sensor_sim')
        self.publisher_ = self.create_publisher(Float64, 'sensor_distance', 10)
        
        self.distance = 2.5
        self.MIN_VAL = 0.03
        self.MAX_VAL = 5.0
        
        self.timer = self.create_timer(0.1, self.timer_callback)

    def timer_callback(self):
        self.distance += random.uniform(-0.1, 0.1)
        self.distance = max(self.MIN_VAL, min(self.MAX_VAL, self.distance))

        msg = Float64()
        msg.data = self.distance
        self.publisher_.publish(msg)
        
        self.get_logger().info(f'Distance: {self.distance:.3f} m')

def main(args=None):
    rclpy.init(args=args)
    node = DistanceSensorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()