#!/usr/bin/env python3
import rospy
from std_msgs.msg import  Bool, Int32
from helper.robot_helper import *
from enums.nodes import Nodes
from enums.topics import Topics
from enums.operations import OPERATIONS
from geometry_msgs.msg import WrenchStamped
from CentralNode.msg import node_response
from enums.response_status import Response
from enums.services import Services
from CentralNode.srv import ScrewList, ScrewListResponse, ScrewListRequest
import json
import numpy as np
from CentralNode.srv import ScrewList
class ApprochAndEngaging:
    def __init__(self,tourqeToStop) -> None:
        self.RobotJoystick = RobotControl(node_name=Nodes.APPROACH_AND_ENGAGE.value,group_name="ScrewIn")
        self.TransformCalculator = frames_transformations()
        #editabel parameters
        self.unscrewNo = OPERATIONS.index(Nodes.UNSCREW)
        self.millingNo = OPERATIONS.index(Nodes.MILLING)
        self.UnscrewFlag = False
        self.engageFlag  = False
        self.SensorRead = WrenchStamped()
        self.NodeToOperate = 0
        self.EngageTourqe = tourqeToStop
        self.BadScrews = []
        self.listOfScrews = []
        #define publishers 
        self.Motor=rospy.Publisher(Topics.ScrewDriverMOTOR_COMMAND.value, Int32, queue_size=1)
        self.startUnscrewing = rospy.Publisher(Topics.UNSCREW_START_FLAG.value, Bool , queue_size=1)
        self.startMilling = rospy.Publisher(Topics.START_MILLING.value, Bool , queue_size=1)        
        self.NodeSuccess=rospy.Publisher(Topics.NODE_SUCCESS.value, node_response, queue_size=1)
        #define subscribers
        rospy.Subscriber(Topics.UNSCREW_DONE.value, Bool, self.unscrewDoneCallback)
        rospy.Subscriber(Topics.ForceSensorWrenchWeightedFilter.value, WrenchStamped, self.SensorCallback)
        rospy.Subscriber(Topics.NODE_TO_OPERATE.value, Int32, self.NodeToOperateCallback)
        #motor commands 
        # 0: stop
        # 1: for rotate clockwise
        self.on = 1
        self.off = 0
        self.motorCommands = Int32()
    def Approach(self,Pose,velocity,acceleration)->None:
        '''
        this function will go to the give cordinates path
        parameters:
        Pose: the pose of the path
        @type Pose: list [x,y,z,roll,pitch,yaw]
        '''
        self.RobotJoystick.go_to_pose_goal_cartesian(Pose,velocity=velocity,acceleration=acceleration,Replanning=True,WaitFlag=False)
        pass

    def engage(self,timeStep,NowScrew,PostionTolerance)->None:
        '''
        this function will generate and execute the spiral shape
        parameters:
        step: the step of the time shape
        @type step: float
        '''
        #constant parameters for spiral shape for M5 cross screw
        recess=0.005
        fs=2*recess
        N_s=60
        fixederror=0.002 #(mm)
        #end
        waypoints = []
        PostionTolerance = 0.001
        #calculate the parameters of the spiral
        tmax=int(((10*math.pi)/N_s)*(math.ceil(N_s/2)))
        #generate a list of screws in spiral shape
        pose = self.RobotJoystick.get_pose()
        xlast,ylast=pose[0],pose[1]
        for i in np.arange(timeStep,tmax,timeStep):
            t=i
            x,y=pose[0]+(((fs/math.pi)*math.sqrt(((8*math.pi*N_s*t)/15))*math.cos(math.sqrt(((8*math.pi*N_s*t)/15))))*0.1),pose[1]+(((fs/math.pi)*math.sqrt(((8*math.pi*N_s*t)/15))*math.sin(math.sqrt(((8*math.pi*N_s*t)/15))))*0.1)
            if (math.sqrt(((x-xlast)**2)+((y-ylast)**2)))>=PostionTolerance:
                waypoints.append([x,y,pose[2],pose[3],pose[4],pose[5]])
                xlast,ylast=x,y
        self.OperateMotor()#operate the screw driver motor
        self.RobotJoystick.go_to_pose_goal_cartesian_waypoints(waypoints,velocity=0.1,acceleration=0.1,list_type=True,waitFlag=False,positionTolerance=PostionTolerance)
        while True:
            if self.SensorRead.wrench.torque.z >= self.EngageTourqe:
                self.stopMotor()
                self.engageFlag = True
                print("engaged")
                break
            #check that the robot reached the last point
            currentPose = self.RobotJoystick.get_pose()
            if all(abs(currentPose[i]-waypoints[-1][i])<PostionTolerance for i in range(len(currentPose))):
                print("broken")
                self.stopMotor()
                self.BadScrews.append(NowScrew)
                self.engageFlag = False
                break
        #end

    def reshapeList(self,ListOfscrews)->list:
        #reshaping the list of screws to 2D (nx6) list array
        return np.reshape(ListOfscrews,(-1,6)).tolist()

    def unscrewDoneCallback(self,msg:Bool)->None:
        #self.UnscrewFlag = msg.data
        pass

    def SensorCallback(self,msg:WrenchStamped)->None:
        self.SensorRead = msg

    def OperateMotor(self)->None:
        self.motorCommands.data = self.on
        while self.Motor.get_num_connections() < 1:
            #ensure that the publisher is connected
            rospy.sleep(0.1)
        self.Motor.publish(self.motorCommands)

    def stopMotor(self):
        self.motorCommands.data = self.off
        while self.Motor.get_num_connections() < 1:
            #ensure that the publisher is connected
            rospy.sleep(0.1)
            pass
        self.Motor.publish(self.motorCommands)

    def NodeToOperateCallback(self,msg):
        self.NodeToOperate = msg.data

    def unscrew(self, truePos ,index):
        self.startUnscrewing.publish(True)
        receivedFlag = rospy.wait_for_message(Topics.UNSCREW_DONE.value, Bool)
        self.UnscrewFlag = receivedFlag.data
        if not receivedFlag.data:
            self.BadScrews.append(truePos)
        else:
            status = node_response()
            status.nodeId = self.unscrewNo
            status.status = Response.IN_PROGRESS.value
            status.extraMessage= str(index)
            self.NodeSuccess.publish(status)

    def mill(self,index):
        self.startMilling.publish(True)
        receivedFlag = rospy.wait_for_message(Topics.FINISHED_MILLING.value, Bool)
        states=node_response()
        states.nodeId=self.millingNo
        states.status=Response.IN_PROGRESS.value
        states.extraMessage=str(index)
        self.NodeSuccess.publish(states)


    def goUpScrews(self,upthersold=0.02):
        pose=self.RobotJoystick.get_pose()
        self.RobotJoystick.go_to_pose_goal_cartesian([pose[0],pose[1],pose[2]+upthersold,pose[3],pose[4],pose[5]],velocity=0.1,acceleration=0.1,Replanning=True,WaitFlag=False)
        pass
    def main(self):
        while not rospy.is_shutdown():
            if self.NodeToOperate == self.unscrewNo:
                #initiate the screw list client
                rospy.wait_for_service(Services.GET_SCREW_LIST.value)
                ScrewResponse = rospy.ServiceProxy(Services.GET_SCREW_LIST.value,ScrewList)
                self.listOfScrews=self.reshapeList(ScrewResponse.screwList)
                for index,screw in enumerate(self.listOfScrews):
                    self.Approach(screw,velocity=0.1,acceleration=0.1)
                    self.engage(timeStep=0.01,NowScrew=screw,PostionTolerance=0.001)
                    #end
                    if (self.engageFlag):
                        TrueScrewPosition=self.RobotJoystick.get_pose()
                        self.unscrew(truePos=TrueScrewPosition,index=index)
                        self.engageFlag = False
                    self.goUpScrews(upthersold=0.02)
                    #end of one screw cycle
                state=node_response()
                state.nodeId=self.unscrewNo
                state.status=Response.SUCCESSFULL.value
                state.extraMessage=json.dumps(self.BadScrews)
                self.NodeSuccess.publish(state)

            if self.NodeToOperate == self.millingNo:
                rospy.wait_for_service(Services.GET_SCREW_LIST.value)
                ScrewResponse = rospy.ServiceProxy(Services.GET_SCREW_LIST.value,ScrewList)
                self.listOfScrews=self.reshapeList(ScrewResponse.screwList)
                #initiate the screw list client
                for index,screw in enumerate(self.listOfScrews):
                    self.Approach(screw,velocity=0.1,acceleration=0.1)
                    self.mill(index)
                    self.goUpScrews(upthersold=0.02)
                    #end of one screw cycle
                pass
                state=node_response()
                state.nodeId=self.millingNo
                state.status=Response.SUCCESSFULL.value
                self.NodeSuccess.publish(state)  
            

test=ApprochAndEngaging()
waysTest= [0.3935, -0.1186, 0.2469-0.005, -3.1232910411003725, -0.019481121950260524, 0.07178239976862723]

# test.Approach(waysTest,velocity=0.1,acceleration=0.1)
# # test.Spiralshape(0.01)
test.OperateMotor()