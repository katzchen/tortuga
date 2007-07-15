from ai.modernAI.newStateMachine import stateMachine
from module import Module,ModuleManager

class aiStateMachine(stateMachine):
    def __init__(self,args = None,interFuncs = None,interStates = None):
        self.controller = ModuleManager.get().get_module("Controller")
        self.vehicle = ModuleManager.get().get_module("Vehicle")
        self.vision = ModuleManager.get().get_module("Vision")

        stateMachine.__init__(self,args,interFuncs,interStates)
    
    def startState(self,args,interFuncs,interStates):
        raise Exception("start state needs to be defined")