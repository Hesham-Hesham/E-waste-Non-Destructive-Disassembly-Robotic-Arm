import rospy
import sys
sys.path.append(".../")
from enums.nodes import Nodes
from enums.response_status import Response
from CentralNode.srv import ScrewList, Schedular, ScrewListResponse, SchedularResponse
#from enums.nodes import Nodes
from enums.topics import Topics
from enums.services import Services
from std_msgs.msg import Int32, Bool
from CentralNode.msg import node_response as NodeResponse
from storage.db import RobotDatabase
import storage.storage_keys as StorageKeys
from enums.operations import OPERATIONS, getChangeToolIndices
from handlers.success_handler import handleSuccess
from handlers.in_progress_handler import handleInProgress


class CentralNode:
    def __init__(self):
        self.subscribers = {}
        self.publishers = {}
        self.services = {}
        self.currentNode = 0
        self.objectExists = False
        self.getScrewIndex, self.returnScrewIndex, self.getMillingIndex, self.returnMillingIndex = getChangeToolIndices()

    def initPublishers(self):
        self.publishers = {}
        self.publishers[Topics.NODE_TO_OPERATE] = rospy.Publisher(Topics.NODE_TO_OPERATE.value, Int32,queue_size=1,latch=True)
        self.publishers[Topics.START_COLLISION_DETECTED] = rospy.Publisher(Topics.START_COLLISION_DETECTED.value, Bool,queue_size=1)

    def initSubscribers(self):
        self.subscribers = {}
        self.subscribers[Topics.NODE_SUCCESS] = rospy.Subscriber(Topics.NODE_SUCCESS.value, NodeResponse, self.nodeSuccessCallback,queue_size=1)
        # self.subscribers[Topics.COLLISION_DETECTED] = rospy.Subscriber(Topics.COLLISION_DETECTED, Bool)
    
    def initServices(self):
        self.services = {}
        self.services[Services.GET_SCREW_LIST.value] = rospy.Service(Services.GET_SCREW_LIST.value, ScrewList, self.getScrewList)
        self.services[Services.SCHEDULAR.value] = rospy.Service(Services.SCHEDULAR.value, Schedular, self.getSchedular)

    def getScrewList(self):
        screwList = RobotDatabase.readFromDB(StorageKeys.LIST_OF_SCREWS)
        response = ScrewListResponse()

        screwIndex = -1
        try:
            screwIndex = RobotDatabase().readFromDB(StorageKeys.SCREW_INDEX)
        except:
            screwIndex = 0

        response.screwList = screwList[screwIndex:]
        return response
    
    def getSchedular(self):

        wayPoints, waypointTypes, checkTorque, velocity, acceleration = self.currentSchedule[StorageKeys.WAYPOINTS], self.currentSchedule[StorageKeys.WAYPOINTS_TYPES], self.currentSchedule[StorageKeys.CHECK_FORCE], self.currentSchedule[StorageKeys.VEL], self.currentSchedule[StorageKeys.ACC] 

        startIndex = -1
        try:
            startIndex = RobotDatabase().readFromDB(StorageKeys.CHANGE_TOOL_SCHEDULE_INDEX)
        except:
            startIndex = 0

        response = SchedularResponse()
        response.waypoints = wayPoints[startIndex * 6:]
        response.waypoint_types = waypointTypes[startIndex:]
        response.checkTorque = checkTorque[startIndex:]
        response.velocity = velocity[startIndex:]
        response.acceleration = acceleration[startIndex:]
        return response

    def start(self):
        rospy.init_node(Nodes.CENTRAL.value)
        self.initPublishers()
        self.initSubscribers()
        rate = rospy.Rate(10)

        while not rospy.is_shutdown():
            self.publishers[Topics.NODE_TO_OPERATE].publish(self.currentNode)
            rospy.spin()


    def config(self):
        db = RobotDatabase()
        data = db.readAllFromDB()
        if self.objectExists:
            if db.isNotEmpty():
                pass

                

    def nodeSuccessCallback(self,nodeResponse:NodeResponse):
        print(f"Node success callback {nodeResponse.status}")
        print(Response.SUCCESSFULL.value)
        if nodeResponse.status == Response.IN_PROGRESS.value:
            handleInProgress(self.currentNode, nodeResponse, self.currentSchedule)
        elif nodeResponse.status == Response.SUCCESSFULL.value:
            handleSuccess(self.currentNode, nodeResponse)
            RobotDatabase().addToDB(StorageKeys.OPERATION_DONE, self.currentNode)
            if self.currentNode < len(OPERATIONS):
                RobotDatabase().addToDB(StorageKeys.OPERATION_INPROGRESS, self.currentNode + 1)
                self.currentNode += 1
                self.setSchedule()
                self.publishers[Topics.NODE_TO_OPERATE].publish(self.currentNode)
    def setSchedule(self):
        if self.currentNode == self.getScrewIndex:
            self.currentSchedule = RobotDatabase().getScrewSchedule()
        elif self.currentNode == self.returnScrewIndex:
            self.currentSchedule = RobotDatabase().returnScrewSchedule()
        elif self.currentNode == self.getMillingIndex:
            self.currentSchedule = RobotDatabase().getMillingSchedule()
        elif self.currentNode == self.returnMillingIndex:
            self.currentSchedule = RobotDatabase().returnMillingSchedule()
        
if __name__ == '__main__':
    try:
        c = CentralNode()
        c.start()
    except rospy.ROSInterruptException:
        pass 