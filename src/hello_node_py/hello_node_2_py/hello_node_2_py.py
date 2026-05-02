import rclpy
from rclpy.node import Node

class HelloNodePy(Node):
    def __init__(self):
        super().__init__('hello_node_2_py')
        self.get_logger().info('Hello 2🐱🐱2!')

def main():
    rclpy.init()
    node = HelloNodePy()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
