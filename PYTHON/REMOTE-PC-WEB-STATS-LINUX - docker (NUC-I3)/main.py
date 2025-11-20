from flask import Flask
import logging
#from gevent.pywsgi import WSGIServer

app = Flask(__name__)
from flask_cors import CORS
cors = CORS(app, resources={r"/*": {"origins": "*"}})

import json
import  psutil

import dbus
# require access to system bus
bus = dbus.SystemBus()

# access systemd manager using its path
proxy = bus.get_object('org.freedesktop.systemd1', '/org/freedesktop/systemd1')

# access standard dbus interface to read/write properties
props = dbus.Interface(proxy, dbus.PROPERTIES_IFACE)

# print version
print(str(props.GetAll("org.freedesktop.systemd1.Manager")["Version"]))

# access the Manager interface
manager = dbus.Interface(proxy, "org.freedesktop.systemd1.Manager")
def is_service_active(service):
    try:
        manager.GetUnit(service)
        return True
    except:
        return False

def stop(service):
    if is_service_active(service):
        try:
            manager.StopUnit(service, 'replace')
        except:
            print("stop service failed")
service = "mc-survival.service"
@app.route('/shutdown', methods=['GET'])
def shutdown():
    # invoke shutdown method
    stop(service)
    while is_service_active(service):
        pass
    manager.PowerOff()
    return "Success", 200, {"Access-Control-Allow-Origin": "*"}

@app.route('/reboot', methods=['GET'])
def reboot():
    # invoke reboot method
    stop(service)
    while is_service_active(service):
        pass
    manager.Reboot()
    return "Success", 200, {"Access-Control-Allow-Origin": "*"}

@app.route('/stats', methods=['GET'])
def pc_stats():
    # temps
    cpu_temp = psutil.sensors_temperatures()['coretemp']
    current_cpu_temp = cpu_temp[0][1]

    # usage
    cpu_usage = psutil.cpu_percent()

    memory_usage = psutil.virtual_memory()[2]
    data = {"cpu_temp":current_cpu_temp,"cpu_usage":cpu_usage,"memory_usage":memory_usage}
    return json.dumps(data)

if __name__ == '__main__':
    gunicorn_logger = logging.getLogger('gunicorn.error')
    app.logger.handlers = gunicorn_logger.handlers
    app.logger.setLevel(gunicorn_logger.level)
    # Debug/Development
    #app.run(debug=True, host="0.0.0.0", port="5000")
    app.run()
    # Production
    #http_server = WSGIServer(('', 5000), app)
    #http_server.serve_forever()
