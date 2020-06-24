from flask import Flask
app = Flask(__name__)
from flask import request
import os

def getPickupLocation(currentBookingInfo):
    print("getPickupLocation set: ", [currentBookingInfo["fromLocation_lat"], currentBookingInfo["fromLocation_lng"]])
    return([currentBookingInfo["fromLocation_lat"], currentBookingInfo["fromLocation_lng"]])

def getFinalLocation(currentBookingInfo):
    print("getFinalLocation set: ", [currentBookingInfo["toLocation_lat"], currentBookingInfo["toLocation_lng"]])
    return([currentBookingInfo["toLocation_lat"], currentBookingInfo["toLocation_lng"]])

def startNavigator(destination):
    programCommand = "rosrun simple_navigation_goals simple_navigation_goals  _param_x:="+str(destination[0])+" _param_y:="+str(destination[1])
    if os.system(programCommand) == 0:
        return "True"
    else:
        return "False"

@app.route('/movebase')
def movebase():
    isAuth = request.args.get('resp')
    isAuth2 = request.args.get('resp1')
    var=[float(isAuth),float(isAuth2)]
    print(var)
    # isAuth= request.args.get('resp2')
    # pickupLocation = getPickupLocation(isAuth)
    # dropoffLocation = getFinalLocation(isAuth)

    resp = startNavigator(var)
    
    # return resp
    return resp
   

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=2015)
