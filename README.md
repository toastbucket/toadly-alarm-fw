# Alarm
Alarm system FW for ambulance camper. Takes signals from door sensors and will begin alarming if activated while enabled.

## Development
### Environment Setup
For reference, review [Zephyr's Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)

Install west:
```
pip install west
```

<!--TODO: find a good way to lock to a version-->
Pull Zephyr wherever seems good:
```
west init <path-to-zephyr>
cd <path-to-zephyr>
west update
```

Export Zephyr's CMake package
```
west zephyr-export
```

Install Zephyr's Python requirements
```
pip install -r <path-to-zephyr>/zephyr/scripts/requirements.txt
```

Install Zephyr's [latest SDK](https://github.com/zephyrproject-rtos/sdk-ng/releases)
```
cd ~/Downloads
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/<latest>/zephyr-sdk-<latest>_linux-x86_64.tar.gz
tar xvf zephyr-sdk-<latest>_linux-x86_64.tar.gz -C .local/opt/
cd .local/opt/zephyr-sdk-<latest>_linux-x86_64.tar.gz
./setup.sh
```

Install udev rules:
```
sudo cp ~/.local/opt/zephyr-sdk-<latest>/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
sudo udevadm control --reload
```

### Build on Host
```
cd <path-to-this-project>
source <path-to-zephyr>/zephyr/zephyr-env.sh
west build .
```

### Build with Docker
Launch docker:
```
docker run -ti -v <path-to-this-project>:/workdir/alarm -v <path-to-zephyr>:/workdir/zephyr docker.io/zephyrprojectrtos/zephyr-build:latest
```

Then configure and build:
```
$ cd alarm
$ source /workdir/zephyr/zephyr/zephyr-env.sh
$ west build .
```

### Flash
```
west flash
```
