from Comunicaciones import *
from robodk.robolink import *  
from robodk.robomath import *  
from time import sleep
import threading

lock = threading.Lock()

def Manejo_robodk(Coms:Comunicaciones,RDK):
    Repetido = False
    Hondarob2 = RDK.Item('Honda1Robot2', ITEM_TYPE_PROGRAM)
    Chopperrob2 = RDK.Item('Chopper1Robot2', ITEM_TYPE_PROGRAM)
    Hondarob1rep = RDK.Item('Honda2Robot1', ITEM_TYPE_PROGRAM)
    Chopperrob1rep = RDK.Item('Chopper2Robot1', ITEM_TYPE_PROGRAM)
    Hondarob1 = RDK.Item('Honda1Robot1', ITEM_TYPE_PROGRAM)
    Chopperrob1 = RDK.Item('Chopper1Robot1', ITEM_TYPE_PROGRAM)
    while True:
        sleep(0.5)
        if Coms.cambio == False:
            continue
        lock.acquire()
        if Coms.Robot2.usado == False and RDK.getParam("Robot2Operando") == 0:
            Coms.Robot2.usado = True
            RDK.setParam("Robot2Operando",1)
            RDK.setParam("ColorElegido2",Coms.Robot2.color_chasis)
            if Coms.Robot2.tipo_chasis == 1:
                if Hondarob2.Valid():
                    Hondarob2.RunProgram()
                sleep(0.5)
            elif Coms.Robot2.tipo_chasis == 2:
                if Chopperrob2.Valid():
                    Chopperrob2.RunProgram()
                sleep(0.5)
        if Coms.Robot1.tipo_chasis == Coms.Robot2.tipo_chasis:
            Repetido = True
        else:
            Repetido = False
        if Coms.Robot1.usado == False and RDK.getParam("Robot1Operando") == 0:
            Coms.Robot1.usado = True
            RDK.setParam("Robot1Operando",1)
            RDK.setParam("ColorElegido1",Coms.Robot1.color_chasis)
            if Repetido == True:
                if Coms.Robot1.tipo_chasis == 1 :
                    if Hondarob1rep.Valid():
                        Hondarob1rep.RunProgram()
                        continue
                elif Coms.Robot1.tipo_chasis == 2:
                    if Chopperrob1rep.Valid():
                        Chopperrob1rep.RunProgram()   
                        continue             
            if Coms.Robot1.tipo_chasis == 1 :
                if Hondarob1.Valid():
                    Hondarob1.RunProgram()
                    continue
            elif Coms.Robot1.tipo_chasis == 2:
                if Chopperrob1.Valid():
                    Chopperrob1.RunProgram()
                    continue
        lock.release()   

def Mandar_situacion(Coms:Comunicaciones,RDK):
    lock.acquire()
    Coms.enviar_estado_robots((not RDK.getParam("Robot1Operando")),(not RDK.getParam("Robot2Operando")))
    lock.release()
    sleep(1)

broker = "mqtt.dsic.upv.es"
topic = "giirob/pr2/ironline/python"
username = "giirob"
password = "UPV2024"

# broker = "broker.emqx.io"
# topic = "pruebas/placa_cojones/python"
# username = None
# password = None

if __name__ == "__main__":
    
    comunicaciones = Comunicaciones(broker,topic,username,password)
    comunicaciones.conectar()
    RDK = Robolink()
    Reset = RDK.Item('Reset', ITEM_TYPE_PROGRAM)
    if Reset.Valid():
        Reset.RunProgram()
    RDK.setParam("Robot1Operando",0)
    RDK.setParam("Robot2Operando",0)
    RDK.setParam("ColorElegido1",0)
    RDK.setParam("ColorElegido2",0)
    
    comunicaciones.enviar_estado_robots(True, True) 
    thread_manejo = threading.Thread(target=Manejo_robodk, args=(comunicaciones, RDK))
    thread_manejo.start()
    while True:
        try:
            Mandar_situacion(comunicaciones,RDK)
        except KeyboardInterrupt:
            thread_manejo.join()
            print("Interrumpido por el usuario.")
            break
    
    
