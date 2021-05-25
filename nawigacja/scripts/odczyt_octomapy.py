import rospy
from octomap_msgs.msg import Octomap

def callback(msg):
    print("wiadomosc z octomapy  len int8[] data: {}".format(len(msg.data)))
    print("wiadomosc z octomapy frame id: {}".format(msg.id))
    print("wiadomosc z octomapy resolution: {}".format(msg.resolution))
    print("wiadomosc z octomapy binary: {}".format(msg.binary))
    print("wiadomosc z octomapy int8[1]: {}".format(msg.data[100]))
    print("-------------------------------------------------------------------")

rospy.init_node('sub_node',anonymous=True)
sub = rospy.Subscriber('/octomap_binary', Octomap, callback) # subscribe data from octomap_binary topic
rospy.spin()