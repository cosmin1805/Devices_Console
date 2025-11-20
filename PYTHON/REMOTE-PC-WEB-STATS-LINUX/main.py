import json #for json output

from flask import Flask
from gevent.pywsgi import WSGIServer 
app = Flask(__name__)
from flask_cors import CORS
cors = CORS(app, resources={r"/*": {"origins": "*"}})

import  psutil
import os


@app.route('/shutdown', methods=['GET'])
def shutdown():
    os.system("shutdown now")
    return "Success", 200, {"Access-Control-Allow-Origin": "*"}

@app.route('/reboot', methods=['GET'])
def reboot():
    os.system("reboot now")
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
   # Debug/Development
    #app.run(debug=True, host="0.0.0.0", port="5000")
    app.run()
    # Production
    #http_server = WSGIServer(('', 5000), app)
    #http_server.serve_forever()
