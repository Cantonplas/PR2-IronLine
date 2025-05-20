from main import *
from Comunicaciones import *

comunicaciones = Comunicaciones()
RDK = Robolink()
Reset = RDK.Item('Reset', ITEM_TYPE_PROGRAM)
if Reset.Valid():
        Reset.RunProgram()
RDK.setParam("Robot1Operando",0)
RDK.setParam("Robot2Operando",0)
RDK.setParam("ColorElegido1",0)
RDK.setParam("ColorElegido2",0)

comunicaciones.cambio = True
comunicaciones.Robot1 = Solicitud(1, CodigoColores.id_rojo)
comunicaciones.Robot2 = Solicitud(1, CodigoColores.id_negro)
Manejo_robodk(comunicaciones,RDK)