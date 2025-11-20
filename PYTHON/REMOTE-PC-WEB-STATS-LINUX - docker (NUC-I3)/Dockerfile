FROM debian 

# install required packages
RUN apt-get -q -y update && apt-get -q -y install --no-install-recommends \
    python3 python3-pip python3-setuptools python3-wheel \
    pkg-config \
    dbus libdbus-1-dev \
    build-essential \
    libglib2.0 libglib2.0-dev && \
    rm -rf /var/lib/apt/lists/*

# adds the sample scripts
WORKDIR /root
COPY . /root

# uses pip to install python module (it needs compiler)
RUN pip install --no-cache-dir -r requirements.txt --break-system-packages


# runs python interpreter as main process
CMD ["gunicorn", "--bind", "0.0.0.0:2000", "main:app"]
