import json

from flask import Flask
from gevent.pywsgi import WSGIServer
app = Flask(__name__)
from flask_cors import CORS
cors = CORS(app, resources={r"/*": {"origins": "*"}})

import  psutil
import os
import wmi



def pc_get_stats():
    # usage
    memory_usage = psutil.virtual_memory()[2]
    cpu_usage=psutil.cpu_percent(interval=1)

    #GPU TEMPS
    w = wmi.WMI(namespace="root\OpenHardwareMonitor")
    temperature_infos = w.Sensor()
    gpu_temp = 0
    for sensor in temperature_infos:
        if sensor.SensorType == 'Temperature' and sensor.Name=='GPU Hot Spot':
            gpu_temp = sensor.Value
            break
    data = {"memory_usage": memory_usage,"cpu_usage":cpu_usage,"cpu_temp":gpu_temp}
    return data
@app.route('/shutdown', methods=['GET'])
def shutdown():
    os.system("shutdown /s /t 1")
    return "Success", 200, {"Access-Control-Allow-Origin": "*"}

@app.route('/reboot', methods=['GET'])
def reboot():
    os.system("shutdown /r /t 1")
    return "Success", 200, {"Access-Control-Allow-Origin": "*"}

@app.route('/stats', methods=['GET'])
def pc_stats():
    data = pc_get_stats()
    return json.dumps(data)

if __name__ == '__main__':
    # Debug/Development
    # app.run(debug=True, host="0.0.0.0", port="5000")
    # Production
    http_server = WSGIServer(('192.168.50.3', 2000), app)
    http_server.serve_forever()