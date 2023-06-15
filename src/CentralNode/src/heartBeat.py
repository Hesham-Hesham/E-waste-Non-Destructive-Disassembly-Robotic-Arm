#!/usr/bin/env python3
import rospy
import roslaunch
import rosnode
from enums.nodes import Nodes
class HeartBeat:
    '''
    Objective : This class is used to check if the robot is running or not
                If the robot is not running and in automatic Mode then it will start the robot
    -------------------------------------
    Attributes: None
    '''
    def __init__(self,heartRate) -> None:
        '''
        @param heartRate : the interval at which the heartbeat should be checked
        @type heartRate : int (seconds)
        '''

        '''the following line disable rossignal handler so we can launch form callback'''
        roslaunch.pmon._init_signal_handlers = self.dummy_function
        '''end'''
        rospy.init_node(Nodes.HeartBeat.value)
        #initiate the roslaunch api
        self.launch = roslaunch.scriptapi.ROSLaunch()
        self.launch.start()
        #end
        self.HeartRate = heartRate
        #interval checker for the all nodes 
        rospy.Timer(rospy.Duration(self.HeartRate), self.NodesHeartBeat)
        #list all the runnung nodes
        self.NodesExist = Nodes.NodesToBeOperated.value
        self.NodeInfo = Nodes.NodesToBeOperatedInfo.value
        pass

    def NodesHeartBeat(self,event)->None:
        Operated , _ = rosnode.rosnode_ping_all()
        #remove the '/' from the node name
        for node in Operated:
            Operated[Operated.index(node)] = node.replace("/","")
        print("Nodes that are running {}".format(Operated))
        #see the which nodes are not running
        not_running = list(set(self.NodesExist).difference(Operated))
        print("Nodes that are not running {}".format(not_running))
        #start the nodes that are not running by looping through the list
        for node in not_running:
            print("Node {} is not running".format(node))
            node = roslaunch.core.Node(package=self.NodeInfo[node][0],node_type=self.NodeInfo[node][1],name =self.NodeInfo[node][2])
            process=self.launch.launch(node)
            print("Node {} is started".format(node))
    def dummy_function(self)->None: 
        '''
        Objective : This is a dummy function to disable the rossignal handler
        '''
        pass
HeartBeat=HeartBeat(heartRate = 10)#heartRate in seconds
rospy.spin() 
Heart=HeartBeat(heartRate = 10)#heartRate in seconds
while not rospy.is_shutdown():
    rospy.sleep(1)
    pass
