#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64, Bool

class EmergencyStopNode(Node):
    def __init__(self):
        super().__init__('emergency_stop_node')
        self.subscription = self.create_subscription(
            Float64,
            'sensor_distance', 
            self.listener_callback,
            10)
        self.publisher_ = self.create_publisher(Bool, 'cmd/stop', 10)

    def listener_callback(self, msg):
        stop_msg = Bool()
        
        if msg.data < 2.0:
            stop_msg.data = True
        else:
            stop_msg.data = False
            
        self.publisher_.publish(stop_msg)
        
        self.get_logger().info(f'Distance: {msg.data:.2f} | Stop Command: {stop_msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = EmergencyStopNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()